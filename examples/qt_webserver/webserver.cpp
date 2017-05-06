/*
 * Example of using Qt to run a web server using httpdlib.
 * Copyright (C) 2017  Andreas Wass
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "webserver.h"
#include "httpdlib/buffer/adapter/adapter.h"
#include "httpdlib/buffer/double_buffer.h"
#include "httpdlib/buffer_response.h"
#include "httpdlib/callable_response_generator.h"
#include "httpdlib/codes/codes.h"
#include "httpdlib/filesystem_response_generator.h"
#include "httpdlib/memory_response.h"
#include "httpdlib/response_generator_collection.h"
#include "httpdlib/string_util/string_util.h"
#include <QDebug>
#include <QFile>
#include <QTcpSocket>

static httpdlib::response_generator_collection generators;
static httpdlib::filesystem_response_generator *response_generator;

// To be able to serve QFile we build an adapter and then build a response.
// Builds an adapter that can be used with buffers.
// The general adapter class takes ownership of the pointer via
// std::unique_ptr<QFile>.
static auto adapter(std::unique_ptr<QFile> file) {
    // std::bind can't be used for read because QFile::read has a bunch of
    // overloads, so to get around this make a small lambda.
    // The lambda (and binds) should use the raw pointer
    // but ownership must later be transferred to adapter via std::move
    QFile *ptr = file.get();
    auto read_callable = [ptr](char *buf, std::size_t max_len) {
        return ptr->read(buf, static_cast<qint64>(max_len));
    };

    // Adapter will take ownership of pointer to release it to get the raw
    // pointer (used by lambda and by by bind etc.) and let adapter take
    // ownership of the raw pointer
    return httpdlib::buffer::adapter::adapter(
        std::move(file), std::bind(&QFile::size, ptr), read_callable,
        std::bind(&QFile::atEnd, ptr));
}

// Shows how easy it is to adapt a response to for instance a QFile
// using the generic buffer_response with double_buffer and a custom adapter.
static auto qfile_response(std::unique_ptr<QFile> file) {
    return httpdlib::buffer_response(
        httpdlib::buffer::double_buffer(adapter(std::move(file))));
}

// This function is used together with callable_response_generator to generate
// responses.
static std::unique_ptr<httpdlib::interface::response>
resource_response(const httpdlib::request &r) {
    if (httpdlib::string_util::ends_with(r.uri(), ".json")) {
        auto *retval = new httpdlib::memory_response;

        retval->set_code(200);
        retval->set_header("content-type", "application/json;charset=utf-8");
        std::string data = "{\"uri\":\"";
        data += r.uri() + "\",\"method\": \"" + r.method() + "\"}";
        retval->set_data(data.c_str());

        return std::unique_ptr<httpdlib::interface::response>(retval);
    }
    std::unique_ptr<QFile> file(new QFile(":/www/test.jpg"));
    if (file->open(QFile::ReadOnly)) {
        return qfile_response(std::move(file));
    }
    return httpdlib::memory_response::default_for_code(500);
}

WebServer::WebServer(QObject *parent) : QObject(parent) {
    // Filter so only GET requests will go through
    response_generator = new httpdlib::filesystem_response_generator(PUB_HTML);
    response_generator->add_filter([](const auto &r) {
        return r.method() == "GET" &&
               httpdlib::string_util::ends_with(r.uri(), ".json") == false &&
               r.uri() != "/test.jpg";
    });

    auto qrc_generator =
        httpdlib::callable_response_generator(resource_response);
    qrc_generator->add_filter([](const auto &r) {
        return r.uri() == "/test.jpg" ||
               httpdlib::string_util::ends_with(r.uri(), ".json");
    });

    generators.add_response_generator(response_generator);
    generators.add_response_generator(std::move(qrc_generator));

    m_tcp_server = new QTcpServer(this);

    connect(this, &WebServer::closeSocket, this, &WebServer::onCloseSocket,
            Qt::QueuedConnection);

    connect(m_tcp_server, &QTcpServer::newConnection, this,
            &WebServer::onNewConnection);

    if (m_tcp_server->listen(QHostAddress::Any, 8088)) {
        qDebug() << "Listening on " << m_tcp_server->serverPort();
    }

    m_second_timer.setInterval(1000);
    m_second_timer.setSingleShot(false);
    connect(&m_second_timer, &QTimer::timeout, this,
            &WebServer::onSecondTimeout);

    m_second_timer.start();
}

void WebServer::onNewConnection() {
    qDebug() << "New connection";
    QTcpSocket *socket = m_tcp_server->nextPendingConnection();
    m_request_data[socket].reset();

    socket->setProperty("timeout", 5);

    connect(socket, &QTcpSocket::disconnected, this,
            &WebServer::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &WebServer::onReadyRead);
    connect(socket, &QTcpSocket::bytesWritten, this,
            &WebServer::onBytesWritten);
}

void WebServer::onReadyRead() {
    auto *socket = qobject_cast<QTcpSocket *>(sender());
    socket->setProperty("timeout", 5);
    auto &request = m_request_data[socket];
    auto allData = socket->readAll();
    qDebug() << "Received data \"" << allData << "\"";
    request << allData;

    if (request) {
        auto writer = [&socket](const char *data, std::size_t length) {
            qDebug() << "Writing " << length << " bytes";
            qint64 retval = socket->write(data, length);
            if (retval <= 0) {
                if (retval < 0) {
                    qDebug() << "SOCKET ERROR: " << socket->error();
                }
                return static_cast<std::size_t>(0);
            }
            return static_cast<std::size_t>(retval);
        };

        std::unique_ptr<httpdlib::interface::response> response;
        response = generators.get_response(request);

        if (response->code() >= 200 && response->code() < 300) {
            response->set_code(httpdlib::codes::ok);
        }
        response->write_next(writer);
        m_responses[socket] = std::move(response);
        request.reset();
    }
    else if (request.error()) {
    }
}

void WebServer::onBytesWritten(qint64) {
    auto *socket = qobject_cast<QTcpSocket *>(sender());
    socket->setProperty("timeout", 5);
    auto &response = m_responses[socket];
    if (!response->done()) {
        auto writer = [&socket](const char *data, std::size_t length) {
            qDebug() << "Writing " << length << " bytes";
            qint64 retval = socket->write(data, length);
            if (retval <= 0) {
                if (retval < 0) {
                    qDebug() << "SOCKET ERROR: " << socket->error();
                }
                return static_cast<std::size_t>(0);
            }
            return static_cast<std::size_t>(retval);
        };
        response->write_next(writer);
    }
    else {
        auto &request = m_request_data[socket];
        if (request.header_value("connection") == "close") {
            socket->close();
        }
    }
}

void WebServer::onDisconnected() {
    qDebug() << "Disconnected";
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    qDebug() << socket->error();
    m_responses.erase(socket);
    m_request_data.erase(socket);
    socket->deleteLater();
}

void WebServer::onSecondTimeout() {
    for (auto &c : m_request_data) {
        auto timeout = c.first->property("timeout").toInt();
        timeout--;
        c.first->setProperty("timeout", timeout);
        if (timeout <= 0) {
            // Emit a signal so the actual closing is
            // done in the event loop to make sure
            // the map isn't modified in onDisconnected() unexpectedly
            qDebug() << "Connection timeout...";
            emit closeSocket(c.first);
        }
    }
}

void WebServer::onCloseSocket(QTcpSocket *socket) {
    socket->close();
}

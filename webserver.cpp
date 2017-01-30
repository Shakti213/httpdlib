#include "webserver.h"
#include "httpdlib/codes.h"
#include "httpdlib/filesystem_response_generator.h"
#include "httpdlib/memory_response.h"
#include "httpdlib/response_generator_collection.h"
#include "httpdlib/string_util.h"
#include <QDebug>
#include <QTcpSocket>

static httpdlib::response_generator_collection generators;
static httpdlib::filesystem_response_generator *response_generator;

class json_generators : public httpdlib::interface::response_generator
{

    // response_generator interface
public:
    // Just a simple generator that generates a JSON response
    // based on some stuff.
    json_generators() {
        add_filter([](const auto &r) {
            return httpdlib::string_util::ends_with(r.uri(), ".json");
        });
    }

    std::unique_ptr<httpdlib::interface::response>
    get_response(const httpdlib::request &request) override {
        auto *retval = new httpdlib::memory_response;

        retval->set_code(200);
        retval->set_header("content-type", "application/json;charset=utf-8");
        std::string data = "{\"uri\":\"";
        data += request.uri() + "\",\"method\": \"" + request.method() + "\"}";
        retval->set_data(data.c_str());

        return std::unique_ptr<httpdlib::interface::response>(retval);
    }
};

WebServer::WebServer(QObject *parent) : QObject(parent) {
    // Filter so only GET requests will go through
    response_generator = new httpdlib::filesystem_response_generator(
        R"|(C:\Users\andreaswass\www-data)|");
    response_generator->add_filter([](const auto &r) {
        return r.method() == "GET" &&
               httpdlib::string_util::ends_with(r.uri(), ".json") == false;
    });

    generators.add_response_generator(response_generator);
    generators.add_response_generator(new json_generators);

    m_tcp_server = new QTcpServer(this);

    connect(m_tcp_server, &QTcpServer::newConnection, this,
            &WebServer::onNewConnection);

    if (m_tcp_server->listen(QHostAddress::Any, 8088)) {
        qDebug() << "Listening on " << m_tcp_server->serverPort();
    }
}

void WebServer::onNewConnection() {
    qDebug() << "New connection";
    QTcpSocket *socket = m_tcp_server->nextPendingConnection();
    m_request_data[socket].reset();

    connect(socket, &QTcpSocket::disconnected, this,
            &WebServer::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &WebServer::onReadyRead);
    connect(socket, &QTcpSocket::bytesWritten, this,
            &WebServer::onBytesWritten);
}

void WebServer::onReadyRead() {
    auto *socket = qobject_cast<QTcpSocket *>(sender());
    auto &request = m_request_data[socket];
    auto allData = socket->readAll();
    qDebug() << "Received data \"" << allData << "\"";
    request << allData;

    if (request) {
        auto writer = [&socket](const char *data, std::size_t length) {
            qDebug() << "Writing " << length << " bytes";
            qint64 retval = socket->write(data, length);
            // socket->flush();
            if (retval <= 0) {
                if (retval < 0) {
                    qDebug() << "SOCKET ERROR: " << socket->error();
                }
                return static_cast<std::size_t>(0);
            }
            return static_cast<std::size_t>(retval);
        };

        auto response = generators.get_response(request);
        response->set_code(httpdlib::codes::ok);
        response->write_next(writer);
        m_responses[socket] = std::move(response);
        request.reset();
        /*try {
            if (response_generator.try_write_response(request, writer) == 0) {
                httpdlib::memory_response resp(httpdlib::codes::not_found);
                resp.write(writer, resp.ContinueOnZeroBytesWritten);
            }
        }
        catch (std::exception &e) {
            qDebug() << e.what();
        }
        qDebug() << "Resetting...";
        request.reset();*/
    }
    else if (request.error()) {
    }
}

void WebServer::onBytesWritten(qint64) {
    auto *socket = qobject_cast<QTcpSocket *>(sender());
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
}

void WebServer::onDisconnected() {
    qDebug() << "Disconnected";
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    qDebug() << socket->error();
    m_responses.erase(socket);
    m_request_data.erase(socket);
    socket->deleteLater();
}

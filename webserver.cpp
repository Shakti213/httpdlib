#include "webserver.h"
#include "httpdlib/memory_response.h"
#include "httpdlib/filesystem_response_generator.h"
#include <QDebug>
#include <QTcpSocket>

httpdlib::filesystem_response_generator response_generator(R"|(C:\Users\andreaswass\www-data)|");

WebServer::WebServer(QObject *parent):
    QObject(parent)
{
    m_tcp_server = new QTcpServer(this);

    connect(m_tcp_server, &QTcpServer::newConnection, this, &WebServer::onNewConnection);

    if(m_tcp_server->listen(QHostAddress::Any, 8088)) {
        qDebug() << "Listening on " << m_tcp_server->serverPort();
    }
}

void WebServer::onNewConnection()
{
    qDebug() << "New connection";
    QTcpSocket *socket = m_tcp_server->nextPendingConnection();
    m_request_data[socket].reset();

    connect(socket, &QTcpSocket::disconnected, this, &WebServer::onDisconnected);
    connect(socket, &QTcpSocket::readyRead, this, &WebServer::onReadyRead);
}

void WebServer::onReadyRead()
{
    auto *socket = qobject_cast<QTcpSocket*>(sender());
    auto &request = m_request_data[socket];
    auto allData = socket->readAll();
    qDebug() << "Received data \"" << allData << "\"";
    request << allData;

    if(request.parse_state() == httpdlib::request::Finished) {
        auto resp = response_generator.get_response(request);
        auto writer = [&socket](const char *data, std::size_t length) {
            return static_cast<std::size_t>(socket->write(data, length));
        };
        resp->write(writer);

        request.reset();
    }
}

void WebServer::onDisconnected()
{
    qDebug() << "Disconnected";
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    m_request_data.erase(socket);
    socket->deleteLater();
}

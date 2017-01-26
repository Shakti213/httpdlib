#include "webserver.h"
#include "httpdlib/memory_response.h"
#include "httpdlib/filesystem_response_generator.h"
#include "httpdlib/codes.h"
#include <QDebug>
#include <QTcpSocket>

class method_filter: public  httpdlib::interface::response_generator_filter
{
    // response_generator_filter interface
public:
    bool passes_filter(const httpdlib::request &request) const override
    {
        return request.method() == "GET";
    }
};

httpdlib::filesystem_response_generator response_generator(R"|(C:\Users\andreaswass\www-data)|");

WebServer::WebServer(QObject *parent):
    QObject(parent)
{
    response_generator.add_filter(new method_filter);
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

    if(request) {
        auto writer = [&socket](const char *data, std::size_t length) {
            return static_cast<std::size_t>(socket->write(data, length));
        };
        if(response_generator.try_write_response(request, writer) == 0) {
            httpdlib::memory_response resp(httpdlib::codes::not_found);
            resp.write(writer);
        }

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

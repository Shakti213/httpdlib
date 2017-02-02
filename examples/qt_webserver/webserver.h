#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "httpdlib/interface/response.h"
#include "httpdlib/request.h"
#include <QObject>
#include <QTcpServer>
#include <map>
#include <memory>

class WebServer : public QObject
{
    Q_OBJECT

    QTcpServer *m_tcp_server;
    std::map<QTcpSocket *, httpdlib::request> m_request_data;
    std::map<QTcpSocket *, std::unique_ptr<httpdlib::interface::response>>
        m_responses;

public:
    WebServer(QObject *parent = nullptr);

public slots:
    void onNewConnection();
    void onReadyRead();
    void onBytesWritten(qint64);
    void onDisconnected();
};

#endif // WEBSERVER_H

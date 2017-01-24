#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <QObject>
#include <QTcpServer>
#include <map>
#include "httpdlib/http_request.h"

class WebServer: public QObject
{
    Q_OBJECT

    QTcpServer *m_tcp_server;
    std::map<QTcpSocket*, httpdlib::http_request> m_request_data;
public:
    WebServer(QObject *parent=nullptr);

public slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
};

#endif // WEBSERVER_H

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

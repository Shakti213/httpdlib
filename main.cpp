#include "httpdlib/request.h"
#include "webserver.h"
#include <QCoreApplication>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);

    WebServer web_server;

    return app.exec();
}

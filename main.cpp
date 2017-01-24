#include <QCoreApplication>
#include <string>
#include "webserver.h"
#include "httpdlib/http_request.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    WebServer web_server;

    return app.exec();
}

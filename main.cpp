#include "chatserver.h"
#include "chatclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatServer w;
    ChatClient w;
    w.show();

    return a.exec();
}

#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QMainWindow>

namespace Ui {
class ChatServer;
}

class ChatServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatServer(QWidget *parent = 0);
    ~ChatServer();

private:
    Ui::ChatServer *ui;
};

#endif // CHATSERVER_H

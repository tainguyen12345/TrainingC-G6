#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QMainWindow>

#include <QTcpServer>

class QTcpSocket;
class QMessageBox;


namespace Ui {
class ChatServer;
}

class ChatServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatServer(QWidget *parent = 0);
    ~ChatServer();

private slots:
    void send_message();
    void receive_message();
    void add_connect();
    void remove_connect();

    void on_pushButton_Listen_clicked();


private:
    QTcpSocket *socket;
    QTcpServer *server;

    Ui::ChatServer *ui;
};

#endif // CHATSERVER_H

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QMainWindow>

namespace Ui {
class ChatClient;
}

class ChatClient : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatClient(QWidget *parent = 0);
    ~ChatClient();

private:
    Ui::ChatClient *ui;
};

#endif // CHATCLIENT_H

#include "chatserver.h"
#include "ui_chatserver.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>


static const int DEFAULT_PORT = 6789;
int port;
QHostAddress server_add;

ChatServer::ChatServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatServer)
{
    ui->setupUi(this);
    server = new QTcpServer(this);
    socket = new QTcpSocket(this);
    //set Ip and Port
    ui->lineEdit_Ip->setText("192.168.12.51");
    ui->lineEdit_Ip->setEnabled(false);
    ui->lineEdit_Port->setText("6789");
    ui->lineEdit_Port->setEnabled(false);

    //set text contentChat, pushButton_linten,
    ui->textEdit_content->setEnabled(false);
    ui->pushButton_Listen->setText("Listen");
    qDebug() << "No connect!";

    connect(ui->pushButton_Send,SIGNAL(clicked(bool)),this,SLOT(send_message()));
    connect(ui->lineEdit_Send,SIGNAL(returnPressed()),this,SLOT(send_message()));
    connect(server,SIGNAL(newConnection()),this,SLOT(add_connect()));
    //set address and port
    server_add = QHostAddress::AnyIPv4;
    port = DEFAULT_PORT;

}

ChatServer::~ChatServer()
{
    delete ui;
}

void ChatServer::send_message(){
    QMessageBox box;
    if(!socket->isOpen()){
        ui->lineEdit_Send->clear();
        qDebug()<< "Socket is not open, Cann't send message now";
        box.information(this,tr("The Title"),tr("Socket is close! Cann't send message now!"));
    }
    else{
        bool a = socket->write(ui->lineEdit_Send->text().toLatin1());
        if(a){
            qDebug() << "Send message success!";
            ui->textEdit_content->append("Server: " + ui->lineEdit_Send->text());
            ui->lineEdit_Send->clear();
        }
        else{
            qDebug() << socket->errorString();
            box.information(this,tr("The Title"),tr("Message error!"));
        }
    }
}

//receive message -> compelate
void ChatServer::receive_message(){
    qDebug() << "Have a messge!";
    ui->textEdit_content->append("Client: "+socket->readAll());
}

//add connect --> compelate
void ChatServer::add_connect(){
    socket = server->nextPendingConnection();
    QString m = "Hello Client!";
    qDebug() << "Have new connect";
    connect(socket,SIGNAL(readyRead()),this,SLOT(receive_message()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(remove_connect()));
    socket->write(m.toLatin1());
    server->close();
}

//remove connect --> compelate
void ChatServer::remove_connect(){
    qDebug() << "Socket close!";
    QMessageBox box;
    box.setText("Socket close!");
    box.exec();
    server->close();
    server->listen(server_add,port);
}

//--> Compelate
void ChatServer::on_pushButton_Listen_clicked()
{
    QMessageBox box;
    server->close();
    if(socket->isOpen()){
        socket->close();
    }
    if(ui->pushButton_Listen->text() == "Listen"){
        server->listen(server_add, port);
        if(server->isListening()){
            qDebug() << "Server is listening...";
            ui->pushButton_Listen->setText("Sleep");
        }
        else{
            box.information(this,tr("The Title"),tr("Open Server error!"));
            qDebug() << "Server listen is error!";
        }
    }
    else{
        server->close();
        if(server->isListening()){
            qDebug() << "Close server is false.";
        }
        else{
            qDebug() << "Server is Close.";
            ui->textEdit_content->append("Socket is close!");
            ui->pushButton_Listen->setText("Listen");
        }
    }
}

//box file --> Compelate
void ChatServer::on_toolButton_file_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"","/","");
    if(filename.isEmpty()){
        return;
    }
    ui->textEdit_content->append(filename);
}


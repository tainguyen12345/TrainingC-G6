#include "chatserver.h"
#include "ui_chatserver.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QBuffer>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

static const int DEFAULT_PORT = 6789;
static const int DEFAULT_PORT =5555;
int port;
QHostAddress server_addr;
ChatServer::ChatServer(QMainWindow *parent) :
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
    socket = new QTcpSocket(this);
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()),this, SLOT(addConnection()));
    connect(ui ->editmes, SIGNAL(returnPressed()), this, SLOT(sendMessage()));
    connect(ui->btnsend,SIGNAL(clicked(bool)), this, SLOT(sendMessage()));
    server_addr = QHostAddress::LocalHost;
    port = DEFAULT_PORT;
    ui ->editport ->setText("5555");
    ui ->editport ->setEnabled(false);
    ui ->editIP ->setText("192.168.11.17 ");
    ui ->editIP ->setEnabled(false);
    server_addr = QHostAddress::AnyIPv4;
}

ChatServer::~ChatServer()
{
    server->close();
    socket->close();
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
    ui->lineEdit_Send->clear();
void ChatServer::on_btnlistent_clicked()
{
    port = ui->editport->text().toInt();
    bool b=server->listen(server_addr,port);
    if(b)
    {
        server->close();
        qDebug("server started");
        ui->btnlistent->setEnabled(true);
        //ui->btnStop->setEnabled(true);
    }
   else
    {
        qDebug("server can't start! check IP or Port");
    }
}
//void ChatServer::on_btnStop_clicked()
//{
//   if(server->isListening())
//   {
//       server->close();
//        ui->btnStart->setEnabled(true);
//        ui->btnStop->setEnabled(false);
//    }
//}
void ChatServer::addConnection()
{
    QTcpSocket* connection =server->nextPendingConnection();
    connections.append(connection);
    QBuffer *buffer= new QBuffer(this);
    buffer->open(QIODevice::ReadWrite);
    buffers.insert(connection, buffer);
    connect(connection, SIGNAL(disconnected()),this, SLOT(removeConnection()));
    connect(connection, SIGNAL(readyRead()), this, SLOT(receiveMessage()));

//receive message -> compelate
void ChatServer::receive_message(){
    qDebug() << "Have a messge!";
    ui->textEdit_content->append("Client: "+socket->readAll());
}
void ChatServer::removeConnection()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    QBuffer *buffer = buffers.take(socket);
    buffer->close();
    ui->btnlistent->setEnabled(true);
    buffer->deleteLater();

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
    connections.removeAll(socket);

    socket->deleteLater();
}
void ChatServer::receiveMessage()

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
{

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    QBuffer *buffer = buffers.value(socket);

    qint64 bytes = buffer->write(socket->readAll());

    buffer->seek(buffer->pos() - bytes);

    while (buffer->canReadLine())

    {
            QByteArray line = buffer->readLine();
            ui->editchat->append(line.simplified());

            foreach (QTcpSocket* connection, connections)

            {

                    connection->write(line);

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
void ChatServer::sendMessage(){
   bool a = socket->write(ui->editmes->text().toLatin1());
   if(a){
        qDebug() << "Send message is success!!!";
        ui->editchat->append("Client: " + ui->editmes->text());
    }
    else{
        qDebug() << "Send message is error!!!";
   }
    ui->editmes->clear();
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


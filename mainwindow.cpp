#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

static const int DEFAULT_PORT = 6789;
int port;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    MainWindow w;
//    w.setFixedSize(200,300);
    socket = new QTcpSocket(this);
    time = new QTimer(this);

    //set Ip and Port
    ui->lineEdit_Ip->setText("192.168.12.51");
    ui->lineEdit_Ip->setEnabled(false);
    ui->lineEdit_Port->setText("6789");
    ui->lineEdit_Port->setEnabled(false);

    //set text Notify and pushButton_connect
    qDebug() << "Not have connect to server!";
    ui->pushButton_Connect->setText("Connect");
    ui->textEdit_Content->setEnabled(false);

    if(!socket->isOpen()){
        qDebug() << "Socket not Open!";
    }

    //
    connect(ui->lineEdit_Send,SIGNAL(returnPressed()),this,SLOT(send_message()));
    connect(ui->pushButton_Send,SIGNAL(clicked(bool)),this,SLOT(send_message()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(receive_message()));

    connect(time,SIGNAL(timeout()),this,SLOT(time_connect()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(socket_disconnect()));
    connect(socket,SIGNAL(connected()),this,SLOT(time_close()));
    port = DEFAULT_PORT;
}

MainWindow::~MainWindow()
{
    delete ui;
}

//send message --> Conpelate
void MainWindow::send_message(){
    QMessageBox box;
    bool a = socket->write(ui->lineEdit_Send->text().toLatin1());
    if(a){
        qDebug() << "Send message success!";
        ui->textEdit_Content->append("Client: " + ui->lineEdit_Send->text());
    }
    else{
        qDebug() << socket->errorString();
        box.setText("Message error!");
        box.exec();
    }
    ui->lineEdit_Send->clear();
}

//receive message --> Compelate
void MainWindow::receive_message(){
    qDebug() << "Have a message!";
    ui->textEdit_Content->append("Server: " + socket->readAll());
}

//connect/disconnect to server
void MainWindow::on_pushButton_Connect_clicked(){
    QMessageBox box;
    if(ui->pushButton_Connect->text() == "Connect"){
        socket->connectToHost("192.168.12.51",port);
        if(socket->isOpen()){
            qDebug() << "Connecting...!";
            ui->pushButton_Connect->setText("Disconncet");
            time->start(3000);
        }
    }
    else{
        socket->disconnectFromHost();
        qDebug() << "Disconnected to server!";
        ui->pushButton_Connect->setText("Connect");
    }
}

// --> Compelate
void MainWindow::socket_disconnect(){
    QMessageBox box;
    qDebug() << "Server close!";
    ui->pushButton_Connect->setText("Connect");
    box.setText("Server close!");
    box.exec();
}

//box file --> compelate
void MainWindow::on_toolButton_file_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,"","/","");
    if(filename.isEmpty()){
        return;
    }
    ui->textEdit_Content->append(filename);
}

//time out connect to server
void MainWindow::time_connect(){
    QMessageBox box;
    box.setText("Server busy!");
    qDebug() << "Server Busy!";
    ui->pushButton_Connect->setText("Connect");
    time->stop();
    box.exec();
    socket->close();
}

void MainWindow::time_close(){
    time->stop();
}

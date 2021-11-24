#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  //,tcpClient(new TcpClient())
  //,thread(new QThread())
  ,timerOthers(new QTimer())
{
    ui->setupUi(this);
    connect(timerOthers,SIGNAL(timeout()),this,SLOT(on_otherTask()));

    timerOthers->start(400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_hangup_clicked()
{
    if(tcpClient)
        tcpClient->finTask();
}

void MainWindow::on_btn_request_clicked()
{
    TcpClient::InfoAddr_t face;
    face.ip = "127.0.0.1";
    face.port = 10018;
    face.message = ui->edit_message->text();
    tcpClient = new TcpClient(&face);
    QThread* thread = new QThread();
    tcpClient->moveToThread(thread);
    connect(thread,SIGNAL(started()),tcpClient,SLOT(on_doTask()));
    connect(tcpClient,SIGNAL(sendResp(QString)),this,SLOT(on_sendResp(QString)));
    connect(tcpClient,SIGNAL(doneTask()),thread,SLOT(quit()));
    connect(tcpClient,SIGNAL(doneTask()),thread,SLOT(deleteLater()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    thread->start();
}

void MainWindow::on_otherTask(){
    qDebug() << "Others... ";
}

void MainWindow::on_sendResp(QString in_str){
    ui->label_response->setText(in_str);
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "tcp_client.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_otherTask();
    void on_sendResp(QString in_str);

private slots:
    void on_btn_hangup_clicked();

    void on_btn_request_clicked();

private:
    Ui::MainWindow *ui;
    TcpClient* tcpClient;
    //QThread* thread;
    QTimer* timerOthers;
};

#endif // MAINWINDOW_H

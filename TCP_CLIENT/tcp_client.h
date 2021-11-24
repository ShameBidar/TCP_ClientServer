#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>


class TcpClient : public QObject
{
    Q_OBJECT
public:
    typedef struct{
        QString ip;
        QString message;
        int port;
    }InfoAddr_t;

    explicit TcpClient(InfoAddr_t* addrInfo, QObject *parent = nullptr);
    ~TcpClient();
    void finTask();

signals:
    void doneTask();
    void sendResp(QString in_str);

public slots:
    void on_doTask();

private:
    bool flgRunning;
    int sockfd;
    char message[50];
};

#endif // TCP_CLIENT_H

#include "tcp_client.h"

TcpClient::TcpClient(InfoAddr_t* addrInfo, QObject *parent) : QObject(parent)
  ,flgRunning(false)
{
    struct sockaddr_in serverAddr;
    char myAddr[30] = {0};
    memcpy(myAddr,addrInfo->ip.toStdString().c_str(),addrInfo->ip.size());
    memcpy(message,addrInfo->message.toStdString().c_str(),addrInfo->message.size());

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0){
        qErrnoWarning("Error on Openning Socket");
        while(true);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(addrInfo->port);
    if(inet_pton(AF_INET,myAddr,&serverAddr.sin_addr)<=0){
        qErrnoWarning("Invalid Address");
        while(true);
    }

    if(::connect(sockfd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) < 0){
        qErrnoWarning("Connect Failed");
        while(true);
    }

}

TcpClient::~TcpClient(){

}


void TcpClient::on_doTask(){
    int counter = 0;
    flgRunning = true;
    char resp[128];
    pollfd pfd[] = {{0,POLLIN,0},{sockfd,POLLIN,0}};
    while(flgRunning)
    {
        send(sockfd,message,strlen(message),0);
        bzero(resp,sizeof(resp));
        while (true) {
            poll(pfd,2,0);
            if(pfd[1].revents & POLLIN){
                int ret = read(sockfd,resp,sizeof(resp));
                if(ret){
                    QString temp(resp);
                    emit sendResp(temp);
                }
                break;
            }
        }

//        qDebug() << "ret: " << ret << " " << resp;
//

        QThread::msleep(500);
    }
    emit doneTask();
}


void TcpClient::finTask(){
    flgRunning = false;
}

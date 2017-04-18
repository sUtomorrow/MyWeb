#ifndef MTCPSOCKET_H
#define MTCPSOCKET_H
#include<QtNetwork/QTcpSocket>
#include<qdebug.h>
class MTcpSocket:public QTcpSocket{
    Q_OBJECT
public:
    int threadId;
    MTcpSocket(qintptr socketDescriptor,int threadId);
//    QString DecodeHttp(QString httpCon);
public slots:
    void response();
    void sendmDis();
signals:
    void mDisconnect(int);  //tcp链接已断开时发送此信号，在主线程中接收，传递当前连接的子线程在线程池中的编号，以便回收线程资源。
};

#endif // MTCPSOCKET_H

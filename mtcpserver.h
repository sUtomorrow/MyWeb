#ifndef MTCPSERVER_H
#define MTCPSERVER_H
#include<QtNetwork/QTcpServer>
#include<QThread>
#include"mtcpsocket.h"
#include"threadpool.h"
class MTcpServer:public QTcpServer{
    Q_OBJECT
public:
    ThreadPool* tpool;
    QString* rootDirPath;
    MTcpServer(int maxThreadNum,QString rootDirPath);
    ~MTcpServer();
protected:
    void incomingConnection(int socketDescriptor);
public slots:
    void recoverThread(int threadId);
};

#endif // MTCPSERVER_H

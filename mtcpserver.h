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
    void mRecvDealInfo(QString reqIp,int reqPost,int threadId,QString requestHttp,QString responseHttp);
signals:
    void mPassDetailToWindow(QString reqIp,int reqPost,int threadId,QString requestHttp,QString responseHttp);
};

#endif // MTCPSERVER_H

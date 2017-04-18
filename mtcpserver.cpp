#include "mtcpserver.h"

MTcpServer::MTcpServer(int maxThreadNum):QTcpServer(){
    tpool = new ThreadPool(maxThreadNum);
}

MTcpServer::~MTcpServer(){
    delete tpool;
}
/**
 * @brief MTcpServer::incomingConnection    重载父类QTcpServer的incomingConnection方法,此方法在有Tcp连接请求时触发
 * @param socketDescriptor  新连接建立的QTcpSocket描述
 */
void MTcpServer::incomingConnection(int socketDescriptor){
    MTcpSocket* newsocket = new MTcpSocket(socketDescriptor,0);
    QThread* sockThread = tpool->getThread(&(newsocket->threadId)); //从线程池中取出一个可用线程
    if(sockThread==NULL){   //取出线程失败
        qDebug("线程池已枯竭!");
        delete newsocket;
        return ;
    }
    newsocket->moveToThread(sockThread);    //将要进行的网络操作转给新线程
    connect(newsocket,SIGNAL(readyRead()),newsocket,SLOT(response()));  //待接收数据已准备好,由response函数处理请求
    connect(sockThread, SIGNAL(finished()),newsocket, SLOT(deleteLater())); //线程结束后回收分配的socket资源
    connect(newsocket,SIGNAL(disconnected()),newsocket,SLOT(sendmDis()));   //连接关闭信号和MTcpSocket的sendmDis槽关联
    connect(newsocket,SIGNAL(mDisconnect(int)),this,SLOT(recoverThread(int)));  //将MTcpSocket的sendmDis发送的mDisconnect信号和线程回收槽关联，并传递要回收的线程号
    sockThread->start();
}
/**
 * @brief MTcpServer::recoverThread 线程回收
 * @param threadId  要回收的线程号
 */
void MTcpServer::recoverThread(int threadId){
    qDebug("recover thread id:%d",threadId);
    tpool->recoverThread(threadId);
}

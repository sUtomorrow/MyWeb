#include "mtcpsocket.h"

/**
 * @brief MTcpSocket::MTcpSocket
 * @param socketDescriptor  要处理的socket描述
 * @param threadId  使用的线程的线程id,使用结束后将作为参数返回给线程池的线程回收函数
 */
MTcpSocket::MTcpSocket(qintptr socketDescriptor, int threadId):QTcpSocket(){
    this->setSocketDescriptor(socketDescriptor);
    this->threadId = threadId;
}
/**
 * @brief MTcpSocket::response 对当前http请求作出回应
 */
void MTcpSocket::response(){
    QByteArray recvmsg = this->readAll();   //读取请求数据
    qDebug()<<recvmsg.data();
    QString ConType("text/html");
    QString Content("<html><h1>Hello World</h1></html>");
    QString ReString = QString("HTTP/1.1 200 OK\r\nServer: Lty's Server V1.0\r\nAccept-Ranges: bytes\r\nContent-Length: %1\r\nConnection: close\r\nContent-Type: %2\r\n\r\n%3").arg(Content.length()).arg(ConType).arg(Content);
    QByteArray sendmsg(ReString.toUtf8());
    qDebug()<<sendmsg.data();
    this->write(ReString.toUtf8());
    this->flush();
}
/**
 * @brief MTcpSocket::sendmDis 自定义槽,当socket连接关闭时触发，将给mDisconnect信号传递使用的线程号并发送mDisconnect信号
 */
void MTcpSocket::sendmDis(){
    emit(mDisconnect(this->threadId));
}

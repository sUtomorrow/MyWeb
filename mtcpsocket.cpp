#include "mtcpsocket.h"

/**
 * @brief MTcpSocket::MTcpSocket
 * @param socketDescriptor  要处理的socket描述
 * @param threadId  使用的线程的线程id,使用结束后将作为参数返回给线程池的线程回收函数
 */
MTcpSocket::MTcpSocket(qintptr socketDescriptor,int threadId):QTcpSocket(){
    this->setSocketDescriptor(socketDescriptor);
    this->threadId = threadId;
}

/**
 * @brief MTcpSocket::response 对当前http请求作出回应
 */
void MTcpSocket::response(){
    QByteArray recvmsg = this->readAll();   //读取请求数据
    QStringList* HeadAttr = GetUrlFrom(QString(recvmsg));
    qDebug() << HeadAttr->at(ATTR_GET);
    qDebug() << HeadAttr->at(ATTR_CONNECTION);
    qDebug() << HeadAttr->at(ATTR_ACCEPT);
    qDebug() << HeadAttr->at(ATTR_REFERER);
    qDebug() << recvmsg.data();
    qDebug()<<CheckUrl(HeadAttr->at(ATTR_GET));
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

/**
 * @brief GetUrlFrom    解析http请求头
 * @param HttpHeader    http请求头部
 * @return      QStringList类型，里面包含解析出的各种属性。可以通过定义的ATTR_GET、ATTR_CONNECTION、ATTR_ACCEPT、ATTR_REFERER宏来访问
 */
QStringList* MTcpSocket::GetUrlFrom(QString HttpHeader){
    QStringList* headAttr = new QStringList();
    QRegExp reGet = QRegExp("GET (.+) HTTP/.+\r\n");
    QRegExp reConnection = QRegExp("Connection: (.+)\r\n");
    QRegExp reAccept = QRegExp("Accept: (.+)\r\n");
    QRegExp reReferer = QRegExp("Referer: (.+)\r\n");
    reGet.setMinimal(true);
    reConnection.setMinimal(true);
    reAccept.setMinimal(true);
    reReferer.setMinimal(true);
    if(reGet.indexIn(HttpHeader)==-1){
        headAttr->append(QString(""));
        qDebug()<<"match failed";
    }else{
        headAttr->append(reGet.cap(1));
    }
    if(reConnection.indexIn(HttpHeader)==-1){
        headAttr->append(QString(""));
    }else{
        headAttr->append(reConnection.cap(1));
    }
    if(reAccept.indexIn(HttpHeader)==-1){
        headAttr->append(QString(""));
    }else{
        headAttr->append(reAccept.cap(1));
    }
    if(reReferer.indexIn(HttpHeader)==-1){
        headAttr->append(QString(""));
    }else{
        headAttr->append(reReferer.cap(1));
    }
    return headAttr;
}

/**
 * @brief MTcpSocket::CheckUrl  检查Url中是否有非法字符
 * @param Url   待检查的Url
 * @return  true表示没有非法字符，false表示有非法字符
 */
bool MTcpSocket::CheckUrl(QString Url){
    QRegExp reIllegal = QRegExp("(\\.\\.|:|;)");
    if(reIllegal.indexIn(Url)==-1){
        return true;
    }else{
        return false;
    }
}

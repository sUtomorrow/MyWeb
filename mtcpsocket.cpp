#include "mtcpsocket.h"

/**
 * @brief MTcpSocket::MTcpSocket
 * @param socketDescriptor  要处理的socket描述
 * @param threadId  使用的线程的线程id,使用结束后将作为参数返回给线程池的线程回收函数
 */
MTcpSocket::MTcpSocket(qintptr socketDescriptor, QString rootDirPath):QTcpSocket(){
    this->rootDirPath = new QString(rootDirPath);
    this->setSocketDescriptor(socketDescriptor);
    this->threadId = -1;
}

MTcpSocket::~MTcpSocket(){
    delete rootDirPath;
}

/**
 * @brief MTcpSocket::response 对当前http请求作出回应
 */
void MTcpSocket::response(){
    QString* ConType;
    QString* ReString;
    QByteArray* data;
    QByteArray* sendmsg;
    QByteArray recvmsg = this->readAll();   //读取请求数据
    QStringList* HeadAttr = GetUrlFrom(QString(recvmsg));
    qDebug() << HeadAttr->at(ATTR_GET);
    qDebug() << HeadAttr->at(ATTR_CONNECTION);
    qDebug() << HeadAttr->at(ATTR_ACCEPT);
    qDebug() << HeadAttr->at(ATTR_REFERER);
    qDebug() << recvmsg.data();
    qDebug() << CheckUrl(HeadAttr->at(ATTR_GET));
    if(CheckUrl(HeadAttr->at(ATTR_GET))){
        data = GetRequestContent(HeadAttr->at(ATTR_GET));
        if(data==NULL){
            ReString = new QString("HTTP/1.1 404 Not Found\r\nServer: Lty's Server V1.0\r\nConnection: close\r\n\r\n");
            qDebug() << ReString->toUtf8().data();
            this->write(ReString->toUtf8());
            this->flush();
            this->close();
        }else{
            ConType = GetConType(HeadAttr->at(ATTR_GET));
            ReString = new QString(QString("HTTP/1.1 200 OK\r\nServer: Lty's Server V1.0\r\nAccept-Ranges: bytes\r\nContent-Length: %1\r\nConnection: close\r\nContent-Type: %2\r\n\r\n").arg(data->length()).arg(*ConType));
            sendmsg = new QByteArray(ReString->toUtf8());
            sendmsg->append(*data);
            qDebug()<<sendmsg->data();
            this->write(*sendmsg);
            this->flush();
        }
    }else{
        ReString = new QString("HTTP/1.1 404 Not Found\r\nServer: Lty's Server V1.0\r\nConnection: close\r\n\r\n");
        qDebug() << ReString->toUtf8().data();
        this->write(ReString->toUtf8());
        this->flush();
        this->close();
        qDebug() << QString("Illegal Url").toUtf8();
        this->close();
    }
    emit mDealingRequest(this->peerAddress().toString(),this->peerPort(),threadId,QString(recvmsg),*ReString);
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
/**
 * @brief MTcpSocket::GetRequestContent 按照url获取相应的文件的字节数组
 * @param Url   http请求头中的文件路径
 * @return  返回对应文件的字节数组
 */
QByteArray* MTcpSocket::GetRequestContent(QString Url){
    QByteArray* data = NULL;
    QString* AbsolutePath=NULL;
    if(!QString("/").compare(Url)){
        AbsolutePath = new QString(*(this->rootDirPath)+"/index.html");
    }else{
        AbsolutePath = new QString(*(this->rootDirPath)+Url);
    }
    QFile* file = new QFile(*AbsolutePath);
    if(file->exists()){
        file->open(QFile::ReadOnly);
        data = new QByteArray(file->readAll());
        file->close();
        return data;
    }else{
        return NULL;
    }
}

QString* MTcpSocket::GetConType(QString Url){
    QRegExp* reSuffix = new QRegExp(QString("\\.(.+)$"));
    if(reSuffix->indexIn(Url)==-1){
        return new QString("text/html");
    }else{
        if(!reSuffix->cap(1).compare("jpg",Qt::CaseInsensitive)||!reSuffix->cap(1).compare("jpeg",Qt::CaseInsensitive)||!reSuffix->cap(1).compare("gif",Qt::CaseInsensitive)||!reSuffix->cap(1).compare("png",Qt::CaseInsensitive)){
            return new QString("image/"+reSuffix->cap(1));
        }else if(!reSuffix->cap(1).compare("html",Qt::CaseInsensitive)||!reSuffix->cap(1).compare("css",Qt::CaseInsensitive)){
            return new QString("text/"+reSuffix->cap(1));
        }else if(!reSuffix->cap(1).compare("ico",Qt::CaseInsensitive)){
            return new QString("image/x-icon");
        }else if(!reSuffix->cap(1).compare("htm",Qt::CaseInsensitive)){
            return new QString("text/html");
        }else{
            return new QString("text/html");
        }
    }
}

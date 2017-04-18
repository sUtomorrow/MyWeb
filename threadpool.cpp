#include "threadpool.h"

ThreadPool::ThreadPool(int maxThreadNum){
    if(maxThreadNum<=0){
        this->curThreadNum = 0;
        this->usingThreadNum = 0;
        this->minUseableId = -1;
        this->maxThreadNum = 0;
        this->threads = NULL;
        this->status = NULL;
    }
    this->curThreadNum = 0;
    this->usingThreadNum = 0;
    this->minUseableId = 0;
    this->maxThreadNum = maxThreadNum;
    this->threads = new QThread*[maxThreadNum];
    this->status = new char[maxThreadNum];
    memset(this->status,STATUS_UNMALLOC,sizeof(char)*maxThreadNum);
    memset(this->threads,0,sizeof(QThread*)*maxThreadNum);
}

ThreadPool::~ThreadPool(){
    int i;
    for(i=0;i < this->usingThreadNum;i++){
        if(this->threads[i])
            this->threads[i]->quit();
    }
    for(i=0;i < this->curThreadNum;i++){
        if(this->threads[i])
            delete this->threads[i];
    }
    delete[] this->threads;
}
/**
 * @brief ThreadPool::getThread 从线程池中分配一个可用线程
 * @param threadIdPtr  //带出分配的线程ID
 * @return
 */
QThread* ThreadPool::getThread(int* threadIdPtr){
    if(minUseableId==-1){
        *threadIdPtr = -1;
        return NULL;
    }else{
        if(minUseableId>=curThreadNum&&status[minUseableId]==STATUS_UNMALLOC){    //当前可用线程未分配空间
            threads[minUseableId] = new QThread();
        }
        status[minUseableId] = STATUS_USING;   //将状态置为使用中
        *threadIdPtr = minUseableId;  //传出线程Id
        while(++minUseableId<maxThreadNum){
            if(status[minUseableId]==STATUS_UNMALLOC||status[minUseableId]==STATUS_UNUSE){    //找到下一个可用的线程号
                break;
            }
        }
        if(minUseableId >= maxThreadNum){  //说明没有可分配线程
            minUseableId = -1;
        }
        usingThreadNum++;   //更新使用中的线程数
        return threads[*threadIdPtr];  //返回分配的线程指针
    }
}
/**
 * @brief ThreadPool::recoverThread 将线程返还线程池
 * @param threadId 要返还的线程号
 */
void ThreadPool::recoverThread(int threadId){
    if(threadId < minUseableId || minUseableId==-1){
        minUseableId = threadId;
        status[threadId] = STATUS_UNUSE;
        threads[threadId]->exit();
    }
    usingThreadNum--;   //更新使用中的线程数
}

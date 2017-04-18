#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <QThread>
#define STATUS_UNMALLOC 'C'
#define STATUS_UNUSE 'E'
#define STATUS_USING 'G'
class ThreadPool{
public:
    ThreadPool(int maxThreadNum);
    ~ThreadPool();
    QThread* getThread(int* ThreadId);
    void recoverThread(int threadId);
private:
    int curThreadNum;
    int maxThreadNum;
    int usingThreadNum;
    int minUseableId;
    char* status;
    QThread** threads;
};

#endif // THREADPOOL_H

#ifndef HTTPTHREAD_H
#define HTTPTHREAD_H

#include <QtCore>

class CHttpThread : public QThread
{
public:
    CHttpThread();

    void run();
};

#endif // HTTPTHREAD_H

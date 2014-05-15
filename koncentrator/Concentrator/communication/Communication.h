#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "HttpThread.h"

class CCommunication
{
public:
    CCommunication();

    int init();

private:
    CHttpThread* thread;
};

#endif // COMMUNICATION_H

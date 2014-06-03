#ifndef MODBUSTHREAD_H
#define MODBUSTHREAD_H

#include <QThread>

namespace NProtocol {

class ModbusThread : public QThread
{
    Q_OBJECT
public:
    explicit ModbusThread(QObject *parent = 0);

protected:
    void run();

signals:

public slots:

};

}

#endif // MODBUSTHREAD_H

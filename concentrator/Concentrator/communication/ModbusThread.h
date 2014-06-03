#ifndef MODBUSTHREAD_H
#define MODBUSTHREAD_H

#include <QThread>

#include <cstdio>

#include <modbus.h>
#include <cstdint>

namespace NProtocol {

class ModbusThread : public QThread
{
    Q_OBJECT
public:
    static constexpr char *ADDRESS_() { return "127.0.0.1"; }
    static constexpr int PORT_() { return 1502; }
    static constexpr int REG_ADDRESS_() { return 0; }

    explicit ModbusThread(QObject *parent = 0);
    ~ModbusThread();

    void close();

protected:
    void run();

signals:

public slots:

private:
    modbus_t *_ctx;
};

}

#endif // MODBUSTHREAD_H

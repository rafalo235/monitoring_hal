#ifndef SENSOR_H
#define SENSOR_H

#include "valuegenerator.h"

#include <cstdio>
#include <modbus/modbus.h>
#include <modbus/modbus-tcp.h>
#include <QThread>

class Sensor : public QThread
{
    Q_OBJECT
public:
    Sensor(int, ValueGenerator *);
    void close();
    void run();
private:
    void reportError();

    int _socket;
    modbus_t *_ctx;
    modbus_mapping_t *_mb_mapping;
    ValueGenerator *_generator;

    static const int DEFAULT_PORT = 1024;
    static const int REG_ADDRESS = 0;
};

#endif // SENSOR_H

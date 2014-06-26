#ifndef SENSOR_H
#define SENSOR_H

#include <modbus.h>
#include "valuegenerator.h"

class Sensor
{
public:
    Sensor(int, ValueGenerator *);
    void listen();
    void loop();
    void close();
private:
    int _socket;
    modbus_t *_ctx;
    modbus_mapping_t *_mb_mapping;
    ValueGenerator *_generator;

    static const int DEFAULT_PORT = 1024;
    static const int REG_ADDRESS = 0;
};

#endif // SENSOR_H

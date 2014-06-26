#ifndef VALUEGENERATOR_H
#define VALUEGENERATOR_H

#include <cstdint>
#include <random>

typedef std::mt19937 MyRNG;

class ValueGenerator
{
public:
    ValueGenerator();
    void initialize();
    uint16_t getValue();
    void close();
private:
    uint16_t *_values;
    int _amount = 256;
    float _meanValue = 10;
    float _deviation = 10;

    int _pointer = 0;
};

#endif // VALUEGENERATOR_H

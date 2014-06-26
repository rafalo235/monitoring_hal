#include "valuegenerator.h"

ValueGenerator::ValueGenerator() {
}

void ValueGenerator::initialize() {
    MyRNG rng;
    rng.seed(time(NULL));

    std::normal_distribution<float> normal_dist(_meanValue, _deviation);

    _values = (uint16_t*) malloc(sizeof(uint16_t) * _amount);
    for (int i = 0 ; i < _amount ; i++) {
        _values[i] = (uint16_t) normal_dist(rng);
    }
}

uint16_t ValueGenerator::getValue() {
    uint16_t v = _values[_pointer++];
    _pointer %= _amount;
    return v;
}

void ValueGenerator::close() {
    free(_values);
}

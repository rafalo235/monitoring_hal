#include "sensor.h"

Sensor::Sensor(int port, ValueGenerator *generator) {
    /* Utworzenie kontekstu przypisanego do localhosta i podanego portu */
    if ( (_ctx = modbus_new_tcp("127.0.0.1", port)) == NULL ) {
        reportError();
        return;
    }
    modbus_set_debug(_ctx, TRUE); // wyświetlanie informacji w konsoli

    /* Zarezerwowanie pamięci na jeden rejestr */
    if ( (_mb_mapping = modbus_mapping_new(0, 0, 1, 0)) == NULL ) {
        modbus_free(_ctx);
        reportError();
        return;
    }
    this->_generator = generator;
}

void Sensor::run() {
    /* Rozpoczęcie nasłuchiwania */
    if ( (_socket = modbus_tcp_listen(_ctx, 1)) == -1) {
        reportError();
        return;
    }

    /* Pętla odbierająca żądania */
    while(modbus_tcp_accept(_ctx, &_socket) >= 0) {

        while(TRUE) {
            uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
            int rc;

            rc = modbus_receive(_ctx, query);
            if (rc != -1) {
                /* Ustaw najświeższą wartość pobraną z generatora */
                _mb_mapping->tab_registers[REG_ADDRESS] = _generator->getValue();
                /* rc is the query size */
                modbus_reply(_ctx, query, rc, _mb_mapping);
            } else {
                /* Connection closed by the client or error */
                break;
            }
        }
    }
}

void Sensor::close() {
    modbus_mapping_free(_mb_mapping);
    modbus_close(_ctx);
    modbus_free(_ctx);
}

void Sensor::reportError() {
    fprintf(stderr, "Error %d: %s", errno, modbus_strerror(errno));
}

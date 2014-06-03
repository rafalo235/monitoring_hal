#include "ModbusThread.h"

namespace NProtocol {

ModbusThread::ModbusThread(QObject *parent) :
    QThread(parent) {

    if ( (_ctx = modbus_new_tcp(ModbusThread::ADDRESS_(), ModbusThread::PORT_())) == NULL ) {
        fprintf(stderr, "Could not create context: %s\n",
            modbus_strerror(errno));
        return;
    }
    modbus_set_debug(_ctx, FALSE);

    if (modbus_connect(_ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n",
            modbus_strerror(errno));
        modbus_free(_ctx);
        return;
    }
}

ModbusThread::~ModbusThread() {
    close();
}

void ModbusThread::close() {
    printf("Closing Modbus connection context...\n");
    modbus_close(_ctx);
    modbus_free(_ctx);
}

void ModbusThread::run() {
    uint16_t recv_value;
    int rc;

    while (TRUE) {
        if ( (rc = modbus_read_registers(_ctx, ModbusThread::REG_ADDRESS_(), 1, &recv_value)) == -1 ) {
            printf("ERROR modbus_read_registers single (%d)\n", rc);
        }
        printf("Modbus register read: 0x%X\n", recv_value);
        sleep(1);
    }

}

}

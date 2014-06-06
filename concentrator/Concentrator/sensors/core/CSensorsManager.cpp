#include "CSensorsManager.h"
#include "configuration/interfaces/IConfiguration.h"
#include <modbus/modbus.h>
#include <cstdio>

namespace NSensors {

bool CSensorsManager::getSensorData(const NEngine::DSensorConfiguration& configuration, CData& value) const {
    modbus_t *ctx;
    uint16_t recv_value;
    int rc;
    CData ret_val;

    if ( (ctx = modbus_new_tcp(
              configuration->getIpv4Address().c_str(),
              configuration->getPort() )) == NULL ) {

        fprintf(stderr, "Could not create context: %s\n",
            modbus_strerror(errno));
        return false;

    }
    modbus_set_debug(ctx, FALSE);

    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connection failed: %s\n",
            modbus_strerror(errno));
        modbus_free(ctx);
        return false;
    }

    if ( (rc = modbus_read_registers(ctx, 0, 1, &recv_value)) == -1 ) {
        printf("ERROR modbus_read_registers single (%d)\n", rc);
    }
    printf("Modbus register read: 0x%X\n", recv_value);

    value.setValue(EValueType::UINT_16, &recv_value);

    printf("Closing Modbus connection context...\n");
    modbus_close(ctx);
    modbus_free(ctx);

    return true;
}

}


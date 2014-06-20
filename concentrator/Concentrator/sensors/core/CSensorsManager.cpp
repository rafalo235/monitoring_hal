#include "CSensorsManager.h"
#include "configuration/interfaces/IConfiguration.h"
#include <modbus/modbus.h>
#include <modbus/modbus-tcp.h>
#include <cstdio>
#include "util/Logger.h"
namespace NSensors {

  bool CSensorsManager::getSensorData(const NEngine::DSensorConfiguration& configuration, CData& value) const {
    modbus_t *ctx;
    uint16_t recv_value;
    int rc;

    if ( (ctx = modbus_new_tcp(
            configuration->getIpv4Address().c_str(),
            configuration->getPort() )) == NULL ) {

      LOG_ERROR("Could not create context: %s\n",
                modbus_strerror(errno));
      return false;

    }
    modbus_set_debug(ctx, FALSE);

    if (modbus_connect(ctx) == -1) {
      LOG_ERROR("Connection failed: %s\n",
                modbus_strerror(errno));
      modbus_free(ctx);
      return false;
    }

    if ( (rc = modbus_read_registers(ctx, 0, 1, &recv_value)) == -1 ) {
      LOG_ERROR("ERROR modbus_read_registers single (%d)\n", rc);
    }
    LOG_DEBUG("Modbus register read: 0x%X\n", recv_value);

    value.setValue(EValueType::UINT_16, &recv_value);

    LOG_DEBUG("Closing Modbus connection context...\n");
    modbus_close(ctx);
    modbus_free(ctx);

    return true;
  }

}


#include <QCoreApplication>

#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <random>
#include <time.h>

#include <unistd.h>
#include <modbus.h>
#include <errno.h>
#include <cstdint>

#include <signal.h>

#define PORT 1502

#define REG_ADDRESS 0
#define REG_VALS_AMOUNT 256
#define MEAN_REG_VAL 10
#define MEAN_REG_VAL_DEVIATION 10

typedef std::mt19937 MyRNG;

int socket;
modbus_t *ctx;
modbus_mapping_t *mb_mapping;
uint16_t *register_values;

void on_sigint(int);
void initialize_register_values();

int main(int argc, char *argv[])
{
    int pointer = 0;
    QCoreApplication a(argc, argv);

    signal(SIGINT, on_sigint);
    initialize_register_values();

    if ( (ctx = modbus_new_tcp("127.0.0.1", PORT)) == NULL ) {
        fprintf(stderr, "Could not create context: %s\n",
            modbus_strerror(errno));
        return -1;
    }
    modbus_set_debug(ctx, TRUE);

    if ( (mb_mapping = modbus_mapping_new(0, 0, 1, 0)) == NULL ) {
        fprintf(stderr, "Failed to allocate the mapping: %s\n",
            modbus_strerror(errno));
        modbus_free(ctx);
        return -2;
    }

    if ( (socket = modbus_tcp_listen(ctx, 1)) == -1) {
        fprintf(stderr, "Failed to open socket: %s\n",
            modbus_strerror(errno));
        modbus_mapping_free(mb_mapping);
        modbus_free(ctx);
        return -3;
    }
    while(modbus_tcp_accept(ctx, &socket) >= 0) {

        while(TRUE) {
            uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
            int rc;

            rc = modbus_receive(ctx, query);
            if (rc != -1) {
                /* rc is the query size */
                modbus_reply(ctx, query, rc, mb_mapping);

                /* Change register value */
                mb_mapping->tab_registers[REG_ADDRESS] = register_values[pointer];
                pointer++;
                pointer %= REG_VALS_AMOUNT;
            } else {
                /* Connection closed by the client or error */
                break;
            }
        }
    }

    printf("Quit the loop: %s\n", modbus_strerror(errno));

    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);

    free(register_values);

    return a.exec();
}

void on_sigint(int sig_num) {
    free(register_values);
    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);
}

void initialize_register_values() {
    MyRNG rng;
    rng.seed(time(NULL));

    std::normal_distribution<float> normal_dist(MEAN_REG_VAL, MEAN_REG_VAL_DEVIATION);

    register_values = (uint16_t*) malloc(sizeof(uint16_t) * REG_VALS_AMOUNT);
    for (int i = 0 ; i < REG_VALS_AMOUNT ; i++) {
        register_values[i] = (uint16_t) normal_dist(rng);
        //register_values[i] = MEAN_REG_VAL + (MEAN_REG_VAL_DEVIATION * (rand() / RAND_MAX) - (MEAN_REG_VAL_DEVIATION / 2));
    }
}

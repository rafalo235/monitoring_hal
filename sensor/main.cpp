#include <QCoreApplication>

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <modbus.h>
#include <errno.h>

#include <signal.h>

#define PORT 1502

int socket;
modbus_t *ctx;
modbus_mapping_t *mb_mapping;

void on_sigint(int);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    signal(SIGINT, on_sigint);

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

    return a.exec();
}

void on_sigint(int sig_num) {
    modbus_mapping_free(mb_mapping);
    modbus_close(ctx);
    modbus_free(ctx);
}

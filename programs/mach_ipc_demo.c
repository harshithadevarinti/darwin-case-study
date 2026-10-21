/* Mach IPC: allocate a port, give ourselves a send right, send a message
 * and receive it back. Demonstrates the port/right/message model.
 * Report section: Inter-Process Communication */
#include <stdio.h>
#include <string.h>
#include <mach/mach.h>

typedef struct {
    mach_msg_header_t header;
    char body[64];
} send_msg_t;

typedef struct {
    mach_msg_header_t header;
    char body[64];
    mach_msg_trailer_t trailer;
} recv_msg_t;

int main(void) {
    mach_port_t port;
    kern_return_t kr;

    kr = mach_port_allocate(mach_task_self(), MACH_PORT_RIGHT_RECEIVE, &port);
    if (kr != KERN_SUCCESS) { fprintf(stderr, "allocate: %s\n", mach_error_string(kr)); return 1; }
    printf("Allocated receive right, port name = %u\n", port);

    kr = mach_port_insert_right(mach_task_self(), port, port, MACH_MSG_TYPE_MAKE_SEND);
    if (kr != KERN_SUCCESS) { fprintf(stderr, "insert_right: %s\n", mach_error_string(kr)); return 1; }
    printf("Added send right to the same port\n");

    send_msg_t out;
    memset(&out, 0, sizeof(out));
    out.header.msgh_bits = MACH_MSGH_BITS(MACH_MSG_TYPE_COPY_SEND, 0);
    out.header.msgh_size = sizeof(out);
    out.header.msgh_remote_port = port;
    out.header.msgh_local_port = MACH_PORT_NULL;
    out.header.msgh_id = 1234;
    strncpy(out.body, "hello from Mach IPC", sizeof(out.body) - 1);

    kr = mach_msg(&out.header, MACH_SEND_MSG, sizeof(out), 0,
                  MACH_PORT_NULL, MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != MACH_MSG_SUCCESS) { fprintf(stderr, "send: %s\n", mach_error_string(kr)); return 1; }
    printf("Sent message id=%d\n", out.header.msgh_id);

    recv_msg_t in;
    memset(&in, 0, sizeof(in));
    kr = mach_msg(&in.header, MACH_RCV_MSG, 0, sizeof(in), port,
                  MACH_MSG_TIMEOUT_NONE, MACH_PORT_NULL);
    if (kr != MACH_MSG_SUCCESS) { fprintf(stderr, "recv: %s\n", mach_error_string(kr)); return 1; }
    printf("Received message id=%d body=\"%s\"\n", in.header.msgh_id, in.body);

    mach_port_deallocate(mach_task_self(), port);
    mach_port_mod_refs(mach_task_self(), port, MACH_PORT_RIGHT_RECEIVE, -1);
    return 0;
}

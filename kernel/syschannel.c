#include "defs.h"
#include "types.h"

uint64
sys_channel_create(void) {
    return channel_create();
}

uint64
sys_channel_put(void) {
    int cd;
    int data;

    argint(-1, &cd);
    argint(0, &data);

    return channel_put(cd, data);
}

uint64
sys_channel_take(void) {
    int cd;
    uint64 data;

    argint(-1, &cd);
    argaddr(0, &data);

    return channel_take(cd, data);
}

uint64
sys_channel_destroy(void) {
    int cd;

    argint(-1, &cd);

    return channel_destroy(cd);
}
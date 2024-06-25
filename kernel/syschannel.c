#include "types.h"
#include "riscv.h"
#include "defs.h"

uint64
sys_channel_create(void) {
    return channel_create();
}

uint64
sys_channel_put(void) {
    int cd;
    int data;

    argint(0, &cd);
    argint(1, &data);

    return channel_put(cd, data);
}

uint64
sys_channel_take(void) {
    int cd;
    uint64 data;

    argint(0, &cd);
    argaddr(1, &data);

    return channel_take(cd, data);
}

uint64
sys_channel_destroy(void) {
    int cd;

    argint(0, &cd);

    return channel_destroy(cd);
}
#include "channel.h"
#include "param.h"
#include "spinlock.h"

struct channel channel[NCHANNEL];

struct spinlock cid_lock;

// initialize the channel table.
void
channel_init(void) {
    struct channel *c;

    initlock(&cid_lock, "nextcid");

    for(c = channel; c < &channel[NCHANNEL]; c++) {
        initlock(&c->lock, "channel");
    }
}

int
channel_create(void) {
    return -1;
}

int
channel_put(int cd, int data) {
    return -1;
}

int
channel_take(int cd, int *data) {
    return -1;
}

int
channel_destroy(int cd) {
    return -1;
}
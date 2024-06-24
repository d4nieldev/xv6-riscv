#include "channel.h"
#include "param.h"
#include "spinlock.h"

struct channel channel[NCHANNEL];

struct spinlock cd_lock;

// initialize the channel table.
void
channel_init(void) {
    struct channel *c;

    initlock(&cd_lock, "nextcd");

    for(c = channel; c < &channel[NCHANNEL]; c++) {
        initlock(&c->lock, "channel");
    }
}

// create a new channel. 
// If possible, a channel descriptor is returned. Otherwise, -1.
int
channel_create(void) {
    acquire(&cd_lock);
    for (int cd = 0; cd < NCHANNEL; cd++) {
        if (!channel[cd].valid) {
            return cd;
        }
    }
    release(&cd_lock);

    return -1;  // all channels are occupied
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
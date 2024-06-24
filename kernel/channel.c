#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "channel.h"
#include "defs.h"
#include "proc.h"

struct channel channel[NCHANNEL];

struct spinlock cd_lock;

// initialize the channel table.
void
channel_init(void) {
    struct channel *c;

    initlock(&cd_lock, "nextcd");

    for(c = channel; c < &channel[NCHANNEL]; c++) {
        initlock(&c->lock, "channel");
        c->empty = 1;
        c->full = 0;
    }
}

// create a new channel. 
// If possible, a channel descriptor is returned. Otherwise, -1.
int
channel_create(void) {
    acquire(&cd_lock);
    for (int cd = 0; cd < NCHANNEL; cd++) {
        if (!channel[cd].occupied) {
            channel[cd].occupied = 1;
            release(&cd_lock);
            return cd;
        }
    }
    release(&cd_lock);

    return -1;  // all channels are occupied
}

int
channel_put(int cd, int data) {
    if (cd < 0 || cd >= NCHANNEL)
        return -1;  // invalid channel
    
    struct channel *c = &channel[cd];

    // must aquire &c->lock in case 2 processes are trying
    // to put at the same time to the same channel.

    acquire(&c->lock);
    if (!channel[cd].occupied) {
        release(&c->lock);
        return -1;
    }

    if (c->full) {
        // channel is full, sleep on the full channel until wakeup
        sleep(&c->full, &c->lock);
    }
    if (!channel[cd].occupied) {
        release(&c->lock);
        return -1;
    }
    c->data = data;
    c->empty = 0;
    c->full = 1;
    wakeup(&c->empty);
    release(&c->lock);

    return 0;
}

int
channel_take(int cd, uint64 data) {
    if (cd < 0 || cd >= NCHANNEL)
        return -1;  // invalid channel
    
    struct channel *c = &channel[cd];
    struct proc *p = myproc();

    // must aquire &c->lock in case 2 processes are trying
    // to take at the same time to the same channel.

    acquire(&c->lock);
    if (!channel[cd].occupied) {
        release(&c->lock);
        return -1;
    }
    if (c->empty) {
        // channel is empty, sleep on the empty channel until wakeup
        sleep(&c->empty, &c->lock);
    }
    if (!channel[cd].occupied) {
        release(&c->lock);
        return -1;
    }
    copyout(p->pagetable, data, (char*) &c->data, sizeof(c->data));  // *data = c->data;
    c->empty = 1;
    c->full = 0;
    wakeup(&c->full);
    release(&c->lock);

    return 0;
}

int
channel_destroy(int cd) {
    if (cd < 0 || cd >= NCHANNEL)
        return -1;  // invalid channel

    struct channel *c = &channel[cd];
    
    acquire(&c->lock);
    if (!channel[cd].occupied) {
        release(&c->lock);
        return -1;
    }
    c->occupied = 0;
    wakeup(&c->empty);
    wakeup(&c->full);
    release(&c->lock);

    return 0;
}

void
destroy_proc_channels(struct proc * p) {
    for (int cd = 0; cd < NCHANNEL; cd++) {
        struct channel *c = &channel[cd];
        if (c->parent == p)
            channel_destroy(cd);
    }
}
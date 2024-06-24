#include "channel.h"
#include "param.h"

struct channel channel[NCHANNEL];

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
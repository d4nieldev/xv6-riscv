#include "kernel/types.h"
#include "user.h"

int main() {
    int cd = channel_create();

    if (cd < 0) {
        printf("Failed to create channel\n");
        exit(1);
    }
    if (fork() == 0) {
        if (channel_put(cd, 42) < 0) {
            printf("Failed to put 42 in channel\n");
            exit(1);
        } else {
            printf("Put 42 in channel\n");
        }
        if (channel_put(cd, 43) < 0) {
            printf("Failed to put 43 in channel\n");
            exit(1);
        } else {
            printf("Put 43 in channel\n");
        }

        if (channel_destroy(cd) < 0) {
            printf("Failed to destroy channel\n");
            exit(1);
        } else {
            printf("Channel was destroyed successfully\n");
        }
    } else {
        int data;
        if (channel_take(cd, &data) < 0) {
            printf("Failed to take data from channel\n");
            exit(1);
        } else {
            printf("Took data=%d from channel\n", data); // 42
        }

        if (channel_take(cd, &data) < 0) { // 43
            printf("Failed to take data from channel\n");
            exit(1);
        } else {
            printf("Took data=%d from channel\n", data); // 43
        }
        
        if (channel_take(cd, &data) < 0) {
            // Sleep until child destroys channel
            printf("Failed to take data from channel (ok)\n");
        } else {
            printf("Took data from channel (error)\n");
            exit(1);
        }
    }
    exit(0);
}
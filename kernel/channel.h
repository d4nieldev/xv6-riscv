struct channel {
    struct spinlock lock;  // lock in case two processes try to access the channel

    struct proc *parent;   // Parent process

    int valid;             // if non-zero, channel contains data
    int data;              // the data
    int state;             // if non-zero, channel is open for communication

    // bonus
    // int refcount;         // the number of processes using this channel
};
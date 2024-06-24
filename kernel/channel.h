struct channel {
    struct spinlock lock;  // lock in case two processes try to access the channel

    struct proc *parent;   // Parent process

    int occupied;          // if non-zero, channel contains data
    int data;              // the data
    
    int full;              // if non-zero, channel is full
    int empty;             // if non-zero, channel is empty

    // bonus
    // int refcount;         // the number of processes using this channel
};
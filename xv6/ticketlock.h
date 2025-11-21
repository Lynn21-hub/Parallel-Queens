struct ticketlock {
    uint ticket ; //current ticket nb being served 
    uint turn; // next ticket nb ti be given 
    struct proc *proc ; //process currently holding the lock
};

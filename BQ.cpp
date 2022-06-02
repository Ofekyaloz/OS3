#include "BQ.h"

BQ::BQ(int max_size) {
    this->_max_size = max_size;
    this->_size = 0;
    semid = semget((__key_t) 0, 1, 0600);
    semarg.val = 1;
    semctl(semid, 0, SETVAL, semarg);
    sops->sem_num = 0;
    sops->sem_flg = 0;
}


int BQ::enqueue(const string &s) {

    // if the queue is full return -1
    if (this->_size == this->_max_size) {
        return -1;
    }

    // decrease the semaphore
    sops->sem_op = -1;
    semop(semid, sops, 1);

    this->queue.push_back(s);
    this->_size++;

    // increase the semaphore
    sops->sem_op = 1;
    semop(semid, sops, 1);

    return 1;
}

string BQ::dequeue() {

    // checks if there is an element in the queue
    if (this->_size > 0) {

        // decrease the semaphore
        sops->sem_op = -1;
        semop(semid, sops, 1);

        // critical section
        string tmp = this->queue.front();
        this->queue.erase(queue.begin());
        this->_size--;

        // increase the semaphore
        sops->sem_op = 1;
        semop(semid, sops, 1);

        return tmp;
    }
    return "";
}


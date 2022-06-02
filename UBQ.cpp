#include "UBQ.h"

UBQ::UBQ() {
    semid = semget((__key_t) 0, 1, 0600);
    semarg.val = 1;
    semctl(semid, 0, SETVAL, semarg);
    sops->sem_num = 0;
    sops->sem_flg = 0;
    this->_size = 0;
}

int UBQ::enqueue(const string& s) {
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

string UBQ::dequeue() {

    // checks if there is an element in the queue
    if (this->_size > 0) {

        // decrease the semaphore
        sops->sem_op = -1;
        semop(semid, sops, 1);

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


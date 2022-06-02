//
// Created by ofek on 5/30/22.
//

#include <iostream>
#include "UBQ.h"

UBQ::UBQ() {
    semid = semget((__key_t) 0, 1, 0600);
    semarg.val = 1;
    semctl(semid, 0, SETVAL, semarg);
    sops->sem_num = 0;
    sops->sem_flg = 0;
    this->_size = 0;
}

int UBQ::enqueue(string s) {
    sops->sem_op = -1;
    semop(semid, sops, 1);

    this->queue.push_back(s);
    this->_size++;

    sops->sem_op = 1;
    semop(semid, sops, 1);

    return 1;
}

string UBQ::dequeue() {
    if (this->_size > 0) {
        sops->sem_op = -1;
        semop(semid, sops, 1);

        string tmp = this->queue.front();
        this->queue.erase(queue.begin());
        this->_size--;

        sops->sem_op = 1;
        semop(semid, sops, 1);

        return tmp;
    }
    return "";
}


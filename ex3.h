#ifndef UNTITLED_EX3_H
#define UNTITLED_EX3_H

#include <iostream>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <cctype>
#include <vector>
#include <sys/sem.h>
#include "mutex"

using namespace std;

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

class UBQ {
    vector<string> queue;
    int _size;
    int semid;
    union semun semarg;
    struct sembuf sops[1];

public:
    UBQ();

    int enqueue(const string &s);

    string dequeue();

    ~UBQ();

};

UBQ::UBQ() {
    semid = semget(IPC_PRIVATE, 1, 0600);
    semarg.val = 1;
    semctl(semid, 0, SETVAL, semarg);
    sops->sem_num = 0;
    sops->sem_flg = 0;
    this->_size = 0;
}

int UBQ::enqueue(const string &s) {
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

    // decrease the semaphore
    sops->sem_op = -1;
    semop(semid, sops, 1);

    string tmp;

    // checks if there is an element in the queue
    if (this->_size > 0) {


        tmp = this->queue.front();
        this->queue.erase(queue.begin());
        this->_size--;

    }
    // increase the semaphore
    sops->sem_op = 1;
    semop(semid, sops, 1);
    return tmp;
}

UBQ::~UBQ() {
    int err = semctl(semid, 0, IPC_RMID, semarg);
    if (err == -1) {
        perror("delete semaphore error");
    }
}

class BQ {
    vector<string> queue;
    int _size;
    int _max_size;
    int semid;
    union semun semarg;
    struct sembuf sops[1];

public:
    BQ(int max_size);

    int enqueue(const string &s);

    string dequeue();

    ~BQ();

};

BQ::BQ(int max_size) {
    this->_max_size = max_size;
    this->_size = 0;
    this->semid = semget(IPC_PRIVATE, 1, 0600);
    this->semarg.val = 1;
    semctl(this->semid, 0, SETVAL, this->semarg);
    this->sops->sem_num = 0;
    this->sops->sem_flg = 0;
}


int BQ::enqueue(const string &s) {

    // decrease the semaphore
    this->sops->sem_op = -1;
    semop(this->semid, this->sops, 1);

    // if the queue is full return -1
    if (this->_size == this->_max_size) {

        // increase the semaphore
        this->sops->sem_op = 1;
        semop(this->semid, this->sops, 1);

        return -1;
    }

    this->queue.push_back(s);
    this->_size++;

    // increase the semaphore
    this->sops->sem_op = 1;
    semop(this->semid, this->sops, 1);

    return 1;
}

string BQ::dequeue() {

    // decrease the semaphore
    this->sops->sem_op = -1;
    semop(this->semid, this->sops, 1);

    string tmp;

    // checks if there is an element in the queue
    if (this->_size > 0) {


        tmp = this->queue.front();
        this->queue.erase(queue.begin());
        this->_size--;

    }
    // increase the semaphore
    this->sops->sem_op = 1;
    semop(this->semid, this->sops, 1);
    return tmp;
}

BQ::~BQ() {
    int err = semctl(semid, 0, IPC_RMID, semarg);
    if (err == -1) {
        perror("delete semaphore error");
    }

}

class Producer {
    int _id;
    int _products_number;
    BQ *_bq;

public:
    Producer(int id, int products_num, int buf_size);

    int getId() const;

    int getProducts() const;

    BQ *getBQ();

};

Producer::Producer(int id, int products_num, int buf_size) {
    this->_id = id;
    this->_products_number = products_num;
    this->_bq = new BQ(buf_size);
}

int Producer::getId() const {
    return this->_id;
}

int Producer::getProducts() const {
    return this->_products_number;
}

BQ *Producer::getBQ() {
    return this->_bq;
}

vector<BQ *> queues;
BQ *editorsBQ;
UBQ *sportsUBQ = new UBQ();
UBQ *newsUBQ = new UBQ();
UBQ *weatherUBQ = new UBQ();

#endif //UNTITLED_EX3_H

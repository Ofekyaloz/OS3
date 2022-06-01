//
// Created by ofek on 5/30/22.
//

#include "UBQ.h"

UBQ::UBQ(int max_size) {
    this->_max_size = max_size;
}

int UBQ::enqueue(string s) {
    if(this->_size == this->_max_size) {
        return -1;
    }

    this->queue.push_back(s);
    return 1;

}

string UBQ::dequeue() {
    if (this->_size > 0) {
        string tmp = this->queue.front();
         this->queue.erase(queue.begin());
         this->_size--;
         return tmp;
    }
    return nullptr;
}

int UBQ::getSize() const {
    return this->_size;
}

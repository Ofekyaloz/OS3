//
// Created by ofek on 5/31/22.
//

#include "Dispatcher.h"

Dispatcher::Dispatcher(int num) {
    this->q_num = num;
}

void Dispatcher::run() {
    for (int i = 0; i < this->q_num; ++i) {
        BQ* bq = queues[i];
        if (bq == nullptr) {
            this->q_num--;
            continue;
        }
        string msg = bq->dequeue();
        if (msg.find("SPORTS") != string::npos) {
            sportsUBQ->enqueue(msg);
        } else if (msg.find("NEWS") != string::npos) {
            newsUBQ->enqueue(msg);
        } else { // WEATHER
            weatherUBQ->enqueue(msg);
        }

    }
}
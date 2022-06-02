#include "ex3.h"

void *produce(void *arg) {
    Producer p = *((Producer *) arg);
    string categories[3] = {"SPORTS", "NEWS", "WEATHER"};
    int productNum = p.getProducts(), id = p.getId();
    BQ *bq = p.getBQ();
    for (int i = 0; i < productNum; ++i) {
        string msg = to_string(id) + " " + categories[i % 3] + " " + to_string(i);

        // trying to push the new msg. if failed go to sleep.
        while (bq->enqueue(msg) < 0)
            usleep(10000);
        usleep(1000);
    }
    while (bq->enqueue("DONE") < 0)
        usleep(1000);

    return nullptr;
}

void *dispatcher(void *arg) {
    while (!queues.empty()) {

        // copy the queues
        vector<BQ *> queues_copy;
        for (BQ *q: queues)
            queues_copy.push_back(q);

        for (BQ *bq: queues_copy) {

            // pop a message and push it to the fit queue.
            string msg = bq->dequeue();
            if (msg.find("SPORTS") != string::npos) {
                sportsUBQ->enqueue(msg);
            } else if (msg.find("NEWS") != string::npos) {
                newsUBQ->enqueue(msg);
            } else if (msg.find("WEATHER") != string::npos) {
                weatherUBQ->enqueue(msg);
            } else if (msg.find("DONE") != string::npos) {

                // finds the queue in the queues vector and deletes it.
                int index = 0;
                for (BQ *q: queues) {
                    if (q == bq)
                        break;
                    index++;
                }
                queues.erase(queues.begin() + index);
            }
            usleep(20000);
        }
        usleep(10000);
    }

    // push DONE to the editors queue.
    string msg = "DONE";
    sportsUBQ->enqueue(msg);
    newsUBQ->enqueue(msg);
    weatherUBQ->enqueue(msg);
    return nullptr;
}

void *co_editor(void *arg) {

    // get the right queue - sports/news/weather.
    UBQ *ubq = ((UBQ *) arg);
    while (true) {
        usleep(10000);
        string msg = ubq->dequeue();
        if (msg.empty())
            continue;

        // if gets DONE stop reading
        if (msg.find("DONE") != string::npos)
            break;

        usleep(10000); // edits...
        // push to the screen manager queue the msg
        while (editorsBQ->enqueue(msg) < 0)
            usleep(20000);
    }
    // push DONE to the screen manager queue.
    editorsBQ->enqueue("DONE");
    return nullptr;
}

void *screen_manager(void *arg) {
    int done = 3;
    // as long as there is a queue-editor, prints the msg
    while (done) {
        string msg = editorsBQ->dequeue();
        if (msg.find("DONE") != string::npos) {
            done--;
        } else if (!msg.empty()) {
            cout << "screen manager: " << msg << endl;
        }
        usleep(10000);
    }
    return nullptr;
}

int main(int argc, char *argv[]) {
    int numProducers = 0, producerID, productsNum, qSize;
    size_t len1;
    vector<Producer *> Producers;
    if (argc < 2) {
        perror("Not enough parameters\n");
        return -1;
    }

    char *line;
    FILE *file = fopen(argv[1], "r");

    // reading the configuration
    while (getline(&line, &len1, file) != -1) {
        producerID = stoi(line);

        if (getline(&line, &len1, file) != -1) {
            productsNum = stoi(line);
        } else {
            editorsBQ = new BQ(producerID);
            break;
        }
        if (getline(&line, &len1, file) != -1) {
            qSize = stoi(line);
        }
        auto *p = new Producer(producerID, productsNum, qSize);
        Producers.push_back(p);
        numProducers++;
        queues.push_back(p->getBQ());
        if (getline(&line, &len1, file) != -1) {

        }
    }

    int err;

    // creates the producers
    pthread_t producers_t[numProducers];
    for (int j = 0; j < numProducers; ++j) {
        err = pthread_create(&producers_t[j], nullptr, produce, (void *) Producers[j]);
        if (err != 0) {
            perror("pthread create failed");
        }

    }

    // creates the dispatcher
    pthread_t dispatcher_t;
    err = pthread_create(&dispatcher_t, nullptr, dispatcher, nullptr);
    if (err != 0) {
        perror("pthread create failed");
    }

    // creates the editors
    pthread_t editors[3];
    for (int j = 0; j < 3; ++j) {
        UBQ *ubq;
        if (j == 0) {
            ubq = sportsUBQ;
        } else if (j == 1) {
            ubq = newsUBQ;
        } else {
            ubq = weatherUBQ;
        }
        err = pthread_create(&editors[j], nullptr, co_editor, (void *) ubq);
        if (err != 0) {
            perror("pthread create failed");
        }
    }

    // creates the screen manager
    pthread_t manager;
    err = pthread_create(&manager, nullptr, screen_manager, nullptr);
    if (err != 0) {
        perror("pthread create failed");
    }

    void *ret;
    pthread_join(manager, &ret);
}

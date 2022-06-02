#include "ex3.h"

void *produce(void *arg) {
    Producer p = *((Producer *) arg);
    string categories[3] = {"SPORTS", "NEWS", "WEATHER"};
    int productNum = p.getProducts(), id = p.getId();
    BQ *bq = p.getBQ();
    for (int i = 0; i < productNum; ++i) {
        string msg = to_string(id) + " " + categories[i % 3] + " " + to_string(i);
        while (bq->enqueue(msg) < 0) {
            usleep(10000);
        }
        usleep(10000);
    }
    while (bq->enqueue("DONE") < 0) {
        usleep(10000);
    }

    return nullptr;
}

void *dispatcher(void *arg) {
    while (!queues.empty()) {
        vector<BQ *> queues_copy;
        for (BQ *q: queues) {
            queues_copy.push_back(q);
        }
        usleep(10000);
        for (BQ *bq: queues_copy) {
            usleep(20000);
            string msg = bq->dequeue();

            if (msg.find("SPORTS") != string::npos) {
                sportsUBQ->enqueue(msg);
            } else if (msg.find("NEWS") != string::npos) {
                newsUBQ->enqueue(msg);
            } else if (msg.find("WEATHER") != string::npos) {
                weatherUBQ->enqueue(msg);
            } else if (msg.find("DONE") != string::npos) {
                int index = 0;
                for (BQ *q: queues) {
                    if (q == bq)
                        break;
                    index++;
                }
                queues.erase(queues.begin() + index);
            }
        }
        usleep(10000);
    }
    string msg = "DONE";
    sportsUBQ->enqueue(msg);
    newsUBQ->enqueue(msg);
    weatherUBQ->enqueue(msg);

    return nullptr;
}

void *co_editor(void *arg) {
    UBQ* ubq = ((UBQ *) arg);
    while (true) {
        usleep(10000);
        string msg = ubq->dequeue();
        if (msg.find("DONE") != string::npos) {
            break;
        }
        if (msg.empty())
            continue;
        usleep(10000); // edits...
        while (editorsBQ->enqueue(msg) < 0) {
            usleep(20000);
        }
    }
    editorsBQ->enqueue("DONE");
    return nullptr;
}

void *screen_manager(void *arg) {
    int done = 3;
    while (done) {
        string msg = editorsBQ->dequeue();
        if (msg.find("DONE") != string::npos) {
            done--;
        } else if (!msg.empty()){
            cout << "screen manager: " << msg << endl;
        }
        usleep(20000);
    }
    return nullptr;
}

int main(int argc, char *argv[]) {
    int conf, coEditorBufSize = 0, numProducers = 0;
    vector<Producer *> Producers;
    char buf[SIZE];
    unsigned int charsRead;
    if (argc < 2) {
        perror("Not enough parameters\n");
        return -1;
    }
    if (access(argv[1], R_OK) == -1) {
        perror("Error in: access\n");
        return -1;
    }
    conf = open(argv[1], O_RDONLY);
    if (conf == -1) {
        perror("Error in: open\n");
        return -1;
    }
    charsRead = read(conf, buf, SIZE);
    if (charsRead == -1) {
        perror("Error in: read\n");
        return -1;
    }
    int canRead = 0;
    while (charsRead != 0) {
        string string1 = "PRODUCER ", string2 = "queue size = ";
        int producerID, productsNum, qSize;
        unsigned int i, len = string1.size(), same = 1;
        for (i = 0; i < len; ++i, ++canRead) {
            if (canRead == SIZE - 1) {
                canRead = 0;
                charsRead = read(conf, buf, SIZE);
                if (charsRead == -1) {
                    perror("Error in: read\n");
                    return -1;
                }
            }
            if (string1[i] != buf[canRead]) {
                same = 0;
                break;
            }
        }
        if (!same)
            break;

        for (int j = 0; j < 3; ++j) {
            char tmp[10];
            int k = 0;
            if (j == 2) {
                len = string2.size();
                for (int l = 0; l < len; ++l, ++canRead) {
                    if (canRead == SIZE - 1) {
                        canRead = 0;
                        charsRead = read(conf, buf, SIZE);
                        if (charsRead == -1) {
                            perror("Error in: read\n");
                            return -1;
                        }
                    }
                    if (string2[l] != buf[canRead]) {
                        same = 0;
                        break;
                    }
                }
                if (!same) {
                    // add error
                    break;
                }
            }
            while (isdigit(buf[canRead])) {
                tmp[k] = buf[canRead];
                k++;
                canRead++;
                if (canRead == SIZE - 1) {
                    canRead = 0;
                    charsRead = read(conf, buf, SIZE);
                    if (charsRead == -1) {
                        perror("Error in: read\n");
                        return -1;
                    }
                }
            }
            tmp[k] = '\0';
            if (!j)
                producerID = stoi(tmp);
            else if (j == 1)
                productsNum = stoi(tmp);
            else if (j == 2)
                qSize = stoi(tmp);
            canRead++;
        }

        // create producer
        auto *p = new Producer(producerID, productsNum, qSize);
        Producers.push_back(p);
        numProducers++;
        queues.push_back(p->getBQ());
        canRead++;
    }

    string string2 = "Co-Editor queue size = ";
    unsigned int i, len = string2.size(), same = 1;
    for (i = 0; i < len; ++i, ++canRead) {
        if (canRead == SIZE - 1) {
            canRead = 0;
            charsRead = read(conf, buf, SIZE);
            if (charsRead == -1) {
                perror("Error in: read\n");
                return -1;
            }
        }
        if (string2[i] != buf[canRead]) {
            same = 0;
            break;
        }
    }
    if (same) {
        char tmp[10];
        int k = 0;
        while (isdigit(buf[canRead])) {
            tmp[k] = buf[canRead];
            k++;
            canRead++;
            if (canRead == SIZE - 1) {
                canRead = 0;
                charsRead = read(conf, buf, SIZE);
                if (charsRead == -1) {
                    perror("Error in: read\n");
                    return -1;
                }
            }
        }
        tmp[k] = '\0';
        coEditorBufSize = stoi(tmp);
    }

    if (coEditorBufSize > 0) {
        editorsBQ = new BQ(coEditorBufSize);
    }


    int err;
    pthread_t dispatcher_t;
    pthread_t pthreads[numProducers];
    for (int j = 0; j < numProducers; ++j) {
        err = pthread_create(&pthreads[j], nullptr, produce, (void *) Producers[j]);
        if (err != 0) {
            perror("pthread create failed");
        }

    }

    err = pthread_create(&dispatcher_t, nullptr, dispatcher, nullptr);
    if (err != 0) {
        perror("pthread create failed");
    }

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

    pthread_t manager;
    err = pthread_create(&manager, nullptr, screen_manager, nullptr);
    if (err != 0) {
        perror("pthread create failed");
    }

    void* retval;
    pthread_join(manager, &retval);
}

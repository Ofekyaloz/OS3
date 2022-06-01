#include "ex3.h"

void *produce(void *arg) {
    Producer p = *((Producer*) arg);
    string categories[3] = {"SPORTS", "NEWS", "WEATHER"};
    int productNum = p.getProducts(), id = p.getId();
    BQ* bq = p.getBQ();
    printf("Producer id %d", id);
    for (int i = 0; i < productNum; ++i) {
        string msg = to_string(id) + categories[i % 3] + to_string(productNum);

        while (bq->enqueue(msg) < 0);

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
        Producers.push_back(new Producer(producerID, productsNum, qSize));
        numProducers++;
        queues.push_back(new BQ(qSize));
        canRead++;
    }

    string string2 = "Co-Editor queue size = ";
    unsigned int i, len = string2.size(), same = 1;
    for (
            i = 0;
            i < len;
            ++i, ++canRead) {
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

    pthread_t pthreads[numProducers];
    int err;
    for (int j = 0; j < numProducers; ++j) {
        err = pthread_create(&pthreads[j], nullptr, produce, (void *) Producers[j]);
        if (err != 0) {
            perror("pthread create failed");
        }
        
    }



}

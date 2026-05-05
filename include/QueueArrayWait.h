#ifndef QUEUEARRAYWAIT_H
#define QUEUEARRAYWAIT_H

#include <iostream>
using namespace std;

#include "WaitRequest.h"

class QueueArrayWait
{
private:
    WaitRequest *data;
    int capacity;
    int size;
    int front;
    int rear;

public:
    QueueArrayWait();
    ~QueueArrayWait();

    void init(int capacityValue);
    int enQueue(const WaitRequest &value);
    int deQueue(WaitRequest &outValue);
    int peek(WaitRequest &outValue) const;
    int isEmpty() const;
    int isFull() const;
    void print() const;
    void clear();

    int getSize() const;
    int getFrontIndex() const;
    int getRearIndex() const;
};

#endif

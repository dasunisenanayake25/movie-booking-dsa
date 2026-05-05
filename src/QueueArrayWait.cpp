#include <iostream>
#include "QueueArrayWait.h"

QueueArrayWait::QueueArrayWait()
{
    data = NULL;
    capacity = 0;
    size = 0;
    front = 0;
    rear = -1;
}

QueueArrayWait::~QueueArrayWait()
{
    clear();
}

void QueueArrayWait::init(int capacityValue)
{
    clear();

    capacity = capacityValue;
    if (capacity > 0)
    {
        data = new WaitRequest[capacity];
    }

    size = 0;
    front = 0;
    rear = -1;
}

int QueueArrayWait::enQueue(const WaitRequest &value)
{
    if (isFull())
    {
        return 0;
    }

    rear = (rear + 1) % capacity;
    data[rear] = value;
    size++;
    return 1;
}

int QueueArrayWait::deQueue(WaitRequest &outValue)
{
    if (isEmpty())
    {
        return 0;
    }

    outValue = data[front];
    front = (front + 1) % capacity;
    size--;

    if (size == 0)
    {
        front = 0;
        rear = -1;
    }

    return 1;
}

int QueueArrayWait::peek(WaitRequest &outValue) const
{
    if (isEmpty())
    {
        return 0;
    }

    outValue = data[front];
    return 1;
}

int QueueArrayWait::isEmpty() const
{
    return size == 0;
}

int QueueArrayWait::isFull() const
{
    return size == capacity;
}

void QueueArrayWait::print() const
{
    int i;
    int index;

    if (isEmpty())
    {
        cout << "(empty)" << endl;
        return;
    }

    for (i = 0; i < size; i++)
    {
        index = (front + i) % capacity;
        cout << data[index].name << " (" << data[index].seatsRequested << " seats)" << endl;
    }
}

void QueueArrayWait::clear()
{
    if (data != NULL)
    {
        delete[] data;
        data = NULL;
    }

    capacity = 0;
    size = 0;
    front = 0;
    rear = -1;
}

int QueueArrayWait::getSize() const
{
    return size;
}

int QueueArrayWait::getFrontIndex() const
{
    return front;
}

int QueueArrayWait::getRearIndex() const
{
    return rear;
}

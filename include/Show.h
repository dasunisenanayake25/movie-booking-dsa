#ifndef SHOW_H
#define SHOW_H

#include <string>
#include "Constants.h"
#include "SinglyLinkedListInt.h"
#include "QueueArrayWait.h"

struct Show
{
    int id;
    int movieId;
    std::string datetime;
    int price;
    int waitCount;
    SinglyLinkedListInt::Node *bookingIdListHead;
    QueueArrayWait *waitQueue;
};

#endif
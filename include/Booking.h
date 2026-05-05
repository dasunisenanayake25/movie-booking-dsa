#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include "Constants.h"

struct Booking
{
    int id;
    int showId;
    std::string customerName;
    int seatCount;
    unsigned char seatR[MAX_SEATS_IN_BOOKING];
    unsigned char seatC[MAX_SEATS_IN_BOOKING];
    int status;
};

#endif

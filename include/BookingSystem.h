#ifndef BOOKINGSYSTEM_H
#define BOOKINGSYSTEM_H

#include <string>
#include "Store.h"
#include "BSTMovieTitle.h"
#include "HashTableIntToIndex.h"
#include "MergeSortShows.h"
#include "QueueArrayWait.h"
#include "SeatingGrid.h"
#include "SinglyLinkedListInt.h"
#include "Seat.h"

class BookingSystem
{
private:
    Store store;
    BSTMovieTitle movieTitleIndex;
    HashTableIntToIndex showIdIndex;
    HashTableIntToIndex bookingIdIndex;
    SinglyLinkedListInt showBookingLists[MAX_SHOWS];
    QueueArrayWait showWaitQueues[MAX_SHOWS];
    SeatingGrid showGrids[MAX_SHOWS];

    void buildMovieIndex();
    void buildShowIndex();
    void buildBookingIndex();
    void syncShowPointers(int showIndex);

    int createBookingRecord(int showIndex, const std::string &name, Seat seats[], int seatCount);
    int allocateContiguousBlock(int showIndex, int seatsNeeded, Seat outSeats[]) const;
    int allocateNearestSeats(int showIndex, int seatsNeeded, Seat outSeats[]) const;
    int allocateGroupSeats(int showIndex, int seatsNeeded, Seat outSeats[]) const;
    void occupySeats(int showIndex, Seat seats[], int seatCount);
    void freeBookingSeats(int bookingIndex);
    void processWaitlist(int showIndex);

public:
    int recommendSeats(int showId, int seatsNeeded, Seat outSeats[]) const;
    BookingSystem();

    void listMovies() const;
    int searchMovie(const std::string &title) const;
    int listShowsForMovieSorted(int movieId, int outShowIds[]) const;
    void printSeatMap(int showId) const;
    int bookSingle(int showId, const std::string &name, int row, int col);
    int bookGroup(int showId, const std::string &name, int seatsNeeded);
    int bookGroupSeats(int showId, const std::string &name, Seat seats[], int seatCount);
    void viewBooking(int bookingId) const;
    int cancelBooking(int bookingId, const std::string &name);
    int joinWaitlist(int showId, const std::string &name, int seatsNeeded);

    int hasMovie(int movieId) const;
    int isShowForMovie(int movieId, int showId) const;
    int getMovieIds(int outIds[]) const;
    int getAvailableCount(int showId) const;
};

#endif

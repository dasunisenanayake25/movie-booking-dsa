#include <iostream>
#include <cstring>
#include <cmath>
#include <string>
using namespace std;

#include "BookingSystem.h"
#include "SampleData.h"

BookingSystem::BookingSystem()
{
    int i;

    SampleData::load(store);

    showIdIndex.init(HASH_BUCKETS);
    bookingIdIndex.init(HASH_BUCKETS);

    for (i = 0; i < MAX_SHOWS; i++)
    {
        showWaitQueues[i].init(MAX_WAIT);
    }

    buildMovieIndex();
    buildShowIndex();
    buildBookingIndex();

    for (i = 0; i < store.showCount; i++)
    {
        syncShowPointers(i);
    }

    // Initialize each SeatingGrid.
    for (i = 0; i < store.showCount; i++)
    {
        showGrids[i].init(ROWS, COLS);
    }

    // Rebuild seat state from authoritative booking records
    for (i = 0; i < store.bookingCount; i++)
    {
        if (store.bookings[i].status == STATUS_CONFIRMED)
        {
            int showIndex;
            if (showIdIndex.get(store.bookings[i].showId, showIndex))
            {
                for (int s = 0; s < store.bookings[i].seatCount; s++)
                {
                    int r = store.bookings[i].seatR[s];
                    int c = store.bookings[i].seatC[s];
                    showGrids[showIndex].selectSeat(r, c);
                }
                showGrids[showIndex].confirmBooking();
            }
        }
    }
}

void BookingSystem::listMovies() const
{
    int ids[MAX_MOVIES];
    int count;
    int i;
    int movieIndex;

    movieTitleIndex.inOrderList(ids, count);

    cout << "Movies:" << endl;
    for (i = 0; i < count; i++)
    {
        movieIndex = findMovieIndexById(store, ids[i]);
        if (movieIndex != -1)
        {
            cout << "ID: " << store.movies[movieIndex].id
                 << " | Title: " << store.movies[movieIndex].title
                 << " | Duration: " << store.movies[movieIndex].durationMin << " min" << endl;
        }
    }
}

int BookingSystem::searchMovie(const std::string &title) const
{
    int movieId = movieTitleIndex.searchExact(title);
    int movieIndex;

    if (movieId == -1)
    {
        cout << "Error: Movie title not found." << endl;
        return -1;
    }

    movieIndex = findMovieIndexById(store, movieId);
    cout << "Movie found: ID " << store.movies[movieIndex].id
         << " | " << store.movies[movieIndex].title
         << " | " << store.movies[movieIndex].durationMin << " min" << endl;
    return movieId;
}

int BookingSystem::listShowsForMovieSorted(int movieId, int outShowIds[]) const
{
    int showIndices[MAX_SHOWS];
    int count = 0;
    int i;

    for (i = 0; i < store.showCount; i++)
    {
        if (store.shows[i].movieId == movieId)
        {
            showIndices[count] = i;
            count++;
        }
    }

    if (count == 0)
    {
        cout << "Error: No shows found for that movie." << endl;
        return 0;
    }

    MergeSortShows::sortIndices(showIndices, count, store);

    cout << "Shows:" << endl;
    for (i = 0; i < count; i++)
    {
        outShowIds[i] = store.shows[showIndices[i]].id;
        cout << "Show ID: " << store.shows[showIndices[i]].id
             << " | Datetime: " << store.shows[showIndices[i]].datetime
             << " | Price: " << store.shows[showIndices[i]].price
             << " | Available Seats: " << showGrids[showIndices[i]].getAvailableCount()
             << " | Waitlist: " << store.shows[showIndices[i]].waitCount << endl;
    }

    return count;
}

void BookingSystem::printSeatMap(int showId) const
{
    int showIndex;

    if (!showIdIndex.get(showId, showIndex))
    {
        cout << "Error: Invalid show ID." << endl;
        return;
    }

    cout << "Seat Map for Show " << showId << " (" << store.shows[showIndex].datetime << ")" << endl;
    showGrids[showIndex].displayGrid();
}

int BookingSystem::bookSingle(int showId, const std::string &name, int row, int col)
{
    int showIndex;
    Seat seat;
    int bookingId;

    if (!showIdIndex.get(showId, showIndex))
    {
        cout << "Error: Invalid show ID." << endl;
        return 0;
    }

    if (row < 1 || row > ROWS || col < 1 || col > COLS)
    {
        cout << "Error: Seat position out of range." << endl;
        return 0;
    }

    if (!showGrids[showIndex].isAvailable(row - 1, col - 1))
    {
        cout << "Error: That seat is already booked." << endl;
        return 0;
    }

    seat.r = (unsigned char)(row - 1);
    seat.c = (unsigned char)(col - 1);
    if (store.bookingCount >= MAX_BOOKINGS)
    {
        cout << "Error: Booking storage is full." << endl;
        return 0;
    }

    bookingId = createBookingRecord(showIndex, name, &seat, 1);
    if (bookingId == -1)
    {
        return 0;
    }
    occupySeats(showIndex, &seat, 1);

    cout << "Booking Confirmed!" << endl;
    cout << "Booking ID: " << bookingId << endl;
    cout << "Seats: (" << row << "," << col << ")" << endl;
    cout << "Total price: " << store.shows[showIndex].price << endl;
    return 1;
}

int BookingSystem::bookGroup(int showId, const std::string &name, int seatsNeeded)
{
    int showIndex;
    Seat seats[MAX_SEATS_IN_BOOKING];
    int bookingId;
    int i;

    if (!showIdIndex.get(showId, showIndex))
    {
        cout << "Error: Invalid show ID." << endl;
        return 0;
    }

    if (seatsNeeded < 2 || seatsNeeded > MAX_SEATS_IN_BOOKING)
    {
        cout << "Error: Group seat count must be between 2 and " << MAX_SEATS_IN_BOOKING << "." << endl;
        return 0;
    }

    if (!allocateGroupSeats(showIndex, seatsNeeded, seats))
    {
        cout << "Error: Not enough seats available for this group." << endl;
        return 0;
    }

    if (store.bookingCount >= MAX_BOOKINGS)
    {
        cout << "Error: Booking storage is full." << endl;
        return 0;
    }

    bookingId = createBookingRecord(showIndex, name, seats, seatsNeeded);
    if (bookingId == -1)
    {
        return 0;
    }
    occupySeats(showIndex, seats, seatsNeeded);

    cout << "Booking Confirmed!" << endl;
    cout << "Booking ID: " << bookingId << endl;
    cout << "Seats: ";
    for (i = 0; i < seatsNeeded; i++)
    {
        cout << "(" << (int)seats[i].r + 1 << "," << (int)seats[i].c + 1 << ")";
        if (i < seatsNeeded - 1)
        {
            cout << ", ";
        }
    }
    cout << endl;
    cout << "Total price: " << store.shows[showIndex].price * seatsNeeded << endl;
    return 1;
}

int BookingSystem::bookGroupSeats(int showId, const std::string &name, Seat seats[], int seatCount)
{
    int showIndex;
    int bookingId;
    int i;

    if (!showIdIndex.get(showId, showIndex))
    {
        cout << "Error: Invalid show ID." << endl;
        return 0;
    }

    if (seatCount < 1 || seatCount > MAX_SEATS_IN_BOOKING)
    {
        cout << "Error: Invalid seat count." << endl;
        return 0;
    }

    for (i = 0; i < seatCount; i++)
    {
        if (!showGrids[showIndex].isAvailable(seats[i].r, seats[i].c))
        {
            cout << "Error: One or more recommended seats are no longer available." << endl;
            return 0;
        }
    }

    bookingId = createBookingRecord(showIndex, name, seats, seatCount);
    if (bookingId == -1)
    {
        return 0;
    }
    occupySeats(showIndex, seats, seatCount);

    cout << "Booking Confirmed!" << endl;
    cout << "Booking ID: " << bookingId << endl;
    cout << "Seats: ";
    for (i = 0; i < seatCount; i++)
    {
        cout << "(" << (int)seats[i].r + 1 << "," << (int)seats[i].c + 1 << ")";
        if (i < seatCount - 1)
        {
            cout << ", ";
        }
    }
    cout << endl;
    cout << "Total price: " << store.shows[showIndex].price * seatCount << endl;
    return 1;
}

void BookingSystem::viewBooking(int bookingId) const
{
    int bookingIndex;
    int showIndex;
    int movieIndex;
    int i;

    if (!bookingIdIndex.get(bookingId, bookingIndex))
    {
        cout << "Error: Booking ID not found." << endl;
        return;
    }

    if (!showIdIndex.get(store.bookings[bookingIndex].showId, showIndex))
    {
        cout << "Error: Show for booking not found." << endl;
        return;
    }

    movieIndex = findMovieIndexById(store, store.shows[showIndex].movieId);

    cout << "Booking ID: " << store.bookings[bookingIndex].id << endl;
    cout << "Customer: " << store.bookings[bookingIndex].customerName << endl;
    if (movieIndex != -1)
    {
        cout << "Movie: " << store.movies[movieIndex].title << endl;
    }
    cout << "Show ID: " << store.bookings[bookingIndex].showId << endl;
    cout << "Show Time: " << store.shows[showIndex].datetime << endl;
    cout << "Seat Count: " << store.bookings[bookingIndex].seatCount << endl;
    cout << "Status: ";
    if (store.bookings[bookingIndex].status == STATUS_CONFIRMED)
    {
        cout << "CONFIRMED" << endl;
    }
    else
    {
        cout << "CANCELLED" << endl;
    }

    cout << "Seats: ";
    for (i = 0; i < store.bookings[bookingIndex].seatCount; i++)
    {
        cout << "(" << (int)store.bookings[bookingIndex].seatR[i] + 1
             << "," << (int)store.bookings[bookingIndex].seatC[i] + 1 << ")";
        if (i < store.bookings[bookingIndex].seatCount - 1)
        {
            cout << ", ";
        }
    }
    cout << endl;
    cout << "Total price: " << store.shows[showIndex].price * store.bookings[bookingIndex].seatCount << endl;
}

int BookingSystem::cancelBooking(int bookingId, const std::string &name)
{
    int bookingIndex;
    int showIndex;

    if (!bookingIdIndex.get(bookingId, bookingIndex))
    {
        cout << "Error: Booking ID not found." << endl;
        return 0;
    }

    if (store.bookings[bookingIndex].status != STATUS_CONFIRMED)
    {
        cout << "Error: Booking is already cancelled." << endl;
        return 0;
    }

    if (store.bookings[bookingIndex].customerName != name)
    {
        cout << "Error: Customer name does not match booking record." << endl;
        return 0;
    }

    if (!showIdIndex.get(store.bookings[bookingIndex].showId, showIndex))
    {
        cout << "Error: Show record not found." << endl;
        return 0;
    }

    freeBookingSeats(bookingIndex);
    store.bookings[bookingIndex].status = STATUS_CANCELLED;
    showBookingLists[showIndex].removeValue(bookingId);
    syncShowPointers(showIndex);

    cout << "Booking cancelled successfully." << endl;
    processWaitlist(showIndex);
    return 1;
}

int BookingSystem::joinWaitlist(int showId, const std::string &name, int seatsNeeded)
{
    int showIndex;
    WaitRequest request;

    if (!showIdIndex.get(showId, showIndex))
    {
        cout << "Error: Invalid show ID." << endl;
        return 0;
    }

    if (seatsNeeded < 1 || seatsNeeded > MAX_SEATS_IN_BOOKING)
    {
        cout << "Error: Invalid number of seats for waitlist request." << endl;
        return 0;
    }

    request.name = name;
    request.seatsRequested = seatsNeeded;

    if (!showWaitQueues[showIndex].enQueue(request))
    {
        cout << "Error: Waitlist is full." << endl;
        return 0;
    }

    syncShowPointers(showIndex);
    cout << "Added to waitlist for show " << showId << "." << endl;
    return 1;
}

int BookingSystem::hasMovie(int movieId) const
{
    return findMovieIndexById(store, movieId) != -1;
}

int BookingSystem::isShowForMovie(int movieId, int showId) const
{
    int showIndex;
    if (!showIdIndex.get(showId, showIndex))
    {
        return 0;
    }
    return store.shows[showIndex].movieId == movieId;
}

int BookingSystem::getMovieIds(int outIds[]) const
{
    int count;
    movieTitleIndex.inOrderList(outIds, count);
    return count;
}

void BookingSystem::buildMovieIndex()
{
    int i;
    for (i = 0; i < store.movieCount; i++)
    {
        movieTitleIndex.insert(store.movies[i].id, store.movies[i].title);
    }
}

void BookingSystem::buildShowIndex()
{
    int i;
    for (i = 0; i < store.showCount; i++)
    {
        showIdIndex.put(store.shows[i].id, i);
    }
}

void BookingSystem::buildBookingIndex()
{
    int i;
    int showIndex;
    for (i = 0; i < store.bookingCount; i++)
    {
        bookingIdIndex.put(store.bookings[i].id, i);
        if (showIdIndex.get(store.bookings[i].showId, showIndex))
        {
            showBookingLists[showIndex].insertLast(store.bookings[i].id);
        }
    }
}

void BookingSystem::syncShowPointers(int showIndex)
{
    store.shows[showIndex].bookingIdListHead = showBookingLists[showIndex].getHead();

    if (showWaitQueues[showIndex].isEmpty())
    {
        store.shows[showIndex].waitQueue = NULL;
    }
    else
    {
        store.shows[showIndex].waitQueue = &showWaitQueues[showIndex];
    }

    store.shows[showIndex].waitCount = showWaitQueues[showIndex].getSize();
}

int BookingSystem::createBookingRecord(int showIndex, const std::string &name, Seat seats[], int seatCount)
{
    int bookingIndex;
    int i;
    int bookingId;

    if (store.bookingCount >= MAX_BOOKINGS)
    {
        cout << "Error: Booking storage is full." << endl;
        return -1;
    }

    bookingIndex = store.bookingCount;
    bookingId = store.nextBookingId;

    store.bookings[bookingIndex].id = bookingId;
    store.bookings[bookingIndex].showId = store.shows[showIndex].id;
    store.bookings[bookingIndex].customerName = name;
    store.bookings[bookingIndex].seatCount = seatCount;
    store.bookings[bookingIndex].status = STATUS_CONFIRMED;

    for (i = 0; i < seatCount; i++)
    {
        store.bookings[bookingIndex].seatR[i] = seats[i].r;
        store.bookings[bookingIndex].seatC[i] = seats[i].c;
    }

    store.bookingCount++;
    store.nextBookingId++;
    bookingIdIndex.put(bookingId, bookingIndex);
    showBookingLists[showIndex].insertLast(bookingId);
    syncShowPointers(showIndex);

    return bookingId;
}

int BookingSystem::allocateContiguousBlock(int showIndex, int seatsNeeded, Seat outSeats[]) const
{
    int row;
    int start;
    int offset;
    int found = 0;
    int bestRow = -1;
    int bestStart = -1;
    double bestScore = 1000000.0;
    double rowCenter = (ROWS - 1) / 2.0;
    double colCenter = (COLS - 1) / 2.0;
    double segmentCenter;
    double score;

    for (row = 0; row < ROWS; row++)
    {
        for (start = 0; start <= COLS - seatsNeeded; start++)
        {
            found = 1;
            for (offset = 0; offset < seatsNeeded; offset++)
            {
                if (!showGrids[showIndex].isAvailable(row, start + offset))
                {
                    found = 0;
                    break;
                }
            }

            if (found)
            {
                segmentCenter = start + (seatsNeeded - 1) / 2.0;
                score = (fabs((double)row - rowCenter) * 100.0) + fabs(segmentCenter - colCenter);

                if (bestRow == -1 || score < bestScore)
                {
                    bestRow = row;
                    bestStart = start;
                    bestScore = score;
                }
            }
        }
    }

    if (bestRow == -1)
    {
        return 0;
    }

    for (offset = 0; offset < seatsNeeded; offset++)
    {
        outSeats[offset].r = (unsigned char)bestRow;
        outSeats[offset].c = (unsigned char)(bestStart + offset);
    }

    return 1;
}

int BookingSystem::allocateNearestSeats(int showIndex, int seatsNeeded, Seat outSeats[]) const
{
    Seat freeSeats[ROWS * COLS];
    int distance[ROWS * COLS];
    int totalFree = 0;
    int r;
    int c;
    int i;
    int j;
    int best;
    int bestDist;
    int bestRowScore;
    int currentRowScore;

    for (r = 0; r < ROWS; r++)
    {
        for (c = 0; c < COLS; c++)
        {
            if (showGrids[showIndex].isAvailable(r, c))
            {
                freeSeats[totalFree].r = (unsigned char)r;
                freeSeats[totalFree].c = (unsigned char)c;
                distance[totalFree] = abs((2 * r) - (ROWS - 1)) + abs((2 * c) - (COLS - 1));
                totalFree++;
            }
        }
    }

    if (totalFree < seatsNeeded)
    {
        return 0;
    }

    for (i = 0; i < totalFree - 1; i++)
    {
        best = i;
        bestDist = distance[i];
        bestRowScore = abs((2 * freeSeats[i].r) - (ROWS - 1));

        for (j = i + 1; j < totalFree; j++)
        {
            currentRowScore = abs((2 * freeSeats[j].r) - (ROWS - 1));

            if (distance[j] < bestDist ||
                (distance[j] == bestDist && currentRowScore < bestRowScore) ||
                (distance[j] == bestDist && currentRowScore == bestRowScore && freeSeats[j].r < freeSeats[best].r) ||
                (distance[j] == bestDist && currentRowScore == bestRowScore && freeSeats[j].r == freeSeats[best].r && freeSeats[j].c < freeSeats[best].c))
            {
                best = j;
                bestDist = distance[j];
                bestRowScore = currentRowScore;
            }
        }

        if (best != i)
        {
            Seat tempSeat = freeSeats[i];
            int tempDist = distance[i];
            freeSeats[i] = freeSeats[best];
            distance[i] = distance[best];
            freeSeats[best] = tempSeat;
            distance[best] = tempDist;
        }
    }

    for (i = 0; i < seatsNeeded; i++)
    {
        outSeats[i] = freeSeats[i];
    }

    return 1;
}

int BookingSystem::allocateGroupSeats(int showIndex, int seatsNeeded, Seat outSeats[]) const
{
    if (showGrids[showIndex].getAvailableCount() < seatsNeeded)
    {
        return 0;
    }

    if (allocateContiguousBlock(showIndex, seatsNeeded, outSeats))
    {
        return 1;
    }

    return allocateNearestSeats(showIndex, seatsNeeded, outSeats);
}

void BookingSystem::occupySeats(int showIndex, Seat seats[], int seatCount)
{
    int i;
    for (i = 0; i < seatCount; i++)
    {
        showGrids[showIndex].selectSeat(seats[i].r, seats[i].c);
    }
    showGrids[showIndex].confirmBooking();
}

void BookingSystem::freeBookingSeats(int bookingIndex)
{
    int showIndex;
    int i;

    if (!showIdIndex.get(store.bookings[bookingIndex].showId, showIndex))
    {
        return;
    }

    for (i = 0; i < store.bookings[bookingIndex].seatCount; i++)
    {
        showGrids[showIndex].cancelSeat(
            store.bookings[bookingIndex].seatR[i],
            store.bookings[bookingIndex].seatC[i]);
    }
}

void BookingSystem::processWaitlist(int showIndex)
{
    WaitRequest request;
    WaitRequest removed;
    Seat seats[MAX_SEATS_IN_BOOKING];

    while (showWaitQueues[showIndex].peek(request))
    {
        if (!allocateGroupSeats(showIndex, request.seatsRequested, seats))
        {
            break;
        }

        int newBookingId = createBookingRecord(showIndex, request.name, seats, request.seatsRequested);
        if (newBookingId == -1)
        {
            break;
        }
        occupySeats(showIndex, seats, request.seatsRequested);
        showWaitQueues[showIndex].deQueue(removed);

        cout << "Waitlist auto-booked: " << request.name
             << " | Booking ID: " << newBookingId
             << " | Seats: " << request.seatsRequested << endl;
    }

    syncShowPointers(showIndex);
}
int BookingSystem::recommendSeats(int showId, int seatsNeeded, Seat outSeats[]) const
{
    int showIndex;
    if (!showIdIndex.get(showId, showIndex))
    {
        cout << "Error: Invalid show ID." << endl;
        return 0;
    }

    if (seatsNeeded < 1 || seatsNeeded > MAX_SEATS_IN_BOOKING)
    {
        cout << "Error: Invalid number of seats requested." << endl;
        return 0;
    }

    if (showGrids[showIndex].getAvailableCount() < seatsNeeded)
    {
        return 0;
    }

    return allocateGroupSeats(showIndex, seatsNeeded, outSeats);
}

int BookingSystem::getAvailableCount(int showId) const
{
    int showIndex;
    if (!showIdIndex.get(showId, showIndex))
    {
        return 0;
    }
    return showGrids[showIndex].getAvailableCount();
}

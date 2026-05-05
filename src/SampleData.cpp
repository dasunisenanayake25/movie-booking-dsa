#include "SampleData.h"
#include <string>

void SampleData::load(Store &store)
{
    const std::string times[3] = {
        "2026-03-03 10:00",
        "2026-03-03 14:00",
        "2026-03-03 18:00"};

    const std::string titles[MAX_MOVIES] = {
        "Inception",
        "Interstellar",
        "The Dark Knight",
        "Avatar",
        "Titanic",
        "Gladiator",
        "Coco",
        "Frozen",
        "Joker",
        "Dune"};

    const int durations[MAX_MOVIES] = {148, 169, 152, 162, 194, 155, 105, 102, 122, 155};
    const int basePrices[MAX_MOVIES] = {1200, 1400, 1300, 1500, 1250, 1350, 1100, 1000, 1450, 1600};

    int i;
    int t;
    int movieId;

    initStore(store);

    for (i = 0; i < MAX_MOVIES; i++)
    {
        movieId = addMovie(store, titles[i], durations[i]);
        for (t = 0; t < 3; t++)
        {
            addShow(store, movieId, times[t], basePrices[i] + (t * 100));
        }
    }

    // Pre-populate show 0 with 3 booked seats backed by a real Booking record.
    // This ensures viewBooking(), cancelBooking(), and waitlist processing all
    // see a consistent state — no orphaned occupied seats without a booking ID.
    {
        int bi = store.bookingCount;

        store.bookings[bi].id            = store.nextBookingId;
        store.bookings[bi].showId        = store.shows[0].id;
        store.bookings[bi].customerName  = "Sample Customer";
        store.bookings[bi].seatCount     = 3;
        store.bookings[bi].seatR[0]      = 4;
        store.bookings[bi].seatC[0]      = 5;
        store.bookings[bi].seatR[1]      = 4;
        store.bookings[bi].seatC[1]      = 6;
        store.bookings[bi].seatR[2]      = 5;
        store.bookings[bi].seatC[2]      = 5;
        store.bookings[bi].status        = STATUS_CONFIRMED;

        store.bookingCount++;
        store.nextBookingId++;
    }
}

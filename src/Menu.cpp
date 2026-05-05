#include <iostream>
#include <string>
using namespace std;

#include "Menu.h"
#include "ConsoleIO.h"
#include "Constants.h"

void Menu::run(BookingSystem &system)
{
    int mainChoice;
    int movieId;
    int movieIds[MAX_MOVIES];
    int movieIdCount;
    int movieShowIds[MAX_SHOWS];
    int showCount;
    int showId;
    int showChoice;
    int bookingId;
    int row;
    int col;
    int seatsNeeded;
    int answer;
    std::string title;
    std::string name;

    while (1)
    {
        ConsoleIO::clearScreen();
        cout << endl;
        ConsoleIO::printDivider();
        cout << "        MOVIE TICKET BOOKING" << endl;
        ConsoleIO::printDivider();
        cout << "1) Book Tickets" << endl;
        cout << "2) Search Movie" << endl;
        cout << "3) View My Booking" << endl;
        cout << "4) Cancel booking" << endl;
        cout << "5) Exit" << endl;

        mainChoice = ConsoleIO::readInt("Enter choice: ", 1, 5);
        ConsoleIO::printDivider();

        if (mainChoice == 1)
        {
            movieIdCount = system.getMovieIds(movieIds);
            system.listMovies();
            movieId = ConsoleIO::readIntFromList("Enter movie ID: ", movieIds, movieIdCount);

            showCount = system.listShowsForMovieSorted(movieId, movieShowIds);
            if (showCount == 0)
            {
                continue;
            }
            showId = ConsoleIO::readIntFromList("Enter show ID: ", movieShowIds, showCount);

            while (1)
            {
                ConsoleIO::clearScreen();
                ConsoleIO::printDivider();
                system.printSeatMap(showId);
                cout << "1) Book Single Seat" << endl;
                cout << "2) Book Group Seats" << endl;
                cout << "3) Join Waitlist" << endl;
                cout << "4) Back" << endl;

                showChoice = ConsoleIO::readInt("Enter choice: ", 1, 4);
                ConsoleIO::printDivider();

                if (showChoice == 1)
                {
                    name = ConsoleIO::readName("Enter customer name: ");
                    row = ConsoleIO::readInt("Enter row (1-" + std::to_string(ROWS) + "): ", 1, ROWS,
                                             "enter a row number between 1 and " + std::to_string(ROWS));
                    col = ConsoleIO::readInt("Enter column (1-" + std::to_string(COLS) + "): ", 1, COLS,
                                             "enter a column number between 1 and " + std::to_string(COLS));
                    if (system.bookSingle(showId, name, row, col))
                    {
                        cout << endl;
                        ConsoleIO::pause();
                    }
                }
                else if (showChoice == 2)
                {
                    Seat recommended[MAX_SEATS_IN_BOOKING];

                    name = ConsoleIO::readName("Enter customer name: ");
                    seatsNeeded = ConsoleIO::readInt("Enter number of seats (2-" + std::to_string(MAX_SEATS_IN_BOOKING) + "): ",
                                                     2, MAX_SEATS_IN_BOOKING,
                                                     "enter a group size between 2 and " + std::to_string(MAX_SEATS_IN_BOOKING));

                    if (system.recommendSeats(showId, seatsNeeded, recommended))
                    {
                        cout << "Suggested seats: ";
                        for (int i = 0; i < seatsNeeded; i++)
                        {
                            cout << "(" << (int)recommended[i].r + 1 << "," << (int)recommended[i].c + 1 << ")";
                            if (i < seatsNeeded - 1)
                            {
                                cout << ", ";
                            }
                        }
                        cout << endl;
                        answer = ConsoleIO::readInt("Confirm booking? 1 Yes / 2 No: ", 1, 2);
                        if (answer == 1)
                        {
                            if (system.bookGroupSeats(showId, name, recommended, seatsNeeded))
                            {
                                cout << endl;
                                ConsoleIO::pause();
                            }
                        }
                    }
                    else
                    {
                        int avail = system.getAvailableCount(showId);
                        cout << "Only " << avail << " seats available (you requested " << seatsNeeded << ")." << endl;
                        answer = ConsoleIO::readInt("Join waitlist? 1 Yes / 2 No: ", 1, 2);
                        if (answer == 1)
                        {
                            if (system.joinWaitlist(showId, name, seatsNeeded))
                            {
                                cout << endl;
                                ConsoleIO::pause();
                            }
                        }
                    }
                }
                else if (showChoice == 3)
                {
                    name = ConsoleIO::readName("Enter customer name: ");
                    seatsNeeded = ConsoleIO::readInt("Enter number of seats for waitlist (1-" + std::to_string(MAX_SEATS_IN_BOOKING) + "): ",
                                                     1, MAX_SEATS_IN_BOOKING,
                                                     "enter a seat count between 1 and " + std::to_string(MAX_SEATS_IN_BOOKING));
                    if (system.joinWaitlist(showId, name, seatsNeeded))
                    {
                        cout << endl;
                        ConsoleIO::pause();
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else if (mainChoice == 2)
        {
            title = ConsoleIO::readLine("Enter exact movie title: ");
            if (system.searchMovie(title) == -1)
            {
                cout << "\nAvailable Movies:" << endl;
                system.listMovies();
            }
            cout << endl;
            ConsoleIO::pause();
        }
        else if (mainChoice == 3)
        {
            bookingId = ConsoleIO::readInt("Enter booking ID: ", 0, 1000000,
                                           "enter the booking ID shown on your confirmation");
            system.viewBooking(bookingId);
            cout << endl;
            ConsoleIO::pause();
        }
        else if (mainChoice == 4)
        {
            bookingId = ConsoleIO::readInt("Enter booking ID: ", 0, 1000000,
                                           "enter the booking ID shown on your confirmation");
            name = ConsoleIO::readName("Enter customer name: ");
            if (system.cancelBooking(bookingId, name))
            {
                cout << endl;
                ConsoleIO::pause();
            }
        }
        else if (mainChoice == 5)
        {
            cout << "Exiting system." << endl;
            break;
        }
    }
}

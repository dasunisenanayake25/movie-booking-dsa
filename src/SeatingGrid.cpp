#include <iomanip>
#include "SeatingGrid.h"

SeatingGrid::SeatingGrid()
{
    grid = NULL;
    rows = 0;
    cols = 0;
}

SeatingGrid::~SeatingGrid()
{
    if (grid != NULL)
    {
        for (int r = 0; r < rows; r++)
        {
            delete[] grid[r];
        }
        delete[] grid;
        grid = NULL;
    }
}

int SeatingGrid::encodeKey(int row, int col) const
{
    return row * cols + col;
}

int SeatingGrid::isValid(int row, int col) const
{
    return row >= 0 && row < rows && col >= 0 && col < cols;
}

void SeatingGrid::init(int rowCount, int colCount)
{
    if (grid != NULL)
    {
        for (int r = 0; r < rows; r++)
        {
            delete[] grid[r];
        }
        delete[] grid;
        grid = NULL;
    }

    selectedSeats.clear();
    seatMap.init(rowCount * colCount);

    rows = rowCount;
    cols = colCount;

    grid = new int *[rows];
    for (int r = 0; r < rows; r++)
    {
        grid[r] = new int[cols];
        for (int c = 0; c < cols; c++)
        {
            grid[r][c] = AVAILABLE;
            seatMap.put(encodeKey(r, c), encodeKey(r, c));
        }
    }
}

int SeatingGrid::selectSeat(int row, int col)
{
    if (!isValid(row, col))
    {
        return 0;
    }

    if (grid[row][col] != AVAILABLE)
    {
        return 0;
    }

    grid[row][col] = SELECTED;
    selectedSeats.insertLast(encodeKey(row, col));
    return 1;
}

int SeatingGrid::deselectSeat(int row, int col)
{
    if (!isValid(row, col))
    {
        return 0;
    }

    if (grid[row][col] != SELECTED)
    {
        return 0;
    }

    grid[row][col] = AVAILABLE;
    selectedSeats.removeValue(encodeKey(row, col));
    return 1;
}

int SeatingGrid::confirmBooking()
{
    if (selectedSeats.getSize() == 0)
    {
        return 0;
    }

    SinglyLinkedListInt::Node *current = selectedSeats.getHead();
    while (current != NULL)
    {
        int key = current->value;
        int row = key / cols;
        int col = key % cols;
        grid[row][col] = BOOKED;
        current = current->next;
    }

    selectedSeats.clear();
    return 1;
}

int SeatingGrid::cancelSeat(int row, int col)
{
    if (!isValid(row, col))
    {
        return 0;
    }

    if (grid[row][col] != BOOKED)
    {
        return 0;
    }

    grid[row][col] = AVAILABLE;
    return 1;
}

int SeatingGrid::isAvailable(int row, int col) const
{
    if (!isValid(row, col))
    {
        return 0;
    }
    return grid[row][col] == AVAILABLE;
}

void SeatingGrid::suggestSeats(int count) const
{
    for (int r = 0; r < rows; r++)
    {
        int consecutive = 0;
        int startCol = -1;

        for (int c = 0; c < cols; c++)
        {
            if (grid[r][c] == AVAILABLE)
            {
                if (consecutive == 0)
                {
                    startCol = c;
                }
                consecutive++;

                if (consecutive == count)
                {
                    cout << "  Suggested: Row " << (char)('A' + r)
                         << ", Seats " << (startCol + 1)
                         << " - " << (c + 1) << endl;
                    return;
                }
            }
            else
            {
                consecutive = 0;
                startCol = -1;
            }
        }
    }

    cout << "  No " << count << " consecutive seats available." << endl;
}

void SeatingGrid::displayGrid() const
{
    cout << "\n";
    cout << "  +-";
    for (int c = 0; c < cols; c++)
    {
        cout << "----";
    }
    cout << "-+" << endl;

    int totalWidth = cols * 4;
    int labelLen = 6;
    int padding = (totalWidth - labelLen) / 2;
    cout << "  | ";
    for (int i = 0; i < padding; i++) cout << " ";
    cout << "SCREEN";
    for (int i = 0; i < padding; i++) cout << " ";
    cout << " |" << endl;

    cout << "  +-";
    for (int c = 0; c < cols; c++)
    {
        cout << "----";
    }
    cout << "-+" << endl;

    cout << "\n";

    cout << "     ";
    for (int c = 0; c < cols; c++)
    {
        cout << setw(3) << (c + 1) << " ";
    }
    cout << "\n\n";

    for (int r = 0; r < rows; r++)
    {
        cout << "  " << (char)('A' + r) << "  ";
        for (int c = 0; c < cols; c++)
        {
            if (grid[r][c] == AVAILABLE)
            {
                cout << "[ ] ";
            }
            else if (grid[r][c] == SELECTED)
            {
                cout << "[*] ";
            }
            else
            {
                cout << "[X] ";
            }
        }
        cout << "\n";
    }

    cout << "\n";
    cout << "  [ ] Available   [*] Selected   [X] Booked" << endl;
    cout << "  +-";
    for (int c = 0; c < cols; c++)
    {
        cout << "----";
    }
    cout << "-+" << endl;

    cout << "  Available: " << getAvailableCount()
         << "   Selected: "  << getSelectedCount()
         << "   Booked: "    << getBookedCount()
         << endl;

    cout << "  +-";
    for (int c = 0; c < cols; c++)
    {
        cout << "----";
    }
    cout << "-+" << endl;
    cout << "\n";
}

int SeatingGrid::getAvailableCount() const
{
    int count = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (grid[r][c] == AVAILABLE)
            {
                count++;
            }
        }
    }
    return count;
}

int SeatingGrid::getSelectedCount() const
{
    return selectedSeats.getSize();
}

int SeatingGrid::getBookedCount() const
{
    int count = 0;
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            if (grid[r][c] == BOOKED)
            {
                count++;
            }
        }
    }
    return count;
}
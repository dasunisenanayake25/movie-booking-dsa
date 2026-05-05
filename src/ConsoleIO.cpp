#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

#include "ConsoleIO.h"

int ConsoleIO::readInt(const std::string &prompt, int minValue, int maxValue,
                       const std::string &hint)
{
    int value;

    while (1)
    {
        cout << prompt;
        cin >> value;

        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Please enter a valid integer." << endl;
            continue;
        }

        cin.ignore(10000, '\n');

        if (value < minValue || value > maxValue)
        {
            if (hint.empty())
            {
                cout << "Error: Please enter a value between " << minValue << " and " << maxValue << "." << endl;
            }
            else
            {
                cout << "Error: " << hint << "." << endl;
            }
            continue;
        }

        return value;
    }
}

int ConsoleIO::readIntFromList(const std::string &prompt, const int validIds[], int count)
{
    int value;
    int i;

    while (1)
    {
        cout << prompt;
        cin >> value;

        if (cin.fail() || cin.peek() != '\n')
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error: Please enter a valid integer." << endl;
            continue;
        }

        cin.ignore(10000, '\n');

        for (i = 0; i < count; i++)
        {
            if (validIds[i] == value)
            {
                return value;
            }
        }

        cout << "Error: " << value << " is not a valid ID from the list above." << endl;
    }
}

std::string ConsoleIO::readLine(const std::string &prompt)
{
    std::string input;
    while (1)
    {
        cout << prompt;
        std::getline(cin, input);

        if (input.empty())
        {
            cout << "Error: Input cannot be empty." << endl;
            continue;
        }
        return input;
    }
}

std::string ConsoleIO::readName(const std::string &prompt)
{
    return readLine(prompt);
}

void ConsoleIO::printDivider()
{
    cout << "----------------------------------------" << endl;
}

void ConsoleIO::pause()
{
    std::string dummy;
    cout << "Press Enter to continue...";
    std::getline(cin, dummy);
}

void ConsoleIO::clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

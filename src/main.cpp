#include <iostream>
using namespace std;

#ifdef _WIN32
#include <windows.h>
#endif

#include "BookingSystem.h"
#include "Menu.h"

int main()
{
#ifdef _WIN32
    SetConsoleTitleA("NoirCinema");
#endif

    BookingSystem system;
    Menu::run(system);
    return 0;
}

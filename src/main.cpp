#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "BookingSystem.h"
#include "Menu.h"

using namespace std;

int main()
{
#ifdef _WIN32
    SetConsoleTitleA("NoirCinema");
#endif

    BookingSystem system;
    Menu::run(system);
    return 0;
}

#ifndef CONSOLEIO_H
#define CONSOLEIO_H

#include <string>

class ConsoleIO
{
public:
    static int readInt(const std::string &prompt, int minValue, int maxValue,
                       const std::string &hint = "");
    static int readIntFromList(const std::string &prompt, const int validIds[], int count);
    static std::string readLine(const std::string &prompt);
    static std::string readName(const std::string &prompt);
    static void printDivider();
    static void pause();
    static void clearScreen();
};

#endif

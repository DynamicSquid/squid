#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <string>

void SetCursorPosition(SHORT x, SHORT y)
{
    static HANDLE chandle = GetStdHandle(STD_OUTPUT_HANDLE);

    std::cout.flush();

    COORD coord{ x, y };
    SetConsoleCursorPosition(chandle, coord);
}

void DrawSquid(const std::string& colour)
{
    static std::string squid[] = {
        "        l        ",
        "     lldddll     ",
        "    ddddddddd    ",
        "     hdddddh     ",
        "     ldddddl     ",
        " hdlldddddddlldh ",
        "    ld dhd dl    ",
        "   hh ld dl hh   ",
        "      h   h      "
    };

    std::cout << "\033[38;2;" << colour << "m";
    for (int a = 0; a < sizeof(squid) / sizeof(squid[0]); ++a)
    {
        for (int b = 0; b < squid[a].length(); ++b)
        {
            SetCursorPosition(b, a);
            if (squid[a][b] == 'd')
                std::cout << '\xDB';
            else if (squid[a][b] == 'l')
                std::cout << '\xDC';
            else if (squid[a][b] == 'h')
                std::cout << '\xDf';
        }

        std::cout << '\n';
    }

    std::cout << "\033[48;2;" << colour << ";38;5;0m";

    SetCursorPosition(7, 4);
    std::cout << '\xDC';
    SetCursorPosition(9, 4);
    std::cout << '\xDC';
    SetCursorPosition(7, 5);
    std::cout << '\xDf';
    SetCursorPosition(9, 5);
    std::cout << '\xDf';

    std::cout << "\033[0m";
    SetCursorPosition(0, 0);
}

int main()
{
    HANDLE chandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cinfo{ 100, FALSE };
    SetConsoleCursorInfo(chandle, &cinfo);

    int r = 255, g = 0, b = 0;
    while (true)
    {
        DrawSquid(std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        if (r == 255 && g != 255 && b == 0)
            g++;
        else if (r != 0 && g == 255 && b == 0)
            r--;
        else if (r == 0 && g == 255 && b != 255)
            b++;
        else if (r == 0 && g != 0 && b == 255)
            g--;
        else if (r != 255 && g == 0 && b == 255)
            r++;
        else if (r == 255 && g == 0 && b != 0)
            b--;
    }
}

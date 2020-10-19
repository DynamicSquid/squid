#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

std::vector<std::string> squid_h{
    "      ldl      ",
    "   ldddddddl   ",
    "   hdddddddh   ",
    "     ddddd     ",
    "ll  ddddddd  ll",
    " hhhdhdddhdhhh ",
    "  ldh d d hdl  ",
    "     dh hd     "
};

std::vector<std::string> squid_l{
    "       l       ",
    "    lldddll    ",
    "   ddddddddd   ",
    "    hdddddh    ",
    "    ldddddl    ",
    "hdlldddddddlldh",
    "   ld dhd dl   ",
    "  hh ld dl hh  ",
    "     h   h     "
};

std::vector<std::string> squid = squid_h;
std::mutex mutexSquid;

int x, y;

static HANDLE chandle = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO cinfo;

void Output(char text, int x, int y)
{
    COORD coord{ (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(chandle, coord);

    std::cout << text;
}

void DrawSquid(const std::string& colour)
{
    mutexSquid.lock();

    GetConsoleScreenBufferInfo(chandle, &cinfo);

    for (std::size_t a = 0; a < squid.size(); ++a)
    {
        for (std::size_t b = 0; b < squid[a].length(); ++b)
        {
            std::cout << (squid == squid_h && a == 4 && (b == 6 || b == 8) ? "\033[30m" : "\033[38;2;" + colour + "m");
            if (squid[a][b] == 'd')
                Output('\xDB', x + b, y + a);
            else if (squid[a][b] == 'l')
                Output('\xDC', x + b, y + a);
            else if (squid[a][b] == 'h')
                Output('\xDf', x + b, y + a);
        }

        std::cout << '\n';
    }

    if (squid == squid_l)
    {
        std::cout << "\033[48;2;" + colour + ";38;5;0m";
        Output('\xDC', x + 6, y + 4);
        Output('\xDC', x + 8, y + 4);
        Output('\xDf', x + 6, y + 5);
        Output('\xDf', x + 8, y + 5);
    }

    std::cout << "\033[0m" << std::flush;
    mutexSquid.unlock();
}

void ClearSquid()
{
    std::cout << "\033[0m";
    for (std::size_t a = 0; a < squid.size(); ++a)
    {
        for (std::size_t b = 0; b < squid[a].length(); ++b)
            Output(' ', x + b, y + a);
    }

    std::cout.flush();
}

void Movement()
{
    while (true)
    {
        char ch = _getch();
        mutexSquid.lock();

        if (ch == 'w' && squid == squid_l && y > 0)
        {
            ClearSquid();
            squid = squid_h;
        }
        else if (ch == 'w' && squid == squid_h && y > 1)
        {
            ClearSquid();
            squid = squid_l;
            y--;
        }
        else if (ch == 'a' && x > 0)
        {
            ClearSquid();
            x--;
        }
        else if (ch == 's' && squid == squid_l)
        {
            ClearSquid();
            squid = squid_h;
            y++;
        }
        else if (ch == 's' && squid == squid_h)
        {
            ClearSquid();
            squid = squid_l;
        }
        else if (ch == 'd')
        {
            ClearSquid();
            x++;
        }

        mutexSquid.unlock();
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    CONSOLE_CURSOR_INFO cinfo{ 100, FALSE };
    SetConsoleCursorInfo(chandle, &cinfo);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(chandle, &info);

    x = (info.dwSize.X / 2) - (squid[0].length() / 2);
    y = 7;

    std::thread movement(Movement);

    int r = 255, g = 0, b = 0;
    while (true)
    {
        DrawSquid(std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b));

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

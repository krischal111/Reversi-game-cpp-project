#ifndef MAIN
#define MAIN
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "datafiles.cpp"

int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

int main()
{
    reversiBoard b;
    system("clear");
    b.printboard();
    char c, redraw, okay;
    while(c != 'q' || b.check_game_over())
    {
        c = getch();
        switch(c)
        {
            case 'w':
            case 'W':
                redraw = true;
                b.cur_row--;
                break;
            case 'a':
            case 'A':
                redraw = true;
                b.cur_col--;
                break;
            case 's':
            case 'S':
                redraw = true;
                b.cur_row++;
                break;
            case 'd':
            case 'D':
                redraw = true;
                b.cur_col++;
                break;
            case 'f':
            case 'F':
            case ' ':
            case  13:
                redraw = true;
                okay = true;
                break;
            default:
                break;
        }
        if(redraw)
        {
            redraw = false;
            if(okay)
            {
                okay = false;
                b.make_move_at(b.cur_row, b.cur_col, b.turn);
            }
            b.cur_row = (b.cur_row+total_rows )% total_rows;
            b.cur_col = (b.cur_col+total_columns )% total_columns;
            system("clear");
            b.printboard();
        }
    }
}

#endif
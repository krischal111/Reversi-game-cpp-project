#ifndef DATATHINGS
#define DATATHINGS

#include <iostream>
#include "customcalc.cpp"

typedef struct {bool d[3][3];} Directions;
enum board_details{total_rows = 8, total_columns = 8};

    const char s_player_pieces[4][3] = {"  ","@@", "\xdb\xdb","02"};
    const char s_empty_or_moves[2][3] = {"  ", "'."};

struct renderdata
{
    private:
    static const int e_bits = 3;
    static const int b_density = 8/3;
    public:
    char board[total_rows*total_columns/b_density];
    char b(int row, int column)
    {
        int n = (row)*total_columns+(column);
        return bits(board[n/b_density], n%b_density*e_bits-1, e_bits);
    }
    char number_of_moves;
    char flags; // f0 for gameover
                // f1 for seeing possible moves or not
                // f2 
                // f3 for seeing pass state
                // f4 for
                // f5 for
                // f6 for
                // f7 for
    char gameover(){return bit(flags,0);}
    char show_available_moves(){return bit(flags, 1);}
    char passes(){return bits(flags,3,2);};
};

class reversiBoard
{
    typedef enum  {emptyblock = 0, blackpiece  = 1, whitepiece  = 2, totalpieces } Board_pieces;
    typedef enum  {noplayer   = 0, blackplayer = 1, whiteplayer = 2, totalplayers} The_player;
    typedef enum  {looking = 0, waiting_for_move = 1} Board_condition;
    
    private:
    Board_condition board_mode = waiting_for_move;
    int cur_row = 2, cur_col = 3;

    public:
    char b[total_rows][total_columns] =
    {
        {0,0,0,0, 0,0,0,0},
        {0,0,0,0, 0,0,0,0},
        {0,0,0,0, 0,0,0,0},
        {0,0,0,0, 0,0,0,0},

        {0,0,0,0, 0,0,0,0},
        {0,0,0,0, 0,0,0,0},
        {0,0,0,0, 0,0,0,0},
        {0,0,0,0, 0,0,0,0}
    };

    int turn = blackplayer; // Theplayer
    int pass = 0;

    reversiBoard()
    {
        int r = total_rows/2;
        int c = total_columns/2;
        b[r][c] = b[r-1][c-1] = whitepiece;
        b[r][c-1] = b[r-1][c] = blackpiece;
    }

    void calculate_board();

    void printboard()
    {
        std::cout <<std::endl<<'|';
        for(int j = 0; j<total_columns; j++)
            std::cout << "------\b";
        std::cout << "\b|  "<<std::endl; // ##### enter

        for(int i = 0; i<total_rows; i++)
        {
            if(i)
            {
                std::cout << '|';
                for(int j = 0; j<total_columns; j++)
                std::cout << "----+";
                std::cout<<"\b|  "<<std::endl;
            }
            std::cout << '|';
            for(int j = 0; j<total_columns; j++)
            {
                bool cursorhere = (cur_row == i) && (cur_col == j);
                int thispiece = b[i][j];
                std::cout << (char)(cursorhere? '(':' ')<< 
                (char *)((thispiece == emptyblock) ? 
                    s_empty_or_moves[(has_moves_here(i,j, turn)?1:0)]:
                    (s_player_pieces[thispiece])) << char(cursorhere?')':' ' )<<'|';
            }
            std::cout << "\b| "<< std::endl;
            
        }
        std::cout << '|';
        for(int j = 0; j<total_columns; j++)
            std::cout << "------\b";
        std::cout << "\b|  "<<std::endl; // ##### enter
    }


    bool has_moves_here(int row, int column, int our_player)
    {
        if(b[row][column] != emptyblock) // can have moves only on an empty block
            return false;
        // implied else
        Directions dir = start_marching(row, column, (Board_pieces)our_player);
        return dir.d[1][1];
    }

    bool make_move_at(int row, int column, int our_player)
    {
        if(b[row][column] != emptyblock)
            return false;

        Directions dir = start_marching(row, column, (Board_pieces)our_player);

        if(dir.d[1][1])
        {
            for(int i = 0; i<3; i++)
            for(int j = 0; j<3; j++)
            {
                if(dir.d[i][j])
                {
                    move_till(row,column, i-1, j-1, (Board_pieces)our_player);
                }
            }
            b[row][column] = (Board_pieces)our_player;
            pass = 0;
            nextturn();
        }
        return dir.d[1][1];
    }

    Directions start_marching(int row, int column, Board_pieces the_piece)
    {
        Directions directions;
        bool tempbool = false;

        for(int i = 0; i<3; i++)
        for(int j = 0; j<3; j++)
        {
            int r = row+i-1;
            int c = column+j-1;
            auto thispiece = b[r][c];

            bool hasmovehere = 
            (   (thispiece == the_piece || thispiece == emptyblock)? 
                false :
                march_till(r,c, i-1, j-1, the_piece)
            );
            directions.d[i][j] = hasmovehere;    
            tempbool = tempbool || hasmovehere;
        }
        directions.d[1][1] = tempbool;
        return directions;
    }

    bool march_till(int row, int column, int rinc, int cinc, Board_pieces the_piece)
    {
        if(rinc == 0 && cinc == 0)      // no marching
        return false;
        // implied else

        row     +=  rinc;
        column  +=  cinc;
        if( row<0 || row>=total_rows || column<0 || column>=total_columns) // overflow
        return false;
        // implied else

        auto thispiece = b[row][column];

        if(thispiece == emptyblock) // empty block
        return false;

        // implied else
        if(thispiece == the_piece)  // found the piece
        {
            return true;
        }
        else
        {
            return march_till(row, column, rinc, cinc, the_piece); // march deeper
        }
    }

    void move_till(int row, int column, int rinc, int cinc, Board_pieces the_piece)
    {
        if(rinc == 0 && cinc == 0)
        return;

        row     += rinc;
        column  += cinc;
        auto thispiece = b[row][column];

        if(thispiece == the_piece)
        {
            return;
        }
        else
        {   
            b[row][column] = the_piece;
            move_till(row, column, rinc, cinc, the_piece);
            return;
        }
    }

    bool check_game_over()
    {
        bool something_everywhere = true;
        for(int i = 0; i<total_rows; i++)
        for(int j = 0; j<total_columns; j++)
        {
            something_everywhere &= (b[i][j] != emptyblock);
        }

        return something_everywhere || (pass==2);
    }


    int nextturn()
    {
        pass = 0;
        turn++;
        if(turn == totalpieces)
        turn = 1;
        return turn;
    }

    friend int main();
};
#endif // datafiles.cpp included

//#ifndef MAIN
//#define MAIN
#include <iostream>
#include <assert.h>

void run_the_test();
int main()
{
    run_the_test();
    return 0;
    reversiBoard b;
    b.printboard();

    int r, c;
    r = 2;
    c = 3;
    if(b.make_move_at(r,c,b.turn))
    {
        if(!b.check_game_over())
        std::cout <<std::endl<< "Game ain't over yet";
        std::cout<< std::endl<<"Making move at "<< r<< ", " << c << std::endl;
    }
    std::cout<<std::endl;
    b.printboard();

    r = 2;
    c = 2;
    if(b.make_move_at(r,c,b.turn))
    {
        if(!b.check_game_over())
        std::cout <<std::endl<< "Game ain't over yet";
        std::cout<< std::endl<<"Making move at "<< r<< ", " << c << std::endl;
    }
    std::cout<<std::endl;
    b.printboard();
    

    return 0;
}

void run_the_test()
{
    int x = 0;
    std::cout<< x<<std::endl<<(setbit(x,2,1) == 4)<<std::endl << x << std::endl;
    assert(x==4);
    assert(setbits(x,3,3,1011) == 6);
}
//#endif // Main function
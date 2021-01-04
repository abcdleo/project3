#include <iostream>
#include <fstream> // test only
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;

/******************************************************
 * In your algorithm, you can just use the the funcitons
 * listed by TA to get the board information.(functions 
 * 1. ~ 4. are listed in next block)
 * 
 * The STL library functions is not allowed to use.
******************************************************/

/*************************************************************************
 * 1. int board.get_orbs_num(int row_index, int col_index)
 * 2. int board.get_capacity(int row_index, int col_index)
 * 3. char board.get_cell_color(int row_index, int col_index)
 * 4. void board.print_current_board(int row_index, int col_index, int round)
 * 
 * 1. The function that return the number of orbs in cell(row, col)
 * 2. The function that return the orb capacity of the cell(row, col)
 * 3. The function that return the color fo the cell(row, col)
 * 4. The function that print out the current board statement
*************************************************************************/
ofstream fileout2("out2.data");

class futureBoard
{
private:
    int Board_copy[5][6];
    int Board_capacity[5][6];
    char Board_color[5][6];
    char color;
    char opponentColor;

    int round = 0;

public:
    futureBoard(Board board, Player player)
    {
        color = player.get_color();
        if (color == 'r')
            opponentColor = 'b';
        else if (color == 'b')
            opponentColor = 'r';
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 6; j++)
            {
                Board_copy[i][j] = board.get_orbs_num(i, j);
                Board_capacity[i][j] = board.get_capacity(i, j);
                Board_color[i][j] = board.get_cell_color(i, j);
                round += board.get_orbs_num(i, j);
            }
    };
    void findBestMove(Board board, int *put)
    {
        int BestMove = -10000, BestMoveRow, BestMoveCol;
        int thisMove;

        int Board_record[30];
        char Board_color_record[30];

        fileout2 << "round : " << round << endl; //test only
        this->record(Board_record, Board_color_record);

        //this->print_current_board();
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (board.get_cell_color(i, j) == color)
                {
                    Board_copy[i][j]++;
                    this->game_processing();
                    //this->print_current_board();
                    thisMove = this->minmax(1, false);
                    this->recover(Board_record, Board_color_record);
                    //cout << thisMove << endl;

                    if (BestMove <= thisMove) //max
                    {
                        BestMove = thisMove;
                        BestMoveRow = i;
                        BestMoveCol = j;
                    }
                }
                //else if (Board_color[i][j] == 'w')
                else if (board.get_cell_color(i, j) == 'w')
                {
                    Board_copy[i][j]++;
                    Board_color[i][j] = color;
                    this->game_processing();
                    //this->print_current_board();
                    thisMove = this->minmax(1, false);
                    this->recover(Board_record, Board_color_record);
                    //Board_color[i][j] = 'w';
                    //Board_copy[i][j]--;
                    //cout << thisMove << endl;

                    if (BestMove <= thisMove) //max
                    {
                        BestMove = thisMove;
                        BestMoveRow = i;
                        BestMoveCol = j;
                    }
                }
            }
        }

        fileout2 << "after all : \n";                                            // test only
        this->print_current_board();                                             // test only
        fileout2 << "The End" << endl                                            //test only
                 << BestMove << " " << BestMoveRow << " " << BestMoveCol << endl //test only
                 << endl;                                                        //test only
        put[0] = BestMoveRow;
        put[1] = BestMoveCol;
    };

    inline int minmax(int depth, bool isMax)
    {
        int BestScore = -10000, Score;
        int isOver;
        int Board_record[30];
        char Board_color_record[30];

        //  fileout2 << "before this->game_processing() : \n"; // test only
        // this->print_current_board();                       // test only
        this->record(Board_record, Board_color_record);

        isOver = this->gameOver();
        //cout << "this->gameOver() = " << isOver << endl;
        if (isOver == 300 || isOver == -300 || depth == 3)
        {
            //fileout2 << " terminal\n";
            return isOver - depth;
        }

        if (isMax) // my turn
        {
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 6; j++)
                {
                    if (Board_color[i][j] == color)
                    {
                        Board_copy[i][j]++;

                        this->game_processing();
                        //   fileout2 << "after this->game_processing() : \n"; // test only
                        //  this->print_current_board();                      // test only

                        Score = this->minmax(depth + 1, false);

                        this->recover(Board_record, Board_color_record);
                        //   fileout2 << "after this->recover() : \n"; // test only
                        //   this->print_current_board();              // test only

                        //Board_copy[i][j]--;

                        if (BestScore < Score) //max
                            BestScore = Score;
                    }
                    else if (Board_color[i][j] == 'w')
                    {
                        Board_copy[i][j]++;
                        Board_color[i][j] = color;

                        this->game_processing();
                        //  fileout2 << "after this->game_processing() : \n"; // test only
                        //  this->print_current_board();                      // test only

                        Score = this->minmax(depth + 1, false);

                        this->recover(Board_record, Board_color_record);
                        //  fileout2 << "after this->recover() : \n"; // test only
                        //  this->print_current_board();              // test only

                        //Board_color[i][j] = 'w';
                        //Board_copy[i][j]--;

                        if (BestScore < Score) //max
                            BestScore = Score;
                    }
                }
            //this->recover(Board_record, Board_color_record);
            fileout2 << "at the depth " << depth << ", after this->recover() : " << endl // test only
                     << "BestScore = " << BestScore << endl;                             // test only
            this->print_current_board();                                                 // test only                                                                           // test only
            return BestScore;
        }
        else // opponent turn
        {
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 6; j++)
                {
                    if (Board_color[i][j] == opponentColor)
                    {
                        Board_copy[i][j]++;

                        this->game_processing();
                        //  fileout2 << "after this->game_processing() : \n"; // test only
                        //   this->print_current_board();                      // test only

                        Score = this->minmax(depth + 1, true);

                        this->recover(Board_record, Board_color_record);
                        //  fileout2 << "after this->recover() : \n"; // test only
                        //  this->print_current_board();              // test only

                        //Board_copy[i][j]--;

                        if (BestScore > Score) // min
                            BestScore = Score;
                    }
                    else if (Board_color[i][j] == 'w')
                    {
                        Board_copy[i][j]++;
                        Board_color[i][j] = opponentColor;

                        this->game_processing();
                        //   fileout2 << "after this->game_processing() : \n"; // test only
                        //   this->print_current_board();                      // test only

                        Score = this->minmax(depth + 1, true);

                        this->recover(Board_record, Board_color_record);
                        //   fileout2 << "after this->recover() : \n"; // test only
                        //   this->print_current_board();              // test only

                        //Board_color[i][j] = 'w';
                        //Board_copy[i][j]--;

                        if (BestScore > Score) // min
                            BestScore = Score;
                    }
                }
            //this->recover(Board_record, Board_color_record);
            fileout2 << "at the depth " << depth << ", after this->recover() : " << endl // test only
                     << "BestScore = " << BestScore << endl;                             // test only
            this->print_current_board();                                                 // test only
            return BestScore;
        }
    };
    inline int gameOver()
    {
        int myOrb = 0;
        int opponentOrb = 0;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 6; j++)
            {
                if (Board_color[i][j] == color)
                    myOrb++;
                else if (Board_color[i][j] == opponentColor)
                {
                    opponentOrb++;
                    //fileout2 << "ememy\n";
                }
            }
        if (myOrb == 0) //Opponent is the winner
            return -300;
        else if (opponentOrb == 0) //I am the winner
            return 300;
        else
            return myOrb - opponentOrb;
    };
    inline void game_processing()
    {
        int row[1000];
        int col[1000];
        char primecolor;
        int affectedRow, affectedCol;
        int start = -1, end = -1;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 6; j++)
                if (Board_copy[i][j] == Board_capacity[i][j])
                {
                    end++;
                    row[end] = i;
                    col[end] = j;
                }
        while (start != end)
        {
            start++;
            primecolor = Board_color[row[start]][col[start]];
            for (int diff = -7; diff < 8; diff++)
            {
                bool out_of_range = false;
                if (diff == -4)
                    diff = -1;
                else if (diff == 0)
                    diff = 1;
                else if (diff == 2)
                    diff = 5;

                if (row[start] == 0 && diff >= -7 && diff <= -5)
                    out_of_range = true;
                else if (row[start] == 4 && diff >= 5 && diff <= 7)
                    out_of_range = true;
                if (col[start] == 0 && (diff == -7 || diff == -1 || diff == 5))
                    out_of_range = true;
                else if (col[start] == 5 && (diff == -5 || diff == 1 || diff == 7))
                    out_of_range = true;

                if (!out_of_range)
                {
                    affectedRow = row[start] + diff / 6;
                    affectedCol = col[start] + diff % 6;
                    Board_copy[affectedRow][affectedCol]++;
                    Board_color[affectedRow][affectedCol] = primecolor;
                    if (Board_copy[affectedRow][affectedCol] == Board_capacity[affectedRow][affectedCol])
                    {
                        end++;
                        row[end] = affectedRow;
                        col[end] = affectedCol;
                    }
                }
            }
            Board_copy[row[start]][col[start]] = 0;
            Board_color[row[start]][col[start]] = 'w';
        }
    };
    inline void print_current_board()
    {
        int orb_num;
        char symbol;
        fileout2 << "===========================================================================================\n";
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {

                symbol = Board_color[i][j];
                switch (symbol)
                {
                case 'r':
                    symbol = 'O';
                    break;
                case 'b':
                    symbol = 'X';
                    break;
                default:
                    break;
                }

                orb_num = Board_copy[i][j];
                switch (orb_num)
                {
                case 0:
                    fileout2 << "|       | ";
                    break;
                case 1:
                    fileout2 << "|" << symbol << "      | ";
                    break;
                case 2:
                    fileout2 << "|" << symbol << symbol << "     | ";
                    break;
                case 3:
                    fileout2 << "|" << symbol << symbol << symbol << "    | ";
                    break;
                case 4:
                    fileout2 << "|" << symbol << symbol << symbol << symbol << "   | ";
                    break;
                case 5:
                    fileout2 << "|" << symbol << symbol << symbol << symbol << symbol << "  | ";
                    break;
                case 6:
                    fileout2 << "|" << symbol << symbol << symbol << symbol << symbol << symbol << " | ";
                    break;
                default:
                    fileout2 << "|" << symbol << symbol << symbol << symbol << symbol << symbol << symbol << "| ";
                    break;
                }
            }
            fileout2 << endl;
        }
        fileout2 << "===========================================================================================\n";
        fileout2 << endl;
    }
    inline void record(int *Board_record, char *Board_color_record)
    {
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                Board_record[i * 6 + j] = Board_copy[i][j];
                Board_color_record[i * 6 + j] = Board_color[i][j];
            }
        }
    };
    inline void recover(int *Board_record, char *Board_color_record)
    {
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                Board_copy[i][j] = Board_record[i * 6 + j];
                Board_color[i][j] = Board_color_record[i * 6 + j];
            }
        }
    };
};

void algorithm_A(Board board, Player player, int index[])
{
    futureBoard currentBoard(board, player);
    currentBoard.findBestMove(board, index);
}

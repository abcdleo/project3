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

class Board_process
{
private:
    int Board_num[5][6];
    int Board_capacity[5][6];
    char Board_color[5][6];
    bool Board_exploded[5][6];
    friend class Board;

public:
    Board_process()
    {

        ////// Initialize the borad with correct capacity //////
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 6; j++)
            {
                Board_num[i][j] = 0;
                Board_capacity[i][j] = 8;
                Board_color[i][j] = 'w';
                Board_exploded[i][j] = false;
            }
        // The corners of the board
        Board_capacity[0][0] = 3, Board_capacity[0][5] = 3,
        Board_capacity[4][0] = 3, Board_capacity[4][5] = 3;

        // The edges of the board
        Board_capacity[0][1] = 5, Board_capacity[0][2] = 5, Board_capacity[0][3] = 5, Board_capacity[0][4] = 5,
        Board_capacity[1][0] = 5, Board_capacity[2][0] = 5, Board_capacity[3][0] = 5,
        Board_capacity[1][5] = 5, Board_capacity[2][5] = 5, Board_capacity[3][5] = 5,
        Board_capacity[4][1] = 5, Board_capacity[4][2] = 5, Board_capacity[4][3] = 5, Board_capacity[4][4] = 5;
    }
    void Board_copy(Board_process &object)
    {
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 6; j++)
            {
                this->Board_num[i][j] = object.get_orbs_num(i, j);
                this->Board_color[i][j] = object.get_cell_color(i, j);
            }
    }
    void place_orb(int i, int j, char color)
    {
        Board_num[i][j]++;
        Board_color[i][j] = color;
        if (this->cell_is_full(i, j))
        {
            this->cell_explode(i, j);
            this->cell_reaction_marker();
            this->cell_chain_reaction(color);
        }
    }
    bool cell_is_full(int i, int j)
    {
        if (Board_num[i][j] >= Board_capacity[i][j])
        {
            Board_exploded[i][j] = true;
            return true;
        }
        else
            return false;
    }
    void add_orb(int i, int j, char color)
    {
        Board_num[i][j]++;
        Board_color[i][j] = color;
    }

    void cell_reset(int i, int j)
    {
        Board_num[i][j] = 0;
        Board_color[i][j] = 'w';
    }

    void cell_explode(int i, int j)
    {

        int orb_num;
        char color = Board_color[i][j];

        cell_reset(i, j);

        if (i + 1 < 5)
        {
            add_orb(i + 1, j, color);
        }

        if (j + 1 < 6)
        {
            add_orb(i, j + 1, color);
        }

        if (i - 1 >= 0)
        {
            add_orb(i - 1, j, color);
        }

        if (j - 1 >= 0)
        {
            add_orb(i, j - 1, color);
        }
        if (i + 1 < 5 && j - 1 >= 0)
        {
            add_orb(i + 1, j - 1, color);
        }
        if (i - 1 >= 0 && j - 1 >= 0)
        {
            add_orb(i - 1, j - 1, color);
        }
        if (i + 1 < 5 && j + 1 < 6)
        {
            add_orb(i + 1, j + 1, color);
        }
        if (i - 1 >= 0 && j + 1 < 6)
        {
            add_orb(i - 1, j + 1, color);
        }
    }

    void cell_reaction_marker()
    {

        // Mark the next cell whose number of orbs is equal to the capacity
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                this->cell_is_full(i, j);
            }
        }
    }

    void cell_chain_reaction(char color)
    {

        bool chain_reac = true;

        while (chain_reac)
        {
            chain_reac = false;
            for (int i = 0; i < 5; i++)
            {
                for (int j = 0; j < 6; j++)
                {
                    if (Board_exploded[i][j])
                    {
                        this->cell_explode(i, j);
                        chain_reac = true;
                    }
                }
            }
            if (win_the_game(color))
                return;
            this->cell_reaction_marker();
        }
    }

    bool win_the_game(char player_color)
    {
        bool win = true;

        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (Board_color[i][j] == player_color || Board_color[i][j] == 'w')
                    continue;
                else
                {
                    win = false;
                    break;
                }
            }
            if (!win)
                break;
        }
        return win;
    }

    int get_orbs_num(int i, int j)
    {
        return Board_num[i][j];
    }

    int get_capacity(int i, int j)
    {
        return Board_capacity[i][j];
    }

    char get_cell_color(int i, int j)
    {
        return Board_color[i][j];
    }
    void set_orbs_num(int num, int i, int j)
    {
        Board_num[i][j] = num;
    }

    void set_cell_color(char color, int i, int j)
    {
        Board_color[i][j] = color;
    }
};

class futureBoard
{
private:
    Board_process originBoard;
    char color;
    char opponentColor;

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
                originBoard.set_orbs_num(board.get_orbs_num(i, j), i, j);
                originBoard.set_cell_color(board.get_cell_color(i, j), i, j);
            }
    };

    void findBestMove(Board board, int *put)
    {
        int BestMove = -99999999999, BestMoveRow, BestMoveCol;
        int thisMove;

        Board_process currentBoard;

        currentBoard.Board_copy(originBoard);

        //this->print_current_board();
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (board.get_cell_color(i, j) != opponentColor)
                {
                    currentBoard.place_orb(i, j, color);

                    thisMove = this->minmax(currentBoard, 1, opponentColor);
                    currentBoard.Board_copy(originBoard);
                    //cout << thisMove << endl;

                    if (BestMove < thisMove) //max
                    {
                        BestMove = thisMove;
                        BestMoveRow = i;
                        BestMoveCol = j;
                    }
                }
            }
        }

        //fileout2 << "after all : \n";                                            // test only
        //this->print_current_board();                                             // test only
        //fileout2 << "The End" << endl                                            //test only
        //         << BestMove << " " << BestMoveRow << " " << BestMoveCol << endl //test only
        //         << endl;                                                        //test only
        put[0] = BestMoveRow;
        put[1] = BestMoveCol;
    };

    inline int minmax(Board_process nodeBoard, int depth, char playerColor)
    {
        int BestScore = -99999999999, Score;
        Board_process currentBoard;

        //  fileout2 << "before this->game_processing() : \n"; // test only
        // this->print_current_board();                       // test only
        currentBoard.Board_copy(nodeBoard);

        if (currentBoard.win_the_game(color))
            return 99999999999;
        else if (currentBoard.win_the_game(opponentColor))
            return -99999999999;
        if (depth == 3)
            return calculateScore(currentBoard);

        if (playerColor == color) // my turn
        {
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 6; j++)
                {
                    if (currentBoard.get_cell_color(i, j) != opponentColor)
                    {
                        currentBoard.place_orb(i, j, color);
                        Score = this->minmax(currentBoard, depth + 1, opponentColor);
                        currentBoard.Board_copy(nodeBoard);
                        if (BestScore < Score) //max
                            BestScore = Score;
                    }
                }
            //this->recover(Board_record, Board_color_record);
            //fileout2 << "at the depth " << depth << ", after this->recover() : " << endl // test only
            //         << "BestScore = " << BestScore << endl;                             // test only
            //this->print_current_board();                                                 // test only                                                                           // test only
            return BestScore;
        }
        else // opponent turn
        {
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 6; j++)
                {
                    if (currentBoard.get_cell_color(i, j) != color)
                    {
                        currentBoard.place_orb(i, j, opponentColor);
                        Score = this->minmax(currentBoard, depth + 1, color);
                        currentBoard.Board_copy(nodeBoard);
                        if (BestScore > Score) // min
                            BestScore = Score;
                    }
                }
            //this->recover(Board_record, Board_color_record);
            //fileout2 << "at the depth " << depth << ", after this->recover() : " << endl // test only
            //         << "BestScore = " << BestScore << endl;                             // test only
            //this->print_current_board();                                                 // test only
            return BestScore;
        }
    };
    inline int calculateScore(Board_process currentBoard)
    {
        int score = 0;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (currentBoard.get_cell_color(i, j) != opponentColor)
                    score += calculateOrbScore(currentBoard, i, j, color);
                else
                    score -= calculateOrbScore(currentBoard, i, j, opponentColor);
            }
        }
        return score;
    }
    inline int calculateOrbScore(Board_process currentBoard, int row, int col, char playerColor)
    {
        int score = 0;
        int pos = row * 6 + col;
        int player_limit_count = 0, opponent_limit_count = 0;

        if (currentBoard.get_orbs_num(row, col) >= 1)
            score += 3510 * (currentBoard.get_orbs_num(row, col) + 10) / (currentBoard.get_capacity(row, col) + 10);
        for (int diff = -7; diff < 8; diff++)
        {
            bool out_of_range = false;

            if (diff == -4)
                diff = -1;
            else if (diff == 0)
                diff = 1;
            else if (diff == 2)
                diff = 5;

            if (row == 0 && diff >= -7 && diff <= -5)
                out_of_range = true;
            else if (row == 4 && diff >= 5 && diff <= 7)
                out_of_range = true;
            if (col == 0 && (diff == -7 || diff == -1 || diff == 5))
                out_of_range = true;
            else if (col == 5 && (diff == -5 || diff == 1 || diff == 7))
                out_of_range = true;

            if (!out_of_range)
            {
                int LookRow = (pos + diff) / 6;
                int LookCol = (pos + diff) % 6;
                if (currentBoard.get_cell_color(row, col) == playerColor || currentBoard.get_cell_color(row, col) == 'w') //opponent
                    score--;
            }
        }

        return score;
    }
    /*
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
    }*/
};

void algorithm_A(Board board, Player player, int index[])
{
    static int round = -1;
    round += 2;
    futureBoard currentBoard(board, player);
    currentBoard.findBestMove(board, index);
}

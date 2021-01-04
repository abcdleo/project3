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
    //friend class futureBoard;

public:
    Board_process()
    {
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
                this->Board_num[i][j] = object.get_num(i, j);
                this->Board_color[i][j] = object.get_color(i, j);
            }
    }
    void placeOrb(int i, int j, char color)
    {
        Board_num[i][j]++;
        Board_color[i][j] = color;
        if (this->isFull(i, j))
        {
            this->explode(i, j);
            this->chainReactionMarker();
            this->chainReaction(color);
        }
    }
    bool isFull(int i, int j)
    {
        if (Board_num[i][j] >= Board_capacity[i][j])
        {
            Board_exploded[i][j] = true;
            return true;
        }
        else
            return false;
    }
    void addOrb(int i, int j, char color)
    {
        Board_num[i][j]++;
        Board_color[i][j] = color;
    }
    void reset(int i, int j)
    {
        Board_num[i][j] = 0;
        Board_exploded[i][j] = false;
        Board_color[i][j] = 'w';
    }
    void explode(int i, int j)
    {

        int orb_num;
        char color = Board_color[i][j];

        reset(i, j);

        if (i + 1 < 5)
        {
            addOrb(i + 1, j, color);
        }

        if (j + 1 < 6)
        {
            addOrb(i, j + 1, color);
        }

        if (i - 1 >= 0)
        {
            addOrb(i - 1, j, color);
        }

        if (j - 1 >= 0)
        {
            addOrb(i, j - 1, color);
        }
        if (i + 1 < 5 && j - 1 >= 0)
        {
            addOrb(i + 1, j - 1, color);
        }
        if (i - 1 >= 0 && j - 1 >= 0)
        {
            addOrb(i - 1, j - 1, color);
        }
        if (i + 1 < 5 && j + 1 < 6)
        {
            addOrb(i + 1, j + 1, color);
        }
        if (i - 1 >= 0 && j + 1 < 6)
        {
            addOrb(i - 1, j + 1, color);
        }
    }
    void chainReactionMarker()
    {
        // Mark the next cell whose number of orbs is equal to the capacity
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                this->isFull(i, j);
            }
        }
    }
    void chainReaction(char color)
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
                        this->explode(i, j);
                        chain_reac = true;
                    }
                }
            }
            if (isWin(color))
                return;
            this->chainReactionMarker();
        }
    }
    bool isWin(char player_color)
    {
        int player = 0, opponent = 0, empty = 0;
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                if (Board_color[i][j] == player_color)
                    player++;
                else if (Board_color[i][j] == 'w')
                    empty++;
                else
                    opponent++;
            }
        }
        if (player + opponent >= 2 && opponent == 0)
            return true;
        else
            return false;
    }
    int get_num(int i, int j)
    {
        return Board_num[i][j];
    }
    int getCapacity(int i, int j)
    {
        return Board_capacity[i][j];
    }
    char get_color(int i, int j)
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
    void print_current_board()
    {

        int orb_num;
        char symbol;
        fileout2 << "=============================================================" << endl;
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {

                symbol = get_color(i, j);
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

                orb_num = get_num(i, j);
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
        fileout2 << "=============================================================" << endl
                 << endl;
    }
};

//void findBestMove(Board_process originBoard, int *put, char color, char opponentColor);
int minmax(Board_process nodeBoard, int depth, bool isMax, char color, char opponentColor);
int calculateScore(Board_process currentBoard, char color, char opponentColor);
int calculateOrbScore(Board_process currentBoard, int row, int col, char playerColor, bool isEmpty);
bool willexplosive(Board_process currentBoard, char Color, int row, int col);

void algorithm_A(Board board, Player player, int index[])
{
    //static int round = -1;
    //round += 2;

    //fileout2 << "hello : \n";

    char color = player.get_color();
    char opponentColor;
    Board_process originBoard;

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
    //originBoard.print_current_board();
    //findBestMove(originBoard, index, color, opponentColor);

    int BestMove = -999999999, BestMoveRow, BestMoveCol;
    int thisMove;

    Board_process currentBoard;

    currentBoard.Board_copy(originBoard);

    //this->print_current_board();
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (originBoard.get_color(i, j) != opponentColor)
            {
                currentBoard.placeOrb(i, j, color);
                //currentBoard.print_current_board(); //test only
                thisMove = minmax(currentBoard, 1, false, color, opponentColor);
                //fileout2 << "thismove = " << thisMove << endl;
                currentBoard.Board_copy(originBoard);
                //currentBoard.print_current_board(); //test only
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

    fileout2 << "after all : \n"; // test only
    //this->print_current_board();                                             // test only
    fileout2 << "The End" << endl                                            //test only
             << BestMove << " " << BestMoveRow << " " << BestMoveCol << endl //test only
             << endl;                                                        //test only
    index[0] = BestMoveRow;
    index[1] = BestMoveCol;
}

int minmax(Board_process nodeBoard, int depth, bool isMax, char color, char opponentColor)
{
    Board_process currentBoard;

    //  fileout2 << "before this->game_processing() : \n"; // test only
    // this->print_current_board();                       // test only
    currentBoard.Board_copy(nodeBoard);

    if (currentBoard.isWin(color))
        return 99999999;
    else if (currentBoard.isWin(opponentColor))
        return -99999999;
    if (depth == 3)
        return calculateScore(currentBoard, color, opponentColor);

    if (isMax) // my turn
    {
        int BestScore = -99999999, Score;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 6; j++)
            {
                if (currentBoard.get_color(i, j) != opponentColor)
                {
                    currentBoard.placeOrb(i, j, color);
                    //fileout2 << "after placeOrb :\n";
                    //currentBoard.print_current_board();
                    Score = minmax(currentBoard, depth + 1, false, color, opponentColor);
                    //fileout2 << "Score = " << Score << endl;
                    currentBoard.Board_copy(nodeBoard);
                    //fileout2 << "after recover :\n";
                    //currentBoard.print_current_board();
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
        int BestScore = 99999999, Score;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 6; j++)
            {
                if (currentBoard.get_color(i, j) != color)
                {
                    currentBoard.placeOrb(i, j, opponentColor);
                    //fileout2 << "after placeOrb :\n";
                    //currentBoard.print_current_board();
                    Score = minmax(currentBoard, depth + 1, true, color, opponentColor);
                    //fileout2 << "Score = " << Score << endl;
                    currentBoard.Board_copy(nodeBoard);
                    //fileout2 << "after recover :\n";
                    //currentBoard.print_current_board();
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
}
int calculateScore(Board_process currentBoard, char color, char opponentColor)
{
    int score = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        { /*
            if (currentBoard.get_color(i, j) == color)
                score += 3510 * (currentBoard.get_num(i, j) + 10) / (currentBoard.getCapacity(i, j) + 10);
            else if (currentBoard.get_color(i, j) == opponentColor)
                score -= 3510 * (currentBoard.get_num(i, j) + 10) / (currentBoard.getCapacity(i, j) + 10);
            */
            if (currentBoard.get_color(i, j) == color)
                score += calculateOrbScore(currentBoard, i, j, color, false);
            else if (currentBoard.get_color(i, j) == 'w')
                score += 0; //calculateOrbScore(currentBoard, i, j, color, true);
            else
                score -= calculateOrbScore(currentBoard, i, j, opponentColor, false);
        }
    }
    return score;
}
int calculateOrbScore(Board_process currentBoard, int row, int col, char playerColor, bool isEmpty)
{
    int score = 0;
    int pos = row * 6 + col;
    int fraction = currentBoard.get_num(row, col);
    int denominator = currentBoard.getCapacity(row, col);

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
            if (currentBoard.get_color(LookRow, LookCol) == playerColor)
                score += 3510 * (currentBoard.get_num(LookRow, LookCol) + 10) / (currentBoard.getCapacity(LookRow, LookCol) + 10);
        }
    }
    if (!isEmpty)
        score += 3510 * (fraction + 10) / (denominator + 10);
    return score;
}

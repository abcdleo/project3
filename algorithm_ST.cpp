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
ofstream fileout2("out2.data"); //test only

static int round = -1;

class Board_process
{
private:
    int Board_num[5][6];
    int Board_capacity[5][6];
    char Board_color[5][6];
    bool Board_exploded[5][6];

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

int minmax(Board_process nodeBoard, int depth, bool isMax, int a, int b, char color, char opponentColor);
int calculateScore(Board_process currentBoard, char color, char opponentColor);
int calculateOrbScore(Board_process currentBoard, int row, int col, char color, char opponentColor, bool isMax, bool isEmpty);
bool willexplosive(Board_process currentBoard, char Color, int row, int col);
int traverse(int count);
int weight(int i);
void algorithm_B(Board board, Player player, int index[])
{
    char color = player.get_color();
    char opponentColor;
    int BestMove = -100000000, BestMoveRow, BestMoveCol;
    int thisMove;
    bool cut = false;
    Board_process originBoard;

    round += 2;
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

    Board_process currentBoard;

    currentBoard.Board_copy(originBoard);

    for (int i = 0; i < 30 && !cut; i++)
    {
        int row = traverse(i) / 6;
        int col = traverse(i) % 6;
        if (originBoard.get_color(row, col) != opponentColor)
        {
            currentBoard.placeOrb(row, col, color);
            thisMove = minmax(currentBoard, 1, false, BestMove, 99999999, color, opponentColor);
            currentBoard.Board_copy(originBoard);
            if (BestMove < thisMove) //max
            {
                BestMove = thisMove;
                BestMoveRow = row;
                BestMoveCol = col;
            }
            if (99999999 <= BestMove)
                cut = true;
        }
    }
    fileout2 << "The End" << endl                                            //test only
             << BestMove << " " << BestMoveRow << " " << BestMoveCol << endl //test only
             << endl;                                                        //test only
    index[0] = BestMoveRow;
    index[1] = BestMoveCol;
}

int minmax(Board_process nodeBoard, int depth, bool isMax, int a, int b, char color, char opponentColor)
{
    bool cut = false;
    Board_process currentBoard;
    currentBoard.Board_copy(nodeBoard);

    if (currentBoard.isWin(color))
        return 99999999;
    else if (currentBoard.isWin(opponentColor))
        return -99999999;
    if (depth == 5) // 4 is safe
        return calculateScore(currentBoard, color, opponentColor);

    if (isMax) // my turn
    {
        int BestScore = -99999999, Score = a;
        for (int i = 0; i < 30 && !cut; i++)
        {
            int row = traverse(i) / 6;
            int col = traverse(i) % 6;
            if (currentBoard.get_color(row, col) != opponentColor)
            {
                currentBoard.placeOrb(row, col, color);
                Score = minmax(currentBoard, depth + 1, false, BestScore, b, color, opponentColor);
                currentBoard.Board_copy(nodeBoard);
                if (BestScore < Score) //max
                    BestScore = Score;
                if (b <= BestScore)
                    cut = true;
            }
        }
        return BestScore;
    }
    else // opponent turn
    {
        int BestScore = 99999999, Score = b;
        for (int i = 0; i < 30 && !cut; i++)
        {
            int row = traverse(i) / 6;
            int col = traverse(i) % 6;
            if (currentBoard.get_color(row, col) != color)
            {
                currentBoard.placeOrb(row, col, opponentColor);
                Score = minmax(currentBoard, depth + 1, true, a, BestScore, color, opponentColor);
                currentBoard.Board_copy(nodeBoard);
                if (BestScore > Score) // min
                    BestScore = Score;
                if (BestScore <= a)
                    cut = true;
            }
        }
        return BestScore;
    }
}
int traverse(int i)
{
    switch (i)
    {
    case 0:
        return 0;
    case 1:
        return 5;
    case 2:
        return 24;
    case 3:
        return 29;
    case 4:
        return 2;
    case 5:
        return 3;
    case 6:
        return 26;
    case 7:
        return 27;
    case 8:
        return 12;
    case 9:
        return 17;
    case 10:
        return 1;
    case 11:
        return 4;
    case 12:
        return 6;
    case 13:
        return 11;
    case 14:
        return 18;
    case 15:
        return 23;
    case 16:
        return 25;
    case 17:
        return 28;
    case 18:
        return 14;
    case 19:
        return 15;
    case 20:
        return 8;
    case 21:
        return 9;
    case 22:
        return 13;
    case 23:
        return 16;
    case 24:
        return 20;
    case 25:
        return 21;
    case 26:
        return 7;
    case 27:
        return 10;
    case 28:
        return 19;
    case 29:
        return 22;
    default:
        return 0;
    }
}
int weight(int i)
{
    if (i == 0 || i == 5 || i == 24 | i == 29) //corner
        return 14;
    else if (i == 2 || i == 3 || i == 26 | i == 27) //edges
        return 13;
    else if (i == 12 || i == 17) //edges
        return 12;
    else if (i == 1 || i == 4 || i == 25 | i == 28) //edges
        return 11;
    else if (i == 6 || i == 11 || i == 18 | i == 23) //edges
        return 10;
    else if (i == 14 || i == 15)
        return 6;
    else if (i == 8 || i == 9 || i == 20 | i == 21)
        return 5;
    else if (i == 13 || i == 16)
        return 4;
    else
        return 3;
}
int calculateScore(Board_process currentBoard, char color, char opponentColor)
{
    int score = 0;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (currentBoard.get_color(i, j) == color)
                score += 2 * calculateOrbScore(currentBoard, i, j, color, opponentColor, true, false);
            else if (currentBoard.get_color(i, j) == 'w')
                score += 0; //calculateOrbScore(currentBoard, i, j, color, true);
            else
                score -= calculateOrbScore(currentBoard, i, j, color, opponentColor, false, false);
        }
    }
    return score;
}
int calculateOrbScore(Board_process currentBoard, int row, int col, char color, char opponentColor, bool isMax, bool isEmpty)
{
    double score = 0;
    int my_limit = currentBoard.getCapacity(row, col) - currentBoard.get_num(row, col);
    if (!isEmpty)
    {
        if (round >= 30)
            score += 30 - 2 * (currentBoard.getCapacity(row, col) - currentBoard.get_num(row, col));
        score += 3 * weight(row * 6 + col);
    }
    if (round >= 60)
    {
        for (int i = -1; i < 2; i++)
        {
            for (int j = -1; j < 2; j++)
            {
                if (row + i >= 0 && row + i < 5 && col + j >= 0 && col + j < 6)
                    if (currentBoard.get_num(row + i, col + j) + 1 == currentBoard.getCapacity(row + i, col + j))
                        if (currentBoard.get_color(row + i, col + j) == color)
                            score += 2;
                        else
                        {
                            score = 0;
                            return score;
                        }
            }
        }
    }
    return score;
}
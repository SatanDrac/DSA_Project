#ifndef AI_AGENT_H
#define AI_AGENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

// Constants
#define BOARD_SIZE 3
#define MAX_MAZE_SIZE 20
#define MAX_QUEUE_SIZE 1000
#define MAX_STACK_SIZE 1000
#define MEMO_TABLE_SIZE 1000

// Common structures
typedef struct {
    int x, y;
} Point;

typedef struct {
    int x, y;
} MazePoint;

// Tic-Tac-Toe structures
typedef struct MemoEntry {
    char key[BOARD_SIZE * BOARD_SIZE + 1];
    int score;
    int move_i, move_j;
    struct MemoEntry* next;
} MemoEntry;

typedef struct {
    MemoEntry** table;
    int size;
    int count;
} MemoTable;

// Maze structures
typedef struct {
    MazePoint points[MAX_QUEUE_SIZE];
    int front, rear;
} MazeQueue;

typedef struct {
    MazePoint points[MAX_STACK_SIZE];
    int top;
} MazeStack;

typedef struct {
    char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
    int width, height;
    MazePoint start, end;
} Maze;

// Utility function declarations
void clearInputBuffer();
int getIntegerInput(const char* prompt, int min, int max);
float getFloatInput(const char* prompt, float min, float max);

// Tic-Tac-Toe function declarations
void playTicTacToeWithLevels();

// Maze solver function declarations
void solveMaze(int algorithm);

#endif
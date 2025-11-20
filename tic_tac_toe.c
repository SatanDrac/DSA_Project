#include "ai_agent.h"

// Tic-Tac-Toe Helper Functions
void initMemoTable(MemoTable* table, int size) {
    table->size = size;
    table->count = 0;
    table->table = (MemoEntry**)calloc(size, sizeof(MemoEntry*));
    if (table->table == NULL) {
        printf("Memory allocation failed for memo table!\n");
        exit(1);
    }
}

unsigned int hashString(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void memoInsert(MemoTable* table, const char* key, int score, int move_i, int move_j) {
    unsigned int index = hashString(key) % table->size;
    MemoEntry* newEntry = (MemoEntry*)malloc(sizeof(MemoEntry));
    if (newEntry == NULL) {
        printf("Memory allocation failed for memo entry!\n");
        return;
    }
    
    strcpy(newEntry->key, key);
    newEntry->score = score;
    newEntry->move_i = move_i;
    newEntry->move_j = move_j;
    newEntry->next = table->table[index];
    table->table[index] = newEntry;
    table->count++;
}

int memoLookup(MemoTable* table, const char* key, int* move_i, int* move_j) {
    unsigned int index = hashString(key) % table->size;
    MemoEntry* entry = table->table[index];
    
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            if (move_i) *move_i = entry->move_i;
            if (move_j) *move_j = entry->move_j;
            return entry->score;
        }
        entry = entry->next;
    }
    return INT_MIN;
}

void printBoard(int board[BOARD_SIZE][BOARD_SIZE]) {
    printf("\n   0   1   2\n");
    for (int i = 0; i < BOARD_SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < BOARD_SIZE; j++) {
            char symbol = ' ';
            if (board[i][j] == 1) symbol = 'X';
            else if (board[i][j] == -1) symbol = 'O';
            
            printf(" %c ", symbol);
            if (j < BOARD_SIZE - 1) printf("|");
        }
        printf("\n");
        if (i < BOARD_SIZE - 1) {
            printf("  ---+---+---\n");
        }
    }
    printf("\n");
}

int checkWinner(int board[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        int rowSum = 0, colSum = 0;
        for (int j = 0; j < BOARD_SIZE; j++) {
            rowSum += board[i][j];
            colSum += board[j][i];
        }
        if (abs(rowSum) == BOARD_SIZE) return board[i][0];
        if (abs(colSum) == BOARD_SIZE) return board[0][i];
    }
    
    int diag1 = board[0][0] + board[1][1] + board[2][2];
    int diag2 = board[0][2] + board[1][1] + board[2][0];
    
    if (abs(diag1) == BOARD_SIZE) return board[0][0];
    if (abs(diag2) == BOARD_SIZE) return board[0][2];
    
    int emptyCount = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 0) emptyCount++;
        }
    }
    if (emptyCount == 0) return 0;
    
    return INT_MIN;
}

void getAvailableMoves(int board[BOARD_SIZE][BOARD_SIZE], Point moves[], int* count) {
    *count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == 0) {
                moves[*count].x = i;
                moves[*count].y = j;
                (*count)++;
            }
        }
    }
}

void boardToString(int board[BOARD_SIZE][BOARD_SIZE], char* str) {
    int index = 0;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            str[index++] = '0' + (board[i][j] + 1);
        }
    }
    str[index] = '\0';
}

int minimax(int board[BOARD_SIZE][BOARD_SIZE], int depth, int isMaximizing, int alpha, int beta, MemoTable* memo, int* nodesEvaluated) {
    (*nodesEvaluated)++;
    
    char key[BOARD_SIZE * BOARD_SIZE + 1];
    boardToString(board, key);
    
    int cachedMoveI, cachedMoveJ;
    int cachedScore = memoLookup(memo, key, &cachedMoveI, &cachedMoveJ);
    if (cachedScore != INT_MIN) {
        return cachedScore;
    }
    
    int winner = checkWinner(board);
    if (winner != INT_MIN) {
        if (winner == 1) return 10 - depth;
        else if (winner == -1) return depth - 10;
        else return 0;
    }
    
    Point moves[BOARD_SIZE * BOARD_SIZE];
    int moveCount;
    getAvailableMoves(board, moves, &moveCount);
    
    if (isMaximizing) {
        int bestScore = INT_MIN;
        int bestMoveI = -1, bestMoveJ = -1;
        
        for (int i = 0; i < moveCount; i++) {
            int x = moves[i].x, y = moves[i].y;
            board[x][y] = 1;
            int score = minimax(board, depth + 1, 0, alpha, beta, memo, nodesEvaluated);
            board[x][y] = 0;
            
            if (score > bestScore) {
                bestScore = score;
                bestMoveI = x;
                bestMoveJ = y;
            }
            
            if (score > alpha) alpha = score;
            if (beta <= alpha) break;
        }
        
        memoInsert(memo, key, bestScore, bestMoveI, bestMoveJ);
        return bestScore;
    } else {
        int bestScore = INT_MAX;
        int bestMoveI = -1, bestMoveJ = -1;
        
        for (int i = 0; i < moveCount; i++) {
            int x = moves[i].x, y = moves[i].y;
            board[x][y] = -1;
            int score = minimax(board, depth + 1, 1, alpha, beta, memo, nodesEvaluated);
            board[x][y] = 0;
            
            if (score < bestScore) {
                bestScore = score;
                bestMoveI = x;
                bestMoveJ = y;
            }
            
            if (score < beta) beta = score;
            if (beta <= alpha) break;
        }
        
        memoInsert(memo, key, bestScore, bestMoveI, bestMoveJ);
        return bestScore;
    }
}

Point getAIMove(int board[BOARD_SIZE][BOARD_SIZE], MemoTable* memo, int* nodesEvaluated) {
    *nodesEvaluated = 0;
    clock_t start = clock();
    
    Point bestMove = {-1, -1};
    int bestScore = INT_MIN;
    
    Point moves[BOARD_SIZE * BOARD_SIZE];
    int moveCount;
    getAvailableMoves(board, moves, &moveCount);
    
    if (moveCount == 1) {
        bestMove = moves[0];
    } else {
        for (int i = 0; i < moveCount; i++) {
            int x = moves[i].x, y = moves[i].y;
            board[x][y] = 1;
            int score = minimax(board, 0, 0, INT_MIN, INT_MAX, memo, nodesEvaluated);
            board[x][y] = 0;
            
            if (score > bestScore) {
                bestScore = score;
                bestMove = moves[i];
            }
        }
    }
    
    clock_t end = clock();
    double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("AI evaluated %d nodes in %.4f seconds\n", *nodesEvaluated, timeTaken);
    printf("Memoization cache size: %d\n", memo->count);
    
    return bestMove;
}

Point getAIMoveWithDifficulty(int board[BOARD_SIZE][BOARD_SIZE], MemoTable* memo, int* nodesEvaluated, int difficulty) {
    Point moves[BOARD_SIZE * BOARD_SIZE];
    int moveCount;
    getAvailableMoves(board, moves, &moveCount);
    
    if (moveCount == 1) {
        return moves[0];
    }
    
    srand(time(NULL));
    
    switch (difficulty) {
        case 1: { // EASY: Mostly random moves
            if (rand() % 100 < 20) { // 20% chance to make a smart move
                return getAIMove(board, memo, nodesEvaluated);
            } else {
                return moves[rand() % moveCount];
            }
        }
        
        case 2: { // MEDIUM: Mix of random and smart moves
            if (rand() % 100 < 60) { // 60% chance to make a smart move
                return getAIMove(board, memo, nodesEvaluated);
            } else {
                return moves[rand() % moveCount];
            }
        }
        
        case 3: { // HARD: Always optimal
            return getAIMove(board, memo, nodesEvaluated);
        }
        
        default:
            return getAIMove(board, memo, nodesEvaluated);
    }
}

// Main Tic-Tac-Toe Game Function
void playTicTacToeWithLevels() {
    int board[BOARD_SIZE][BOARD_SIZE] = {{0}};
    int currentPlayer = 1;
    MemoTable memo;
    initMemoTable(&memo, MEMO_TABLE_SIZE);
    
    printf("\n=== TIC-TAC-TOE WITH DIFFICULTY LEVELS ===\n");
    printf("Choose difficulty level:\n");
    printf("1. Easy (Mostly random moves)\n");
    printf("2. Medium (Mix of random and smart)\n");
    printf("3. Hard (Perfect AI - Unbeatable)\n");
    
    int difficulty = getIntegerInput("Enter difficulty (1-3): ", 1, 3);
    
    char* difficultyNames[] = {"Easy", "Medium", "Hard"};
    printf("\nStarting game with %s difficulty!\n", difficultyNames[difficulty - 1]);
    printf("AI: X, You: O\n");
    
    printf("\nWho starts first?\n");
    printf("1. AI\n");
    printf("2. You\n");
    int startFirst = getIntegerInput("Enter choice (1-2): ", 1, 2);
    currentPlayer = (startFirst == 1) ? 1 : -1;
    
    while (1) {
        printBoard(board);
        int winner = checkWinner(board);
        
        if (winner != INT_MIN) {
            if (winner == 1) printf("AI wins!\n");
            else if (winner == -1) printf("You win! Congratulations!\n");
            else printf("It's a draw!\n");
            break;
        }
        
        if (currentPlayer == 1) {
            printf("AI is thinking...\n");
            int nodesEvaluated;
            Point move = getAIMoveWithDifficulty(board, &memo, &nodesEvaluated, difficulty);
            board[move.x][move.y] = 1;
            printf("AI plays at position (%d, %d)\n", move.x, move.y);
            currentPlayer = -1;
        } else {
            while (1) {
                printf("Enter your move (row column, 0-2 for both): ");
                int row, col;
                if (scanf("%d %d", &row, &col) == 2) {
                    if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == 0) {
                        board[row][col] = -1;
                        break;
                    } else {
                        printf("Invalid move! Try again.\n");
                    }
                } else {
                    printf("Please enter two valid numbers.\n");
                    clearInputBuffer();
                }
            }
            currentPlayer = 1;
        }
    }
    
    // Free memory
    for (int i = 0; i < memo.size; i++) {
        MemoEntry* entry = memo.table[i];
        while (entry != NULL) {
            MemoEntry* temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(memo.table);
}
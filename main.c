#include "ai_agent.h"

int main() {
    printf("========================================\n");
    printf("      AI AGENT: Games & Algorithms\n");
    printf("========================================\n");
    
    while (1) {
        printf("\nMain Menu:\n");
        printf("1. Tic-Tac-Toe with AI Levels\n");
        printf("2. Maze Solver with BFS\n");
        printf("3. Maze Solver with DFS\n");
        printf("4. Exit\n");
        
        int choice = getIntegerInput("Enter your choice (1-4): ", 1, 4);
        
        switch (choice) {
            case 1:
                playTicTacToeWithLevels();
                break;
            case 2:
                solveMaze(1);  // BFS
                break;
            case 3:
                solveMaze(2);  // DFS
                break;
            case 4:
                printf("Thanks for playing!\n");
                return 0;
        }
    }
    
    return 0;
}
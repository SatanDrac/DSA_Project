#include "ai_agent.h"

// Maze Helper Functions
void initMazeQueue(MazeQueue* q) {
    q->front = q->rear = -1;
}

void enqueueMaze(MazeQueue* q, MazePoint p) {
    if (q->rear == MAX_QUEUE_SIZE - 1) return;
    if (q->front == -1) q->front = 0;
    q->rear++;
    q->points[q->rear] = p;
}

MazePoint dequeueMaze(MazeQueue* q) {
    MazePoint p = q->points[q->front];
    if (q->front == q->rear) {
        q->front = q->rear = -1;
    } else {
        q->front++;
    }
    return p;
}

int isMazeQueueEmpty(MazeQueue* q) {
    return q->front == -1;
}

void initMazeStack(MazeStack* s) {
    s->top = -1;
}

void pushMaze(MazeStack* s, MazePoint p) {
    if (s->top == MAX_STACK_SIZE - 1) return;
    s->top++;
    s->points[s->top] = p;
}

MazePoint popMaze(MazeStack* s) {
    MazePoint p = s->points[s->top];
    s->top--;
    return p;
}

int isMazeStackEmpty(MazeStack* s) {
    return s->top == -1;
}

void generateMaze(Maze* maze, int width, int height, float wallDensity) {
    maze->width = width;
    maze->height = height;
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            maze->maze[i][j] = ' ';
        }
    }
    
    srand(time(NULL));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if ((float)rand() / RAND_MAX < wallDensity) {
                maze->maze[i][j] = '#';
            }
        }
    }
    
    maze->start.x = 0; maze->start.y = 0;
    maze->end.x = height - 1; maze->end.y = width - 1;
    maze->maze[0][0] = 'S';
    maze->maze[height-1][width-1] = 'E';
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (i < height && j < width) maze->maze[i][j] = ' ';
            if (height-1-i >= 0 && width-1-j >= 0) 
                maze->maze[height-1-i][width-1-j] = ' ';
        }
    }
    maze->maze[0][0] = 'S';
    maze->maze[height-1][width-1] = 'E';
}

void printMaze(Maze* maze, MazePoint path[], int pathLength) {
    printf("\n+");
    for (int j = 0; j < maze->width; j++) printf("-");
    printf("+\n");
    
    for (int i = 0; i < maze->height; i++) {
        printf("|");
        for (int j = 0; j < maze->width; j++) {
            int onPath = 0;
            for (int k = 0; k < pathLength; k++) {
                if (path[k].x == i && path[k].y == j && 
                    maze->maze[i][j] != 'S' && maze->maze[i][j] != 'E') {
                    onPath = 1;
                    break;
                }
            }
            
            if (onPath) printf("·");
            else printf("%c", maze->maze[i][j]);
        }
        printf("|\n");
    }
    
    printf("+");
    for (int j = 0; j < maze->width; j++) printf("-");
    printf("+\n");
}

int isValidMazeMove(Maze* maze, int x, int y, int visited[MAX_MAZE_SIZE][MAX_MAZE_SIZE]) {
    return (x >= 0 && x < maze->height && y >= 0 && y < maze->width &&
            maze->maze[x][y] != '#' && !visited[x][y]);
}

int bfsSolveMaze(Maze* maze, MazePoint path[], int* pathLength, int* nodesExplored) {
    int visited[MAX_MAZE_SIZE][MAX_MAZE_SIZE] = {0};
    MazePoint parent[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
    MazeQueue q;
    
    initMazeQueue(&q);
    enqueueMaze(&q, maze->start);
    visited[maze->start.x][maze->start.y] = 1;
    parent[maze->start.x][maze->start.y] = (MazePoint){-1, -1};
    *nodesExplored = 0;
    
    int directions[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    
    while (!isMazeQueueEmpty(&q)) {
        MazePoint current = dequeueMaze(&q);
        (*nodesExplored)++;
        
        if (current.x == maze->end.x && current.y == maze->end.y) {
            *pathLength = 0;
            MazePoint temp = current;
            
            while (temp.x != -1 && temp.y != -1) {
                path[*pathLength] = temp;
                (*pathLength)++;
                temp = parent[temp.x][temp.y];
            }
            
            for (int i = 0; i < *pathLength / 2; i++) {
                MazePoint temp = path[i];
                path[i] = path[*pathLength - 1 - i];
                path[*pathLength - 1 - i] = temp;
            }
            return 1;
        }
        
        for (int i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];
            
            if (isValidMazeMove(maze, nx, ny, visited)) {
                visited[nx][ny] = 1;
                parent[nx][ny] = current;
                enqueueMaze(&q, (MazePoint){nx, ny});
            }
        }
    }
    
    return 0;
}

int dfsSolveMaze(Maze* maze, MazePoint path[], int* pathLength, int* nodesExplored) {
    int visited[MAX_MAZE_SIZE][MAX_MAZE_SIZE] = {0};
    MazePoint parent[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
    MazeStack s;
    
    initMazeStack(&s);
    pushMaze(&s, maze->start);
    visited[maze->start.x][maze->start.y] = 1;
    parent[maze->start.x][maze->start.y] = (MazePoint){-1, -1};
    *nodesExplored = 0;
    
    int directions[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    
    while (!isMazeStackEmpty(&s)) {
        MazePoint current = popMaze(&s);
        (*nodesExplored)++;
        
        if (current.x == maze->end.x && current.y == maze->end.y) {
            *pathLength = 0;
            MazePoint temp = current;
            
            while (temp.x != -1 && temp.y != -1) {
                path[*pathLength] = temp;
                (*pathLength)++;
                temp = parent[temp.x][temp.y];
            }
            
            for (int i = 0; i < *pathLength / 2; i++) {
                MazePoint temp = path[i];
                path[i] = path[*pathLength - 1 - i];
                path[*pathLength - 1 - i] = temp;
            }
            return 1;
        }
        
        for (int i = 0; i < 4; i++) {
            int nx = current.x + directions[i][0];
            int ny = current.y + directions[i][1];
            
            if (isValidMazeMove(maze, nx, ny, visited)) {
                visited[nx][ny] = 1;
                parent[nx][ny] = current;
                pushMaze(&s, (MazePoint){nx, ny});
            }
        }
    }
    
    return 0;
}

// Main Maze Solver Function
void solveMaze(int algorithm) {
    int width = getIntegerInput("Enter maze width (5-20): ", 5, 20);
    int height = getIntegerInput("Enter maze height (5-20): ", 5, 20);
    float wallDensity = getFloatInput("Enter wall density (0.1-0.4): ", 0.1, 0.4);
    
    Maze maze;
    generateMaze(&maze, width, height, wallDensity);
    
    MazePoint path[MAX_MAZE_SIZE * MAX_MAZE_SIZE];
    int pathLength = 0;
    int nodesExplored = 0;
    
    printf("\n=== MAZE SOLVER ===\n");
    printf("Initial maze:\n");
    printMaze(&maze, path, pathLength);
    
    clock_t start = clock();
    int solved = 0;
    
    if (algorithm == 1) {
        printf("Solving using BFS...\n");
        solved = bfsSolveMaze(&maze, path, &pathLength, &nodesExplored);
    } else {
        printf("Solving using DFS...\n");
        solved = dfsSolveMaze(&maze, path, &pathLength, &nodesExplored);
    }
    
    clock_t end = clock();
    double timeTaken = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    if (solved) {
        printf("\nSolution found! Path length: %d\n", pathLength);
        printf("Nodes explored: %d\n", nodesExplored);
        printf("Time taken: %.4f seconds\n", timeTaken);
        printf("\nSolution path:\n");
        printMaze(&maze, path, pathLength);
    } else {
        printf("\nNo solution found!\n");
        printf("Nodes explored: %d\n", nodesExplored);
        printf("Time taken: %.4f seconds\n", timeTaken);
    }
}
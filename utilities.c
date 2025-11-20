#include "ai_agent.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n');
}

int getIntegerInput(const char* prompt, int min, int max) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1) {
            if (value >= min && value <= max) {
                clearInputBuffer();
                return value;
            }
        }
        printf("Invalid input! Please enter a number between %d and %d.\n", min, max);
        clearInputBuffer();
    }
}

float getFloatInput(const char* prompt, float min, float max) {
    float value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%f", &value) == 1) {
            if (value >= min && value <= max) {
                clearInputBuffer();
                return value;
            }
        }
        printf("Invalid input! Please enter a number between %.1f and %.1f.\n", min, max);
        clearInputBuffer();
    }
}
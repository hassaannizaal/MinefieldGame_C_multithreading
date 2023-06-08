//Muhammad_Hassaan_Nizaal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define Grid 10
#define N_Mines 15
#define N_Thread 4

typedef struct {
    int isMine;
    int isRevealed;
    int number;
} Tile;

Tile grid[Grid][Grid];
int isGameOver = 0;
int score = 0;

pthread_mutex_t gameMutex;
pthread_mutex_t scoreMutex;
pthread_mutex_t gridMutex;

void initializeGrid()
 {
    int i, j, count = 0;
    for (i = 0; i < Grid; i++) 
{
        for (j = 0; j < Grid; j++)
 {
            grid[i][j].isMine = 0;
            grid[i][j].isRevealed = 0;
            grid[i][j].number = 0;
        }
    }

    while (count < N_Mines) 
{
        int row = rand() % Grid;
        int col = rand() % Grid;
        if (!grid[row][col].isMine) 
{
            grid[row][col].isMine = 1;
            count++;
        }
    }
}

void revealTile(int row, int col) 
{
    if (row >= 0 && row < Grid && col >= 0 && col < Grid) 
{
        pthread_mutex_lock(&gridMutex);
        if (grid[row][col].isRevealed) {
            pthread_mutex_unlock(&gridMutex);
            return;
        }

        grid[row][col].isRevealed = 1;

        if (grid[row][col].isMine) 
{
            for (int i = 0; i < Grid; i++) 
{
                for (int j = 0; j < Grid; j++) 
{
                    grid[i][j].isRevealed = 1;
                }
            }
            printf("Game over! Mine exploded!\n");
            isGameOver = 1;
            pthread_mutex_unlock(&gridMutex);
            return;
        }

        int randomNum = rand() % 4; // Generates a random number between 0 and 3
        grid[row][col].number = randomNum;
        score += grid[row][col].number;

        pthread_mutex_unlock(&gridMutex);
    }
}

void printGrid() 
{
    int i, j;
    for (i = 0; i < Grid; i++) 
{
        for (j = 0; j < Grid; j++) 
{
            if (grid[i][j].isRevealed) 
{
                if (grid[i][j].isMine)
                    printf("⧆ ");
                else
                    printf("%d ", grid[i][j].number);
            } else {
                printf("▢ ");
            }
        }
        printf("\n");
    }
}



void* userInputThread(void* arg) 
{
    while (!isGameOver) 
{
        char input[10];
        int row, col;

        printf("Enter row and column (0 to 9) or 'exit' to end the game: ");
        fgets(input, 10, stdin);

        if (strcmp(input, "exit\n") == 0) 
{
            pthread_mutex_lock(&gameMutex);
            isGameOver = 1;
            pthread_mutex_unlock(&gameMutex);
            break;
        }

        int scanned = sscanf(input, "%d %d" , &row, &col);
        if (scanned != 2) {
            printf("Invalid input. Please enter two integers separated by a space.\n");
            continue;
        }

        if (row < 0 || row >= Grid || col < 0 || col >= Grid) 
{
            printf("Row and column must be between 0 and 9.\n");
            continue;
        }

        pthread_mutex_lock(&gridMutex);
        if (grid[row][col].isRevealed) 
{
            pthread_mutex_unlock(&gridMutex);
            printf("Tile already revealed. Please choose another tile.\n");
            continue;
        }
        pthread_mutex_unlock(&gridMutex);

        revealTile(row, col);

        pthread_mutex_lock(&gameMutex);
        if (isGameOver) 
{
            printf("\n");
            printGrid();
            pthread_mutex_unlock(&gameMutex);
            break;
        }
        pthread_mutex_unlock(&gameMutex);

        printGrid();
    }

    return NULL;
}



void* gridProcessingThread(void* arg) 
{
    long threadId = (long)arg;
    int startRow = (Grid / N_Thread) * threadId;
    int endRow = (Grid / N_Thread) * (threadId + 1);

    for (int i = startRow; i < endRow; i++) 
{
        for (int j = 0; j < Grid; j++) 
{
            pthread_mutex_lock(&gridMutex);
            if (grid[i][j].isMine) 
{
                pthread_mutex_unlock(&gridMutex);
                continue;
            }

            int count = 0;
            for (int x = i - 1; x <= i + 1; x++) 
{
                for (int y = j - 1; y <= j + 1; y++) 
{
                    if (x >= 0 && x < Grid && y >= 0 && y < Grid) 
{
                        if (grid[x][y].isMine)
                            count++;
                    }
                }
            }
            grid[i][j].number = count;

            pthread_mutex_unlock(&gridMutex);
        }
    }

    return NULL;
}


void displayMenu() 
{
    printf("\n");
    printf("----- Minefield Game Menu -----\n");
    printf("1. Start a New Game\n");
    printf("2. Game Instructions\n");
    printf("3. Exit\n");
}


int main() 
{
    srand(time(NULL));

    pthread_mutex_init(&gameMutex, NULL);
    pthread_mutex_init(&scoreMutex, NULL);
    pthread_mutex_init(&gridMutex, NULL);

    while (1) 
{
        displayMenu();
        int choice;
        printf("\n");
        printf("\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // Clear the newline character from the input buffer

        switch (choice) {
            case 1:
                
                pthread_mutex_lock(&gameMutex);
                isGameOver = 0;
                pthread_mutex_unlock(&gameMutex);

                pthread_mutex_lock(&scoreMutex);
                score = 0;
                pthread_mutex_unlock(&scoreMutex);

                initializeGrid();
                printGrid();

                pthread_t userInputThreadId;
                pthread_create(&userInputThreadId, NULL, userInputThread, NULL);

                pthread_t gridProcessingThreadIds[N_Thread];
                for (long i = 0; i < N_Thread; i++) {
                    pthread_create(&gridProcessingThreadIds[i], NULL, gridProcessingThread, (void*)i);
                }

                pthread_join(userInputThreadId, NULL);

                for (long i = 0; i < N_Thread; i++) {
                    pthread_join(gridProcessingThreadIds[i], NULL);
                }

                pthread_mutex_lock(&scoreMutex);
                printf("\n");
                printf("Game over! Final score: %d\n", score);
                pthread_mutex_unlock(&scoreMutex);

                break;

            case 2:
                // Display game instructions
                printf("----- Game Instructions -----\n");
                printf("1. You are presented with a grid of tiles.\n");
                printf("2. Some tiles contain hidden mines.\n");
                printf("3. You need to reveal the tiles without triggering a mine.\n");
                printf("4. Each tile you reveal will show a number indicating the number of adjacent mines.\n");
                printf("5. Enter the row and column of the tile you want to reveal.\n");
                printf("6. Your score will be the sum of the numbers on the revealed tiles.\n");
                printf("7. If you trigger a mine, the game will be over.\n\n");
                printf("_______________________Enjoy the game!_______________________\n");
                break;

            case 3:
                // Exit the game
                printf("Exiting the game...\n");
                pthread_mutex_destroy(&gameMutex);
                pthread_mutex_destroy(&scoreMutex);
                pthread_mutex_destroy(&gridMutex);
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

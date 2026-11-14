#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// sys
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define SLEEP(s) Sleep((s) * 1000)
#define CREATE_DIR(path) _mkdir(path)
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#define SLEEP(s) sleep(s)
#define CREATE_DIR(path) mkdir(path, 0755)
#endif

#define COLOR_GREEN "\x1b[32m"

int durationPomo;
char WPERM = 'w';
char APERM = 'r';

int main(void);

void initStorage(void)
{
    CREATE_DIR("data");
}

void loadSettings(void)
{
    FILE *f = fopen("data/settings.dat", "r");

    if (f == NULL)
    {
        return;
    }

    char key[16];
    int value;

    while (fscanf(f, "%15s %d", key, &value) == 2)
    {
        if (strcmp(key, "d") == 0)
        {
            durationPomo = value;
        }
    }

    fclose(f);
}

void saveSettings(void)
{
    FILE *f = fopen("data/settings.dat", "w");
    if (f == NULL)
    {
        return;
    }

    fprintf(f, "d %d\n", durationPomo);
    fclose(f);
}

void cleanScreen(void)
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void makeArt(void)
{
    FILE *file;
    char buffer[256];
    file = fopen("assets/treeart.txt", "r");

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(file);
}

void startPomodoro(void)
{
    int totalTime = durationPomo * 60;
    printf("Starting pomodoro...\n");

    makeArt();

    while (totalTime >= 0)
    {
        int m = totalTime / 60;
        int s = totalTime % 60;

        printf("\rTime remaining: %02d:%02d", m, s);
        fflush(stdout);

        if (totalTime == 0)
        {
            break;
        }

        SLEEP(1);
        totalTime--;
    }

    printf("\n\nTime's up!\n");
}

void configPomodoro(void)
{
    int placeholder;

    do
    {
        printf("Duration (In minutes): ");
        scanf("%d", &placeholder);
        if (placeholder <= 0)
        {
            printf("Negative values or 0 is not valid. Try again.\n");
        }
        else
        {
            durationPomo = placeholder;
            break;
        }
    } while (placeholder <= 0);

    /*
    while (placeholder <= 0)
    {
        printf("Duration (In minutes): ");
        scanf("%d", &placeholder);
        if (placeholder <= 0)
        {
            printf("Negative values or 0 is not valid. Try again.\n");
        }
        else
        {
            durationPomo = placeholder;
            break;
        }
    }*/

    saveSettings();
}

int main()
{
    int choose;

    initStorage();
    loadSettings();

    do
    {
        printf(COLOR_GREEN "1: Start Pomodoro!\n");
        printf("2: Settings\n");
        printf("3: Exit\n");
        printf(": ");

        scanf("%d", &choose);

        switch (choose)

        {
        case 1:
            cleanScreen();
            startPomodoro();
            cleanScreen();
            break;
        case 2:
            cleanScreen();
            configPomodoro();
            cleanScreen();
            break;
        case 3:
            cleanScreen();
            printf("Goodbye!");
            break;
        default:
            printf("Invalid option.\n");
            break;
        }

    } while (choose != 3);

    return 0;
}
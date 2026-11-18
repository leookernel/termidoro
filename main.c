#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// sys
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define SLEEP(s) Sleep((s) * 1000)
#define CREATE_DIR(path) _mkdir(path)
DWORD oldMode;
HANDLE hInput;
#else
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#define SLEEP(s) sleep(s)
#define CREATE_DIR(path) mkdir(path, 0755)
struct termios oldt, newt;
#endif

#define COLOR_GREEN "\x1b[32m"

int durationPomo;
int durationBreak;
int sessions;
char errorMessage[] = "Negative values or 0 is not valid. Try again.\n";

int main(void);

void disableTerm(void)
{
#ifdef _WIN32
    hInput = GetStdHandle(STD_INPUT_HANDLE);

    if (GetConsoleMode(hInput, &oldMode))
    {
        SetConsoleMode(hInput, oldMode & ~ENABLE_ECHO_INPUT);
    }
#else
    if (tcgetattr(STDIN_FILENO, &oldt) == 0)
    {
        newt = oldt;
        newt.c_lflag &= ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }
#endif
}

void enableTerm(void)
{
#ifdef _WIN32
    SetConsoleMode(hInput, oldMode);
#else
    tcsetattr(STDERR_FILENO, TCSANOW, &oldt);
#endif
}

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
        else if (strcmp(key, "b") == 0)
        {
            durationBreak = value;
        }
        else if (strcmp(key, "s") == 0)
        {
            sessions = value;
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
    fprintf(f, "b %d\n", durationBreak);
    fprintf(f, "s %d\n", sessions);
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

void makeArt(char art[])
{
    FILE *file;
    char buffer[256];
    if (strcmp(art, "treeart") == 0)
    {
        file = fopen("assets/treeart.txt", "r");
    }
    else if (strcmp(art, "logoart") == 0)
    {
        file = fopen("assets/termidoro.txt", "r");
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        printf("%s", buffer);
    }

    fclose(file);
}

static int readInt(const char *prompt)
{
    char line[64];
    int value;

    while (1)
    {
        printf("%s", prompt);

        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            return -1;
        }

        if (sscanf(line, "%d", &value) == 1 && value > 0)
        {
            return value;
        }

        printf("%s", errorMessage);
    }
}

void startTimer(const char *prompt, int totalTime, int count, int sessions)
{
    while (totalTime >= 0)
    {
        int m = totalTime / 60;
        int s = totalTime % 60;

        printf(prompt, count, sessions, m, s);
        fflush(stdout);

        if (totalTime == 0)
        {
            break;
        }

        SLEEP(1);
        totalTime--;
    }
}

void startPomodoro(void)
{
    int totalPomoTime = durationPomo * 60;
    int totalBreakTime = durationBreak * 60;

    makeArt("treeart");

    disableTerm();

    int count = 1;
    while (count < sessions + 1)
    {
        startTimer("\rSession: %d/%d - Time remaining: %02d:%02d\033[K", totalPomoTime, count, sessions);
        startTimer("\rSession: %d/%d - Break remaining: %02d:%02d\033[K", totalBreakTime, count, sessions);
        count++;
    }

    enableTerm();
}

void configPomodoro(void)
{
    int value;

    if ((value = readInt("Timer duration (In minutes): ")) <= 0)
        return;
    durationPomo = value;

    if ((value = readInt("Break duration (In minutes): ")) <= 0)
        return;
    durationBreak = value;

    if ((value = readInt("Sessions: ")) <= 0)
        return;
    sessions = value;

    saveSettings();
}

int main()
{
    int choose;

    initStorage();
    loadSettings();
    cleanScreen();
    printf(COLOR_GREEN "");

    do
    {
        makeArt("logoart");

        printf("\n\n1: Start Pomodoro!\n");
        printf("2: Settings\n");
        printf("3: Exit\n");
        printf(": ");

        scanf("%d", &choose);
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;

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
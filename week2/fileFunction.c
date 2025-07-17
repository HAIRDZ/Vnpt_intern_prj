#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_MODE_LENGTH      4
#define MAX_LINE_LENGTH      256
#define MAX_CMD_LENGTH       10

static FILE *currentFile = NULL;
static char fileName[MAX_FILENAME_LENGTH];

void openFile(void) {
    if (currentFile) {
        fclose(currentFile);
        currentFile = NULL;
    }

    printf("Enter filename: ");
    if (fgets(fileName, sizeof(fileName), stdin)) {
        size_t len = strlen(fileName);
        if (len > 0 && fileName[len - 1] == '\n') {
            fileName[len - 1] = '\0';
        }
    }

    char fileMode[MAX_MODE_LENGTH];
    printf("Mode (r/w/a): ");
    if (fgets(fileMode, sizeof(fileMode), stdin)) {
        size_t len = strlen(fileMode);
        if (len > 0 && fileMode[len - 1] == '\n') {
            fileMode[len - 1] = '\0';
        }
    }

    currentFile = fopen(fileName, fileMode);
    if (!currentFile) {
        perror("fopen");
    } else {
        printf("File '%s' opened in mode '%s'.\n", fileName, fileMode);
    }
}

void readFile(void) {
    if (!currentFile) {
        printf("No file open.\n");
        return;
    }

    fseek(currentFile, 0, SEEK_SET);
    int ch;
    while ((ch = fgetc(currentFile)) != EOF) {
        putchar(ch);
    }
    putchar('\n');
}

void writeFile(void) {
    if (!currentFile) {
        printf("No file open.\n");
        return;
    }

    printf("Enter text (empty line to finish):\n");
    char lineBuffer[MAX_LINE_LENGTH];
    while (fgets(lineBuffer, sizeof(lineBuffer), stdin)) {
        if (lineBuffer[0] == '\n') {
            break;
        }
        fputs(lineBuffer, currentFile);
    }
    fflush(currentFile);
}

void closeFile(void) {
    if (currentFile) {
        fclose(currentFile);
        currentFile = NULL;
        printf("File closed.\n");
    } else {
        printf("No file to close.\n");
    }
}

void clearConsole(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

int main(void) {
    char command[MAX_CMD_LENGTH];

    while (1) {
        clearConsole();
        printf("\n=== FILE MENU ===\n");
        printf("1 - Open\n");
        printf("2 - Read\n");
        printf("3 - Write\n");
        printf("4 - Close\n");
        printf("exit - Exit\n");
        printf("Enter command: ");

        if (!fgets(command, sizeof(command), stdin)) {
            continue;
        }
        size_t len = strlen(command);
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }

        if (strcmp(command, "1") == 0) {
            openFile();
        } else if (strcmp(command, "2") == 0) {
            readFile();
        } else if (strcmp(command, "3") == 0) {
            writeFile();
        } else if (strcmp(command, "4") == 0) {
            closeFile();
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Invalid command.\n");
        }

        printf("\nPress Enter to continue...");
        getchar();
    }

    if (currentFile) {
        fclose(currentFile);
    }

    return 0;
}

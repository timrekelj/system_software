#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1000

void appendToFile(const char *filename) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Napaka pri odpiranju datoteke");
        return;
    }

    char line[MAX_LINE_LENGTH];
    printf("Vnesite vrstice za dodajanje (Ctrl + D za konec vnosa):\n");

    while (fgets(line, MAX_LINE_LENGTH, stdin) != NULL) {
        fputs(line, file);
    }

    fclose(file);
}

void deleteLine(const char *filename, int lineNumber) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Napaka pri odpiranju datoteke");
        return;
    }

    char line[MAX_LINE_LENGTH];
    FILE *tempFile = fopen("tempfile", "w");
    int currentLine = 1;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (currentLine != lineNumber) {
            fputs(line, tempFile);
        }
        currentLine++;
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("tempfile", filename);
}

void insertLine(const char *filename, int lineNumber) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Napaka pri odpiranju datoteke");
        return;
    }

    char line[MAX_LINE_LENGTH];
    FILE *tempFile = fopen("tempfile", "w");
    int currentLine = 1;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        if (currentLine == lineNumber) {
            char newLine[MAX_LINE_LENGTH];
            printf("Vnesite novo vrstico za vstavljanje: ");
            fgets(newLine, MAX_LINE_LENGTH, stdin);
            fputs(newLine, tempFile);
        }
        fputs(line, tempFile);
        currentLine++;
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename("tempfile", filename);
}

void changeLine(const char *filename, int lineNumber) {
    deleteLine(filename, lineNumber);
    insertLine(filename, lineNumber);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uporaba: %s <filename> [a | d <n> | i <n> | c <n>]\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];

    char operation = argv[2][0];

    switch (operation) {
    case 'a':
        appendToFile(filename);
        break;
    case 'd':
        if (argc == 4) {
            int lineNumber = atoi(argv[3]);
            deleteLine(filename, lineNumber);
        } else {
            printf("Uporaba: %s <filename> d <n>\n", argv[0]);
            return 1;
        }
        break;
    case 'i':
        if (argc == 4) {
            int lineNumber = atoi(argv[3]);
            insertLine(filename, lineNumber);
        } else {
            printf("Uporaba: %s <filename> i <n>\n", argv[0]);
            return 1;
        }
        break;
    case 'c':
        if (argc == 4) {
            int lineNumber = atoi(argv[3]);
            changeLine(filename, lineNumber);
        } else {
            printf("Uporaba: %s <filename> c <n>\n", argv[0]);
            return 1;
        }
        break;
    default:
        printf("Nepodprt ukaz: %c\n", operation);
        return 1;
    }

    return 0;
}

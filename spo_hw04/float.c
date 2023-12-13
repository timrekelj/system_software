#include <stdio.h>

int main(int argc, char *argv[]) {
    // Create new file if there is no arguments
    if (argc == 1) {
        FILE *file = fopen("stevila", "wb");
        if (file == NULL) {
            perror("Napaka pri ustvarjanju datoteke");
            return 1;
        }

        int count = 5;
        fwrite(&count, sizeof(int), 1, file);

        // Create array of floats to be written in file
        float numbers[] = {1.11, 2.22, 3.33, 4.44, 5.55};

        // Write numbers in file
        fwrite(numbers, sizeof(float), count, file);

        fclose(file);
    // Read from file if you get filename in arguments
    } else if (argc == 2) {
        FILE *file = fopen(argv[1], "rb");
        if (file == NULL) {
            perror("Napaka pri odpiranju datoteke");
            return 1;
        }
        int count;
        fread(&count, sizeof(int), 1, file);

        printf("Seznam stevil v datoteki:\n");

        // Get all floats from file and print them
        float number;
        for (int i = 0; i < count; i++) {
            fread(&number, sizeof(float), 1, file);
            printf("%.2f\n", number);
        }

        fclose(file);
    } else {
        printf("Wrong usage");
        return -1;
    }
    return 0;
}

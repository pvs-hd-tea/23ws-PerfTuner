#include <stdio.h>

#define BUFFER_SIZE 4096

int main() {
    FILE* file = fopen("input.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytesRead = 0;
    do {
        bytesRead = fread(buffer, 1, BUFFER_SIZE, file);
        // Process the read data
        // ...
    } while (bytesRead == BUFFER_SIZE);

    if (ferror(file)) {
        perror("Error reading file");
        return -1;
    }

    fclose(file);
    return 0;
}
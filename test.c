#include <stdio.h>
#include <stdlib.h>

char *int2bin(int a, char *buffer, int buf_size) {
    buffer += (buf_size - 1);

    for (int i = 31; i >= 0; i--) {
        *buffer-- = (a & 1) + '0';

        a >>= 1;
    }

    return buffer;
}

#define BUF_SIZE 33

int main() {
    char buffer[BUF_SIZE];
    buffer[BUF_SIZE - 1] = '\0';

    int num = 22;

    int2bin(num, buffer, BUF_SIZE - 1);

    printf("a = %s", buffer);
}
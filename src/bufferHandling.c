#include <stdio.h>

#include "bufferHandling.h"
#include "def.h"

int readUARTLine(char* lineBuffer) {
	
    int i = 0;
    int ci;

    while (1) {
        ci = getchar();
        if (ci == EOF) return -1;

        unsigned char c = (unsigned char)ci;

        if (c == '\r' || c == '\n') {
            lineBuffer[i] = '\0';
            fputs("\r\n", stdout);
            fflush(stdout);


            return i;
        }

        if (c == '\b' || c == 127) {
            if (i > 0) {
                i--;
                putchar('\b'); putchar(' '); putchar('\b');
                fflush(stdout);
            }
            continue;
        }

        if (i < MAX_BUFFER_LENGTH - 1) {
            lineBuffer[i++] = c;
            putchar(c);
            fflush(stdout);
        }
    }
}

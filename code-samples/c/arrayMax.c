#include <stdio.h>

int main() {
    int I, MAX;
    int X[21];

    X[0] = 11;
    X[1] = 66;
    X[2] = 128;
    X[3] = 49;
    X[4] = 133;
    X[5] = 46;
    X[6] = 15;
    X[7] = 87;
    X[8] = 55;
    X[9] = 37;
    X[10] = 78;
    X[11] = 44;
    X[12] = 33;
    X[13] = 38;
    X[14] = 85;
    X[15] = 6;
    X[16] = 150;
    X[17] = 4;
    X[18] = 1;
    X[19] = 55;
    X[20] = 78;
  
    for (I = 0; I < 21; ++I)
        printf("%d\n", X[I]);

    MAX = X[0];
    for (I = 0; I < 21; ++I)
        if(MAX < X[I])
            MAX = X[I];
    printf("%d\n", MAX);

    return 0;
}

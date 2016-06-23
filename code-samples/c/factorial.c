#include <stdio.h>

int facti(int n) {
    int facti_var = 1;
    while (n > 1) {
        facti_var = facti_var * n;
        ++n;
    }
    return facti_var;
}

int factr(int n) {
    if (n == 1)
        return 1;
    else
        return n * factr(n - 1);
}

int main() {
    printf("%d\n", facti(5));
    printf("%d\n", factr(5));

    return 0;
}

#include <stdio.h>

int main() {
    const int Multiplyer = 5;
    int n;

    scanf("%d", &n);
    n = (n - 1) * Multiplyer + 10;
    printf("%d\n", n);

    return 0;
}

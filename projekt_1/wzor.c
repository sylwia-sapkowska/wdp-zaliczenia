#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "zbior_ary.h"

int main() {
    int op;
    scanf("%d", &op);

    zbior_ary *tab = (zbior_ary*)malloc((unsigned)(op+2) * sizeof(zbior_ary));
    int cnt = 1;
    
    while (op--){
        char c;
        scanf(" %c", &c);

        if (c == 'C') {
            int a, q, b;
            scanf("%d %d %d", &a, &q, &b);
            tab[cnt++] = ciag_arytmetyczny(a, q, b);
            continue;
        }
        if (c == 'S'){
            int a;
            scanf("%d", &a);
            tab[cnt++] = singleton(a);
            continue;
        }
        if (c == 'U'){
            int a, b;
            scanf("%d %d", &a, &b);
            tab[cnt++] = suma(tab[a], tab[b]);
            continue;
        }
        if (c == 'I'){
            int a, b;
            scanf("%d %d", &a, &b);
            tab[cnt++] = iloczyn(tab[a], tab[b]);
            continue;
        }
        if (c == 'R'){
            int a, b;
            scanf("%d %d", &a, &b);
            tab[cnt++] = roznica(tab[a], tab[b]);
            continue;
        }
        if (c == 'N'){
            int a, b;
            scanf("%d %d", &a, &b);
            printf("%d\n", nalezy(tab[a], b));
            continue;
        }
        if (c == 'M'){
            int a;
            scanf("%d", &a);
            printf("%d\n", moc(tab[a]));
            continue;
        }
        if (c == 'A'){
            int a;
            scanf("%d", &a);
            printf("%d\n", ary(tab[a]));
            continue;
        }
    }
    return 0;
}

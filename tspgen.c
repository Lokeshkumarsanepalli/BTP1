/* tspgen.c  --  "TSP Generator"
 * Input : file.seq  (one test vector or response per line, bit i = flip-flop i)
 * Output: file.gr   (N, then an NxN weight matrix)
 *   a[i][j] = a[j][i] = number of patterns where bit i != bit j
 *   a[i][i] = -1
 * i.e. the total bit-difference between flip-flops i and j over all vectors
 * and responses. This is the weighted graph of the paper (Fig 1).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: %s in.seq out.gr\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 1; }

    char **pat = NULL; int np = 0, cap = 0, n = 0;
    char line[4096];
    while (fgets(line, sizeof line, f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (!line[0] || line[0] == '#') continue;
        if (!n) n = (int)strlen(line);
        else if ((int)strlen(line) != n) { fprintf(stderr, "pattern length mismatch\n"); return 1; }
        if (np == cap) { cap = cap ? cap * 2 : 64; pat = realloc(pat, cap * sizeof *pat); }
        pat[np++] = strdup(line);
    }
    fclose(f);

    int *a = calloc((size_t)n * n, sizeof *a);
    for (int p = 0; p < np; p++)
        for (int i = 0; i < n; i++)
            for (int j = i + 1; j < n; j++)
                if (pat[p][i] != pat[p][j]) { a[i*n+j]++; a[j*n+i]++; }
    for (int i = 0; i < n; i++) a[i*n+i] = -1;

    FILE *o = fopen(argv[2], "w");
    fprintf(o, "%d\n", n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) fprintf(o, "%d ", a[i*n+j]);
        fprintf(o, "\n");
    }
    fclose(o);
    printf("tspgen: %d flip-flops, %d patterns -> %s\n", n, np, argv[2]);
    return 0;
}

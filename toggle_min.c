/* toggle_min.c  --  picks the best cut of the ring (scan-in / scan-out choice)
 * Input : file.seq, file.ring
 * Output: file.ch  (final chain order, 0-based indices, first = scan-in)
 * Weighted transitions of one pattern = sum over adjacent bit changes at
 * position p (1-based) of (N - p). Try all N rotations of the ring, keep
 * the one with the smallest total over all patterns.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 4) { fprintf(stderr, "usage: %s in.seq in.ring out.ch\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1], "r");
    char **pat = NULL; int np = 0, cap = 0, n = 0; char line[4096];
    while (fgets(line, sizeof line, f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (!line[0] || line[0] == '#') continue;
        if (!n) n = (int)strlen(line);
        if (np == cap) { cap = cap ? cap * 2 : 64; pat = realloc(pat, cap * sizeof *pat); }
        pat[np++] = strdup(line);
    }
    fclose(f);

    int *ring = malloc(n * sizeof *ring);
    f = fopen(argv[2], "r");
    for (int i = 0; i < n; i++) fscanf(f, "%d", &ring[i]);
    fclose(f);

    long best = -1; int bestk = 0;
    for (int k = 0; k < n; k++) {
        long total = 0;
        for (int p = 0; p < np; p++)
            for (int pos = 1; pos < n; pos++) {
                char x = pat[p][ring[(k + pos - 1) % n]];
                char y = pat[p][ring[(k + pos) % n]];
                if (x != y) total += n - pos;
            }
        printf("cut %d: ", k);
        for (int i = 0; i < n; i++) printf("ff%d%s", ring[(k+i)%n] + 1, i < n-1 ? "-" : "");
        printf("  weighted transitions = %ld\n", total);
        if (best < 0 || total < best) { best = total; bestk = k; }
    }
    f = fopen(argv[3], "w");
    for (int i = 0; i < n; i++) fprintf(f, "%d\n", ring[(bestk + i) % n]);
    fclose(f);
    printf("toggle_min: best = %ld, scan-in = ff%d, scan-out = ff%d\n",
           best, ring[bestk] + 1, ring[(bestk + n - 1) % n] + 1);
    return 0;
}

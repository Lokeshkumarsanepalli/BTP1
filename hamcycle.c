/* hamcycle.c  --  "TSP Solver" (greedy nearest-neighbour heuristic)
 * Input : file.gr
 * Output: file.ring  (flip-flop indices, 0-based, in cycle order)
 * Start at ff1 (index 0). From the current node always take the least
 * weighted edge to a node not yet visited. Cost: O(n^2).
 * The result is a Hamiltonian cycle (last node connects back to the first).
 */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 3) { fprintf(stderr, "usage: %s in.gr out.ring\n", argv[0]); return 1; }
    FILE *f = fopen(argv[1], "r");
    if (!f) { perror(argv[1]); return 1; }
    int n; fscanf(f, "%d", &n);
    int *a = malloc((size_t)n * n * sizeof *a);
    for (int i = 0; i < n * n; i++) fscanf(f, "%d", &a[i]);
    fclose(f);

    int *seen = calloc(n, sizeof *seen), *ring = malloc(n * sizeof *ring);
    int cur = 0; seen[0] = 1; ring[0] = 0;
    long cost = 0;
    for (int k = 1; k < n; k++) {
        int best = -1;
        for (int j = 0; j < n; j++)
            if (!seen[j] && (best < 0 || a[cur*n+j] < a[cur*n+best])) best = j;
        cost += a[cur*n+best];
        seen[best] = 1; ring[k] = best; cur = best;
    }
    cost += a[cur*n+0];                       /* closing edge */

    FILE *o = fopen(argv[2], "w");
    for (int k = 0; k < n; k++) fprintf(o, "%d\n", ring[k]);
    fclose(o);

    printf("hamcycle: cycle = ");
    for (int k = 0; k < n; k++) printf("ff%d-", ring[k] + 1);
    printf("ff1   (cycle weight %ld)\n", cost);
    return 0;
}

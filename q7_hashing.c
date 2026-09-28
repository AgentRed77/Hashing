/* Q7: Hash table (Division Method + Linear Probing) vs Linear Search */
#include <stdio.h>
#include <stdlib.h>
#define EMPTY -1
#define MAXN 50

int hash(int key, int m) { return key % m; }          /* Division Method */

void show(int t[], int m) {
    for (int i = 0; i < m; i++) {
        if (t[i] == EMPTY) printf("[%d:--] ", i);
        else printf("[%d:%d] ", i, t[i]);
    }
    printf("\n");
}

/* returns number of collisions (probes beyond the first) */
int insert(int t[], int m, int key) {
    int h = hash(key, m), i = 0;
    while (i < m && t[(h + i) % m] != EMPTY) i++;
    if (i == m) { printf("Table full\n"); return 0; }
    t[(h + i) % m] = key;
    return i;
}

int hash_search(int t[], int m, int key, int *probes) {
    int h = hash(key, m); *probes = 0;
    for (int i = 0; i < m; i++) {
        int idx = (h + i) % m; (*probes)++;
        if (t[idx] == EMPTY) return -1;       /* stop at empty slot */
        if (t[idx] == key) return idx;
    }
    return -1;
}

int linear_search(int a[], int n, int key, int *comps) {
    *comps = 0;
    for (int i = 0; i < n; i++) { (*comps)++; if (a[i] == key) return i; }
    return -1;
}

void run(int a[], int n, int m, int verbose) {
    int t[MAXN], total_coll = 0;
    for (int i = 0; i < m; i++) t[i] = EMPTY;
    printf("\n=== Table size m = %d, h(k) = k mod %d ===\n", m, m);
    for (int i = 0; i < n; i++) {
        int home = hash(a[i], m);
        int c = insert(t, m, a[i]);
        total_coll += c;
        if (verbose) {
            printf("Insert %d -> h=%d %s\n  ", a[i], home,
                   c ? "** COLLISION **" : "(no collision)");
            if (c) printf("(placed %d slots later)\n  ", c);
            show(t, m);
        }
    }
    printf("Final table: "); show(t, m);
    printf("Extra probes caused by collisions: %d | Load factor = %d/%d = %.2f\n",
           total_coll, n, m, (double)n / m);
}

int main(void) {
    int a[MAXN], n = 0, m = 10;
    FILE *f = fopen("input.txt", "r");
    if (!f) { printf("input.txt missing\n"); return 1; }
    while (fscanf(f, "%d", &a[n]) == 1) n++;
    fclose(f);

    printf("Song IDs:"); for (int i = 0; i < n; i++) printf(" %d", a[i]); printf("\n");
    run(a, n, m, 1);

    /* rebuild table for searching */
    int t[MAXN]; for (int i = 0; i < m; i++) t[i] = EMPTY;
    for (int i = 0; i < n; i++) insert(t, m, a[i]);

    int keys[] = {105, 525, 840, 999};
    printf("\n--- Search comparison (m = %d) ---\n", m);
    printf("%-6s | %-12s | %-12s\n", "Key", "Hash probes", "Linear comps");
    int th = 0, tl = 0;
    for (int k = 0; k < 4; k++) {
        int p, c;
        int r1 = hash_search(t, m, keys[k], &p);
        int r2 = linear_search(a, n, keys[k], &c);
        printf("%-6d | %-3d (%s) | %-3d (%s)\n", keys[k], p,
               r1 >= 0 ? "found    " : "not found", c, r2 >= 0 ? "found" : "not found");
        th += p; tl += c;
    }
    printf("Average: hashing = %.2f, linear = %.2f\n", th / 4.0, tl / 4.0);

    /* extra: prime table size */
    run(a, n, 11, 0);
    for (int i = 0; i < 11; i++) t[i] = EMPTY;
    for (int i = 0; i < n; i++) insert(t, 11, a[i]);
    th = 0;
    for (int k = 0; k < 4; k++) { int p; hash_search(t, 11, keys[k], &p); th += p; }
    printf("Average probes with m = 11: %.2f\n", th / 4.0);
    return 0;
}

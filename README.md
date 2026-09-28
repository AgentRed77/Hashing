# PCCST303 – Assignment 2, Question 7: Hashing for a Music Application

**Data:** song IDs `105, 210, 315, 420, 525, 630, 735, 840`
**Files:** `q7_hashing.c` (source) · `input.txt` (input data) · `output.txt` (program output) · this README (analysis, table, conclusion)
**Run:** `gcc q7_hashing.c -o q7 && ./q7`

## a) Hash table (Division Method, m = 10, linear probing)
h(k) = k mod 10. Every ID ends in 0 or 5, so only slots 0 and 5 are ever "home" slots.

| Insert | h(k) | Collision? | Placed at | Table after insertion (slot:key) |
|---|---|---|---|---|
| 105 | 5 | No | 5 | 5:105 |
| 210 | 0 | No | 0 | 0:210, 5:105 |
| 315 | 5 | **Yes** | 6 | 0:210, 5:105, 6:315 |
| 420 | 0 | **Yes** | 1 | 0:210, 1:420, 5:105, 6:315 |
| 525 | 5 | **Yes** | 7 | + 7:525 |
| 630 | 0 | **Yes** | 2 | + 2:630 |
| 735 | 5 | **Yes** | 8 | + 8:735 |
| 840 | 0 | **Yes** | 3 | + 3:840 |

Final table: `[0:210] [1:420] [2:630] [3:840] [4:--] [5:105] [6:315] [7:525] [8:735] [9:--]`
**6 of 8 insertions collided** (12 extra probes in total). Slots 0–3 and 5–8 form two clusters (primary clustering).

## b) Search results (comparisons)
| Key | Hashing probes | Linear search comps | Result |
|---|---|---|---|
| 105 | 1 | 1 | found |
| 525 | 3 | 5 | found |
| 840 | 4 | 8 | found |
| 999 | 1 | 8 | not found |
| **Average** | **2.25** | **5.50** | |

## c) Analysis
**Load factor:** α = n/m = 8/10 = **0.8**.

**Effect of collisions.** All keys are multiples of 105 and 10 shares a factor (5) with 105, so keys only land in slots 0 and 5. Keys placed later sit further from home (525 needs 3 probes, 840 needs 4). The worst case here, 4 probes, is already half of a linear scan. Unsuccessful search for a key whose home slot is 0 or 5 would need 5 probes (e.g. 1050 → slots 0,1,2,3,4).

**Theory vs observation**
| | Hashing (linear probing) | Linear search |
|---|---|---|
| Average (successful) | ≈ ½(1 + 1/(1−α)) = 3.0 at α = 0.8 (observed 2.67 over the 3 stored keys) | n/2 = 4 (observed 4.67) |
| Average (unsuccessful) | ≈ ½(1 + 1/(1−α)²) = 13 (theory, large-table formula) | n = 8 |
| Best case | O(1) | O(1) |
| Worst case | O(n) (all keys in one cluster) | O(n) |
| Expected with a good hash and low α | **O(1)** | O(n) |
| Space | O(m) | O(n) |

(The unsuccessful-search theory is for large tables with random hashing; our 999 search took only 1 probe because its home slot 9 happened to be empty. The clustered slots 0–3 and 5–8 make lookups for keys hashing to 0 or 5 the costly ones.)

**Better table size.** With a prime m = 11 (extra run in the program) the IDs land in distinct slots (0 collisions, load 0.73) and the average search drops to **1.25 probes**. The problem is the poor choice of m = 10, not hashing itself.

## Comparison table
| Criterion | Hashing (m = 10) | Hashing (m = 11) | Linear search |
|---|---|---|---|
| Collisions | 6 of 8 inserts | 0 | n/a |
| Avg. comparisons (4 searches) | 2.25 | 1.25 | 5.50 |
| Insert | O(1) avg | O(1) | O(1) append |
| Search | O(1) avg, O(n) worst | O(1) | O(n) |
| Extra space | table of m slots | m slots | none beyond array |
| Needs sorted data | No | No | No |

## Final conclusion
Hashing is suitable for the music application, because song lookup by ID is exact-match, and it needs about 2.25 comparisons on average against 5.50 for linear search, a gap that grows with the catalogue size (O(1) vs O(n)). Its performance depends on the table size: m = 10 clusters these keys (all multiples of 105/5), whereas a prime size such as 11 removed all collisions. For a real catalogue, choose a prime m with load factor ≤ 0.7, and use chaining or double hashing to resolve collisions. Hash tables do not support ordered/range queries, so if those are needed a balanced BST would be better.

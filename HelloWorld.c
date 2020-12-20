#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#define SQRZ 4        // # squares
int sqr[] = { 1, 2,   // ze squares
	      2, 3,   
	      3, 1,   
	      4, 1 };
int scz;              // # scenarios
struct rec {     // record
    int e;       // enclosure
    int u;       // layoUt
    int flip;    // flip bits
    int p[SQRZ]; // permutation
} *rec;              
enum { A, B, C, D, E, F }; // layout types
// ..
void store(int e, int u, int flip, int *p)
{
    static int j = 0;

    struct rec *r = &rec[j++];
    
    r->e = e;
    r->u = u;
    r->flip = flip;
    for (int i = 0; i < SQRZ; i++)
	r->p[i] = p[i];
}
// factorial
int fctl(int n)
{
    int f;

    for (f = n; --n > 1; f *= n)
	;

    return f;
}
// wtf that is the question
void initp(int *p)
{
    for (int j = 0; j < SQRZ; j++) p[j] = j;
}
// thatz
int max(int n, ...)
{
    va_list ap;
    int c, m;   // current, maximum

    va_start(ap, n);
    m = va_arg(ap, int);
    for (int j = n; j > 1; j--)
    	if ((c = va_arg(ap, int)) > m) m = c;
    va_end(ap);

    return m;
}
// get ze bit at position p
int getbit(int n, int p)
{
    return (n & (1 << p)) >> p;
}
// get side length of (j + 1)th square
int getlen(int side, int j, int flip)
{
    int offset = getbit(flip, j) ? 1 : 0;

    if (side) offset = !offset; // y 

    return *(sqr + j*2 + offset);
}
#define x(J) getlen(0, p[(J) - 1], flip)
#define y(J) getlen(1, p[(J) - 1], flip)
// do the math
void visit(int u, int flip, int *p)
{
    int x, y;
    switch (u) {
    case A:
    	/* +---+---+---+---+
    	 * |   |   |   |   |
    	 * |   |   |   |   |
    	 * |   |   |   |   |
    	 * | 1 | 2 | 3 | 4 |
    	 * |   |   |   |   |
    	 * |   |   |   |   |
    	 * |   |   |   |   |
    	 * +---+---+---+---+ */
    	x = x(1) + x(2) + x(3) + x(4);
    	y = max(4, y(1), y(2), y(3), y(4));
    	break;
    case B:
    	/* +-------+-------+
    	 * |   1   |       |
    	 * +-------+       |
    	 * |   2   |       |
    	 * +-------+   4   |
    	 * |       |       |
    	 * |   3   |       |
    	 * |       |       |
    	 * +-------+-------+ */
    	x = max(3, x(1), x(2), x(3)) + x(4);
    	y = max(2, y(1) + y(2) + y(3), y(4));
    	break;
    case C:
    	/* +-------+-------+
    	 * |       |   3   |
    	 * |   1   +-------+
    	 * |       |   4   |
    	 * +-------+-------+
    	 * |               |
    	 * |       2       |
    	 * |               |
    	 * +---------------+ */
    	x = max(2, x(1) + max(2, x(3), x(4)), x(2));
    	y = max(2, y(1), y(3) + y(4)) + y(2);
    	break;
    case D:
    	/* +---+---+-------+
    	 * |   |   |       |
    	 * | 1 |   |       |
    	 * |   |   |       |
    	 * +---+ 3 |   4   |
    	 * |   |   |       |
    	 * | 2 |   |       |
    	 * |   |   |       |
    	 * +---+---+-------+ */
    	x = max(2, x(1), x(2)) + x(3) + x(4);
    	y = max(3, y(1) + y(2), y(3), y(4));
    	break;
    case E:
    	/* +-------+-------+
    	 * |       |       |
    	 * |   1   |   3   |
    	 * |       |       |
    	 * +-------+-------+
    	 * |       |       |
    	 * |   2   |   4   |
    	 * |       |       |
    	 * +-------+-------+ */
    	x = max(2, x(1), x(2)) + max(2, x(3), x(4));
    	y = max(2, y(1), y(3)) + max(2, y(2), y(4));
    	break;
    default:
    	break;
    }
    store(x*y, u, flip, p);
}
// odd predicate
#define oddp(N) ((N) & 1)
// yef!
#define swap(A, I, J) ({			\
	    typeof (*A) t = A[I];		\
	    A[I] = A[J];			\
	    A[J] = t; })
// generate permutations (Heap's algorithm)
void genp(int u, int flip, int *p)
{
    static int c[SQRZ];

    int j = SQRZ;

    do { c[--j] = 0; } while (j);
    visit(u, flip, p);
    while (j < SQRZ) {
    	if (c[j] < j) {
	    oddp(j) ? swap(p, c[j], j) : swap(p, 0, j);
    	    visit(u, flip, p);
    	    c[j]++;
    	    j = 0;
    	} else {
    	    c[j] = 0;
    	    j++;
    	}
    }
}
// dump record
void dumprec(int j)
{
    static char *ch = "ABCDE";

    struct rec *r = &rec[j];
    
    printf("%-12s%d\n", "enclosure", r->e);
    printf("%-12s%c\n", "layout", *(ch + r->u));
    printf("%-12s", "flip");
    for (j = SQRZ - 1; j > -1; j--)
	printf("%d", getbit(r->flip, j));
    printf("\n%-12s", "permutation");
    for (j = 0; j < SQRZ; j++)
	printf((j < SQRZ - 1) ? "%d " : "%d\n", r->p[j]);
}
// OK
void bblesort(void)
{    
    int t, j;
    int bound = scz - 1;
    for (;;) {
	t = 0;
	for (j = 0; j < bound; j++) {
	    if (rec[j].e > rec[j + 1].e) {
		swap(rec, j, j + 1);
		t = j;
	    }
	}
	if (t == 0) break;
	bound = t;
    }
}
// dump result
void spit(void)
{
    int j = 0;

    do { dumprec(j++); } while (rec[j].e == rec[0].e);
}
int main(void)
{
    int flipz = 1 << SQRZ; // # flips
    int u, flip;
    int p[SQRZ];
    
    scz = F*flipz*fctl(SQRZ);
    rec = calloc(scz, sizeof *rec);
    //			  ¿!! test zone !!!
    for (u = A; u < F; u++) {
    	for (flip = 0; flip < flipz; flip++) {
    	    initp(p);
    	    genp(u, flip, p);
    	}
    }
    bblesort();
    spit();
    free(rec);
    return 0;
}

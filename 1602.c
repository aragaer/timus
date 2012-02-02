#include <stdio.h>

#ifdef ONLINE_JUDGE
#define debug(x)
#else
#define debug(x) printf x
#endif

int main() {
    int n, k, floor;
    float u, v;
    float t1, t2, t3;

    scanf("%d %d %f %f", &n, &k, &u, &v);
    debug(("%d\n%d\n%f\n%f\n", n, k, u, v));

    floor = n - ((k-1)*v + 15)/u;
    if (floor < 1)
        floor = 1;

    debug(("Waiting for elevator at floor %d will take %f seconds\n", floor+1, (k-1)*v + 15 + floor * v * 2 + 5 ));
    debug(("Calling elevator from floor %d will take %f seconds\n", floor, (n-floor)*u + (floor-1) * v * 2 + 5 ));
    debug(("Going down to floor 1 will take %f seconds\n", (n-1)*u));

    t1 = (k-1)*v + 15 + floor * v * 2 + 5; /* Going down to floor+1 and waiting a bit */
    t2 = (n-floor)*u + (floor-1) * v * 2 + 5; /* Going down to floor and calling immediately */
    t3 = (n-1)*u; /* Going all the way */

    if (t1 <= t2)
        floor = t1 <= t3
            ? floor + 1
            : 1;
    else
        floor = t2 <= t3
            ? floor
            : 1;

    printf("%d\n", floor);
    exit(0);
}


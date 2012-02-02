#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define ONLINE_JUDGE
#define MAX_TOKENS 50

#ifdef ONLINE_JUDGE
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...)
#endif

#define NO_VALUE    -1

typedef long long int val_t;

val_t concat(val_t val, val_t tok) {
    char tmp[10];
    val_t res;
    if (val == NO_VALUE && tok == NO_VALUE)
        return 1;
    if (tok == NO_VALUE) {
        debug("concat %llu and (nothing)\n", val);
        return val;
    }
    if (val == NO_VALUE) {
        debug("got token %llu\n", tok);
        return tok;
    }
    debug("concat %llu and %llu\n", val, tok);
    sprintf(tmp, "%llu%llu", val, tok);
    sscanf(tmp, "%llu", &res);
    return res;
}

val_t sum(val_t val, val_t tok) {
    if (val == NO_VALUE)
        val = 0;
    if (tok == NO_VALUE)
        tok = 1;
    debug("add %llu and %llu\n", val, tok);
    return val + tok;
}

val_t mul(val_t val, val_t tok) {
    if (val == NO_VALUE)
        val = 1;
    debug("multiply %llu and %llu\n", val, tok);
    return val * tok;
}

val_t divide(val_t val, val_t tok) {
    if (val == NO_VALUE) {
        debug("got value to divide = %llu\n", tok);
        return tok;
    }
    debug("divide %llu by %llu\n", val, tok);
    return val / tok;
}

typedef val_t (*eval_func)(val_t, val_t);

__inline val_t dv(eval_func f) {
    return 1;
//    return f == mul || f == divide;
}

char *expr;

val_t my_eval(eval_func func) {
    val_t val = NO_VALUE, tok;

    debug("in my_eval for string %s\n", expr);

    while (1) {
        if (!isdigit(*expr) || sscanf(expr, "%llu", &tok) == 0)
            tok = NO_VALUE;
        debug("scanned token %lld\n", tok);
        while (isdigit(*expr))
            expr++;

        if (*expr)
            debug("looking at symbol [%c]\n", *expr);
        else
            debug("end of line\n");
        switch (*expr++) {
        case '\r':
            continue;
        case ';':
            val = func(val, tok);
            break;
        case ')':
        case '\0':
            debug("end of subexpression\n");
            return func(val, tok);
        case '/':
            debug("div begins\n");
            tok = concat(tok, my_eval(divide));
            debug("done calculating div\n");
            return func(val, tok);
        case '*':
            debug("mul begins\n");
            tok = concat(tok, my_eval(mul));
            debug("done calculating mul\n");
            return func(val, tok);
        case '+':
            debug("sum begins\n");
            tok = concat(tok, my_eval(sum));
            debug("done calculating sum\n");
            return func(val, tok);
        case '(':
            debug("subexpr begins\n");
            tok = concat(tok, my_eval(concat));
            debug("subexpr ends\n");
            val = func(val, tok);
            break;
        }
        debug("val = %lld\n", val, tok);
    }
    return val;
}

int main() {
    int l = 1;
    char line[51];
    while (scanf("%s", line) != EOF) {
        expr = line;
        printf("Expression %d evaluates to: %llu\n", l++, my_eval(concat));
    }
    debug("exiting\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ONLINE_JUDGE
#define MAX_WORD_COUNT  50000
#define MAX_WORD_LEN    50

char words[MAX_WORD_COUNT][MAX_WORD_LEN];

int pool_used;
struct node_s {
#ifndef ONLINE_JUDGE
    char name[MAX_WORD_LEN];
#endif
    struct node_s *children[10];
    char *word;
} pool[MAX_WORD_COUNT*MAX_WORD_LEN], root;

char num_from_letter[256], nl_code[] = "22233344115566070778889990";

char number[101];
struct solution {
    int len;
    char *word;
    struct solution *next;
} known_solutions[101];

#define NO_BEST 101

#ifndef ONLINE_JUDGE
#define debug(...) printf(__VA_ARGS__)
void print_tree(struct node_s *node, int depth) {
    int i, j;
    if (node->word)
        printf(" (w)\n");
    else
        printf("\n");
    for (i = 0; i < 10; i++)
        if (node->children[i]) {
            for (j = 0; j < depth; j++)
                printf(" ");
            printf("%d", i);
            print_tree(node->children[i], depth+1);
        }
}
#else
#define debug(...)
#endif

/*@null@*/
static __inline struct node_s *new_node(void) {
    int i;
    struct node_s *new = pool + pool_used;
    pool_used++;
    new->word = NULL;
    for (i = 0; i < 10; i++)
        new->children[i] = NULL;
    return new;
}

struct solution *solve_from_position(int pos) {
    char *num = number + pos;
    struct node_s *ptr = &root;
    struct solution *s = known_solutions + pos, *f;
#ifndef ONLINE_JUDGE
    int i;
    printf("%s(%d) = %s\n", __func__, pos, num);
#endif
    if (s->len)
        return s;

    s->len = NO_BEST;
    for (; number[pos]; pos++) { // number goes on
        int n = number[pos] - '0';
        if (ptr->word) { // we can end the word here, let's see what happens
            debug("accept word '%s' and continue\n", ptr->word);
            f = solve_from_position(pos);
            if (f->len + 1 < s->len) {
                debug("word '%s' is good, we've got us a %d words-long subsolution which is better than %d\n", ptr->word, f->len+1, s->len);
                s->word = ptr->word;
                s->len = f->len + 1;
                s->next = f;
            }
        }
#ifndef ONLINE_JUDGE
        for (i = 0; i < 10; i++)
            if (ptr->children[i])
                printf("can pick code %d\n", i);
#endif

        // keep going on a longer word
        if (ptr->children[n]) // alright!
            ptr = ptr->children[n];
        else // can't advance
            return s;
        debug("acceptable code %d in position %d\n", n, pos);
    }
    debug("word ended\n");

// that's the end of the number

    if (ptr->word) {    // we won
        debug("Everyting ends with '%s'\n", ptr->word);
        s->len = 1;
        s->word = ptr->word;
        s->next = NULL;
    }
    return s;
}

int main() {
    int dict_size, i;
    char tmp[MAX_WORD_LEN + 1];
    for (i = 'a'; i <= 'z'; i++)
        num_from_letter[i] = nl_code[i - 'a'];
    while (1) {
        int l;
        char letter, *word, *wptr;
        struct solution *s;
        scanf("%s", number);
        if (number[0] == '-')
            break;

#ifndef ONLINE_JUDGE
        printf("Number %s\n", number);
#endif
        l = strlen(number);

        for (i = 0; i < 10; i++)
            root.children[i] = NULL;
        for (i = 0; i < 101; i++)
            known_solutions[i].len = 0;
        pool_used = 0;

        (void) scanf("%d", &dict_size);
        (void) scanf("%c", &letter); // newline after number
        if (letter == '\r')
            (void) scanf("%c", &letter);
        for (i = 0; i < dict_size; i++) {
            struct node_s *ptr = &root;
            for (wptr = word = words[i]; ; wptr++) {
                int n;
                (void) scanf("%c", wptr);
                if (*wptr == '\r')
                    (void) scanf("%c", wptr);

                if (*wptr == '\n') {
                    *wptr = '\0';
                    if (ptr->word == NULL)
                        ptr->word = word;
                    break;
                }

                n = num_from_letter[*wptr] - '0';

                if (ptr->children[n] == NULL)
                    ptr->children[n] = new_node();
                ptr = ptr->children[n];
            };
        }
#ifndef ONLINE_JUDGE
        print_tree(&root, 0);
        printf("used up %d nodes\n", pool_used);
#endif
        solve_from_position(0);
        if (known_solutions->len == NO_BEST)
            printf("No solution.\n");
        else
            for (s = known_solutions; s; s = s->next)
                printf("%s%c", s->word, s->next ? ' ' : '\n');
    }
}


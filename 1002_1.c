#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ONLINE_JUDGE
#define MAX_WORD_COUNT  50000
#define MAX_WORD_LEN    50
#define MAX_CHARS 300*1024
#define NUMLEN 100

char words[MAX_CHARS];

int pool_used;
struct node_s {
    struct node_s *children[10];
    char *word;
} pool[MAX_CHARS], root;

char num_from_letter[256], nl_code[] = "22233344115566070778889990";

char number[NUMLEN+1];
struct solution {
    int len;
    char *word;
    struct solution *next;
} known_solutions[NUMLEN];

#define NO_BEST NUMLEN + 1

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
    struct node_s *ptr = &root;
    struct solution *s = known_solutions + pos, *f;
    int n;
#ifndef ONLINE_JUDGE
    printf("%s(%d) = %s\n", __func__, pos, number + pos);
#endif
    if (s->len) // solved already
        return s;

    s->len = NO_BEST;
    for (; number[pos]; pos++) { // number goes on
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

        n = number[pos] - '0';

        // keep going on a longer word
        if (ptr->children[n]) // alright!
            ptr = ptr->children[n];
        else // can't advance
            return s;
        debug("acceptable code %d in position %d\n", n, pos);
    }

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
    for (i = 'a'; i <= 'z'; i++)
        num_from_letter[i] = nl_code[i - 'a'];
    while (1) {
        char *word, *wptr;
        struct solution *s;
        scanf("%s", number);
        if (number[0] == '-')
            break;

#ifndef ONLINE_JUDGE
        printf("Number %s\n", number);
#endif
        for (i = 0; i < 10; i++)
            root.children[i] = NULL;
        for (i = 0; i < NUMLEN; i++)
            known_solutions[i].len = 0;
        pool_used = 0;
        wptr = words;

        (void) scanf("%d", &dict_size);
        (void) scanf("\n\r"); // newline after number
        for (i = 0; i < dict_size; i++) {
            struct node_s *ptr = &root;
            for (word = wptr; ; wptr++) {
                int n;
                (void) scanf("%c", wptr);
                if (*wptr == '\r')
                    (void) scanf("%c", wptr);

                if (*wptr == '\n') {
                    *wptr = '\0';
                    if (ptr->word == NULL)
                        ptr->word = word;
                    wptr++;
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
        printf("used up %d bytes of words storage\n", wptr - words);
#endif
        solve_from_position(0);
        if (known_solutions->len == NO_BEST)
            printf("No solution.\n");
        else
            for (s = known_solutions; s; s = s->next)
                printf("%s%c", s->word, s->next ? ' ' : '\n');
    }
}


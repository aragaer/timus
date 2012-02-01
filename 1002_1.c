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
char *solution[200];
int known_solutions[101];

#define NO_BEST 101

#ifndef ONLINE_JUDGE
#define debug(...) printf(__VA_ARGS__)
void print_tree(struct node_s *node, int depth) {
    int i;
    for (i = 0; i < depth; i++)
        printf("\t");
    printf("%s", node->name);
    if (node->word)
        printf(" (w)\n");
    else
        printf("\n");
    for (i = 0; i < 10; i++)
        if (node->children[i])
            print_tree(node->children[i], depth+1);
}
#else
#define debug(...)
#endif

/*@null@*/
static __inline struct node_s *new_node(void) {
    int i;
    struct node_s *new = pool + pool_used;
    pool_used++;
    for (i = 0; i < 10; i++)
        new->children[i] = NULL;
    return new;
}

int try_solve(char *num, int limit) {
    struct node_s *ptr = &root;
#ifndef ONLINE_JUDGE
    printf("%s(%s, %d)\n", __func__, num, limit);
#endif
    while (1) {
        int n;
        if (!*num)      // that's the end of the number
            if (ptr->word) {    // we won
                debug("Everyting ends with '%s'\n", ptr->word);
                solution[0] = ptr->word;
                return 1;
            } else {
                debug("number ended. that was wrong way... return %d\n", limit+1);
                return limit;
            }

        // number goes on
        n = *num - '0';
        if (ptr->word) { // fork here
            int *res = known_solutions + (num - number);
            debug("accept word '%s' and continue\n", ptr->word);
            if (*res == 0)
                *res = try_solve(num, limit - 1);
            if (*res < limit - 1) {
                solution[*res] = ptr->word;
                limit = *res+1;
            }
        }

        // we have no choice - keep going
        if (ptr->children[n]) { // alright!
            ptr = ptr->children[n];
            num++;
        } else {
            debug("can't continue. that was wrong way. return %d\n", limit+1);
            return limit; // can't advance
        }
    }
}

int main() {
    int dict_size, i;
    char tmp[MAX_WORD_LEN + 1];
    for (i = 'a'; i <= 'z'; i++)
        num_from_letter[i] = nl_code[i - 'a'];
    while (1) {
        int l;
        char letter, *word, *wptr;
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
            known_solutions[i] = 0;
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
#ifndef ONLINE_JUDGE
                    if (ptr == &root) {
                        printf("the fuck?! word %d of %d %s\n", i, dict_size, word);
                        exit(1);
                    }
#endif
                    if (ptr->word == NULL) {
                        ptr->word = word;
                    }
                    break;
                }

                n = num_from_letter[*wptr] - '0';

                if (ptr->children[n] == NULL) {
                    struct node_s *new = new_node();
#ifndef ONLINE_JUDGE
                    memcpy(new->name, word, wptr-word+1);
                    new->name[wptr-word+1] = '\0';
#endif
                    ptr->children[n] = new;
                }
                ptr = ptr->children[n];
            };
        }
#ifndef ONLINE_JUDGE
//        print_tree(&root, 0);
        printf("used up %d nodes\n", pool_used);
#endif
        i = try_solve(number, NO_BEST);
        if (i >= NO_BEST)
            printf("No solution.\n");
        else
            while (i--)
                printf("%s%c", solution[i], i ? ' ' : '\n');
    }
}


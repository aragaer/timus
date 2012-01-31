#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ONLINE_JUDGE
#define MAX_WORD_COUNT  50000
#define MAX_WORD_LEN    50

struct word {
    int len;
    char word[MAX_WORD_LEN + 1];
    char code[MAX_WORD_LEN + 1];
} words[MAX_WORD_COUNT + 1];

char *solution[200];

char number[101];

char num_from_letter[256], nl_code[] = "22233344115566070778889990";

void encode(struct word *word) {
    char *w = word->word, *c = word->code;
    while (*w)
        *c++ = num_from_letter[*w++];
    *c = '\0';
}

#define NO_BEST 999

int try_solve(char *num, int limit) {
    struct word *last = words;
    int res = 0;
#ifndef ONLINE_JUDGE
    printf("%s(%s, %d)\n", __func__, num, limit);
#endif
    while (1) {
        while (strncmp(num, last->code, last->len))
            last++;
        if (!last->len)
            return limit + 1;
        
#ifndef ONLINE_JUDGE
        printf("word %s matches!\n", last->word);
#endif
        if (num[last->len] == '\0') {
#ifndef ONLINE_JUDGE
            printf("word %s is a win!!\n", last->word);
#endif
            solution[0] = last->word;
            return 1;
        }
        if (limit == 1) {
            last++;
            continue;
        }
        res = try_solve(num + last->len, limit - 1);
#ifndef ONLINE_JUDGE
        printf("We've got solution of %s in %d words\n", num, res == limit ? NO_BEST : res);
#endif
        if (res < limit) {
#ifndef ONLINE_JUDGE
            printf("we've got a better solution!\n");
#endif
            solution[res] = last->word;
            limit = res;
        }
        last++;
    }
}

int main() {
    int dict_size, i;
    for (i = 'a'; i <= 'z'; i++)
        num_from_letter[i] = nl_code[i - 'a'];
    while (1) {
        scanf("%s", number);
#ifndef ONLINE_JUDGE
        printf("\nNow playing with number %s\n", number);
#endif
        if (number[0] == '-')
            break;

        scanf("%d", &dict_size);
        for (i = 0; i < dict_size; i++) {
            scanf("%s", words[i].word);
            words[i].len = strlen(words[i].word);
            encode(words + i);
#ifndef ONLINE_JUDGE
            printf("Got word %s code %s\n", words[i].word, words[i].code);
#endif
        }
        words[i].len = 0;
        words[i].code[0] = '\0';
        i = try_solve(number, NO_BEST);
        if (i >= NO_BEST)
            printf("No solution.\n");
        else
            while (i--)
                printf("%s%c", solution[i], i ? ' ' : '\n');
    }
}

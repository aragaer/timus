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
    struct word *next;
};

struct word words[MAX_WORD_COUNT + 1];

struct dict_s {
    struct word *head, *tail;
} dict[10];

char *solution[200];

char number[101];

char num_from_letter[256], nl_code[] = "22233344115566070778889990";

void encode(struct word *word) {
    char *w = word->word, *c = word->code;
    while (*w)
        *c++ = num_from_letter[*w++];
    *c = '\0';
}

#ifndef ONLINE_JUDGE
void print_dict(char num) {
    struct dict_s *d = dict + num - '0';
    struct word *w;
    printf("Dictionary %c contains words:\n", num);
    for (w = d->head; w; w = w->next)
        printf("%s (%s)\n", w->word, w->code);
}
#endif

#define NO_BEST 999

int try_solve(char *num, int limit) {
    int res = 0;
    struct dict_s *d = dict + num[0] - '0';
    struct word *w;
    int l = strlen(num);

    int buf[MAX_WORD_LEN/sizeof(int) + 1];
    memcpy(buf, num, l + 1);
    num = (char *) buf;

#ifndef ONLINE_JUDGE
    printf("%s(%s, %d)\n", __func__, num, limit);
    print_dict(num[0]);
#endif
//    printf("num is %s\n", num);
    for (w = d->head; w; w = w->next) {
        if (w->len > l)
            continue;
//        printf("word %s, code %s\n", w->word, w->code);

        // fast implementation of strncmp
        int diffs = 0;
        int i, *iw = (int *) w->code, *in = (int *) num;
        for (i = 0; i < w->len/sizeof(int); i++) {
//            printf("%d: %c%c%c%c vs %c%c%c%c\n", i, w->code[i*4],
//                w->code[i*4+1], w->code[i*4+1], w->code[i*4+3],
//                num[i*4], num[i*4+1], num[i*4+2], num[i*4+3]);
//            printf("%d: %08X vs %08X\n", i, *iw, *in);
            diffs |= *iw++ ^ *in++;
        }

        for (i *= sizeof(int); i < w->len; i++) {
//            printf("%d: %02X vs %02X\n", i, w->code[i], num[i]);
            diffs |= w->code[i] ^ num[i];
        }

        if (diffs)
            continue;

#ifndef ONLINE_JUDGE
        printf("word %s matches!\n", w->word);
#endif
        if (num[w->len] == '\0') {
#ifndef ONLINE_JUDGE
            printf("word %s is a win!!\n", w->word);
#endif
            solution[0] = w->word;
            return 1;
        }
        if (limit == 1)
            continue;
        res = try_solve(num + w->len, limit - 1);
#ifndef ONLINE_JUDGE
        printf("We've got solution of %s in %d words\n", num, res == limit ? NO_BEST : res);
#endif
        if (res < limit) {
#ifndef ONLINE_JUDGE
            printf("we've got a better solution!\n");
#endif
            solution[res] = w->word;
            limit = res;
        }
    }

    return limit + 1;
}

int main() {
    int dict_size, i;
    char tmp[MAX_WORD_LEN + 1];
    for (i = 'a'; i <= 'z'; i++)
        num_from_letter[i] = nl_code[i - 'a'];
    while (1) {
        int l;
        scanf("%s", number);
        for (i = 0; i < 10; i++)
            dict[i].head = dict[i].tail = NULL;
#ifndef ONLINE_JUDGE
        printf("\nNow playing with number %s\n", number);
#endif
        if (number[0] == '-')
            break;

        l = strlen(number);

        scanf("%d", &dict_size);
        for (i = 0; i < dict_size; i++) {
            struct dict_s *d;
            struct word *w;
            int wl;

            scanf("%s", tmp);
            wl = strlen(tmp);
            if (wl > l) // The word is too long - ignore it
                continue;
            d = dict + num_from_letter[tmp[0]] - '0';
            w = words + i;
            if (d->tail)
                d->tail->next = w;
            else
                d->head = w;
            d->tail = w;
            w->len = wl;
            memcpy(w->word, tmp, w->len + 1);
            encode(w);
#ifndef ONLINE_JUDGE
            printf("Got word %s code %s\n", w->word, w->code);
#endif
        }
        for (i = 0; i < 10; i++) {
            struct word *w = dict[i].tail;
            if (!w)
                continue;
            w->next = NULL;
        }
        i = try_solve(number, NO_BEST);
        if (i >= NO_BEST)
            printf("No solution.\n");
        else
            while (i--)
                printf("%s%c", solution[i], i ? ' ' : '\n');
    }
}


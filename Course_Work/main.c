#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SENT_BUF 100
#define WORDS_BUF 50
#define TEXT_BUF 40
#define WORD_BUF 30

#define COURSE_WORK_INFO "Course work for option 4.16, created by Dmitrii Serpyakov.\n"

#define ERROR EXIT_SUCCESS
#define PRINT_SENTENCES 0
#define PRINT_BLUE_AFTER_DEFINE 1
#define REPLACE_SEQUENTIAL_LETTERS 2
#define REMOVE_SENTENCES_DIVISIBLE_BY_3 3
#define SORT_BY_WORD_LENGTH 4
#define PRINT_HELP 5

typedef struct Word {
    char* word;
    char* sep;
    size_t length;
} word_t;

typedef struct sentence {
    char* str;
    word_t* words;
    size_t words_counter;
    size_t size;
    size_t index;
} sentence_t;

typedef struct text {
    sentence_t** sentences;
    size_t sentence_count;
    size_t capacity;
} text_t;

void print_sentences();

void print_help();

void print_blue_after_define();

void replace_sequential_letters();

void remove_sentences_divisible_by_3();

void sort_by_word_length();

void separate_sentence(sentence_t* sent);

int is_separator(char ch);

sentence_t* read_sentence() {
    char ch;
    size_t size = SENT_BUF;
    size_t ch_count = 0; // Количество символов в предложении
    char* sent = malloc(size * sizeof(char));

    if (!sent) {
        printf("Error: memory allocation error [sentence]");
        free(sent);
        exit(ERROR);
    }

    char* tmp = NULL;
    sentence_t* s = NULL;

    while ((ch = getchar()) == '\t' || ch == ' ') {
    } // Убираем пробелы в начале предложения

    if (ch == '\n') // Если строка является просто переходом на новую строку, то обрабатываем её отдельно
    {
        sent[0] = '\n';
        sent[1] = '\0';

        s = (sentence_t*) malloc(sizeof(sentence_t));

        if (!s) {
            printf("Error: memory allocation error [sentence]");
            free(s->str);
            for (int i = 0; i < s->words_counter; i++) {
                free(s->words[i].word);
                free(s->words[i].sep);
            }
            free(s->words);
            free(s);
            exit(ERROR);
        }

        s->str = sent;
        s->size = 1;

        return s;
    }

    while (1) {
        if (ch_count == size - 2) // Оставляем место для нулевого терминатора
        {
            size += SENT_BUF;
            tmp = realloc(sent, size);

            if (!tmp) {
                printf("Error: memory allocation error [temporary variable]");
                free(tmp);
                exit(ERROR);
            }

            sent = tmp;
        }

        if (ch == '\n') {
            sent[0] = '\0'; // Делаем предложение несуществующим
            break;
        }

        sent[ch_count++] = ch;

        if (ch == '.') {
            ch = getchar();
            break;
        }
        // Чтобы точка в конце предложения тоже записывалась

        ch = getchar();
    }

    sent[ch_count] = '\0';

    s = (sentence_t*) malloc(sizeof(sentence_t));

    if (!s) {
        printf("Error: memory allocation error [sentence]");
        for (int i = 0; i < s->words_counter; i++) {
            free(s->words[i].word);
            free(s->words[i].sep);
        }
        free(s->words);
        free(s);
        exit(ERROR);
    }

    s->str = sent;
    s->size = strlen(sent);
    separate_sentence(s);

    return s;
}


int is_separator(char ch) {
    if (isalnum(ch) || ch == '-')
        return 0;
    return 1;
}

void separate_sentence(sentence_t* sent) {
    size_t capacity = WORDS_BUF;

    sent->words = malloc(capacity * sizeof(word_t));
    if (!sent->words) {
        printf("Error: memory allocation error [words]");
        free(sent->words);
        exit(ERROR);
    }

    sent->words_counter = 0;

    char* s = sent->str;
    if (!sent->str) {
        printf("Error: memory allocation error [string]");
        free(sent->str);
        exit(ERROR);
    }

    char* word = NULL;
    char* sep_text = NULL;

    size_t idx = 0;
    size_t sep_length = 0;
    size_t word_start = 0;
    size_t word_length = 0;
    size_t sep_start = 0;

    while (s[idx] != '\0') {
        word_start = idx;

        while (!is_separator(s[idx])) idx++;

        word_length = idx - word_start;

        if (word_length == 0) {
            idx++;
            continue;
        }

        word = (char*) malloc(word_length + 1);

        if (!word) {
            printf("Error: memory allocation error [word]");
            free(word);
            exit(ERROR);
        }

        memcpy(word, &s[word_start], word_length);
        word[word_length] = '\0';

        sep_start = idx;

        while (is_separator(s[idx]) && s[idx] != '\0') idx++;

        sep_length = idx - sep_start;

        sep_text = (char*) malloc(sep_length + 1);

        if (!sep_text) {
            printf("Error: memory allocation error [separators]");
            free(sep_text);
            exit(ERROR);
        }

        memcpy(sep_text, &s[sep_start], sep_length);
        sep_text[sep_length] = '\0';

        if (sent->words_counter == capacity) {
            capacity += WORDS_BUF;
            word_t* tmp = realloc(sent->words, capacity * sizeof(word_t));
            if (!tmp) {
                printf("Error: memory allocation error [temporary variable]");
                for (size_t i = 0; i < sent->words_counter; i++) {
                    free(sent->words[i].word);
                    free(sent->words[i].sep);
                }
                free(sent->words);
                exit(ERROR);
            }
            sent->words = tmp;
        }

        sent->words[sent->words_counter].word = word;
        sent->words[sent->words_counter].sep = sep_text;
        sent->words[sent->words_counter].length = word_length;
        sent->words_counter++;
    }
}

void delete_duplicates(text_t* text) {
    for (int i = 0; i < text->sentence_count; i++) {
        for (int j = i + 1; j < text->sentence_count; j++) {
            char* str1 = text->sentences[i]->str;
            size_t len1 = text->sentences[i]->size;
            char* str2 = text->sentences[j]->str;
            size_t len2 = text->sentences[j]->size;

            if (len1 == len2) {
                int equal = 1;
                for (int k = 0; k < len1; k++) {
                    if (tolower(str1[k]) != tolower(str2[k])) {
                        equal = 0;
                        break;
                    }
                }

                if (equal) {
                    free(text->sentences[j]->str);
                    free(text->sentences[j]);

                    for (int k = j; k < text->sentence_count - 1; k++)
                        text->sentences[k] = text->sentences[k + 1];

                    text->sentence_count--;
                    j--;
                }
            }
        }
    }
}

text_t* read_text() {
    size_t size = TEXT_BUF;
    size_t sent_count = 0; // Количество предложений в тексте
    size_t new_line_count = 0; // Количество новых строк в тексте
    sentence_t** text = (sentence_t**) malloc(size * sizeof(sentence_t*));

    if (!text) {
        printf("Error: memory allocation error [text]");
        free(text);
        exit(ERROR);
    }

    sentence_t** tmp = NULL;
    // Создание переменной здесь для возможности правильного освобождения памяти через оператор goto
    sentence_t* sent = NULL;
    text_t* t = NULL; // Инициализация, если не сработает malloc, то будет NULL

    do {
        if (sent_count == size - 1) {
            // Проверка на нехватку памяти для последующего расширения
            size += SENT_BUF;
            tmp = (sentence_t**) realloc(text, size * sizeof(sentence_t*));
            if (!tmp) {
                printf("Error: memory allocation error [temporary variable]");
                free(tmp);
                exit(ERROR);
            }
            text = tmp;
        }

        sent = read_sentence();

        if (sent->str[0] == '\n') {
            new_line_count++; // По условию окончания ввода текста
            free(sent->str);
            free(sent); // Если предложения является пустой строкой, то она нам не нужна
        } else {
            new_line_count = 0;

            sent->index = sent_count;
            text[sent_count++] = sent;
        }
    } while (new_line_count < 1);
    // new_line_count < именно единицы так как в предыдущем предложении точно есть первый \n, тогда нам нужен всего 1 \n
    t = malloc(sizeof(text_t));

    if (!t) {
        printf("Error: memory allocation error [text]");
        free(t);
        exit(ERROR);
    }

    t->sentences = text;
    t->capacity = size;
    t->sentence_count = sent_count;

    delete_duplicates(t);

    if (t->sentence_count == 0) {
        printf("User hasn't inputted sentences");
        free(t);
        exit(ERROR);
    }
    return t;
}

void clean_memory(text_t* text) {
    for (int i = 0; i < text->sentence_count; i++) {
        for (int j = 0; j < text->sentences[i]->words_counter; j++) {
            free(text->sentences[i]->words[j].word);
            free(text->sentences[i]->words[j].sep);
        }
        free(text->sentences[i]->str);
        free(text->sentences[i]->words);
        free(text->sentences[i]);
    }
    free(text->sentences);
    free(text);
}

void print_sentences() {
    text_t* t = read_text();
    for (int i = 0; i < t->sentence_count; i++)
        printf("%s\n", t->sentences[i]->str);

    clean_memory(t);
}

void print_help() {
    printf("\n---Help---\n");
    printf("Function 0 prints processed sentences (With no duplicated sentences).\n");
    printf(
        "Function 1 prints sentences. If a sentence contains the phrase <define BLUE>, then all characters following this phrase should be printed in blue.\n");
    printf("Function 2 replaces all characters on substring like <consecutive letters><upper letter>.\n");
    printf("Function 3 deletes all sentences where word count divisional by 3.\n");
    printf(
        "Function 4 sorts by decreasing sum of the lengths of the first and last words; if a sentence has one word, use its length; preserve original order on ties.");
    printf("Function 5 prints information about the course work functions.");
    printf("\n---Help---\n");
}

void print_blue_after_define() {
    text_t* t = read_text();

    size_t define_word_flag = 0;
    size_t toggle_blue = 0;

    for (int i = 0; i < t->sentence_count; i++) {
        for (int j = 0; j < t->sentences[i]->words_counter; j++) {
            char* word = t->sentences[i]->words[j].word;
            char* sep = t->sentences[i]->words[j].sep;

            if (!toggle_blue) {
                printf("%s%s", word, sep);
                if (strcmp(word, "define") == 0) define_word_flag = 1;
                else if (strcmp(word, "BLUE") == 0 && define_word_flag == 1) toggle_blue = 1;
                else define_word_flag = 0;
            } else printf("\033[34m%s\033[0m%s", word, sep);
        }
        printf("\n");
        define_word_flag = 0;
        toggle_blue = 0;
    }

    clean_memory(t);
}

void replace_sequential_letters() {
    text_t* t = read_text();

    for (int i = 0; i < t->sentence_count; i++) {
        for (int j = 0; j < t->sentences[i]->words_counter; j++) {
            char* word = t->sentences[i]->words[j].word;
            char* sep = t->sentences[i]->words[j].sep;

            int sequential_letters_counter = 0;
            size_t idx = 0;
            char symbol;
            while ((symbol = word[idx]) != '\0') {
                while (tolower(symbol) == tolower(word[idx])) {
                    sequential_letters_counter++;
                    idx++;
                }
                if (sequential_letters_counter >= 2) {
                    printf("%d%c", sequential_letters_counter, toupper(symbol));
                    sequential_letters_counter = 0;
                } else {
                    printf("%c", symbol);
                    sequential_letters_counter = 0;
                }
            }
            printf("%s", sep);
        }
        printf("\n");
    }

    clean_memory(t);
}

void remove_sentences_divisible_by_3() {
    text_t* t = read_text();

    for (int i = 0; i < t->sentence_count; i++) {
        if (t->sentences[i]->words_counter % 3 != 0)
            printf("%s\n", t->sentences[i]->str);
    }

    clean_memory(t);
}

int cmp(const void* a, const void* b) {
    const sentence_t* sent1 = *(const sentence_t* const*) a;
    const sentence_t* sent2 = *(const sentence_t* const*) b;

    size_t sum1 = sent1->words[0].length;
    if (sent1->words_counter > 1)
        sum1 += sent1->words[sent1->words_counter - 1].length;

    size_t sum2 = sent2->words[0].length;
    if (sent2->words_counter > 1)
        sum2 += sent2->words[sent2->words_counter - 1].length;

    if (sum1 > sum2) return -1;
    if (sum1 < sum2) return 1;

    return (int) ((long long) sent1->index - (long long) sent2->index);
    // Кастим к long long, так как он, как и size_t, 64битный, только signed
}

void sort_by_word_length() {
    text_t* t = read_text();

    qsort(t->sentences, t->sentence_count, sizeof(sentence_t*), cmp);

    for (int i = 0; i < t->sentence_count; i++)
        printf("%s\n", t->sentences[i]->str);

    clean_memory(t);
}

int main(void) {
    int command;

    printf(COURSE_WORK_INFO);

    scanf("%d", &command);

    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    };

    switch (command) {
        case PRINT_SENTENCES:
            print_sentences();
            break;
        case PRINT_BLUE_AFTER_DEFINE:
            print_blue_after_define();
            break;
        case REPLACE_SEQUENTIAL_LETTERS:
            replace_sequential_letters();
            break;
        case REMOVE_SENTENCES_DIVISIBLE_BY_3:
            remove_sentences_divisible_by_3();
            break;
        case SORT_BY_WORD_LENGTH:
            sort_by_word_length();
            break;
        case PRINT_HELP:
            print_help();
            break;
        default:
            printf("Error: invalid command\n");
            break;
    }
    return 0;
}
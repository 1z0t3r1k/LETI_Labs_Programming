#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALLOC_SIZE 16

char* dynamic_string_reader();

char** dynamic_dimensional_array(char* str);

int count_sents(char* str);

int main() {
    char* str = dynamic_string_reader();
    int return_code = 0;

    if (str) {
        int sents_counter = count_sents(str);
        char** sents = dynamic_dimensional_array(str);
        if (sents) {
            int final_sents_counter = 0;

            for (int i = 0; i < sents_counter; i++) {
                if (!strchr(sents[i], '7')) {
                    printf("%s\n", sents[i]);
                    final_sents_counter++;
                }
            }

            printf("Количество предложений до %d и количество предложений после %d\n", sents_counter - 1,
                   final_sents_counter - 1);

            for (int i = 0; i < sents_counter; i++) {
                free(sents[i]);
            }
            free(sents);
        } else {
            return_code = 1;
        }
        free(str);
    } else return_code = 1;

    return return_code;
}

char* dynamic_string_reader() {
    char* str = (char*) malloc(sizeof(char));
    int error_code = 0;

    if (str) {
        size_t len = 0;
        int c;

        while ((c = getchar()) != EOF && c != '\n' && !error_code) {
            char* temp = (char*) realloc(str, len + 2);
            if (temp) {
                str = temp;
                str[len++] = (char) c;
            }
            else {
                error_code = 1;
            }
        }
        str[len] = '\0';
    }

    return str;
}

int count_sents(char* str) {
    size_t sents_counter = 1;
    size_t len = strlen(str);

    for (size_t i = 0; i < len; i++) {
        if (str[i] == '.' || str[i] == '?' || str[i] == ';') {
            sents_counter++;
        }
    }

    return sents_counter;
}

char** dynamic_dimensional_array(char* str) {
    size_t alloc_size = ALLOC_SIZE;
    char** sents = sents = (char**) malloc(alloc_size * sizeof(char*));
    size_t sentese_idx = 0;
    int error_code = 0;

    if (sents) {
        size_t len = strlen(str);
        size_t sent_start = 0;

        for (size_t i = 0; i <= len && !error_code; i++) {
            if (str[i] == '.' || str[i] == ';' || str[i] == '?' || str[i] == '\0') {
                while (sent_start < i && (str[sent_start] == ' ' || str[sent_start] == '\t')) {
                    sent_start++;
                }

                size_t sent_len = i - sent_start + 1;
                if (sent_len == 0) {
                    sent_start = i + 1;
                } else if (!error_code) {
                    if (sentese_idx >= alloc_size) {
                        alloc_size *= 2;
                        char** temp = (char**) realloc(sents, alloc_size * sizeof(char*));
                        if (temp) {
                            sents = temp;
                        } else {
                            error_code = 1;
                        }
                    }

                    if (!error_code) {
                        sents[sentese_idx] = (char*) malloc(sent_len + 1);
                        if (!sents[sentese_idx]) {
                            error_code = 1;
                        } else {
                            strncpy(sents[sentese_idx], str + sent_start, sent_len);
                            sents[sentese_idx][sent_len] = '\0';
                            sentese_idx++;
                        }
                    }

                    sent_start = i + 1;
                }
            }
        }
    } else error_code = 1;

    if (error_code) {
        for (size_t j = 0; j < sentese_idx; j++) {
            free(sents[j]);
        }
        free(sents);
        sents = NULL;
    }

    return sents;
}

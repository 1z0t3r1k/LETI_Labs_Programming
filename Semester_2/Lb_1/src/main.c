#include <regex.h>
#include <stdio.h>
#include <string.h>
#define N 500
#define REGEX "[a-z_]+:\\/\\/(www\\.)?([[a-zA-Z0-9_.-]+([a-zA-Z0-9_.-]+)+)\\/([[a-zA-Z0-9_.-]+\\/)*([[a-zA-Z0-9_.-]+\\.[[a-zA-Z0-9_.-]*)"
#define FINAL_WORD "Fin.\n"

void read_and_match(regex_t* regex_comp);

int main() {
    regex_t regex_comp;
    int comp = regcomp(&regex_comp, REGEX, REG_EXTENDED);
    if (comp != 0) {
        printf("Compilation error!");
        return 1;
    }
    if (comp != 0) {
        return 0;
    }
    read_and_match(&regex_comp);
    regfree(&regex_comp);
    return 0;
}

void read_and_match(regex_t* regex_comp) {
    char str[N];
    regmatch_t regex_groups[6];
    while (fgets(str, N, stdin) && strcmp(str, FINAL_WORD)) {
        if (!regexec(regex_comp, str, 6, regex_groups, 0)) {
            str[regex_groups[2].rm_eo] = '\0';
            printf("%s - %s", str + regex_groups[2].rm_so, str + regex_groups[5].rm_so);
        }
    }
}
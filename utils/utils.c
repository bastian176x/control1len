#include <string.h>
#include <ctype.h>

void trimWhitespace(char *str) {
    char *start = str;
    char *end;

    // Move 'start' to the first non-whitespace character
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // All spaces?
    if (*start == 0) {
        *str = 0;
        return;
    }

    // Move 'end' to the end of the string
    end = start + strlen(start) - 1;
    // Move 'end' back to the last non-whitespace character
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Null-terminate the trimmed string
    *(end + 1) = '\0';

    // If 'start' is not 'str', move the trimmed string to the beginning
    if (start != str) {
        memmove(str, start, end - start + 2); // +1 for the character, +1 for '\0'
    }
}

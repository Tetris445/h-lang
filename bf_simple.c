#include <stdio.h>

int main() {
    char mem[30000] = {0};
    char *ptr = mem;
    char *code = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.";
    
    for (int i = 0; code[i] != '\0'; i++) {
        switch (code[i]) {
            case '>': ptr++; break;
            case '<': ptr--; break;
            case '+': (*ptr)++; break;
            case '-': (*ptr)--; break;
            case '.': putchar(*ptr); break;
            case '[': 
                if (*ptr == 0) { 
                    int depth = 1; 
                    while (depth > 0) { 
                        i++; 
                        if (code[i] == '[') depth++; 
                        if (code[i] == ']') depth--; 
                    } 
                } 
                break;
            case ']': 
                if (*ptr != 0) { 
                    int depth = 1; 
                    while (depth > 0) { 
                        i--; 
                        if (code[i] == ']') depth++; 
                        if (code[i] == '[') depth--; 
                    } 
                } 
                break;
        }
    }
    printf("\n");
    return 0;
}

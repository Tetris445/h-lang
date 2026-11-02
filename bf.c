#include <stdio.h>
#include <string.h>

int main() {
    char mem[30000] = {0};
    char *ptr = mem;
    char *code = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
    
    for (int i = 0; code[i] != '\0'; i++) {
        switch (code[i]) {
            case '>': ptr++; break;
            case '<': ptr--; break;
            case '+': (*ptr)++; break;
            case '-': (*ptr)--; break;
            case '.': putchar(*ptr); break;
        }
    }
    return 0;
}

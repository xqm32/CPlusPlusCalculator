#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// #define DEBUG
#define HAS_ARG
#define N 1024

class Stack {
   private:
    int __stack[N] = {0};
    int __top = -1;

   public:
#ifdef DEBUG
    void print(const char *fmt) {
        for (int i = 0; i <= __top; ++i) printf(fmt, __stack[i]);
        printf("\n");
    }
#endif
    void push(int n) { __stack[++__top] = n; }
    int pop() { return __stack[__top--]; }
    int top() const { return __stack[__top]; }
    int empty() const { return !(__top + 1); }
    int full() const { return __top < N; }
    int depth() const { return __top + 1; }
};

int getPrior(char c) {
    switch (c) {
        case '#':
            return 0;
        case '(':
            return 1;
        case ')':
            return 1;
        case '+':
            return 2;
        case '-':
            return 2;
        case '*':
            return 3;
        case '/':
            return 3;
        default:
            break;
    }
    return -1;
}

int getAns(char c, int a, int b) {
    switch (c) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            break;
    }
    return -1;
}

int calc(char *e) {
    Stack opStack, nStack;
    if (*e != '#' || e[strlen(e) - 1] != '#') {
        printf("\"%s\" is not a correct expression\n", e);
        return -1;
    }
    for (int a, b; *e;) {
#ifdef DEBUG
        printf("[Rest]\t%s\n", e);
        printf("[Stack]\tnStack: \t");
        nStack.print("%d ");
        printf("[Stack]\topStack:\t");
        opStack.print("%c ");
        printf("\n");
        printf("[Read]\t%c\n", *e);
#endif
        if (isalnum(*e)) {
            nStack.push(strtol(e, &e, 0));
        } else {
            if (opStack.top() == ')') {
                opStack.pop();
                opStack.pop();
                continue;
            } else if (*e == '(') {
                opStack.push(*e++);
                continue;
            } else if (*e == '#') {
                if (opStack.top() == '#') {
                    return nStack.pop();
                } else if (opStack.depth() > 1) {
                    // 若操作符栈里至少有两个元素 ('#', operator, ...)
                    b = nStack.pop();
                    a = nStack.pop();
#ifdef DEBUG
                    printf("[Cacu]\t%d %c %d\n", a, opStack.top(), b);
#endif
                    nStack.push(getAns(opStack.pop(), a, b));
                    // 这里使用 continue 避免循环终止
                    continue;
                }
            } else if (getPrior(opStack.top()) >= getPrior(*e)) {
                b = nStack.pop();
                a = nStack.pop();
#ifdef DEBUG
                printf("[Cacu]\t%d %c %d\n", a, opStack.top(), b);
#endif
                nStack.push(getAns(opStack.pop(), a, b));
            }
            opStack.push(*e++);
        }
    }
#ifdef DEBUG
    printf("\n");
#endif
    return nStack.pop();
}

int main(int argc, char **argv) {
#ifdef HAS_ARG
    printf("The answer is: %d\n", calc(argv[1]));
#else
    printf("The answer is: %d\n", calc("#12+5*(2+3)*6/2-4#"));
#endif
}
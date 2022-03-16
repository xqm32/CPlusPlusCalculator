#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

// #define DEBUG
// #define HAS_ARG
#define N 1024

template <typename T>
class Node {
   public:
    Node *prev = nullptr;
    Node *next = nullptr;
    T value;
    Node(const T &n) { value = n; }
};

template <typename T>
class LinkedList {
   private:
    Node<T> *head = nullptr;
    Node<T> *rear = nullptr;
    size_t len = 0;

   public:
    void push_back(const T &n) {
        if (empty())
            head = rear = new Node<T>(n);
        else {
            auto *t = rear;
            rear->next = new Node<T>(n);
            rear = rear->next;
            rear->prev = t;
        }
        len += 1;
    }
    void push_front(const T &n) {
        if (empty())
            head = rear = new Node<T>(n);
        else {
            auto *t = head;
            head->prev = new Node<T>(n);
            head = head->prev;
            head->next = t;
        }
        len += 1;
    }
    T pop_back() {
        if (empty())
            throw std::out_of_range("Pop back from empty list");
        else {
            auto t = rear->value;
            if (head == rear) {
                delete head;
                head = rear = nullptr;
            } else {
                rear = rear->prev;
                delete head->prev;
            }
            len -= 1;
            return t;
        }
    }
    T pop_front() {
        if (empty())
            throw std::out_of_range("Pop front from empty list");
        else {
            auto t = head->value;
            if (head == rear) {
                delete head;
                head = rear = nullptr;
            } else {
                head = head->next;
                delete head->prev;
            }
            len -= 1;
            return t;
        }
    }
    T front() {
        if (empty())
            throw std::out_of_range("Get front from empty list");
        else
            return head->value;
    }
    T back() {
        if (empty())
            throw std::out_of_range("Get back from empty list");
        else
            return rear->value;
    }
    T length() { return len; }
    int empty() { return len == 0; }
    void print() {
        if (empty())
            std::cout << "(Empty list)" << std::endl;
        else {
            for (auto *t = head; t != rear; t = t->next) {
                std::cout << t->value << " -> ";
            }
            std::cout << rear->value << std::endl;
        }
    }
};

class Stack {
   private:
    LinkedList<int> __stack;

   public:
#ifdef DEBUG
    void print(const char *fmt) { __stack.print(); }
#endif
    void push(int n) { __stack.push_back(n); }
    int pop() { return __stack.pop_back(); }
    int top() { return __stack.back(); }
    int empty() { return __stack.empty(); }
    int depth() { return __stack.length(); }
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
            if (!opStack.empty() && opStack.top() == ')') {
                opStack.pop();
                opStack.pop();
                continue;
            } else if (*e == '(') {
                opStack.push(*e++);
                continue;
            } else if (*e == '#') {
                if (!opStack.empty() && opStack.top() == '#') {
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
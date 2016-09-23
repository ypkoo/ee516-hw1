#ifndef STACK_H
#define STACK_H

// typedef struct stack stack_t;

typedef struct stack stack_t;

int stack_init(stack_t *s);
int is_full(stack_t *s);
int is_empty(stack_t *s);
int push(stack_t *s, const char *value);
int pop(stack_t *s, char *value);
char top(stack_t *s);

#endif
#define MAX_SIZE 256
#define NULL 0

typedef struct stack {
		int top;
		char item[MAX_SIZE];
} stack_t;

int stack_init(stack_t *s) {
	if (s == NULL) return -1;

	s->top = 0;

	return 0;
}

int is_full(stack_t *s) {
	return (s->top == MAX_SIZE - 1);
}

int is_empty(stack_t *s) {
	return (s->top == 0);
}

int push(stack_t *s, const char *value) {
	if (is_full(s) || s == NULL || value == NULL) return -1;

	s->item[s->top] = *value;
	s->top++;

	return 0;
}

int pop(stack_t *s, char *value) {

	if (is_empty(s) || s == NULL || value == NULL) return -1;

	*value = s->item[s->top];
	s->top--;

	return 0;
}

char top(stack_t *s) {
	if (is_empty(s) || s == NULL) return -1;

	// *value = s->item[s->top];

	return s->item[s->top];
}
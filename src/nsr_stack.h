#ifndef __NSR_STACK_H__
#define __NSR_STACK_H__

#define NSR_STACK_INIT_SIZE 8

typedef struct nsr_stack_elem
{
   int _idx;
   char *_string;
} nsr_stack_elem_t;

typedef struct nsr_stack
{
   nsr_stack_elem_t *_elements;
   int _size;
   int _max_size;
} nsr_stack_t;

/**
 * initializes an empty stack
 * @param stack stack to initialize
 */
void nsr_stack_init(nsr_stack_t *stack);

/**
 * returns whether the stack is empty
 * @param stack
 * @return 1 if stack is empty; 0 otherwise
 */
int nsr_stack_empty(const nsr_stack_t *stack);

/**
 * inserts an index `idx' to the stack
 * @param stack
 * @param idx index to insert
 * @param string string to insert
 * @param string_size
 */
void nsr_stack_push(nsr_stack_t *stack, int idx, char string[], 
        const int string_size);

/**
 * deletes the stack
 * @param stack
 */
void nsr_stack_destroy(nsr_stack_t *stack);

/**
 * returns the element from the top of the stack and deletes the element from
 * the stack
 * @param stack stack to pop from
 * @return the element from top of the stack
 */
nsr_stack_elem_t nsr_stack_pop(nsr_stack_t *stack);

/**
 * prints the content of the stack
 * @param stack the stack to print
 */
void nsr_stack_print(const nsr_stack_t *stack);

#endif /* __NSR_STACK_H__ */

#ifndef __NSR_STACK_H__
#define __NSR_STACK_H__

#define NSR_STACK_INIT_SIZE 8

typedef struct nsr_stack_elem
{
   int _idx;
   int _min_dist;
} nsr_stack_elem_t;

typedef struct nsr_stack
{
   nsr_stack_elem_t *_elements;
   int _size;
   int _max_size;
} nsr_stack_t;

void nsr_stack_init(nsr_stack_t *stack);
int nsr_stack_empty(const nsr_stack_t *stack);
void nsr_stack_push(nsr_stack_t *stack, int idx, int min_dist);
void nsr_stack_destroy(nsr_stack_t *stack);
nsr_stack_elem_t nsr_stack_pop(nsr_stack_t *stack);
void nsr_stack_print(const nsr_stack_t *stack);

#endif /* __NSR_STACK_H__ */

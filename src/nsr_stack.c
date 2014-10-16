#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "nsr_stack.h"

void nsr_stack_init(nsr_stack_t *stack)
{
   stack->_size = 0;
   stack->_max_size = NSR_STACK_INIT_SIZE;
   stack->_elements = (nsr_stack_elem_t *) malloc(stack->_max_size *
      sizeof(nsr_stack_elem_t));
}

int nsr_stack_empty(const nsr_stack_t *stack)
{
   return !stack->_size;
}

void nsr_stack_push(nsr_stack_t *stack, int idx)
{
   nsr_stack_elem_t *elements_new;

   if (stack->_size == stack->_max_size)
   {
      stack->_max_size <<= 1;
      elements_new = (nsr_stack_elem_t *) malloc(
         stack->_max_size * sizeof(nsr_stack_elem_t));
      memcpy(elements_new, stack->_elements, stack->_size *
         sizeof(nsr_stack_elem_t));
      free(stack->_elements);
      stack->_elements = elements_new;
   }

   stack->_elements[stack->_size]._idx = idx;
   stack->_size++;
}

void nsr_stack_destroy(nsr_stack_t *stack)
{
   free(stack->_elements);
}

nsr_stack_elem_t nsr_stack_pop(nsr_stack_t *stack)
{
   return stack->_elements[--stack->_size];
}

void nsr_stack_print(const nsr_stack_t *stack)
{
   int i;
   printf("size: %d\n\nstack elements:\n", stack->_size);
   for (i = 0; i < stack->_size; i++)
      printf("idx: %d;\n", stack->_elements[i]._idx);
}

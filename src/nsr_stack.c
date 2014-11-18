#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "nsr_stack.h"

void nsr_stack_init(nsr_stack_t *stack)
{
   stack->_size = 0;
   stack->_bottom = 0;
   stack->_max_size = NSR_STACK_INIT_SIZE;
   stack->_elements = (nsr_stack_elem_t *) malloc(stack->_max_size *
      sizeof(nsr_stack_elem_t));
}

int nsr_stack_empty(const nsr_stack_t *stack)
{
   return stack->_bottom == stack->_size;
}

void nsr_stack_push(nsr_stack_t *stack, int idx, char string[], 
        const int string_size)
{
   nsr_stack_elem_t *elements_new;

   if (stack->_size == stack->_max_size)
   {
       /* TODO not sure if working with string */    
      stack->_max_size <<= 1;
      elements_new = (nsr_stack_elem_t *) malloc(
         stack->_max_size * sizeof(nsr_stack_elem_t));
      memcpy(elements_new, stack->_elements, stack->_size *
         sizeof(nsr_stack_elem_t));
      
      /* TODO: need to do it right*/
      //free(stack->_elements);
      stack->_elements = elements_new;
   }

   stack->_elements[stack->_size]._idx = idx;
   stack->_elements[stack->_size]._string = (char *) malloc(
           (string_size*sizeof(char))+1);
   memcpy(stack->_elements[stack->_size]._string, string, string_size+1);
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

nsr_stack_elem_t nsr_stack_pop_bottom(nsr_stack_t *stack)
{
    if(nsr_stack_empty(stack))
    {
        printf("Trying to get elem from empty stack.\n");
    }
    
    //stack->_size--;
    stack->_bottom++;
    return stack->_elements[stack->_bottom-1];
}

void nsr_stack_print(const nsr_stack_t *stack)
{
   int i;
   printf("size: %d\n\nstack elements:\n", stack->_size);
   for (i = stack->_bottom; i < stack->_size; i++)
      printf("idx: %d string: %s;\n", stack->_elements[i]._idx,
              stack->_elements[i]._string);
}

int nsr_stack_get_size(const nsr_stack_t *stack)
{
    return (stack->_size - stack->_bottom);
}
#include <stdbool.h>     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h> 
#include "./classes/classa.c"

int main() {
  printf("Welcome to calculator\n");
  //jmp_buf begin_try;

  int* numbers_ptr;
  short totalChanges = 0;
  int newSize = sizeof(*numbers_ptr);
  numbers_ptr = calloc(10,  newSize);

  printf("Checking if allocation failed\n");
  
  fflush(stdout);
  if(numbers_ptr == NULL){
    //longjmp(begin_try, 1);
    return 1;
  }
  /*
  if(setjmp(begin_try)){
    fflush(stdout);
  }
  */
  int inputNbr;
  int result;
  bool keepRunning = true;

  while(keepRunning){
    printf("Total Changes: %d\n",  totalChanges + 1);
    if(totalChanges == 10){
      printf("Stopping process\n");
      keepRunning = false;
    } else {
      fflush(stdout);
      result = scanf("%d", &inputNbr);
      printf("Result is %d\n", result);
      if (result == 0){
        continue;
      } else {
        *numbers_ptr++ = inputNbr;
        totalChanges++;
        printf("Adding number: %d\n",  inputNbr);
      }
    }
  }

  for(int i = 0; i < 10; ++i){
    printf("Adding value %d \n", *(--numbers_ptr) );
  }

  fflush(stdout);
  scanf("%d", &inputNbr);
  int r = add_array(10, numbers_ptr);
  printf("Total value: %d\n", r);
  
  free(numbers_ptr);
  fflush(stdout);
  scanf("%d", &inputNbr);
  return 0;
} 
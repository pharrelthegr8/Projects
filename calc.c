/* this is meant to be a basic calculator
 * 1.. prompt the user to give 2 numbers
 * 2.. prompt the user to giver an operation i.e (*, -, +, /)
 * 3.. perform the calculation based on the given operand
 * 4.. display the result
 * 5.. prompt the user to give other numbers*/

#include <stdio.h>
#include <stdlib.h> // this library is required for system()
#include <string.h> // for strcmp

void calculator(); // our function declaration for our calculator program

int main(){
  char response[2];

  calculator();
  printf("Perform another calculation? (Y/n): ");
  scanf(" %c", response); // an ampersand(&) is not used in the case of character arrays

  while(strcmp(response, "y") == 0){
    // we use strcmp to compare strings I prefer to think of it as string comparison which is then shortened to strcmp
    calculator();

    printf("Perform another calculation? (Y/n): ");
    scanf(" %c", response);
  }
}

void calculator(){

  int number1, number2, result;
  char operator;

  system("clear"); // we need this to clear our terminal each time we run our program to give it a fresh feel
    
  printf("----------------calculator-------------------\n");
  printf("\n");

  printf("Enter 1st number: ");
  scanf("%d", &number1);
  printf("Enter 2nd number: ");
  scanf("%d", &number2);
  printf("Input: %d and %d\n", number1, number2);
  printf("\n");

  printf("Enter an operator ( +, -, /, * ): ");
  scanf(" %c", &operator); // we need to skip any leftover characters in the input buffer hence the space before %c
  printf("Operator: %c\n", operator);
  printf("\n");
  
  switch(operator){
    case '+':
      result = number1 + number2;
      break;
    case '-':
      result = number1 - number2;
      break;
    case '*':
      result = number1 * number2;
      break;
    case '/':
      result = number1 / number2;
      break;
    default:
      // reaction to invalid input
      while(operator != '+' && operator != '/' && operator != '*' && operator != '-'){
        printf("Invalid operator!\n");
        printf("\n");

        printf("Please input a valid operator: ");
        scanf(" %c", &operator);
        printf("\n");
      }
  }
  // it is much convenient to use a switch case for this rather than an if else statement since switch cases have jump tables
  // this makes them much faster for this task

  printf("%d %c %d = %d\n",number1, operator, number2, result);
  printf("\n");
}

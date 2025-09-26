// generate a random number hidden from the user and make them guess the number

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void val_gen(void);

int main(){

  char response;

  system("clear");
  printf("**********************************************\n");
  printf("*              GUESS THE NUMBER              *\n");
  printf("**********************************************\n");
  val_gen();
  
  printf("Would you like to guess another number?(y) ");
  scanf(" %c", &response);
  
  while(response == 'y'){
    system("clear");
    printf("********************************************\n");
    printf("*            GUESS THE NUMBER              *\n");
    printf("********************************************\n");
    val_gen();
  
    printf("Would you like to guess another number?(y/n) ");
    scanf(" %c", &response);

  }
}

void val_gen(void)
{
    int number;

  int threshold = 100; // a threshold for our random number generator ( 0, threshold + 1);
  srand(time(NULL)); // seed the random number generator with the current time to ensure randomness
                     // s before rand just means seed
  int rand_num = rand() % (threshold + 1); // adding constraints to our random number generator to ensure it doesnt
                                           // go beyond 100 each time it generates a number that's where % comes in
  printf("Guess a number: ");
  
  // validate input
  while((scanf("%d", &number)) != 1){
    // scanf usually returns a value of 1 when it reads a value successfully any other value means an error occured
    printf("Invalid input. Please enter a valid number: ");
    while(getchar() != '\n'); // this line clears the input buffer
    // without input validation an endless loop is created
  }
  
  while(number != rand_num){
    if(number > rand_num){
      printf("That's too high try a lower number: ");
    }else if(number < rand_num){
      printf("That's too low try a higher number: ");
    }
    // validate input once again
    while((scanf("%d", &number)) != 1){
      printf("Invalid input. Please enter a valid number: ");
      while(getchar() != '\n');
    }
  }
  printf("\n");
  printf("Congratulations! The number was %d\n", rand_num);
  printf("\n");
}

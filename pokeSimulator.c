/*
Peter Fakkel, CS224, pfakkel@gmail.com

Description: This program outputs the probability of catching the user selected Pokemon with the user select ball, then simulates attempting
to catch the Pokemon. A full explanation of the formula and mechanics of catching can be found at https://www.dragonflycave.com/mechanics/gen-vi-vii-capturing
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

/*
  This function uses the Gen6/7 Catch Rate Formula to determine the final catch rate. Since we would use a Grass value of 1, it has been
  omitted along with the Status and o-Power modifiers.
*/
int CalcFinalCaptureRate(const int maxHP, const int currentHP, const int captureRate, const double ballBonus)
{
  return (((3 * maxHP) - (2 * currentHP)) * captureRate * ballBonus) / (3 * maxHP); 
}

/*
  Returns the probability of a successful capture
*/
int ChanceOfCapture(const int finalCaptureRate)
{
  return pow((((double) finalCaptureRate) / 255.0 ), 0.75) * 100;
}

/*
  This function returns true (pokemon breaks out of the ball) when the randomly generated break out power meets or exceeds the break out threshold
*/
int BreakFreeAttempt(int finalCaptureRate)
{
  int breakThreshold = 65536.0 / pow((255.0 / (double) finalCaptureRate), 0.1875);
  int breakPower = rand() % (65535 + 1);
  return breakThreshold <= breakPower;
}


int main(int argc, char *argv[])
{
  srand(time(0));
  int pokemonToCatch = 0;
  printf("Welcome to the Pokemon Catch Chance Calculator! ");

  while(pokemonToCatch < 1 || pokemonToCatch > 3)
  {
    printf("Please enter the number of the Pokemon you would like to catch\n 1. Bulbasaur - Lvl. 5\n 2. Charmeleon - Lvl. 16\n 3. Blastoise - Lvl. 36\n");
    if(fscanf(stdin, "%d", &pokemonToCatch) < 1)
    {
    fprintf(stderr, "fscanf function failed! Exiting now!\n");
    exit(1);
    }
    if (pokemonToCatch < 1 || pokemonToCatch > 3) { printf("Invalid Selection!\n"); }
  }

/*
  The maxHP of each pokemon was calculated with 0 EVs and 31 IVs. For simplicity, the currentHP will always be 10% of the maxHP.
*/
  int maxHP;
  int captureRate = 45; // All Starter Pokemon have a catch rate of 45
  switch(pokemonToCatch)
  {
    case 1 : // Bulbasaur Lvl 5
      maxHP = 21;
      break;
    case 2 : // Charmeleon Lvl 16
      maxHP = 49;
      break;
    case 3 : // Blastoise Lvl 36
      maxHP = 114;
      break;
  }
  int currentHP = (maxHP * 10) / 100; // currentHP will be 10% off maxHP

  //printf("maxHP is: %d, currentHP is: %d\n", maxHP, currentHP); // FOR DEBUGGING
  
  int ballType = 0;
  
  while (ballType < 1 || ballType > 3)
  {
    printf("Please enter the number of the ball you want to throw.\n 1. Poke Ball\n 2. Great Ball\n 3. Ultra Ball\n");
    if(fscanf(stdin, "%d", &ballType) < 1)
    {
    fprintf(stderr, "fscanf function failed! Exiting now!\n");
    exit(1);
    }
    if(ballType < 1 || ballType > 3) { printf("Invalid Selection!\n"); }
  }
  double ballBonus;
  switch(ballType)
  {
    case 1 :
      ballBonus = 1.0;
      break;
    case 2 :
      ballBonus = 1.5;
      break;
    case 3 :
      ballBonus = 2.0;
      break;
  }

 // printf("Your ballBonus is: %f\n", ballBonus);    // FOR DEBUGGING

  int finalCaptureRate = CalcFinalCaptureRate(maxHP, currentHP, captureRate, ballBonus);

  // printf("The final catch rate is: %d\n", finalCaptureRate);   // FOR DEBUGGING

  printf("You have a: %d%% chance of capture!\nLet's see if you catch it!\n\n*Throws ball*\n\n", ChanceOfCapture(finalCaptureRate) );

  int isCaught = 1;
  for(int i = 0; i < 4; ++i) // Pokemon has 4 chances to break out of the ball
  {
    if(!BreakFreeAttempt(finalCaptureRate))
    { 
      if(i != 0)
      { 
        sleep(2); // For dramatic effect
        printf("*Wiggle*\n"); 
      } 
    }
    else 
    {
      isCaught = 0;
      sleep(1);
      printf("\nShoot! The pokemon broke free.\n");
      break;
    }
  }
  if(isCaught == 1) { printf("\nHurray! You caught it!\n"); } 

  return 0;
}

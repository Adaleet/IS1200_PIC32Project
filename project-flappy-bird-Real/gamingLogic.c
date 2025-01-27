/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   Modified file since 2023-11-20 by Adalet Adiljan and Dana Ghafour

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declarations for these labs */
#include <math.h>     /* Computational declarations for the gaming logic */ 
#include <stdio.h>
#include <string.h>

/* ---------------------------------GAMING-LOGIC-------------------------------- */

/* Declaring the global variables */

                          /* BY DANA  */
int state; // Game state variable, representing different states of the game.
int obstacleReset[64*2]; // Array to store the initial positions of obstacles before any reset.
int obstacle[64*2]; // Array to store the current positions of obstacles in the game.
int X = 13; // Initial x-coordinate of the bird.
int Y = 4; // Initial y-coordinate of the bird.
int horizontal; // Tube in the x-axis
int vertical; // Tube in the y-axis
int i;
int j;

/* Interrupt Service Routine */

                          /* BY DANA  */
void user_isr( void ) // Interrupt Service Routine (ISR) for the user. 
// This function is included to handle interrupts, but it's currently empty and does not perform any actions.
{                     
  return; // The "return" statement indicates that the ISR has completed its execution.
}

void display_string_clear (void) { // Metod som har fåtts från lärarna
  int i;
  for (i = 0; i < 4; i++){
    display_string(i, "");  // Skärm-rensning från föregående spelomgången.
  }
}
void clearDisplay(void) { // Function to clear the display by setting all pixels to black
  int i = 0; // Initialize a counter variable for the loop
  // 128 * 32 = 4096, 4096/8 = 512, where 8 is the number of bits in a byte
  for(i = 0; i < 512; i++) { // Loop through each pixel in the display (512 pixels)
    icon[i] = 255; // Set the pixel value to 255, turning it on (black)
  }
}


/*---------------------------------Pixel related, Pre-Game setup ------------------------- ---- */


                          /* BY ADALAT  */
void pixelScreen(int x, int y) {
  // Gränskontroll för att säkerställa att koordinaterna är inom pixelfältet
  if (y < 0 || x < 0 || x > 127 || y > 32) { // Om koordinaterna är utanför gränserna, returnera utan att göra några ändringar
    return;
  } 

  // Justera x och y för pixlar som sträcker sig över flera byte
  if (y >= 8 && y <= 32) {
    x += (y / 8) * 128; // Flytta x till det motsvarande bytet
    y %= 8; // Justera y till den återstående bitpositionen inom bytet
  }

  // Pixelmanipulation baserad på y-koordinaten
  if (y == 0) {
    int write = ~1; // Skapa en bitmask med alla bitar satta utom den första
    icon[x] = icon[x] & write; // Stäng av den första biten för den angivna pixeln
  } 
  else {
    int k = 1; // Initialisera en bitmask med den minst signifikanta biten satt
    int l; // Loopvariabel

    // Iterera genom bitar för att hitta den specifika bit som motsvarar y-koordinaten
    for (l = 1; l < 8; l++) {
      k *= 2; // Multiplicera bitmasken med 2 i varje iteration
      if (y == l) {
        int write = ~k; // Invertera bitmasken för den specifika bitpositionen som matchar y-koordinaten
        icon[x] = icon[x] & write; // Stäng av den angivna biten för pixeln
      }
    }
  }
}


                          /* BY DANA  */
void birdSpace(int birdX, int birdY) {
    pixelScreen(birdX, birdY); // Draw a single point (bird) at the specified coordinates.
}

// Helper function to check if a coordinate is valid


                          /* BY ADALAT  */

/*int isValidCoordinate(int x, int y) {
    // Return 1 if x and y are within valid pixel coordinates, otherwise return 0.
    return (x >= 0 && x < 512 && y >= 0 && y < 32);
}*/


// Helper function to process the coordinate logic
void processCoordinate(int x, int y) { 
    if (y >= 8) {          
      x += 128 * (y / 8); 
      y %= 8;             
  }


    int mask;  
  // (A bitmask is a binary pattern used for selectively manipulating bits within a binary value. 
  // It is employed to perform bitwise operations, such as setting or clearing specific bits.)

    // If the y-coordinate is 0, set the mask to invert the least significant bit.
    if (y == 0) {  
        mask = ~1;  
    } else {  
      // In the case where the y-coordinate is not 0 and less than 8,
      // this block is executed to determine the appropriate bitmask for pixel manipulation.
        int l; 
        int k = 1; // Initialize a bitmask with the least significant bit set.
        for (l = 1; l < 8; l++) {  
            k *= 2;  // Multiply the bitmask by 2 in each iteration to set the next bit.
            if (y == l) {  
                mask = ~k;  // Invert the bitmask for the specific bit position matching the y-coordinate.
                break;  // Exit the loop after finding the match.
            }
        }
    }

    // Check if the result of applying the bitmask to the current pixel is not all 0s.
    if ((~(icon[x] | mask)) != 0) {  
        state = 1;  // Set the game state to 1, indicating a collision with an obstacle.
    }

    // Apply the bitmask to the current pixel in the icon array.
    icon[x] = icon[x] & mask;  
}
                                              


                          /* BY DANA  */
void handleGameOver(){ // This function handles the game over scenario.
  display_image(0, icon); // Display the game over image on the screen (using display_image function).
  delay(100); // Introduce a delay of 100 milliseconds for visibility before proceeding.
}

/*---------------------------User Interaction and Feedback--------------------------------- */

                          /* BY ADALAT  */

void displayGameOver(void) { // This function displays the game over screen.
    display_image(0, icon); // Display the game over image on the screen (using display_image function).
    delay(1000); // Introduce a delay of 1000 milliseconds for visibility before proceeding.
    display_string_clear(); // Clear any existing text on the screen.
    display_string(2, "   Game Over!"); // Display the text "Game Over!" on the screen at line 2.
    display_update(); // Update the display to reflect the changes.
}


                          /* BY DANA & ADALAT  */

void countDown(void){ // This function displays a countdown before starting the game.
  char numberThree[] = "              3";  // String representing "3" with leading spaces.
  char numberTwo[] = "                2";  // String representing "2" with leading spaces.
  char numberOne[] = "                1";  // String representing "1" with leading spaces.
  int len3 = sizeof(numberThree) - 1; // Calculate the length of the string for "3".
  int len2 = sizeof(numberTwo) + 1; // Calculate the length of the string for "2".
  int len1 = sizeof(numberOne) - 1; // Calculate the length of the string for "1".
  int i; // Loop variable.
    for(i = 0; i < len3; i++){ // Loop to display the countdown for "3".
      display_string_clear();  // Clear any existing text on the screen.
      display_string(2, &numberThree[i]);  // Display the current character of "3" on line 2.
      display_update();  // Update the display to reflect the changes.
      delay(30); // Introduce a delay of 30 milliseconds for each character.
    }
    for (i = 0; i < len2; i++) { // Same as above.
        display_string_clear(); 
        display_string(2, &numberTwo[i]);
        display_update(); 
        delay(30); 
    }
    for (i = 0; i < len1; i++) { // Same as above.
        display_string_clear(); 
        display_string(2, &numberOne[i]); 
        display_update(); 
        delay(30); 
    }
    display_string_clear();  // Clear the screen.
    display_string(2, "       Go ! "); // Display "Go!" on line 2.
    display_update(); // Update the display to reflect the changes.
    delay(1000); // Introduce a delay of 1000 milliseconds.
    state = 0; // Set the game state to 0, indicating the start of the game.
}


                          /* BY DANA & ADALAT  */
void menu(void) { // This function displays the main menu screen.
    char spaceBird[] = "          Space Bird ";  // A string representing the main menu title.

    int len = sizeof(spaceBird) - 1; // Calculate the length of the string.

    int i;  
    for(i = 0; i < len; i++){ // Iterate through each character in the string.
      display_string_clear(); // Clear any existing text on the screen.
      display_string(2, &spaceBird[i]); // Display a scrolling version of the main menu title.
      display_update();  // Update the display to reflect the changes.
      delay(70); // Introduce a delay for the scrolling effect.
  }
  if(getbtns()==2){ // Check if button 2 is pressed.
    countDown();  // If button 2 is pressed, initiate the countdown for game start.
  }
}

/*-----------------------------Main Game Loop / Execution Core and Game Logic ----------------------------------------------- */



                          /* BY ADALAT */
         
/* This function is called repetitively from the main program */
void gameLogic( void )
{
  int i;
  int j;

  if (state == 1) {   // Check if the game is in the "Game Over" state.
    displayGameOver(); // Display the game over screen.
    delay(2000); // Introduce a delay of 2000 milliseconds.
    display_update(); // Update the display to reflect the changes.
    delay(2000); // Introduce another delay of 2000 milliseconds.
    display_string_clear(); // Clear any existing text on the screen.

    countDown(); // Initiate the countdown for the next game.
      

// Reset all objects on the game board.
    for (i = 0; i < 64 * 2; i++) {
      obstacle[i] = obstacleReset[i]; // Reset obstacle positions.
    }

    X = 13; // Reset the X coordinate of the bird.
    Y = 4;  // Reset the Y coordinate of the bird.
    clearDisplay(); // Clear the entire display.
    display_image(0, icon); // Display the initial state of the game.
    display_string_clear(); // Clear any existing text on the screen.
    display_update(); // Update the display to reflect the changes.
    state = 0; // Set the game state to 0, indicating the start of the game.
  }

  if (state == 2) {  // Check if the game is in the "Main Menu/Start Screen" state.
    int textPosition = 0; // Initialize the text position for scrolling.
    while (state == 2) { // Loop while in the Main Menu state.
      display_string_clear(); // Clear any existing text on the screen.
      delay(10);
      display_string(2, " Authors: Dana & Adalet " + textPosition); // Display author names with scrolling effect.
      display_update(); // Update the display to reflect the changes.
      delay(70); // Introduce a delay of 70 milliseconds.
      textPosition++;

      if (textPosition >= 60) { // If the text position exceeds 60 (end of scrolling), execute the following.
        menu(); // Display the menu.
        countDown(); // Initiate the countdown for the next game.
        state = 0; // Set the game state to 0, indicating the start of the game.
      }
    }
  }
clearDisplay();

/* ----------------------------------------- Button related -------------------------------------*/ 
  
Y -= 1; // The bird gravitates upwards along with the obstacles

// Logical function for the tubes movement:
int k, l; // Declaration of variables k and l.

for (i = 0; i <= 64; i = i + 2) { // Loop through each obstacle pair (x, y).

    // Draw the tube based on the obstacle position
    for (j = 0; j < 60; j++) { // Loop through the vertical positions of the tube.
        for (k = 0; k < 5; k++) { // Loop through the horizontal positions of the tube.
            if (tubes[(5 * j) + k] == 1) { // Check if the current pixel of the tube is active (1).
                pixelScreen(obstacle[i] + k, obstacle[i + 1] + j); // Display the tube on the screen.
            }
        }
    }

    // Update the tube's position
    obstacle[i] = obstacle[i] - 1; // Move the tube to the left.

    if (obstacle[i + 1] == -80) { // Check if the tube has reached the left edge.
        obstacle[i + 1] = obstacle[i + 1] + 120; // Move the tube to the right side of the screen.
    }

    if (obstacle[i + 1] < -80) { // Check if the tube is completely off the screen.
        obstacle[i + 1] = obstacle[i + 1] + 1; // Increment the vertical position of the tube.
    }
}

int buttonState = getbtns(); // Read the state of the buttons.

switch (buttonState) {
    case 1:
        // Restart the game
        state = 2; // Set the game state to 2 (Main Menu/Start Screen).
        break;

    case 2:
        // When pressing button 2, the player goes back to the main menu.
        state = 2; // Set the game state to 2 (Main Menu/Start Screen).
        break;

    case 4:
        // Move bird downward
        Y += 3; // Move the bird downward by incrementing its Y-coordinate.
        break;

    // Add more cases if needed

    default:
        // Handle other button states or add an empty block if not needed
        break;
}
birdSpace(X, Y); // Update the bird's position on the screen.
display_image(0, icon); // Display the updated screen.
delay(50); 
}
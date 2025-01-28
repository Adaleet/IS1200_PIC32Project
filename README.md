# IS1200_PIC32Project

# Space Bird Game on PIC32 (IS1200 Mini Project)

## Overview
This project is a **real-time interactive game**, inspired by the classic Flappy Bird, developed exclusively for the **ChipKIT Uno32 microcontroller** as part of the IS1200 course. The game challenges players to navigate a bird through obstacles, with increasing difficulty as the game progresses.

Key features include:
- Obstacle movement and collision detection.
- User interaction via button inputs and flickering LED lamps to enhance the interactive user experience.

## Features
- **Dynamic Gameplay**: Obstacle movement and real-time collision detection.
- **Leaderboard**: Displays a countdown before the game starts and shows "Game Over" on the I/O Shield when the player loses.
- **Difficulty Scaling**: Gradually increases the sizes of the obstacles (tubes) to increase difficulty.
- **Modular Design**: Includes state management, countdown timers, and ISRs (Interrupt Service Routines).
- **Custom Graphics**: Pixel-based rendering for a retro gaming experience.

## Technologies Used
- **Programming Languages**: C and Assembly.
- **Microcontroller**: ChipKIT Uno32.
- **Development Environment**: MPLAB X Integrated Development Environment (IDE) with the XC32 Compiler: [See following reference manual](https://ww1.microchip.com/downloads/en/DeviceDoc/61146B.pdf

## How to Play
1. Power on the ChipKIT Uno32 microcontroller.
2. Use the buttons on the board to:
   - Start and restart the game. 
   - Navigate the bird upward or downward.
4. Avoid obstacles by timing your movements.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/adaleet/IS1200_PIC32Project.git

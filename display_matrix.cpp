#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "Arduino_LED_Matrix.h"
#include "display_matrix.h"

#define ROWS 8
#define COLUMNS 12

typedef enum
{
  DISPLAY_NONE,
  DISPLAY_NORTH,
  DISPLAY_EAST,
  DISPLAY_SOUTH,
  DISPLAY_WEST,
  N_OF_DISPLAYS
} displays_t;

// display none
static const uint8_t NONE[8][12] = {
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Row 0
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Row 1
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Row 2
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Row 3
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Row 4
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Row 5
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},  // Row 6
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}   // Row 7
};

// display north
static const uint8_t NORTH[8][12] = {
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},  // Row 0
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},  // Row 1
  {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},  // Row 2
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // Row 3
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},  // Row 4
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},  // Row 5
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},  // Row 6
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0}   // Row 7
};

// display south
static const uint8_t SOUTH[8][12] = {
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},  // Row 0
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},  // Row 1
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},  // Row 2
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},  // Row 3
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // Row 4
  {0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},  // Row 5
  {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},  // Row 6
  {0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0}   // Row 7
};

// display east
static const uint8_t EAST[8][12] = {
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},  // Row 0
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},  // Row 1
  {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},  // Row 2
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // Row 3
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // Row 4
  {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},  // Row 5
  {0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},  // Row 6
  {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}   // Row 7
};

// display west
static const uint8_t WEST[8][12] = {
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},  // Row 0
  {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},  // Row 1
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // Row 2
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // Row 3
  {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // Row 4
  {0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},  // Row 5
  {0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},  // Row 6
  {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}   // Row 7
};

ArduinoLEDMatrix matrix; 
static uint8_t current_state = DISPLAY_NONE;
static uint8_t frame_buffer[ROWS][COLUMNS] = {0};

/* update the display frame buffer based on state input */
static void updateDisplayFrame(displays_t state)
{
  switch(state)
  {
    case DISPLAY_NONE:
      memcpy(frame_buffer, NONE, sizeof(frame_buffer));
      break;
    case DISPLAY_NORTH:
      memcpy(frame_buffer, NORTH, sizeof(frame_buffer));
      break;
    case DISPLAY_EAST:
      memcpy(frame_buffer, EAST, sizeof(frame_buffer));
      break;
    case DISPLAY_SOUTH:
      memcpy(frame_buffer, SOUTH, sizeof(frame_buffer));
      break;
    case DISPLAY_WEST:
      memcpy(frame_buffer, WEST, sizeof(frame_buffer));
      break;
    default:
      break;
  }
}

void DisplayInit(void)
{
  matrix.begin();
}

void DisplayCycle(void)
{
  updateDisplayFrame((displays_t)current_state);
  matrix.renderBitmap(frame_buffer, ROWS, COLUMNS);
  current_state++;
  if(current_state > N_OF_DISPLAYS)
  {
    current_state = DISPLAY_NONE;
  }
}
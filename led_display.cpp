#include <Arduino.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "Arduino_LED_Matrix.h"
#include "led_display.h"

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

void LedInit(void)
{
  matrix.begin();
}

void LedCycle(void)
{
  updateDisplayFrame((displays_t)current_state);
  matrix.renderBitmap(frame_buffer, ROWS, COLUMNS);
  current_state++;
  if(current_state > N_OF_DISPLAYS)
  {
    current_state = DISPLAY_NONE;
  }
}
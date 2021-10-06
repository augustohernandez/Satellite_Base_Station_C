/**
 * file: satellite_common.c
 */

/*****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "data_structs.h"
#include "satellite_common.h"

/*****************************************************************************/

/**
 * line  [char *]          : Pointer to the textline to parse
 * token [char]            : Word separator tag
 * data  [sensor_data_t *] : Pointer to the sensor_data_t data structure
 */
void split_sensor_data(char* line, char token, sensor_data_t* data)
{
  int16_t* data_ptr[4] = {&(data->temperature), &(data->altitude), &(data->velocity), &(data->power)};
  char* save_ptr = line;
  char* split = NULL;
  int16_t i = 0;

  /* Look for the first token and we split it from line */
  split = strtok_r(save_ptr, &token, &save_ptr);
  if (split != NULL)
  {
    /* Convert the value to integer and store it at the apropriate position */
    *data_ptr[i] = atoi(split);

    /* Increment the number of already parsed values */
    i++;
  }

  /* Repeat the process if there are some more tokens */
  while (split != NULL)
  {
    /* Look for successive tokens and split them from line */
    split = strtok_r(NULL, &token, &save_ptr);
    if (split != NULL)
    {
      /* Convert the value to integer and store it to the appropriate position */
      *data_ptr[i] = atoi(split);

      /* Increment the number of already parsed values */
      i++;
    }
  }
}

/*****************************************************************************/

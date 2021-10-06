/**
 * file: base_station_common.c
 */

/*****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>

#include "data_structs.h"
#include "base_station_common.h"

/*****************************************************************************/

/**
 * buffer      [uint8_t  *]      : Pointer to the buffer that holds the data
 * msg_type    [uint8_t  *]      : Pointer to the msg_type variable
 * msg_id      [uint32_t *]      : Pointer to the msg_id variable
 * sensor_data [sensor_data_t *] : Pointer to the sensor_data variable
 */
void parse_sensor_data(uint8_t* buffer, uint8_t* msg_type, uint32_t* msg_id, sensor_data_t* sensor_data)
{
  uint32_t scratch;

  *msg_type = buffer[0];

  scratch = (buffer[4] << 24 | buffer[3] << 16 | buffer[2] << 8 | buffer[1] << 0);
  *msg_id = ntohl(scratch);
  
  scratch = (buffer[6] << 8 | buffer[5]);
  sensor_data->temperature = ntohs(scratch);
  
  scratch = (buffer[8] << 8 | buffer[7]);
  sensor_data->altitude = ntohs(scratch);
  
  scratch = (buffer[10] << 8 | buffer[9]);  
  sensor_data->velocity = ntohs(scratch);

  scratch = (buffer[12] << 8 | buffer[11]);
  sensor_data->power = ntohs(scratch);
}

/*****************************************************************************/

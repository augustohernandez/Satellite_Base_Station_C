/**
 * file: data_structs.h
 */

#ifndef _DATA_STRUCTS_H_
#define _DATA_STRUCTS_H_

/*****************************************************************************/

#include <stdint.h>


/*****************************************************************************/

enum
{
  MESSAGE_TYPE_NONE = 0,
  MESSAGE_TYPE_DATA = 1,
  MESSAGE_TYPE_ACK =  2
};

typedef struct 
{
  int16_t temperature;
  int16_t altitude;
  int16_t velocity;
  int16_t power;
} sensor_data_t;

typedef struct __attribute__((packed))
{
  uint8_t msg_type;
  uint32_t msg_id;
  int16_t temperature;
  int16_t altitude;
  int16_t velocity;
  int16_t power;
} msg_data_t;

typedef struct __attribute__((packed))
{
  uint8_t msg_type;
  uint32_t msg_id;
} msg_ack_t;

/*****************************************************************************/

#endif /* _DATA_STRUCTS_H_ */

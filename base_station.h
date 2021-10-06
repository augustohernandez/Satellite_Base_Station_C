/**
 * file: base_station.h
 */

#ifndef _BASE_STATION_H_
#define _BASE_STATION_H_

/*****************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define BASE_STATION_BUFFER_SIZE 64
/*****************************************************************************/

typedef struct
{
  int                 socket_fd;
  struct sockaddr_in  local_addr;
  struct sockaddr_in  remote_addr;
  FILE*               file_des;
  FILE*               log_des;
  char*               log_ptr;
  uint8_t             data_buffer[BASE_STATION_BUFFER_SIZE];
  uint8_t             log_buffer[BASE_STATION_BUFFER_SIZE];
} sta_data_t;

/*****************************************************************************/

#endif /* _BASE_STATION_H_ */

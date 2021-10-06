/**
 * file: data_link.h
 */

#ifndef _DATA_LINK_H_
#define _DATA_LINK_H_



/*****************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#define DATA_LINK_BUFFER_SIZE 64
/*****************************************************************************/

typedef struct
{
  int                 socket_fd;
  struct sockaddr_in  local_addr;
  struct sockaddr_in  remote_addr;
  FILE*               file_des;
  FILE*               log_des;
  char*               log_ptr;
  uint8_t             data_buffer[DATA_LINK_BUFFER_SIZE];
  uint8_t             log_buffer[DATA_LINK_BUFFER_SIZE];
} sta2_data_t;

/*****************************************************************************/








#endif /* _DATA_LINK_H_ */

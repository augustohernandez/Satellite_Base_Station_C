/**
 * file: satellite.c
 */

/*****************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>
#include <ctype.h>
#include <signal.h>
#include "data_structs.h"
#include "ports.h"
#include "satellite.h"
#include "satellite_common.h"

#define FILENAME "satellite_data.txt"

/*****************************************************************************/
//static msg_ack_t msg2_ack;
static msg_data_t msg_data;
static sat_data_t sat_data;
static sensor_data_t sensor_data;
static bool is_finished = false;

/*****************************************************************************/

static void signal_handler(int dummy);

/*****************************************************************************/
void err_sys(char *mess) { 
	perror(mess);
	exit(1);
}

int main(int argc, char* argv[])
{

  int id_number=1;
  int line_count=0;
  size_t line_buf_size = 0;
  ssize_t line_size;

  sat_data.log_ptr = NULL;
  
  if (argc !=3) {
	fprintf(stderr, "USAGE: %s <ip_server> <target:base_station/data_link>\n", argv[0]);
	exit(1);
  }

  /* Create socket for sending/receiving datagrams */
  if ((sat_data.socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
	err_sys("Error socket!");
  }
  /* Assign a port to socket */
  memset(&sat_data.remote_addr, 0, sizeof(sat_data.remote_addr)); //memory reset
  sat_data.remote_addr.sin_family = AF_INET; //Internet/IP
  sat_data.remote_addr.sin_addr.s_addr = inet_addr(argv[1]); //loopback 127.0.0.1

  /* Optional communication mode */
  if (strcmp(argv[2], "base_station") == 0) {
  sat_data.remote_addr.sin_port = htons(BASE_STATION_UDP_RX_PORT); //server port
  } else if (strcmp(argv[2],"data_link") == 0) {
	sat_data.remote_addr.sin_port = htons(DATA_LINK_UDP_RX_PORT_SATELLITE); //server port
 	} else {
		  fprintf(stderr, "USAGE: %s <ip_server> <target::base_station/data_link>\n", argv[0]);
	exit(1);
  }

  /* Bind client_socket (need it to receiving datagram) */
  sat_data.local_addr.sin_family = AF_INET; //Internet/IP
  sat_data.local_addr.sin_addr.s_addr = htonl(INADDR_ANY); //any interface ip
  sat_data.local_addr.sin_port = htons(SATELLITE_UDP_RX_PORT); //client port

  if (bind(sat_data.socket_fd, (struct sockaddr *) &sat_data.local_addr, sizeof(sat_data.local_addr))<0) {
				err_sys("Error socket!");
  }

  /* Open file to read */
  sat_data.file_des = fopen(FILENAME, "r");
  if (!sat_data.file_des) {
    fprintf(stderr, "Error opening file '%s'\n", FILENAME);
    return EXIT_FAILURE;
  }

  /* Get the first line of the file */
  line_size = getline(&sat_data.log_ptr, &line_buf_size, sat_data.file_des);
  printf("content first_line:%s\n", sat_data.log_ptr);

  /* Loop through until we are done with the file */
  while (line_size>0) {
  /* Increment our line count */
  line_count++;
  /* Get the next line */
  line_size = getline(&sat_data.log_ptr, &line_buf_size, sat_data.file_des);
  
  
  /* Parser one line */  
  split_sensor_data(sat_data.log_ptr , ',', &sensor_data);


  /* Copy and add the necessary information to the structure */
  msg_data.msg_type=(uint8_t)MESSAGE_TYPE_DATA;
  printf("Type: %d\n", (int)msg_data.msg_type);
  msg_data.msg_id=(uint32_t)id_number;
  printf("Id: %d\n", (int)msg_data.msg_id);
  id_number++;
  msg_data.temperature=sensor_data.temperature;
  printf("Temperature: %hi\n", (signed short)msg_data.temperature);
  msg_data.altitude=sensor_data.altitude;
  printf("Altitude: %hi\n", (signed short)msg_data.altitude);
  msg_data.velocity=sensor_data.velocity;
  printf("Velocity: %hi\n", (signed short)msg_data.velocity);
  msg_data.power=sensor_data.power;
  printf("Power: %hi\n", (signed short)msg_data.power);
  
  /* Send line_structure to the server */
  if (sendto(sat_data.socket_fd, &msg_data, sizeof(msg_data), 0, (struct sockaddr *) &sat_data.remote_addr, sizeof(sat_data.remote_addr)) == -1) {
					fprintf(stderr, "sendto error\n");
					exit(1);
  }
 

  /* Sleep random [0 us, 999999 us = 1 second] */
  int max_time=999999;
  srand(time(NULL));
  usleep(rand() % max_time);
  printf("Waiting for a random time...\n");
  
  }
  
  /* Free the allocated line buffer and close the file*/
  free(sat_data.log_ptr);
  sat_data.log_ptr =NULL;
  fclose(sat_data.file_des);
  close(sat_data.socket_fd);
  
  /* We register the CTRL+C interruption signal */
  
  if (signal(SIGINT, signal_handler) == SIG_ERR)
	printf("\ncan't catch SIGINT\n");

  while(!is_finished)
  {
    sleep(1);

  }

  return 0;
}

/*****************************************************************************/

static void signal_handler(int dummy)
{
  if (dummy== SIGINT) printf("Received SIGINT\n");
  is_finished = true;
  
}

/**
 * file: base_station.c
 */

/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "data_structs.h"
#include "ports.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <errno.h>
#include <time.h>
#include <netdb.h>
#include <ctype.h>
#include <sys/time.h>
#include "base_station.h"
#include "base_station_common.h"

#define FILENAME2 "base_station.txt"

/*****************************************************************************/

static sta_data_t sta_data;
static msg_data_t msg_data;
//static sensor_data_t sensor_data;
//static msg_ack_t msg_ack;
static bool is_finished = false;

/*****************************************************************************/

static void signal_handler(int dummy);

void err_sys(char *mess) { 
	perror(mess);
	exit(1);
}


/*****************************************************************************/

struct casting {
		int type;
		int id;
		signed short temperature;
		signed short altitude;
		signed short velocity;
		signed short power;
			  };


int main(int argc, char* argv[])
{

  if (argc !=1) {
	fprintf(stderr, "USAGE: %s <file_name>\n", argv[0]);
	exit(1);
  }
  
  int result;
  int received = 0;
  unsigned int clientlen;
  struct timeval timeout={3,0}; //set timeout for 3 seconds
  struct casting sensor_d;
  time_t now = time(NULL);
  sta_data.log_ptr = NULL;
  /* Create socket for sending/receiving datagrams */
  sta_data.socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); 
  if ((sta_data.socket_fd) < 0) {
	perror("Error socket");
	exit(1);
  }

  /* Assign a port to socket */
  memset(&sta_data.remote_addr, 0, sizeof(sta_data.remote_addr)); //memory reset
  sta_data.remote_addr.sin_family = AF_INET; //Internet/IP
  sta_data.remote_addr.sin_addr.s_addr = ntohl(INADDR_ANY); //any incomming interface
  sta_data.remote_addr.sin_port = ntohs(BASE_STATION_UDP_RX_PORT); //server port


  result = bind(sta_data.socket_fd, (struct sockaddr *) &sta_data.remote_addr, sizeof(sta_data.remote_addr));
  if (result < 0) {
	
	err_sys("Error bind");
	
  }
  /* open file to write */
        sta_data.file_des=fopen(FILENAME2,"w");
	if (sta_data.file_des == NULL) {
		fprintf(stderr, "\nError opend file\n");
	}
	

  /* Infinite loop */
  for ( ; ; ) {
	
	/* set receive UDP message timeout */
	setsockopt(sta_data.socket_fd, SOL_SOCKET,SO_RCVTIMEO, (char*)&timeout, sizeof(struct timeval));

	/* received information from client */
	
	clientlen = sizeof(sta_data.local_addr);
	received = recvfrom(sta_data.socket_fd, &msg_data, sizeof(msg_data), 0, (struct sockaddr *) &sta_data.local_addr, &clientlen);
	
	if ( (received == -1) && (errno == EAGAIN) ) {
		perror("Connection (timed out) > 3 seconds");
		received = recvfrom(sta_data.socket_fd, &msg_data, sizeof(msg_data), 0, (struct sockaddr *) &sta_data.local_addr, &clientlen);
	} else if (received < 0) {

		   perror("Error receiveing datagram from client");
		   exit(1);
		}

	if ((received >= 0) && (msg_data.msg_type == 1)) {
		printf("Datagram received: ok!\n");

		
		/* casting structure to file */
		sensor_d.type = (int)msg_data.msg_type;
		sensor_d.id = (int)msg_data.msg_id;
		sensor_d.temperature = (signed short)msg_data.temperature;
		sensor_d.altitude = (signed short)msg_data.altitude;
		sensor_d.velocity = (signed short)msg_data.velocity;
		sensor_d.power = (signed short)msg_data.power;
		
		printf("Type: %d\n", (int)msg_data.msg_type);
		printf("Id: %d\n", (int)msg_data.msg_id);
		printf("Temperature: %hi\n", (signed short)msg_data.temperature);
		printf("Altitude: %hi\n", (signed short)msg_data.altitude);
		printf("Velocity: %hi\n", (signed short)msg_data.velocity);
		printf("Power: %hi\n", (signed short)msg_data.power);
		
		sta_data.log_ptr=(char*)malloc(sizeof(char)*BASE_STATION_BUFFER_SIZE);

		if (sta_data.log_ptr == NULL) {
						 perror("Memory not allocated\n");
		   				exit(1);
		}
						
		fputs(asctime(localtime(&now)), sta_data.file_des);
		sprintf(sta_data.log_ptr, "%d %hi %hi %d %hi %hi\n", sensor_d.type, sensor_d.id,sensor_d.temperature,sensor_d.altitude,
		sensor_d.velocity, sensor_d.power);

		fputs(sta_data.log_ptr,sta_data.file_des);
		free(sta_data.log_ptr);		

		//parse_sensor_data(&sta_data.data_buffer[BASE_STATION_BUFFER_SIZE], &msg_data.msg_type, &msg_data.msg_id, &sensor_data);

			
	} 

	
     }
  
  fclose(sta_data.file_des); //close file
 
  /* Register CTRL+C interrupt signal */
  if (signal(SIGINT, signal_handler) == SIG_ERR)
	printf("\ncan't catch SIGINT\n");


  while(!is_finished){
  sleep(1);   
  }

  return 0;
}

/*****************************************************************************/

static void signal_handler(int dummy)
{
  if (dummy == SIGINT) printf("Received SIGINT\n");
  is_finished = true;
 
}

/*****************************************************************************/

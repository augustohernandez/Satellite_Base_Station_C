/**
 * file: data_link.c
 */ 
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
#include "data_link.h"
#include "base_station.h"
#include "satellite.h"
#include "satellite_common.h"

#define FILENAME3 "data_link.txt"


/*****************************************************************************/

static msg_data_t msg_data;
static sta2_data_t sta2_data;
static sta2_data_t sta3_data;
static sta2_data_t sta4_data;
static sta2_data_t sta5_data;
//static sensor_data_t sensor_data;
static bool is_finished = false;

/*****************************************************************************/

static void signal_handler(int dummy);

/*****************************************************************************/
void err_sys(char *mess) { 
	perror(mess);
	exit(1);
}

  struct casting2 {
		int type;
		int id;
		signed short temperature;
		signed short altitude;
		signed short velocity;
		signed short power;
			  };


int main(int argc, char* argv[])
{



  /* DOWNLINK implementation SATELLITE-DATA_LINK */

  int result2;
  unsigned int clientlen2;
  int received2 = 0;
  struct casting2 sensor_d2;
  int result3;
  unsigned int clientlen3;
  int received3 = 0;
  time_t now = time(NULL);
  sta2_data.log_ptr = NULL;

  /* Create socket for sending/receiving datagrams */
  sta2_data.socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); 
  if ((sta2_data.socket_fd) < 0) {
	err_sys("Error socket");
	
  }

  /* Assign a port to socket */
  memset(&sta2_data.remote_addr, 0, sizeof(sta2_data.remote_addr)); //memory reset
  sta2_data.remote_addr.sin_family = AF_INET; //Internet/IP
  sta2_data.remote_addr.sin_addr.s_addr = ntohl(INADDR_ANY); //any incomming interface
  sta2_data.remote_addr.sin_port = ntohs(DATA_LINK_UDP_RX_PORT_SATELLITE); //server port
  

  result2 = bind(sta2_data.socket_fd, (struct sockaddr *) &sta2_data.remote_addr, sizeof(sta2_data.remote_addr));
  if (result2 < 0) {
	
	err_sys("Error bind");
	
  }
  /* open file to write */
        sta2_data.file_des=fopen(FILENAME3,"w");
	if (sta2_data.file_des == NULL) {
		fprintf(stderr, "\nError opend file\n");
	}

  /* Infinite loop */
  for ( ; ; ) {
	

	/* received information from client */
	perror("Waiting for satellite data...");
	clientlen2 = sizeof(sta2_data.local_addr);
	received2 = recvfrom(sta2_data.socket_fd, &msg_data, sizeof(msg_data), 0, (struct sockaddr *) &sta2_data.local_addr, &clientlen2);
	
	if (received2 < 0) {

		   err_sys("Error receiveing datagram from client");
		  
		}

	printf("Datagram received: ok!\n");
	printf("Type: %d\n", (int)msg_data.msg_type);
	printf("Id: %d\n", (int)msg_data.msg_id);
	printf("Temperature: %hi\n", (signed short)msg_data.temperature);
	printf("Altitude: %hi\n", (signed short)msg_data.altitude);
	printf("Velocity: %hi\n", (signed short)msg_data.velocity);
	printf("Power: %hi\n", (signed short)msg_data.power);
	
	sensor_d2.type = (int)msg_data.msg_type;
	sensor_d2.id = (int)msg_data.msg_id;
	sensor_d2.temperature = (signed short)msg_data.temperature;
	sensor_d2.altitude = (signed short)msg_data.altitude;
	sensor_d2.velocity = (signed short)msg_data.velocity;
	sensor_d2.power = (signed short)msg_data.power;
	
	sta2_data.log_ptr=(char*)malloc(sizeof(char)*DATA_LINK_BUFFER_SIZE);
		fputs(asctime(localtime(&now)), sta2_data.file_des);
		sprintf(sta2_data.log_ptr, "Data downlink:%d %hi %hi %d %hi %hi\n", sensor_d2.type, sensor_d2.id,sensor_d2.temperature,sensor_d2.altitude,
		sensor_d2.velocity, sensor_d2.power);

		fputs(sta2_data.log_ptr,sta2_data.file_des);
		free(sta2_data.log_ptr);

	}

  /* DOWNLINK implementation DATA_LINK-BASE_STATION */
  
  /* Create socket for sending/receiving datagrams */
  if ((sta3_data.socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
	err_sys("Error socket!");
  }
  /* Assign a port to socket */
  memset(&sta3_data.remote_addr, 0, sizeof(sta3_data.remote_addr)); //memory reset
  sta3_data.remote_addr.sin_family = AF_INET; //Internet/IP
  sta3_data.remote_addr.sin_addr.s_addr = inet_addr(argv[1]); //loopback 127.0.0.1
  sta3_data.remote_addr.sin_port = htons(BASE_STATION_UDP_RX_PORT); //server port

  /* Bind client_socket (need it to receiving datagram) */
  sta3_data.local_addr.sin_family = AF_INET; //Internet/IP
  sta3_data.local_addr.sin_addr.s_addr = htonl(INADDR_ANY); //any interface ip
  sta3_data.local_addr.sin_port = htons(DATA_LINK_UDP_RX_PORT_BASE_STATION); //client port

  if (bind(sta3_data.socket_fd, (struct sockaddr *) &sta3_data.local_addr, sizeof(sta3_data.local_addr))<0) {
				err_sys("Error socket!");
  }

  /* Send line_structure to the server */
  if (sendto(sta3_data.socket_fd, &msg_data, sizeof(msg_data), 0, (struct sockaddr *) &sta3_data.remote_addr, sizeof(sta3_data.remote_addr)) == -1) {
					fprintf(stderr, "sendto error\n");
					exit(1);
  }


  /* UPLINK implementation BASE_STATION-DATA_LINK */
  
  /* Create socket for sending/receiving datagrams */
  sta4_data.socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP); 
  if ((sta4_data.socket_fd) < 0) {
	err_sys("Error socket");
	
  }

  /* Assign a port to socket */
  memset(&sta4_data.remote_addr, 0, sizeof(sta4_data.remote_addr)); //memory reset
  sta4_data.remote_addr.sin_family = AF_INET; //Internet/IP
  sta4_data.remote_addr.sin_addr.s_addr = ntohl(INADDR_ANY); //any incomming interface
  sta4_data.remote_addr.sin_port = ntohs(DATA_LINK_UDP_RX_PORT_BASE_STATION); //server port


  result3 = bind(sta4_data.socket_fd, (struct sockaddr *) &sta4_data.remote_addr, sizeof(sta4_data.remote_addr));
  if (result3 < 0) {
	
	err_sys("Error bind");
	
  }
  /* open file to write */
        //sta4_data.file_des=fopen(FILENAME3,"w");
	//if (sta4_data.file_des == NULL) {
		//fprintf(stderr, "\nError opend file\n");
	//}

  /* Infinite loop */
  for ( ; ; ) {
	

	/* received information from client */
	
	clientlen3 = sizeof(sta4_data.local_addr);
	received3 = recvfrom(sta4_data.socket_fd, &msg_data, sizeof(msg_data), 0, (struct sockaddr *) &sta2_data.local_addr, &clientlen3);
	
	if (received3 < 0) {

		   err_sys("Error receiveing datagram from client");
		  
		}

	printf("Datagram received: ok!\n");
	printf("Type: %d\n", (int)msg_data.msg_type);
	printf("Id: %d\n", (int)msg_data.msg_id);
	printf("Temperature: %hi\n", (signed short)msg_data.temperature);
	printf("Altitude: %hi\n", (signed short)msg_data.altitude);
	printf("Velocity: %hi\n", (signed short)msg_data.velocity);
	printf("Power: %hi\n", (signed short)msg_data.power);


	}

  /* UPLINK implementation DATA_LINK-SATELLITE */

  /* Create socket for sending/receiving datagrams */
  if ((sta5_data.socket_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
	err_sys("Error socket!");
  }
  /* Assign a port to socket */
  memset(&sta5_data.remote_addr, 0, sizeof(sta5_data.remote_addr)); //memory reset
  sta5_data.remote_addr.sin_family = AF_INET; //Internet/IP
  sta5_data.remote_addr.sin_addr.s_addr = inet_addr(argv[1]); //loopback 127.0.0.1
  sta5_data.remote_addr.sin_port = htons(SATELLITE_UDP_RX_PORT); //server port

  /* Bind client_socket (need it to receiving datagram) */
  sta5_data.local_addr.sin_family = AF_INET; //Internet/IP
  sta5_data.local_addr.sin_addr.s_addr = htonl(INADDR_ANY); //any interface ip
  sta5_data.local_addr.sin_port = htons(DATA_LINK_UDP_RX_PORT_SATELLITE); //client port

  if (bind(sta5_data.socket_fd, (struct sockaddr *) &sta5_data.local_addr, sizeof(sta5_data.local_addr))<0) {
				err_sys("Error socket!");
  }

  /* Send line_structure to the server */
  if (sendto(sta5_data.socket_fd, &msg_data, sizeof(msg_data), 0, (struct sockaddr *) &sta5_data.remote_addr, sizeof(sta5_data.remote_addr)) == -1) {
					fprintf(stderr, "sendto error\n");
					exit(1);
  }


  /* close files and sockets */
 
  fclose(sta2_data.file_des);
  fclose(sta4_data.file_des);
  close(sta3_data.socket_fd);
  close(sta5_data.socket_fd);


  /* We register the CTRL+C interruption signal */
  
  if (signal(SIGINT, signal_handler) == SIG_ERR)
	printf("\ncan't catch SIGINT\n");

  while(!is_finished)
  {
    sleep(1);


  }


return 0;
}


/******************************************************************************/

static void signal_handler(int dummy)
{
  if (dummy== SIGINT) printf("Received SIGINT\n");
  is_finished = true;
  
}


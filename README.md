# Satellite_Base_Station_C

## Final project of the Operating System subject of the Bachelor's Degree in Telecommunications Technology and Services Engineering (UOC).

In this practice we will assume that we have a base station and a satellite running the GNU / Linux operating system and that want to exchange telemetry data over a radio link using the TCP / IP protocol stack.

Therefore, a program that simulates the satellite (satellite.c), a program that simulates the base station base_station.c and another program that simulates the communication channel (data_link.c) will be created.

The **satellite.c** program will read the telemetry data generated by the satellite's on-board sensors through a file in CSV (Comma Separated Value) format, process them and send them to the base station using a UDP socket. , User Datagram Protocol).

For its part, the **base_station.c** program will receive the data, store it in a file and send a receipt confirmation.

Finally, the **data_link.c** program will interconnect both
programs and will allow to simulate the effects of the communication channel (losses, delay, etc.). 

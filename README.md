# Satellite_Base_Station_C

## Final Project -Operative Systems: Bachelor's Degree in Telecommunications Technology and Services Engineering (UOC). 

In this practice we will assume that we have a base station and a satellite running the GNU/Linux operating system and that want to exchange telemetry data over a radio link using the TCP/IP protocol stack.

Therefore, a program that simulates the satellite (satellite.c), a program that simulates the base station base_station.c and another program that simulates the communication channel (data_link.c) will be created.

The **satellite.c** program will read the telemetry data generated by the satellite's on-board sensors through a file in CSV (Comma Separated Value) format, process them and send them to the base station using a UDP (User Datagram Protocol) socket.

For its part, the **base_station.c** program will receive the data, store it in a file and send a receipt confirmation.

Finally, the **data_link.c** program will interconnect both
programs and will allow to simulate the effects of the communication channel (losses, delay, etc.). 

<p align="center">
    <img width="600" src="https://user-images.githubusercontent.com/34940932/136261119-2c2ceb5f-1495-495d-891f-3c8ac08b3f97.png" alt="Material Bread logo">
</p>

<p align="center">
Figure 1. Communication system between a satellite and the base station. 
</p>




### Satellite, base station and coummunication channel implementation




![Captura de pantalla de 2021-10-06 20-48-43](https://user-images.githubusercontent.com/34940932/136264716-0a40547a-28dc-4d90-9d2c-647e1a19fe3b.png)


![Captura de pantalla de 2021-10-06 20-52-43](https://user-images.githubusercontent.com/34940932/136265247-3d137969-1385-4699-80a8-7968f6f8ffe9.png)





SATELLITE_NAME = satellite
SATELLITE_OBJS = satellite.o satellite_common.o

SATELLITE1_NAME = satellite1
SATELLITE1_OBJS = satellite1.o satellite_common.o

BASE_STATION_NAME = base_station
BASE_STATION_OBJS = base_station.o base_station_common.o

BASE_STATION2_NAME = base_station2
BASE_STATION2_OBJS = base_station2.o base_station_common.o

DATA_LINK_NAME = data_link
DATA_LINK_OBJS = data_link.o

DATA_LINK3_NAME = data_link3
DATA_LINK3_OBJS = data_link3.o


PROGRAM_NAMES  = $(SATELLITE_NAME) $(BASE_STATION_NAME) $(DATA_LINK_NAME) $(SATELLITE1_NAME) $(BASE_STATION2_NAME) $(DATA_LINK3_NAME)
PROGRAM_OBJS   = $(SATELLITE_OBJS) $(BASE_STATION_OBJS) $(DATA_LINK_OBJS) $(SATELLITE1_OBJS) $(BASE_STATION2_OBJS) $(DATA_LINK3_OBJS)

REBUIDABLES = $(PROGRAM_OBJS) $(PROGRAM_NAMES)

all: $(PROGRAM_NAMES)
	@echo "Finished!"

$(SATELLITE_NAME): $(SATELLITE_OBJS)
	gcc -Wall -Werror -o $@ $^ -I ./

$(SATELLITE1_NAME): $(SATELLITE1_OBJS)
	gcc -Wall -Werror -o $@ $^ -I ./

$(BASE_STATION_NAME): $(BASE_STATION_OBJS)
	gcc -Wall -Werror -o $@ $^ -I ./

$(BASE_STATION2_NAME): $(BASE_STATION2_OBJS)
	gcc -Wall -Werror -o $@ $^ -I ./

$(DATA_LINK_NAME): $(DATA_LINK_OBJS)
	gcc -Wall -Werror -o $@ $^ -I ./

$(DATA_LINK3_NAME): $(DATA_LINK3_OBJS)
	gcc -Wall -Werror -o $@ $^ -I ./

$(PROGRAM_NAME): $(PROGRAM_OBJS)
	gcc -Wall -Werror -o $@ $^ -I ./

%.o: %.c
	gcc -Wall -c $< -I ./

clean:
	rm -f $(REBUIDABLES)
	@echo "Clean done"
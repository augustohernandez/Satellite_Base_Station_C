/**
 * file: base_station_common.h
 */

#ifndef _BASE_STATION_COMMON_H_
#define _BASE_STATION_COMMON_H_

/*****************************************************************************/

void parse_sensor_data(uint8_t* buffer, uint8_t* msg_type, uint32_t* msg_id, sensor_data_t* sensor_data);

/*****************************************************************************/

#endif /* _BASE_STATION_COMMON_H_ */

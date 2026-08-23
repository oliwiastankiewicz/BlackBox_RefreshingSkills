#ifndef ENTITY_H
#define ENTITY_H

#include "entity_base.h"
#include "event.h"

TemperatureSensor *temperature_sensor_create();

Event generate_temperature_reading(Timestamp *timestamp, TemperatureSensor *sensor);

void temperature_update(TemperatureSensor *temperature_sensor, float temperature);
void temperature_update_all(float temperature);

Door *door_create();

Event door_open(Door *door, Timestamp *timestamp);

Event door_close(Door *door, Timestamp *timestamp);

Alarm *alarm_create(int door_id);

Event alarm_trigger(Alarm *alarm, Timestamp *timestamp);

Event alarm_clear(Alarm *alarm, Timestamp *timestamp);

#endif

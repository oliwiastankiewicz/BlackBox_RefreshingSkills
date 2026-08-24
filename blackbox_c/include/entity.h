#ifndef ENTITY_H
#define ENTITY_H

#include "entity_base.h"
#include "event.h"

TemperatureSensor *temperature_sensor_create(EntityRegistry *entity_registry);

Event generate_temperature_reading(Timestamp *timestamp, TemperatureSensor *sensor);

void temperature_update(TemperatureSensor *temperature_sensor, float temperature);
void temperature_update_all(EntityRegistry *entity_registry, float temperature);

Door *door_create(EntityRegistry *entity_registry);

Event door_open(Door *door, Timestamp *timestamp);

Event door_close(Door *door, Timestamp *timestamp);

Alarm *alarm_create(EntityRegistry *entity_registry, int door_id);

Event alarm_trigger(Alarm *alarm, Timestamp *timestamp);

Event alarm_clear(Alarm *alarm, Timestamp *timestamp);

EntityRegistry entity_registry_initialize();

void entity_registry_reset(EntityRegistry *entity_registry);

bool entity_registry_check_door_exists(EntityRegistry *entity_registry, int door_id);

#endif

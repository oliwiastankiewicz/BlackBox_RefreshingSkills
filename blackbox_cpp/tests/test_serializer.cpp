#include <cassert>
#include <iostream>

#include "../include/serializer.h"

void test_temperature_event(){
    Event event;
    Timestamp timestamp;
    Entity entity;

    entity.id = 0;
    entity.id_in_entity_registry_type = 0;
    entity.type = TEMPERATURE_SENSOR;

    timestamp = timestamp_create(0,2,3);

    event = event_create(timestamp, TEMPERATURE_READING, entity);

    event.temperature = 23;

    SerializedEvent result = serialize_event(event);

    assert(result.event_type == "TEMPERATURE_READING");
    assert(result.timestamp == "00:02:03");
    assert(result.entity_id == 0);
    assert(result.entity_type == "TEMPERATURE_SENSOR");
    assert(result.temperature == 23);
}
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include "../../blackbox_c/include/event.h"

struct SerializedEvent{
    std::string timestamp;
    std::string event_type;
    std::string entity_type;
    int entity_id;
    float temperature;
};

SerializedEvent serialize_event(const Event& event);

#endif
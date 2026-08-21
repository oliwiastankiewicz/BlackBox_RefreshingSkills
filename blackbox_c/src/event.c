#include <stdio.h>
#include "..\include\event.h"

Event event_create(char timestamp[7], EventType type, int entity_id){
    Event event;

    for (int i=0 ; i<7 ; i++){
        event.timestamp[i] = timestamp[i];
    }
    
    event.type = type;
    event.entity_id = entity_id;

    return event;
}

const char * event_type_to_string(EventType type){

    switch (type) {
        case TEMPERATURE_READING:
            return "TEMPERATURE_READING";
        case DOOR_OPENED:
            return "DOOR_OPENED";
        case DOOR_CLOSED:
            return "DOOR_CLOSED";
        case ALARM_TRIGGERED:
            return "ALARM_TRIGGERED";
        case ALARM_CLEARED:
            return "ALARM_CLEARED";
        default:
            return "UNKNOWN";
    }
}

void event_print(const Event *event){
    
    const char * eventType = event_type_to_string(event -> type);
    const char * time = event -> timestamp;

    printf("%c%c:%c%c:%c%c %s entity = %d\n", time[0], time[1], time[2], time[3], time[4], time[5], eventType, event -> entity_id);
}
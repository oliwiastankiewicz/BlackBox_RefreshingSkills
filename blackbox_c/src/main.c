#include <stdio.h>
#include "..\include\event.h"

int main(){
    printf("BlackBox - Version 1\n");

    Event event_test = event_create("000001", TEMPERATURE_READING, 0);
    
    event_print(&event_test);

    return 0;
}
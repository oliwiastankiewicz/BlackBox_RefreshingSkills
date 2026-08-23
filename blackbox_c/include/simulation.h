#ifndef SIMULATION_H
#define SIMULATION_H

#include "scenario.h"
#include "queue.h"
#include "timestamp.h"

void simulation_run(int temperature_sensors, int doors_with_alarms, int doors_without_alarms, Timestamp simulation_duration);

#endif
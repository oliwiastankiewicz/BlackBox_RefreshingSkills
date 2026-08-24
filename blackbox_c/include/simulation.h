#ifndef SIMULATION_H
#define SIMULATION_H

#include "scenario.h"
#include "queue.h"
#include "timestamp.h"

void simulation_run(const char *filename, int nr_temperature_sensors, int nr_doors_with_alarms, int nr_doors_without_alarms, Timestamp simulation_duration);

#endif
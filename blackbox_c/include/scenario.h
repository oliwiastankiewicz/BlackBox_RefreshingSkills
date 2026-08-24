#ifndef SCENARIO_H
#define SCENARIO_H

#include "entity.h"
#include "queue.h"
#include "timestamp.h"

void scenario_generate(EntityRegistry *entity_registry, Queue *queue, Timestamp *timestamp);

#endif
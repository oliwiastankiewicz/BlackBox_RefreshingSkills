#include <stdio.h>
#include <stdlib.h>

#include "simulation.h"
#include "timestamp.h"

int main(void)
{
    srand(23);

    printf("========================================\n");
    printf("        BlackBox - C Engine\n");
    printf("========================================\n\n");

    Timestamp duration = timestamp_create(0, 0, 5);

    printf("Running simulation...\n\n");

    simulation_run("blackbox_c/recordings/SIMULATION_1.txt", 2, 1, 1, duration);

    printf("\nSimulation finished.\n");

    return 0;
}
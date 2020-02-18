#include "elevatorCue.h"
#include "hardware.h"

typedef enum {
    DOOR_OPEN,
    DOOR_CLOSED
} DoorState;

typedef struct { 
    DoorState door; 
    int current_floor; 
} ElevatorState; 

//statene våre: doorState, hardwareMovement for motor
#include "elevatorCue.h"
#include "hardware.h"

typedef enum {
    DOOR_OPEN,
    DOOR_CLOSED
} DoorState;



//statene våre: doorState, hardwareMovement for motor
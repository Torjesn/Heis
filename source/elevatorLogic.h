#include "elevatorCue.h"
#include "hardware.h"

typedef enum {
    DOOR_OPEN,
    DOOR_CLOSED
} DoorState;

typedef struct { 
    DoorState door; 
    int current_floor; 
    HardwareMovement movement;
} ElevatorState; 

//statene våre: doorState, hardwareMovement for motor

//Må dokumentere funksjoner

/**
 * @brief Choses the next destination of the elevator. The inside queue is prioritized over the the outside ones
 * @param[in,out] queue_order_up The up order queue decremented by one if next destination
 * @param[in,out] queue_order_dowm The down order queue decremented bye one if next destination
 * @param[in,out] queue_order_inside The inside order queue decremented by one if next destination
 * @return next_floor Return the next floor, -1 if there are none requests pending
*/
#include "elevatorCue.h"
#include "hardware.h"

typedef enum {
    DOOR_OPEN,
    DOOR_CLOSED
} DoorState;

/**
 * @brief Sets the motor state based on destination and current floor
 * @param[out] p_motor_state Reference to motor state, changed based on the other parameters
 * @param[in] destination The next place the elevator is heading
 * @param[in] current_floor The current floor measured by hardware
 */
void set_motor_state(queueState * queue);

//statene våre: doorState, hardwareMovement for motor
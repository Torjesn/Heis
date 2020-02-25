/** @file 
 * @brief A file with all the hardware-functions we use
 * */
#ifndef ELEVATOR_FSM
#define ELEVATOR_FSM
#include "queueV2.h"
#include "hardware.h"
#include <time.h>

#define SECONDS_WAIT_DOOR 3

typedef enum {
    DOOR_OPEN,
    DOOR_CLOSED
} DoorState;


/**
 * @brief The elevator will go down until it sees a valid floor
*/

void sethw_start_procedure_elevator();

/**
 * @brief Sets the lights of the elevator 
 * based on the states of the queue and elevator
 * @param[in] queue Order lights and floor lighs are set based on states in the queue
 * @param [in] elev_state Door lights are set based on the door state
 */

void  sethw_lights(DoorState * door, QueueState2 * p_queue);

/**
 * @brief Tries to close the door, will be stopped if the door_open_timer is bigger than real_time.
 * The function checks if obstruction or stop is pressed, and sets the 3 second delay if true
 * @param[in] real_time Is compared to timer to check if door can be closed
 * @param[in, out] door_open_time Is set to 3 seconds ahead of real_time if stop or obstruction is pressed, then compared to real_time
 * @param[out] elev_state.door Closed if conditions are met
 */

void sethw_try_close_door(DoorState* p_door, clock_t* door_open_timer);


/**
 * @brief Writes to the motor based on the state of the door and the wishes of the queue
 * @param[in] elev_state.door The motor will halt if door is open
 * @param[in] queue.prefered_motor_state If door is closed, the elevator will follow the queue
 */
void sethw_motor(DoorState* p_door, QueueState2* queue);

/**
 * @brief Stops motor, opens doors and executes orders at the floor.
 * @param[out] elev_state.movement Stops the motor
 * @param[out] door_open_timer Sets the timer 3 seconds ahead of real time
 * @param[out] queue Deletes all orders on the current floor, 
 * and decrement all orders higher than the current floor
 */
void sethw_stop_on_floor(DoorState* p_door, clock_t* door_open_timer);

/**
 * @brief Procedure for stop button, motor stops, 
 * queue is emptied and door is opened if elevator is on a floor
 * @param[out] elev_state.movement Stops the motor
 * @param[out] queue Deletes the queue
 * @param[out] elev_state.door Opens door if on floor
 */
void sethw_stop_button_procedure(DoorState* p_door, QueueState2* queue);

#endif
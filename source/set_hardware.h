/** @file 
 * @brief Functions with logic that mainly writes to hardware
 * */
#ifndef SET_HARDWARE
#define SET_HARDWARE
#include "queue.h"
#include "hardware.h"
#include <time.h>

#define SECONDS_WAIT_DOOR 3

typedef enum {
    DOOR_OPEN,
    DOOR_CLOSED
} DoorState;


/**
 * @brief  The start procedure, the elevator goes down until it sees a valid floor
*/

void sethw_start_procedure_elevator();

/**
 * @brief Sets the lights of the elevator 
 * based on the states of the queue, door and stopsign
 * @param[in] p_queue Order lights and floor lighs are set based on states in the queue
 * @param [in] p_door Door light is set based on the door state
 */

void  sethw_lights(DoorState * p_door, QueueState * p_queue);

/**
 * @brief Tries to close the door, will be stopped if the timer is bigger than the real clock time.
 * The function checks if obstruction or stop, and sets timer to clock time + 3 seconds if pressed
 * @param[in, out] p_door_open_timer Is set to 3 seconds ahead of clock time if stop or obstruction is pressed, 
 * then compared to real clock time
 * @param[out] p_door Sets the door to open if real time is bigger than timer
 */

void sethw_try_close_door(DoorState* p_door, clock_t* p_door_open_timer);


/**
 * @brief Writes to the motor based on the state of the door and the wishes of the queue
 * @param[in] p_door The motor will halt if door is open
 * @param[in] p_queue If door is closed, the motor will follow wishes of the queue
 */
void sethw_motor(DoorState* p_door, QueueState* p_queue);

/**
 * @brief Sets the door timer 3 seconds ahead of real clock time, opens the door
 * @param[out] p_door_open_timer Sets the door timer 3 seconds ahead of real time
 * @param[out] p_door Opens the door
 */
void sethw_open_door(DoorState* p_door, clock_t* p_door_open_timer);

/**
 * @brief Procedure for stop button, stops motor, 
 * queue is emptied and door is opened if elevator is on a floor
 * @param[out] p_queue Deletes the queue
 * @param[out] p_door Opens door if on floor
 */
void sethw_stop_button_procedure(DoorState* p_door, QueueState* queue);

#endif
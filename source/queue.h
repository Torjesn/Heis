/**
* @file
* @brief The queue system of the elevator

*/
#ifndef QUEUE_SYSTEM
#define QUEUE_SYSTEM

#include "hardware.h"
#define DOUBLE_FLOOR_STATES 2
#define DEFAULT_FLOOR -1



/**
 * @brief  A struct to keep the different parts of the queue in order 
 * @warning order_up and order_down are arrays of four elements, 
 * even though order_up[3] and and order_down[0] are not valid. 
 * */ 
typedef struct {
    int order_up[HARDWARE_NUMBER_OF_FLOORS]; /** < Array for the orders going up from the outside */
    int order_down[HARDWARE_NUMBER_OF_FLOORS]; /** < Array for the orders going down from the outside */
    int order_inside[HARDWARE_NUMBER_OF_FLOORS]; /** < Orders from the inside */
    int saved_floor; /** < Holds the current floor, with the states between aswell*/
    int current_floor; /** < Holds the current floor, is DEFAULT_FLOOR between floors*/
    int destination; /** < The floor furthest away in the right direction*/
    HardwareMovement preferred_motor_state; /** < What the queue wants the direction of the motor to be */
} QueueState; 

/** 
 * @brief Sets the current_floor state of the queue. 
 * Does not change between floors.
 * @param[out] p_queue, Current floor and saved floor of the queue is set based on hardware and s
*/

void queue_get_current_floor(QueueState * p_queue);

/**
 * @brief Initilizes the queue
 * @param[out] p_queue The values of the queue arrays are set to zero, 
 * the destination set to default
 *
*/
void queue_default_init(QueueState * p_queue);

/**
 * @brief Removes the orders of the current floor in the queue
 * 
 * @param[out] p_queue The queue arrays are set to zero at the current floor
 */
void queue_remove_orders_current_floor(QueueState * p_queue);

/**
 * @brief Checks if the elevator should stop on the floor or not
 * @param[in] p_queue Uses the states of the queue to determine if it should stop or not, 
 * @return Stop Returns 1 if the elevator should stop at a floor, 0 otherwise
 
*/
int queue_check_if_stop_floor(QueueState * p_queue);

/**
 * @brief Sets the elevator input on one of the queue arrays.
 * @param[out] p_queue Sets the queue arrays based on harware input, current_floor state and prefered_motor_state state of the queue.
 * @warning Orders at the same floor we are in will not be added to the queue, because they will be taken automatically 
*/

void queue_get_user_input(QueueState * p_queue);

/**
 * @brief Checks if the queue has any orders, and sets the motor state accordingly to reach them.
 * @param[in, out] p_queue Compares the floor_arrays and saved_floor state to find prefered motorstate, and sets that motor state
 */
void queue_set_preferred_motor_state(QueueState * p_queue);


/**
 * @brief Sets the destionation of the elevator to the floor the furthest away
 * based on the prefered motor state
 * @param[in, out] p_queue Compares the floor_arrays and the prefered_motor_state state to find destination, and sets the destination
 */
void queue_set_destination(QueueState *p_queue);


/**
 * @brief Sets the destionation to DEFAULT_FLOOR if reached, else current floor
 * @param[in,out] p_queue  Sets destination based on current floor
 */
void queue_if_destination_reached_set_deafult(QueueState *p_queue);

#endif
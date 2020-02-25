/**
* @file
* @brief The queue system of the elevator

*/
#ifndef QUEUE_SYSTEM
#define QUEUE_SYSTEM

#include "hardware.h"
#define NUMBER_OF_FLOORS 4



/**
 * @brief  A struct to keep the different parts of the queue in order 
 * @warning order_up and order_down are arrays of four elements, 
 * even though order_up[3] and and order_down[0] are not valid. 
 * */ 
typedef struct {
    int order_up[NUMBER_OF_FLOORS]; /** < Array for the orders going up from the outside */
    int order_down[NUMBER_OF_FLOORS]; /** < Array for the orders going down from the outside */
    int order_inside[NUMBER_OF_FLOORS]; /** < Orders from the inside */
    int saved_floor; /** < The current floor, only changes on floors, not between */
    int current_floor;
    int further_floor;
    HardwareMovement preferred_motor_state; /** < What the queue wants the direction of the motor to be */
} QueueState2; 

/** 
 * @brief Sets the current_floor state of the queue. 
 * Does not change between floors.
 * @param[out] current_floor The current floor of the queue is set based on hardware.
*/

void queue_get_current_floor(QueueState2 * p_queue);

/**
 * @brief Initilizes the queue
 * @param[out] queue Queue-arrays set to zero, counters set to zero, 
 * destination and prefered_motor_state set to deafult
*/
void queue_default_init(QueueState2 * p_queue);


//Alternativt, @param [in, out] queue, vi bruker (meldemsvariabler)
/**
 * @brief Removes the orders of the current floor, and decrement the queues
 * @param[in,out] queue_order_up The up order queue with the current floor removed
 * @param[in,out] queue_order_down The down order queue with the current floor removed
 * @param[in,out] queue_order_inside The inside order queue with the current floor removed

*/
void queue_remove_orders_current_floor(QueueState2 * p_queue);


int queue_check_if_drive_further(QueueState2 * p_queue);

/**
 * @brief Checks if the elevator should stop on the floor or not
 * @param[in] queue_order_up The up order queue 
 * @param[in] queue_order_dowm The down order queue
 * @param[in] queue_order_inside The inside order queue 
 * @param[in] floor The current floor
 * @param[in] preferred_motor_state The current direction of the motor
 * @return Stop Returns 1 if the elevator should stop at a floor, 0 otherwise
 
*/
int queue_check_if_stop_floor(QueueState2 * p_queue);

/**
 * @brief Sets the elevator input on one of the queue arrays. Increments the following queue count.
 * @param[out] queue_order_up If up_button is pressed on a floor, it is set with the 
 * incremented count number in the cue if it is not in the queue
 * @param[out] queue_order_down If down_button is pressed on a floor, it is set with the 
 * incremented count number in the cue if it is not in the queue
 * @param[out] queue_order_inside If down_button is pressed on a floor, it is set with the 
 * incremented count number in the cue if it is not in the queue
 * @param[in, out] queue_count_outside Incremented by one if a request is put in the outside queue
 * @param[in, out] queue_count_inside Incremented by one if a request is put in the inside queue
*/

void queue_get_user_input(QueueState2 * p_queue);

/**
 * @brief Sets the motor state based on destination and current floor
 * @param[out] p_preferred_motor_state Reference to motor state, changed based on the other parameters
 * @param[in] destination The next place the elevator is heading
 * @param[in] current_floor The current floor measured by hardware
 */
void queue_set_preferred_motor_state(QueueState2 * p_queue);

#endif
/**
* @file
* @brief The queue system of the elevator

*/

//DISCLAIMER
//Some of the smaller functions should possibly not be here, we should ask on tuesday
//They should probably be set as static, and not included in headerfile

#include "hardware.h"

const int g_number_of_floors = 4; 

/**
 * @brief  A struct to keep the different parts of the queue in order 
 * */ 
typedef struct {
    int order_up[4]; /** < Array for the orders going up from the outside */
    int order_down[4]; /** < Array for the orders going down from the outside */
    int order_inside[4]; /** < Orders from the inside */
    int count_outside; /** < Total number of orders outside */
    int count_inside; /** < Total numbers of orders inside */
    int destination; /** < The next destination */
    int current_floor; /** < The current floor, only changes on floors, not between */
    HardwareMovement preferred_motor_state; /** < The state of the motor according to queue */
} queueState; 

/**
 * @brief Initilizes the queue
 * @param[out] queue Queue-arrays set to zero, counters set to zero, 
 * destination, current_floor and prefered_motor_state set to deafult
*/
void queue_default_init(queueState * queue);


//Alternativt, @param [in, out] queue, vi bruker (meldemsvariabler)
/**
 * @brief Removes the orders of the current floor, and decrement the queues
 * @param[in,out] queue_order_up The up order queue with the current floor removed
 * @param[in,out] queue_order_down The down order queue with the current floor removed
 * @param[in,out] queue_order_inside The inside order queue with the current floor removed

*/
void queue_remove_orders_current_floor(queueState * queue);


/**
 * @brief Choses the next destination of the elevator. The inside queue is prioritized over the the outside ones
 * @param[in,out] queue_order_up The up order queue decremented by one if next destination
 * @param[in,out] queue_order_dowm The down order queue decremented by one if next destination
 * @param[in,out] queue_order_inside The inside order queue decremented by one if next destination
 * @return next_floor Return the next floor, -1 if there are none requests pending
*/

void queue_get_next_destination(queueState * queue);

/**
 * @brief Checks if the elevator should stop on the floor or not
 * @param[in] queue_order_up The up order queue 
 * @param[in] queue_order_dowm The down order queue
 * @param[in] queue_order_inside The inside order queue 
 * @param[in] floor The current floor
 * @param[in] preferred_motor_state The current direction of the motor
 * @return Stop Returns 1 if the elevator should stop at a floor, 0 otherwise
 
*/
int queue_check_if_stop_floor(queueState * queue);

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

void queue_get_user_input(queueState * queue);

/**
 * @brief Sets the motor state based on destination and current floor
 * @param[out] p_preferred_motor_state Reference to motor state, changed based on the other parameters
 * @param[in] destination The next place the elevator is heading
 * @param[in] current_floor The current floor measured by hardware
 */
void queue_set_preferred_motor_state(queueState * queue);

/**
 * @brief Deletes the queue if stop button is pressed
 * @param[out] p__preferred_motor_state If pressed, preferred_motor_state becomes idle
 * @param[out] queue_order_up If pressed, the up_queue is set to zero
 * @param[out] queue_order_down If pressed, the down_queue is set to zero
 * @param[out] queue_order_inside If pressed, the inside_queue is set to zero
 */
void queue_delete_button(queueState *queue);

/** 
 * @brief Sets the current_floor state of the queue. 
 * Does not change between floors.
 * @param[out] current_floor The current floor of the queue is set based on hardware.
*/
void queue_get_current_floor(ElevatorState elev_state, queueState *queue);






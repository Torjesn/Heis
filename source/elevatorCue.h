/**
* @file
* @brief The queue system of the elevator

*/

//DISCLAIMER
//Some of the smaller functions should possibly not be here, we should ask on tuesday

#include "hardware.h"

typedef struct {
    int count_outside = 0;
    int count_inside = 0;
    int order_up[] = {0,0,0,0}; //kan eventuelt ha tre her, gjør aksessering enklere med fire
    int order_down[] = {0,0,0,0}; // -||-
    int order_inside[] = {0,0,0,0};
    int destination = -1;
    int current_floor = -1; //antar floor er 1-indeksert
    HardwareMovement motor_state = HARDWARE_MOVEMENT_STOP;
} queueState;
 //set dox på denne

/**
 * @brief Removes the orders of the current floor, and decrement the queues
 * @param[in,out] queue_order_up The up order queue with the current floor removed
 * @param[in,out] queue_order_down The down order queue with the current floor removed
 * @param[in,out] queue_order_inside The inside order queue with the current floor removed

*/
void remove_orders_current_floor(queueState * queue);



/**
 * @brief Decrements the values of an array over a chosen limit, used for manipulating queues
 * @param[in,out] array The array being decremented
 * @param[in] limit Numbers bigger than this limit is decremented by one in the array
 * @param[in] length The size of the array

*/
void decrement_array_over_limit(int array[], int limit, int length);

/**
 * @brief Choses the next destination of the elevator. The inside queue is prioritized over the the outside ones
 * @param[in,out] queue_order_up The up order queue decremented by one if next destination
 * @param[in,out] queue_order_dowm The down order queue decremented bye one if next destination
 * @param[in,out] queue_order_inside The inside order queue decremented by one if next destination
 * @return next_floor Return the next floor, -1 if there are none requests pending
*/

void get_next_destination(queueState * queue);


/**
 * @brief Choses the next destination of the elevator. The inside queue is prioritized over the the outside ones
 * @param[in] queue_order_up The up order queue 
 * @param[in] queue_order_dowm The down order queue
 * @param[in] queue_order_inside The inside order queue 
 * @param[in] floor The current floor
 * @param[in] motor_state The current direction of the motor
 * @return floor Returns the number of the floor, or -1 as default
 
*/
int check_if_stop_floor();

/**
 * @brief Polls input from harware and returns the current floor
 * @return Stop Returns 1 if the elevator should stop at a floor, 0 otherwise
*/
int read_floor();


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

void get_elevator_input( queueState * queue);


/**
 * @brief Sets the motor state based on destination and current floor
 * @param[out] p_motor_state Reference to motor state, changed based on the other parameters
 * @param[in] destination The next place the elevator is heading
 * @param[in] current_floor The current floor measured by hardware
 */
void set_state(queueState * queue)


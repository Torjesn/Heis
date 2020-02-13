/**
* @file
* @brief The queue system of the elevator

*/

#include "hardware.h"

/**
 * @brief Removes the orders of the current floor, and decrement the cues
 * @param[in,out] cue_order_up The up order cue with the current floor removed
 * @param[in,out] cue_order_down The down order cue with the current floor removed
 * @param[in,out] cue_order_inside The inside order cue with the current floor removed

*/

void remove_orders_current_floor(int floor, int cue_order_up[], int cue_order_down[], int cue_order_inside[]);



/**
 * @brief Decrements the values of an array over a chosen limit, used for manipulating cues
 * @param[in,out] array The array being decremented
 * @param[in] limit Numbers bigger than this limit is decremented by one in the array
 * @param[in] length The size of the array

*/
void decrement_array_over_limit(int array[], int limit, int length);

/**
 * @brief Choses the next destination of the elevator. The inside cue is prioritized over the the outside ones
 * @param[in,out] cue_order_up The up order cue decremented by one if next destination
 * @param[in,out] cue_order_dowm The down order cue decremented bye one if next destination
 * @param[in,out] cue_order_inside The inside order cue decremented by one if next destination
 * @return next_flor Return the next floor, 0 if there are none requests pending
 //Ikke skrevet ferdig
*/

void get_next_destination(int cue_order_up[],int cue_order_down[], int cue_order_inside[]);


/**
 * @brief Choses the next destination of the elevator. The inside cue is prioritized over the the outside ones
 * @param[in] cue_order_up The up order cue 
 * @param[in] cue_order_dowm The down order cue
 * @param[in] cue_order_inside The inside order cue 
 * @param[in] floor The current floor
 * @param[in] motor_state The current direction of the motor
 * @return stop Returns 1 if the motor should stop, 0 otherwise
*/
int check_if_stop_floor(int floor, int cue_order_up[], int cue_order_down[], int cue_order_inside[], HardwareMovement motor_state);

/**
 * @brief Polls input from harware and returns the current floor
 * @return floor Returns the number of the floor, or 0 as default
 //0 as default burde kanskje endres
*/
int read_floor();

void get_elevator_input( int *p_cue_count_outside, int *p_cue_count_inside, int cue_order_up[], int cue_order_down[], int cue_order_inside[]);


void get_elevator_up_input(int *p_cue_count_outside, int cue_order_up[]);


/**
 * @brief Sets the motor state based on destination and current floor
 * @param[out] p_motor_state Reference to motor state, changed based on the other parameters
 * @param[in] destination The next place the elevator is heading
 * @param[in] current_floor The current floor measured by hardware
 
*/
void set_state(HardwareMovement * p_motor_state, int destination, int current_floor)
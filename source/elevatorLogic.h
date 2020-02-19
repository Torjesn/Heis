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

//Burde vi ha alle de funksjoene vi kaller knyttet til hardware som output-parametere?


/**
 * @brief The elevator wil go down until it sees a valid floor
*/

void start_procedure_elevator();

/**
 * @brief Sets the lights of the elevator 
 * based on the states of the queue and elevator
 * @param[in] queue Order lights and floor lighs are set based on states in the queue
 * @param [in] elev_state Door lights are set based on the door state
 */

void set_lights(ElevatorState * elevator, queueState * queue);


/**
 * @brief Set the deafult elevator_states
 * @param[out] elev_state.current_floor Set to deafult state, -1
 * @param[out] elev_state.door Set to closed
 * @param[out] elev_state.movement Set to stop
 */
void init_elevator_states(ElevatorState* elev_state);


/**
 * @brief Tries to close the door, will be stopped if the door_open_timer is bigger than real_time.
 * The function checks if obstruction or stop is pressed, and sets the 3 second delay if true
 * @param[in] real_time Is compared to timer to checj if door can be closed
 * @param[in, out] door_open_time Is set to 3 seconds ahead of real_time if stop or obstruction is pressed, then compared to real_time
 * @param[out] elev_state.door Closed if conditions are met
 */

void try_close_door(ElevatorState* elev_state, clock_t* real_time, clock_t* door_open_timer);

/**
 * @brief Opens the door if the elevator is at rest at a floor.
 * @param[in] elev_state.movement Checks if the elevator is moving
 * @param[in] elev_state.current_floor Checks if the elevator is at a floor
 * @param[out] elev_state.door Opened if conditions are met
 */

void open_door(ElevatorState* elev_state)k


/**
 * @brief Writes to the motor based on the state of the door and the wishes of the queue
 * @param[in] elev_state.door The motor will halt if door is open
 * @param[in] queue.prefered_motor_state If door is closed, the elevator will follow the queue
 */
void write_to_motor(queueState* queue, ElevatorState* elev_state);

/**
 * @brief Stops motor, opens doors and executes orders at the floor.
 * @param[out] elev_state.movement Stops the motor
 * @param[out] door_open_timer Sets the timer 3 seconds ahead of real time
 * @param[out] queue Deletes all orders on the current floor, 
 * and decrement all orders higher than the current floor
 */
void stop_on_floor(ElevatorState* elev_state, clock_t* door_open_timer, queueState* queue)

/**
 * @brief Procedure for stop button, motor stops, 
 * queue is emptied and door is opened if elevator is on a floor
 * @param[out] elev_state.movement Stops the motor
 * @param[out] queue Deletes the queue
 * @param[out] elev_state.door Opens door if on floor
 */
void stop_button_procedure(ElevatorState* elev_state, queueState* queue);



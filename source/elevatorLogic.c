#include "elevatorCue.h"
#include "hardware.h"
#include "elevatorLogic.h"
#include <time.h>

int read_floor() {
    for (int i = 0; i < NUMBER_OF_FLOORS; ++i ) {
        if(hardware_read_floor_sensor(i)) return i;
    }
    return -1;
}

void start_procedure_elevator() {
    int floor_level =  read_floor();
    
    while (floor_level == -1) {
        floor_level =  read_floor();
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
}

void set_lights(ElevatorState* p_elev_state, QueueState * p_queue){ 
    hardware_command_stop_light(hardware_read_stop_signal()); 
    if (p_elev_state->door == DOOR_OPEN) hardware_command_door_open(1);
    else hardware_command_door_open(0);
    
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
        {
            hardware_command_order_light(i,HARDWARE_ORDER_INSIDE, p_queue->order_inside[i]);
            hardware_command_order_light(i,HARDWARE_ORDER_UP, p_queue->order_up[i]);
            hardware_command_order_light(i, HARDWARE_ORDER_DOWN, p_queue->order_down[i]);
        }
    hardware_command_floor_indicator_on(p_queue->current_floor);

}

void init_elevator_states(ElevatorState* p_elev_state) {
    p_elev_state->door = DOOR_CLOSED;
    p_elev_state->movement = HARDWARE_MOVEMENT_STOP;
}

void try_close_door(ElevatorState* p_elev_state, clock_t* p_door_open_timer) {
    if (hardware_read_obstruction_signal() || hardware_read_stop_signal()) {
        *p_door_open_timer = clock() + SECONDS_WAIT_DOOR * CLOCKS_PER_SEC;
    }
    
    if (clock()>= *p_door_open_timer ) {
        p_elev_state->door = DOOR_CLOSED;
        *p_door_open_timer = clock();
    }
}

void open_door(ElevatorState* p_elev_state) {
    if (
        p_elev_state->movement == HARDWARE_MOVEMENT_STOP
        && p_elev_state->current_floor >= 0
    ) p_elev_state->door = DOOR_OPEN;
}


void write_to_motor( ElevatorState* p_elev_state, QueueState* p_queue) {
    if (p_elev_state->door == DOOR_OPEN) p_elev_state->movement = HARDWARE_MOVEMENT_STOP;
    else p_elev_state->movement = p_queue->preferred_motor_state; 
    hardware_command_movement(p_elev_state->movement);
} 

void stop_on_floor(ElevatorState* p_elev_state, clock_t* p_door_open_timer) {
    *p_door_open_timer = clock() + SECONDS_WAIT_DOOR * CLOCKS_PER_SEC;
    p_elev_state->movement = HARDWARE_MOVEMENT_STOP;
}


void stop_button_procedure(ElevatorState* p_elev_state, QueueState* p_queue) {
    p_elev_state->movement = HARDWARE_MOVEMENT_STOP;
    hardware_command_movement(p_elev_state->movement); //kan nok løses på en penere måte
    queue_default_init(p_queue);
    if (p_elev_state->current_floor >= 0) {
        p_elev_state->door = DOOR_OPEN;
    }
}

void get_current_floor_state(ElevatorState * p_elev_state, QueueState * p_queue) {
    int floor = read_floor();
    p_elev_state->current_floor = floor;
    p_queue->current_floor_not_between = floor;
    if (floor >= 0) {
        p_queue->current_floor = floor;
    }
}


#include "elevatorCue.h"
#include "hardware.h"
#include "elevatorLogic.h"
#include <time.h>

static void start_procedure_elevator() {
    int floor_level =  read_floor();
    while (floor_level == -1) {
        int floor_level =  read_floor();
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN); //kan denne være før, så den slipper å settes hele tiden?
    }
}

void set_lights(ElevatorState elevator, queueState * queue){
    //setter stopp-lys: 
    hardware_command_stop_light(hardware_read_stop_signal()); //hardware_read_stop_signal returnerer 0 hvis den er av, 1, hvis på og command skriver med samme verdier
    //setter døra til åpen: 
    if (elevator.door = DOOR_OPEN) {
        hardware_command_door_open(1);
    } else if(elevator.door = DOOR_CLOSED) {
        hardware_command_door_open(0);
    }
    //setter order lights 
    int order_outside_length = 3;
    int order_inside_length = 4;
    //kølys inne
    for (int i = 0; i < order_inside_length; i++)
        {
            hardware_command_order_light(i+1,HARDWARE_ORDER_INSIDE,   queue->order_inside[i]);
        }
    //order opp
    for (int i = 0; i < order_outside_length; i++)
        {
            hardware_command_order_light(i+1,HARDWARE_ORDER_UP, queue->order_up[i]);
        }
    //order ned 
    for (int i = 0; i < order_outside_length; i++)
        {
            hardware_command_order_light(i+2, HARDWARE_ORDER_DOWN, queue->order_down[i+1]);
        }
    //etasjelys: //siden det er mutually exlusive så er det nok å skru på en av dem. 
    hardware_command_floor_indicator_on(queue->current_floor);

}

void init_elevator_states(ElevatorState* elev_state) {
    elev_state->current_floor = -1;
    elev_state->door = DOOR_CLOSED;
    elev_state->movement = HARDWARE_MOVEMENT_STOP;
}



void try_close_door(ElevatorState* elev_state, clock_t* real_time, clock_t* door_open_timer) {
    if (hardware_read_obstruction_signal() || hardware_read_stop_signal()) {
        *door_open_timer += 3 * CLOCKS_PER_SEC;
    }
    if (*real_time >= *door_open_timer ) {
        elev_state->door = DOOR_CLOSED;
        *door_open_timer = clock();
    }
    *real_time = clock();
}

void open_door(ElevatorState* elev_state) {
    if (
        elev_state->movement == HARDWARE_MOVEMENT_STOP
        && elev_state->current_floor > 0
    )
    elev_state->door = DOOR_OPEN;
}


void write_to_motor(queueState* queue, ElevatorState* elev_state) {
    if ( elev_state->door == DOOR_OPEN) elev_state->movement = HARDWARE_MOVEMENT_STOP;
    else elev_state->movement = elev_state->movement = queue->preferred_motor_state;
    //kunne lagt til en på stopbutton, men dette vil aldri skje
    hardware_command_movement(elev_state->movement);
}


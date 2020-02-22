#include "elevatorCue.h"
#include "hardware.h"
#include "elevatorLogic.h"
#include <time.h>

static int read_floor() {
    for (int i = 0; i < NUMBER_OF_FLOORS; ++i ) {
        if(hardware_read_floor_sensor(i)) return i;
    }
    return -1;
}

void start_procedure_elevator() {
    int floor_level =  read_floor();
    while (floor_level == -1) {
        floor_level =  read_floor();
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN); //kan denne være før, så den slipper å settes hele tiden?
    }
}

void set_lights(ElevatorState* elev_state, queueState * queue){ 
    //setter stopp-lys: 
    hardware_command_stop_light(hardware_read_stop_signal()); //hardware_read_stop_signal returnerer 0 hvis den er av, 1, hvis på og command skriver med samme verdier
    if (elev_state->door == DOOR_OPEN) hardware_command_door_open(1);
    else hardware_command_door_open(0);
    
    for (int i = 0; i < NUMBER_OF_FLOORS; i++)
        {
            hardware_command_order_light(i,HARDWARE_ORDER_INSIDE, queue->order_inside[i]);
            hardware_command_order_light(i,HARDWARE_ORDER_UP, queue->order_up[i]);
            hardware_command_order_light(i, HARDWARE_ORDER_DOWN, queue->order_down[i]);
        }
    hardware_command_floor_indicator_on(queue->current_floor);

}

void init_elevator_states(ElevatorState* elev_state) {
    elev_state->current_floor = -1;
    elev_state->door = DOOR_CLOSED;
    elev_state->movement = HARDWARE_MOVEMENT_STOP;
}

void try_close_door(ElevatorState* elev_state, clock_t* real_time, clock_t* door_open_timer) {
    if (hardware_read_obstruction_signal() || hardware_read_stop_signal()) {
        *door_open_timer = clock() + 3 * CLOCKS_PER_SEC;
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
        && elev_state->current_floor >= 0
    ) elev_state->door = DOOR_OPEN;
}


void write_to_motor( ElevatorState* elev_state, queueState* queue) {
    if (elev_state->door == DOOR_OPEN) elev_state->movement = HARDWARE_MOVEMENT_STOP;
    else elev_state->movement = queue->preferred_motor_state; 
    hardware_command_movement(elev_state->movement);
}

void stop_on_floor(ElevatorState* elev_state,  queueState* queue, clock_t* door_open_timer) {
    *door_open_timer = clock() + 3 * CLOCKS_PER_SEC;
    elev_state->movement = HARDWARE_MOVEMENT_STOP;
    open_door(elev_state);
    queue_remove_orders_current_floor(queue);
    queue_get_next_destination(queue); //gjøre dette bare i etasje?Ja, vi vil vel ikke risikere å skifte vei underveis?
    queue_set_preferred_motor_state(queue); //Kunne lagd om til en funksjon?, we did
}


void stop_button_procedure(ElevatorState* elev_state, queueState* queue) {
    elev_state->movement = HARDWARE_MOVEMENT_STOP;
    hardware_command_movement(elev_state->movement); //kan nok løses på en penere måte
    queue_default_init(queue);
    if (elev_state->current_floor >= 0) {
        elev_state->door = DOOR_OPEN;
    }
}

void get_current_floor_state(ElevatorState * elev_state, queueState * queue) {
    int floor = read_floor();
    elev_state->current_floor = floor;
    if (floor >= 0) {
        queue->current_floor = floor;
    }
}


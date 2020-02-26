#include "hardware.h"
#include <time.h>
#include "set_hardware.h"
#include "queueV2.h"

static int read_floor() { // må gjøre noe fornuftig med hvor denne er plassert
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i ) {
        if(hardware_read_floor_sensor(i)) return i;
    }
    return -1;
}


void sethw_start_procedure_elevator() {
    int floor_level =  read_floor();
    
    while (floor_level == -1) {
        floor_level =  read_floor();
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
}

void sethw_lights(DoorState * p_door, QueueState2 * p_queue){ 
    hardware_command_stop_light(hardware_read_stop_signal()); 
    if (*p_door == DOOR_OPEN) hardware_command_door_open(1);
    else hardware_command_door_open(0);
    
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; i++)
        {
            hardware_command_order_light(i,HARDWARE_ORDER_INSIDE, p_queue->order_inside[i]);
            hardware_command_order_light(i,HARDWARE_ORDER_UP, p_queue->order_up[i]);
            hardware_command_order_light(i, HARDWARE_ORDER_DOWN, p_queue->order_down[i]);
        }
    if (p_queue->current_floor > -1) hardware_command_floor_indicator_on(p_queue->current_floor);
}

void sethw_try_close_door(DoorState * p_door, clock_t* p_door_open_timer) {
    if (hardware_read_obstruction_signal() || hardware_read_stop_signal()) {
        *p_door_open_timer = clock() + SECONDS_WAIT_DOOR * CLOCKS_PER_SEC;
    }
    
    if (clock()>= *p_door_open_timer ) {
        *p_door = DOOR_CLOSED;
        *p_door_open_timer = clock();
    }
}

void sethw_motor(DoorState * p_door, QueueState2 * p_queue) {
    if (*p_door == DOOR_OPEN)  hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    else hardware_command_movement(p_queue->preferred_motor_state);
} 

void sethw_stop_on_floor(DoorState * p_door, clock_t* p_door_open_timer) {
    *p_door_open_timer = clock() + SECONDS_WAIT_DOOR * CLOCKS_PER_SEC;
    *p_door = DOOR_OPEN;
    //burde motor stoppes dirkte her? Ikke nødvendig, men kanskje bra for lesbarhet?
}

void sethw_stop_button_procedure(DoorState * p_door, QueueState2 * p_queue) {
    hardware_command_movement(HARDWARE_MOVEMENT_STOP);
    queue_default_init(p_queue);
    if (p_queue->current_floor >= 0) {
        *p_door = DOOR_OPEN;
    }
}


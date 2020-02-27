#include "hardware.h"
#include "queue.h"


void queue_get_current_floor(QueueState * p_queue) {
    int floor_read = hardware_read_floor();
    p_queue->current_floor = floor_read;
    
     if (floor_read != DEFAULT_FLOOR) {
        p_queue->saved_floor = DOUBLE_FLOOR_STATES * floor_read;
    } 
    
    else if (p_queue->saved_floor % DOUBLE_FLOOR_STATES == 0) {
        if (floor_read == DEFAULT_FLOOR && p_queue->preferred_motor_state == HARDWARE_MOVEMENT_UP ) p_queue->saved_floor++;
        if (floor_read == DEFAULT_FLOOR && p_queue->preferred_motor_state == HARDWARE_MOVEMENT_DOWN ) p_queue->saved_floor--;
    }
}

void queue_default_init(QueueState * p_queue) {
    for (int floor = 0; floor <  HARDWARE_NUMBER_OF_FLOORS; ++floor) {
        p_queue->order_up[floor] = 0;
        p_queue->order_down[floor] = 0; 
        p_queue->order_inside[floor] = 0;  
    }
    p_queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
    p_queue->destination = DEFAULT_FLOOR;
}

void queue_remove_orders_current_floor(QueueState * p_queue) {
    if (p_queue->current_floor != DEFAULT_FLOOR) {
        p_queue->order_up[p_queue->current_floor] = 0;
        p_queue->order_down[p_queue->current_floor] = 0; 
        p_queue->order_inside[p_queue->current_floor] = 0;
    }
}

int queue_check_if_stop_floor(QueueState* p_queue) {
    if(p_queue->current_floor != DEFAULT_FLOOR) {
        if(
            p_queue->destination == p_queue->current_floor 
            || p_queue->order_inside[p_queue->current_floor]
            || (p_queue->order_up[p_queue->current_floor] && (p_queue->preferred_motor_state ==  HARDWARE_MOVEMENT_UP || p_queue->preferred_motor_state == HARDWARE_MOVEMENT_STOP)) 
            || (p_queue->order_down[p_queue->current_floor] && (p_queue->preferred_motor_state ==  HARDWARE_MOVEMENT_DOWN || p_queue->preferred_motor_state == HARDWARE_MOVEMENT_STOP))
        ) 
        {       
            return 1;
        }
    }
    return 0;
}

void queue_get_user_input(QueueState *p_queue) {  
    for (int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; ++floor) {
        if (floor != p_queue->current_floor || p_queue->preferred_motor_state == HARDWARE_MOVEMENT_STOP) {
            if(hardware_read_order(floor, HARDWARE_ORDER_UP))  p_queue->order_up[floor] = 1; 
                    
            if(hardware_read_order(floor, HARDWARE_ORDER_DOWN)) p_queue->order_down[floor] = 1;
                
            if(hardware_read_order(floor, HARDWARE_ORDER_INSIDE)) p_queue->order_inside[floor] = 1;
        }
    }         
}

void queue_set_preferred_motor_state(QueueState *p_queue) { 
    for (int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if (p_queue->order_up[floor]
            || p_queue->order_down[floor]
            || p_queue->order_inside[floor]
        )
        {
            if (p_queue->saved_floor > (floor * DOUBLE_FLOOR_STATES)) {
                p_queue->preferred_motor_state = HARDWARE_MOVEMENT_DOWN;
                break;
            }
            else if (p_queue->saved_floor < (floor * DOUBLE_FLOOR_STATES)){
                p_queue->preferred_motor_state = HARDWARE_MOVEMENT_UP;
                break;
            } 
        }
    }
}

void queue_set_destination(QueueState *p_queue) {
    if (p_queue->preferred_motor_state == HARDWARE_MOVEMENT_UP) {
        for (int floor = HARDWARE_NUMBER_OF_FLOORS-1; floor > 0; floor-- ) {
            if (p_queue->order_up[floor] || p_queue->order_down[floor] || p_queue->order_inside[floor] ) {
                p_queue->destination = floor;
                return;
            }
        }
    }

    if (p_queue->preferred_motor_state == HARDWARE_MOVEMENT_DOWN) {
        for (int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS-1; floor++ ) { 
            if (p_queue->order_up[floor] || p_queue->order_down[floor] || p_queue->order_inside[floor] ) {
                p_queue->destination = floor;
                return;
            }
        }
    }
}

void queue_if_destination_reached_set_deafult(QueueState *p_queue) { 
    if (p_queue->current_floor != DEFAULT_FLOOR) {
        if (p_queue->destination == p_queue->current_floor) {
            p_queue->destination = DEFAULT_FLOOR;
            p_queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
        }
    }
}
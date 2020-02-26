#include "hardware.h"
#include "queueV2.h"

static int read_floor() {
    for (int i = 0; i <  HARDWARE_NUMBER_OF_FLOORS; ++i ) {
        if(hardware_read_floor_sensor(i)) return i;
    }
    return -1;
}

void queue_get_current_floor(QueueState2 * p_queue) {
    int floor = read_floor();
    p_queue->current_floor = floor;
    if (floor > -1) {
        p_queue->saved_floor = floor;
    }
}

void queue_default_init(QueueState2 * p_queue) {
    for (int i = 0; i <  HARDWARE_NUMBER_OF_FLOORS; ++i) {
        p_queue->order_up[i] = 0;
        p_queue->order_down[i] = 0; 
        p_queue->order_inside[i] = 0;  
    }
    p_queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
    p_queue->destination = -1;
}

void queue_remove_orders_current_floor(QueueState2 * p_queue) {
    if (p_queue->current_floor > -1) {
        p_queue->order_up[p_queue->current_floor] = 0;
        p_queue->order_down[p_queue->current_floor] = 0; 
        p_queue->order_inside[p_queue->current_floor] = 0;
    }
}

int queue_check_if_stop_floor(QueueState2* p_queue) {
    if(p_queue->current_floor >=0) {
        if(
            p_queue->destination == p_queue->saved_floor
            || p_queue->order_inside[p_queue->saved_floor]
            || (p_queue->order_up[p_queue->saved_floor] && p_queue->preferred_motor_state ==  HARDWARE_MOVEMENT_UP) 
            || (p_queue->order_down[p_queue->saved_floor] && p_queue->preferred_motor_state ==  HARDWARE_MOVEMENT_DOWN)
        ) 
        {       
            return 1;
        }
    }
    return 0;
}

void queue_get_user_input(QueueState2 *p_queue) {  
    for (int i = 0; i < HARDWARE_NUMBER_OF_FLOORS; ++i) {
        if(hardware_read_order(i, HARDWARE_ORDER_UP))  p_queue->order_up[i] = 1;
                   
        if(hardware_read_order(i, HARDWARE_ORDER_DOWN)) p_queue->order_down[i] = 1;
            
        if(hardware_read_order(i, HARDWARE_ORDER_INSIDE)) p_queue->order_inside[i] = 1;
    }         
}

void queue_set_preferred_motor_state(QueueState2 *p_queue) { 
    for (int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS; floor++) {
        if (p_queue->order_up[floor]
            || p_queue->order_down[floor]
            || p_queue->order_inside[floor]
        )
        {
            if (p_queue->saved_floor > floor) {
                p_queue->preferred_motor_state = HARDWARE_MOVEMENT_DOWN;
                break;
            }
            else if (p_queue->saved_floor < floor){
                p_queue->preferred_motor_state = HARDWARE_MOVEMENT_UP;
                break;
            } 
        }
    }
}

void queue_set_destination(QueueState2 *p_queue) {
    if (p_queue->preferred_motor_state == HARDWARE_MOVEMENT_UP) {
        for (int floor = HARDWARE_NUMBER_OF_FLOORS-1; floor > 0; floor-- ) { // ikke >=?
            if (p_queue->order_up[floor] || p_queue->order_down[floor] || p_queue->order_inside[floor] ) {
                p_queue->destination = floor;
                return;
            }
        }
    }

    if (p_queue->preferred_motor_state == HARDWARE_MOVEMENT_DOWN) {
        for (int floor = 0; floor < HARDWARE_NUMBER_OF_FLOORS-1; floor++ ) { // Skal jeg skjekke i øverste?
            if (p_queue->order_up[floor] || p_queue->order_down[floor] || p_queue->order_inside[floor] ) {
                p_queue->destination = floor;
                return;
            }
        }
    }
}
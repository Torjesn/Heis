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
    if (floor >= 0) {
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
}

void queue_remove_orders_current_floor(QueueState2 * p_queue) {
    p_queue->order_up[p_queue->current_floor] = 0;
    p_queue->order_down[p_queue->current_floor] = 0; 
    p_queue->order_inside[p_queue->current_floor] = 0;
}

int queue_check_if_drive_further(QueueState2 * p_queue) {
    switch(p_queue->preferred_motor_state){
    
    case HARDWARE_MOVEMENT_UP: 
        for (int i = p_queue->saved_floor+1; i < HARDWARE_NUMBER_OF_FLOORS; i++) {
            if (p_queue->order_up[i]) return 1;
            if (p_queue->order_inside[i]) return 1;
        }
        break;
    
    case HARDWARE_MOVEMENT_DOWN:  
        for (int i = p_queue->saved_floor-1; i >= 0; i--){
                if (p_queue->order_down[i]) return 1; //potensiell segfault?
                if (p_queue->order_inside[i]) return 1;
        }
    p_queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP; // redundant?
    return 0;
    }
}

int queue_check_if_stop_floor(QueueState2* p_queue) {
    if(
        p_queue->order_inside[p_queue->current_floor]
        || (p_queue->order_up[p_queue->current_floor] && p_queue->preferred_motor_state ==  HARDWARE_MOVEMENT_UP) 
        || (p_queue->order_down[p_queue->current_floor] && p_queue->preferred_motor_state ==  HARDWARE_MOVEMENT_DOWN)
    ) 
    {       
        return 1;
    }
    return 0;
}

void queue_get_user_input(QueueState2 *p_queue) {  
    for (int i = 0; i < NUMBER_OF_FLOORS; ++i) {
        if(hardware_read_order(i, HARDWARE_ORDER_UP))  p_queue->order_up[i] = 1;
                   
        if(hardware_read_order(i, HARDWARE_ORDER_DOWN)) p_queue->order_down[i] = 1;
            
        if(hardware_read_order(i, HARDWARE_ORDER_INSIDE)) p_queue->order_down[i] = 1;
    }         
}

void queue_set_preferred_motor_state(QueueState2 *p_queue) { 
    for (int i = 0; i < NUMBER_OF_FLOORS; i++) {
        if (p_queue->order_up[i]
            || p_queue->order_down[i]
            || p_queue->order_inside[i]
        )
        {
            if (p_queue->current_floor > i) {
                p_queue->preferred_motor_state == HARDWARE_MOVEMENT_DOWN;
                break;
            }
            else if (p_queue->current_floor > i){
                p_queue->preferred_motor_state == HARDWARE_MOVEMENT_UP;
                break;
            } 
        }
    }
}
#include "hardware.h"
#include "elevatorCue.h"
#include "elevatorLogic.h"
#include "elevatorFSM.h"

void queue_default_init(QueueState * p_queue) {
    for (int i = 0; i < NUMBER_OF_FLOORS; ++i) {
        p_queue->order_up[i] = 0;
        p_queue->order_down[i] = 0; 
        p_queue->order_inside[i] = 0;  
    }
    p_queue->count_outside = 0;
    p_queue->count_inside = 0;
    p_queue->destination = DEFAULT_DESTINATION;
    p_queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
    p_queue->max_times_inside = MAX_TIMES_INSIDE;
}

void queue_remove_orders_current_floor(QueueState * p_queue) {
    if (p_queue->order_up[p_queue->current_floor]) {
        for (int i = 0; i < NUMBER_OF_FLOORS; ++i) {
            if(p_queue->order_up[i] > p_queue->order_up[p_queue->current_floor]) --p_queue->order_up[i];
            if(p_queue->order_down[i] > p_queue->order_up[p_queue->current_floor]) --p_queue->order_down[i];
        }
        --p_queue->count_outside;
        p_queue->order_up[p_queue->current_floor] = 0;
    }
    
    if (p_queue->order_down[p_queue->current_floor]) {
        for (int i = 0; i < NUMBER_OF_FLOORS; ++i) {
            if(p_queue->order_down[i] > p_queue->order_down[p_queue->current_floor]) --p_queue->order_down[i];
            if(p_queue->order_up[i] > p_queue->order_down[p_queue->current_floor]) --p_queue->order_up[i];
        }
        --p_queue->count_outside;
        p_queue->order_down[p_queue->current_floor] = 0;
    }
    
    if (p_queue->order_inside[p_queue->current_floor]) {
        for (int i = 0; i < NUMBER_OF_FLOORS; ++i) {
            if(p_queue->order_inside[i] > p_queue->order_inside[p_queue->current_floor]) p_queue->order_inside[i] -=1;
        }
        --p_queue->count_inside;
        p_queue->order_inside[p_queue->current_floor] = 0;
    }
}

void queue_get_next_destination(QueueState * p_queue) {
        if (p_queue->max_times_inside){
            for(int i = 0; i< NUMBER_OF_FLOORS; ++i) {
                if (p_queue->order_inside[i] == 1) {
                    p_queue->destination = i;
                    p_queue->max_times_inside -= 1;
                    return;
                }
            }
        }
        for(int i = 0; i< NUMBER_OF_FLOORS; ++i) {
            if (p_queue->order_up[i] == 1 || p_queue->order_down[i] == 1) {
                p_queue->destination = i;
                return;
            }
        }
        p_queue->max_times_inside = MAX_TIMES_INSIDE;
        p_queue->destination = DEFAULT_DESTINATION;
}

int queue_check_if_stop_floor(QueueState* p_queue) {
    if(
        p_queue->order_inside[p_queue->current_floor_not_between]
        || p_queue->destination == p_queue->current_floor_not_between
        || (p_queue->order_up[p_queue->current_floor_not_between] && p_queue->preferred_motor_state ==  HARDWARE_MOVEMENT_UP) 
        || (p_queue->order_down[p_queue->current_floor_not_between] && p_queue->preferred_motor_state ==  HARDWARE_MOVEMENT_DOWN)
     ) 
    {       
        return 1;
    }
    return 0;
}

void queue_get_user_input(QueueState *p_queue) {  
    for (int i = 0; i < NUMBER_OF_FLOORS; ++i) {
        if (p_queue->current_floor_not_between != i || p_queue->destination == DEFAULT_DESTINATION) { //skal ikke ta inn ordre på nåværende etasje
            if(hardware_read_order(i, HARDWARE_ORDER_UP)) {
                if(p_queue->order_up[i] == 0) {
                    p_queue->order_up[i] = ++p_queue->count_outside;
                }
            }

            if(hardware_read_order(i, HARDWARE_ORDER_DOWN)) { 
                if(p_queue->order_down[i] == 0) {
                    p_queue->order_down[i] = ++p_queue->count_outside;
                }
            }
            
            if(hardware_read_order(i, HARDWARE_ORDER_INSIDE)) {
                if(p_queue->order_inside[i] == 0) {
                    p_queue->order_inside[i] = ++p_queue->count_inside;
                }
            }
        }
    }
}

void queue_set_preferred_motor_state(QueueState *p_queue) { 
    if (p_queue->destination == DEFAULT_DESTINATION) p_queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
    else if (p_queue->current_floor > p_queue->destination) p_queue->preferred_motor_state = HARDWARE_MOVEMENT_DOWN;
    else if (p_queue->current_floor < p_queue->destination) p_queue->preferred_motor_state = HARDWARE_MOVEMENT_UP;
    else p_queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
}






#include "hardware.h"
#include "elevatorCue.h"
#include "elevatorLogic.h"
#include "elevatorFSM.h"

void queue_default_init(queueState * queue) {
    for (int i = 0; i < NUMBER_OF_FLOORS; ++i) {
        queue->order_up[i] = 0;
        queue->order_down[i] = 0; 
        queue->order_inside[i] = 0;  
    }
    queue->count_outside = 0;
    queue->count_inside = 0;
    queue->destination = DEFAULT_DESTINATION;
    queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
}

static void decrement_array_over_limit(int array[], int limit, int length) {
    for (int i = 0; i < length; ++i) {
        if(array[i] > limit) --array[i];
    }
}
 //decrement_array_over_limit(queue->order_up, queue->order_up[queue->current_floor],NUMBER_OF_FLOORS);
    //decrement_array_over_limit(queue->order_down, queue->order_down[queue->current_floor], NUMBER_OF_FLOORS);
    //decrement_array_over_limit(queue->order_inside, queue->order_inside[queue->current_floor], NUMBER_OF_FLOORS);
void queue_remove_orders_current_floor(queueState * queue) {
    if (queue->order_up[queue->current_floor]) {
    decrement_array_over_limit(queue->order_up, queue->order_up[queue->current_floor],NUMBER_OF_FLOORS);
    --queue->count_outside;
    queue->order_up[queue->current_floor] = 0;
    }
    
    if (queue->order_down[queue->current_floor]) {
    decrement_array_over_limit(queue->order_down, queue->order_down[queue->current_floor], NUMBER_OF_FLOORS);
    --queue->count_outside;
    queue->order_down[queue->current_floor] = 0;
    }
    
    if (queue->order_inside[queue->current_floor]) {
    decrement_array_over_limit(queue->order_inside, queue->order_inside[queue->current_floor], NUMBER_OF_FLOORS);
    --queue->count_inside;
    queue->order_inside[queue->current_floor] = 0;
    }
}

void queue_get_next_destination(queueState * queue) {
    if (queue->destination == queue->current_floor || queue->destination == DEFAULT_DESTINATION) {
        for(int i = 0; i< NUMBER_OF_FLOORS; ++i) {
            if (queue->order_inside[i] == 1) {
                queue->destination = i;
                return;
            }
        }
        for(int i = 0; i< NUMBER_OF_FLOORS; ++i) {
            if (queue->order_up[i] == 1) {
                queue->destination = i;
                return;
            }
            if (queue->order_down[i] == 1) {
                queue->destination = i;
                return;
            }
        }
        queue->destination = DEFAULT_DESTINATION;
    }
}

int queue_check_if_stop_floor(queueState* queue) {
    if (queue->current_floor >= 0 && queue->current_floor < NUMBER_OF_FLOORS) {   //kanskje ikke nødvendig, helt oveerflødig, ettersom current floor ikke er -1
        if(
            queue->order_inside[queue->current_floor]
            || queue->destination == queue->current_floor
            || (queue->order_up[queue->current_floor] && queue->preferred_motor_state ==  HARDWARE_MOVEMENT_UP) 
            || (queue->order_down[queue->current_floor] && queue->preferred_motor_state ==  HARDWARE_MOVEMENT_DOWN)
        ) 
        {       
            return 1;
        }
    }
    return 0;
}


void queue_get_user_input(queueState * queue) {  
    for (int i = 0; i < NUMBER_OF_FLOORS; ++i) {
        if(hardware_read_order(i, HARDWARE_ORDER_UP)) {
            if(queue->order_up[i] == 0) {
                queue->order_up[i] = ++queue->count_outside;
            }
        }

        if(hardware_read_order(i, HARDWARE_ORDER_DOWN)) { 
            if(queue->order_down[i] == 0) {
                queue->order_down[i] = ++queue->count_outside;
            }
        }
        
        if(hardware_read_order(i, HARDWARE_ORDER_INSIDE)) {
            if(queue->order_inside[i] == 0) {
                queue->order_inside[i] = ++queue->count_inside;
            }
        }
    }
}

void queue_set_preferred_motor_state(queueState *queue) { 
    if (queue->destination == DEFAULT_DESTINATION) queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
    else if (queue->current_floor > queue->destination) queue->preferred_motor_state = HARDWARE_MOVEMENT_DOWN;
    else if (queue->current_floor < queue->destination) queue->preferred_motor_state = HARDWARE_MOVEMENT_UP;
    else queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
}






#include "hardware.h"
#include "elevatorCue.h"

void queue_default_init(queueState * queue) {
    for (int i = 0; i < g_number_of_floors; ++i) {
         queue->order_up[i] = 0;
         queue->order_down[i] = 0;
         queue->order_inside[i] = 0;    
    }
    queue->count_outside = 0;
    queue->count_inside = 0;
    queue->destination = -1;
    queue->current_floor = -1;
    queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
}

static void decrement_array_over_limit(int array[], int limit, int length) {
    for (int i = 0; i < length; ++i) {
        if(array[i] > limit) --array[i];
    }
}

void queue_remove_orders_current_floor(queueState * queue) {
    int floor_array = queue->current_floor-1;
    decrement_array_over_limit(queue->order_up, queue->order_up[floor_array], g_number_of_floors);
    decrement_array_over_limit(queue->order_down, queue->order_down[floor_array], g_number_of_floors);
    decrement_array_over_limit(queue->order_inside, queue->order_inside[floor_array],g_number_of_floors);
    
    queue->order_up[floor_array] = 0;
    queue->order_down[floor_array] = 0;
    queue->order_inside[floor_array] = 0;
}



void queue_get_next_destination(queueState * queue) {
    for(int i = 0; i< g_number_of_floors; ++i) {
        if (queue->order_inside[i] == 1) {
           decrement_array_over_limit(queue->order_inside, 0, g_number_of_floors);
            queue->destination = i+1;
            return;
        }
    }
    for(int i = 0; i< g_number_of_floors; ++i) {
        if (queue->order_up[i] == 1) {
            decrement_array_over_limit(queue->order_up, 0, g_number_of_floors);
            decrement_array_over_limit(queue->order_down, 0, g_number_of_floors);
            queue->destination = i+1;
            return;
        }
        if (queue->order_down[i] == 1) {
            decrement_array_over_limit(queue->order_up, 0, g_number_of_floors);
            decrement_array_over_limit(queue->order_down, 0, g_number_of_floors);
            queue->destination = i+1;
            return;
        }
    }
    queue->destination = -1;
}

int queue_check_if_stop_floor(queueState* queue) {
    int floor_array = queue->current_floor-1; 
    if (floor_array >= 0 && floor_array < g_number_of_floors) {   //kanskje ikke nødvendig 
        if(
            queue->order_inside[floor_array] 
            || (queue->order_up[floor_array] && queue->preferred_motor_state ==  HARDWARE_MOVEMENT_UP) 
            || (queue->order_down[floor_array] && queue->preferred_motor_state ==  HARDWARE_MOVEMENT_DOWN)
        ) 
        {       
            return 1;
        }
    }
    return 0;
}


void queue_get_user_input(queueState * queue) {  
    for (int i = 0; i < g_number_of_floors; ++i) {
        if(hardware_read_order(i+1, HARDWARE_ORDER_UP)) {
            if(queue->order_up[i] == 0) {
                queue->order_up[i] = ++queue->count_outside;
            }
        }

        if(hardware_read_order(i+1, HARDWARE_ORDER_DOWN)) { 
            if(queue->order_down[i] == 0) {
                queue->order_down[i] = ++queue->count_outside;
            }
        }
        
        if(hardware_read_order(i+1, HARDWARE_ORDER_INSIDE)) {
            if(queue->order_inside[i] == 0) {
                queue->order_inside[i] = ++queue->count_inside;
            }
        }
    }
}

void queue_set_preferred_motor_state(queueState *queue) { 
    if (queue->destination == -1) queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP;
    else if (queue->current_floor > queue->destination) queue->preferred_motor_state = HARDWARE_MOVEMENT_DOWN;
    else if (queue->current_floor < queue->destination) queue->preferred_motor_state = HARDWARE_MOVEMENT_UP;
    else queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP; //Jeg vet ikke helt hvordan dette vil fungere, hva hvis man trykker på den samme i 
}

void queue_delete_button(queueState *queue) {
    for (int i = 0; i < g_number_of_floors; ++i) {
        queue->order_up[i] = 0;
        queue->order_down[i] = 0;
        queue->order_inside[i] = 0;
    }
    queue->count_outside = 0;
    queue->count_inside = 0;
    queue->preferred_motor_state = HARDWARE_MOVEMENT_STOP; //Kan være at denne dekkes andre plasser
}




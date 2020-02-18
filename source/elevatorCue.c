#include "hardware.h"
#include "elevatorCue.h"

const int g_number_of_floors = 4; //kanskje ikke et g prefix?

void remove_orders_current_floor(queueState * queue) {
    int floor_array = queue->current_floor-1;
    decrement_array_over_limit(queue->order_up, queue->order_up[floor_array], g_number_of_floors);
    decrement_array_over_limit(queue->order_down, queue->order_down[floor_array], g_number_of_floors);
    decrement_array_over_limit(queue->order_inside, queue->order_inside[floor_array],g_number_of_floors);
    
    queue->order_up[floor_array] = 0;
    queue->order_down[floor_array] = 0;
    queue->order_inside[floor_array] = 0;
}

void decrement_array_over_limit(int array[], int limit, int length) {
    for (int i = 0; i < length; ++i) {
        if(array[i] > limit) --array[i];
    }
}

void get_next_destination(queueState * queue) {
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

int check_if_stop_floor(queueState* queue) {
    int floor_array = read_floor()-1;
    if (floor_array >= 0 && floor_array < g_number_of_floors) {   //kanskje ikke nødvendig 
        if(
            queue->order_inside[floor_array] 
            || (queue->order_up[floor_array] && queue->motor_state ==  HARDWARE_MOVEMENT_UP) 
            || (queue->order_down[floor_array] && queue->motor_state ==  HARDWARE_MOVEMENT_DOWN)
        ) 
        {       
            //remove_orders_current_floor(queue);
            //usikker på om denne burde være her
            return 1;
        }
    }
    return 0;
}

int read_floor() {
    for (int i = 1; i <= g_number_of_floors; ++i ) {
        if(hardware_read_floor_sensor(i)) return i;
    }
    return -1;
}

void get_elevator_input(queueState * queue) {  
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

void set_state(queueState *queue) { 
    if (queue->destination == -1) queue->motor_state = HARDWARE_MOVEMENT_STOP;
    else if (queue->current_floor > queue->destination) queue->motor_state = HARDWARE_MOVEMENT_DOWN;
    else if (queue->current_floor < queue->destination) queue->motor_state = HARDWARE_MOVEMENT_UP;
    else queue->motor_state = HARDWARE_MOVEMENT_STOP; //Jeg vet ikke helt hvordan dette vil fungere, hva hvis man trykker på den samme i 
}

void delete_button_ queue(queueState *queue) {
    if (hardware_read_stop_signal()) {
        for (int i = 0; i < g_number_of_floors; ++i) {
            queue->order_up[i] = 0;
            queue->order_down[i] = 0;
            queue->order_inside[i] = 0;
            queue->count_outside = 0;
            queue->count_inside = 0;
        }
        queue->motor_state = HARDWARE_MOVEMENT_STOP; //Kan være at denne dekkes andre plasser
    }
}


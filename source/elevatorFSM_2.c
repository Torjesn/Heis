#include "elevatorFSM_2.h"
#include "queueV2.h"
#include "set_hardware.h"
#include <time.h>
#include <stdlib.h>

void elevator_fsm2() {
    sethw_start_procedure_elevator();
    
    QueueState2 * p_queue = (QueueState2*) malloc(sizeof(*p_queue));
    queue_default_init(p_queue);
    queue_get_current_floor(p_queue);

    DoorState * p_door = (DoorState*) malloc(sizeof(*p_door)); 
    *p_door = DOOR_CLOSED;

    clock_t * p_door_open_timer = (clock_t *) malloc(sizeof(*p_door_open_timer));
    * p_door_open_timer = clock(); 
    
    while (1) {
        queue_get_current_floor(p_queue);
        
        if (hardware_read_stop_signal() ) { 
            sethw_stop_button_procedure(p_door, p_queue);
        } 
        
        else {
            queue_get_user_input(p_queue);

            if (queue_check_if_stop_floor(p_queue)) {
                sethw_stop_on_floor(p_door, p_door_open_timer); 
                queue_set_destination(p_queue); 
                queue_remove_orders_current_floor(p_queue);
            }

            queue_if_destination_reached_set_deafult(p_queue);
            if (p_queue->destination == DEFAULT_FLOOR) {
                queue_set_preferred_motor_state(p_queue);
                queue_set_destination(p_queue);
            }

            sethw_motor(p_door, p_queue);
        }
        
        
        if (*p_door == DOOR_OPEN) { 
                sethw_try_close_door(p_door, p_door_open_timer); 
        }
        sethw_lights(p_door, p_queue);
    }  
}
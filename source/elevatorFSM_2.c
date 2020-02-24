#include "elevatorFSM.h"
#include "queueV2.h"
#include "set_hardware.h"
#include <time.h>
#include <stdlib.h>

void elevator_fsm2() {
    start_procedure_elevator();
    
    QueueState2 * p_queue = (QueueState2*) malloc(sizeof(*p_queue));
    queue_default_init(p_queue);
    queue_get_current_floor(p_queue);

    DoorState * p_door;
    *p_door = DOOR_CLOSED;

    clock_t * p_door_open_timer = (clock_t *) malloc(sizeof(*p_door_open_timer));
    * p_door_open_timer = clock(); 
    
    
    while (1) {
        queue_get_current_floor(p_queue);
        
        if (hardware_read_stop_signal() ) { 
            stop_button_procedure(p_door, p_queue);
        } 
        
        else {
            queue_get_user_input(p_queue);  
            
            if (queue_check_if_stop_floor(p_queue)) {
                stop_on_floor(p_door, p_queue, p_door_open_timer); 
                queue_remove_orders_current_floor(p_queue);

            }
            
            if (!queue_check_if_drive_further(p_queue)) {
                queue_set_preferred_motor_state(p_queue);
            }
            write_to_motor(p_door, p_queue);
        }
        
        set_lights(p_door, p_queue);
        if (*p_door == DOOR_OPEN) { 
                try_close_door(p_door, p_door_open_timer); 
        }
    }  
}
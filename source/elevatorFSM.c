#include "elevatorFSM.h"
#include "elevatorLogic.h"
#include "elevatorCue.h"
#include <time.h>
#include <stdlib.h>

void elevator_fsm() {
    start_procedure_elevator();
    
    QueueState * queue = (QueueState*) malloc(sizeof(*queue));
    queue_default_init(queue);

    ElevatorState * elev_state = (ElevatorState *) malloc(sizeof(*elev_state));
    init_elevator_states(elev_state);

    get_current_floor_state(elev_state, queue);

    clock_t * door_open_timer = (clock_t *) malloc(sizeof(*door_open_timer));
    * door_open_timer = clock(); 
    
    
    while (1) {
        get_current_floor_state(elev_state, queue); // burde denne være her?
        
        if (hardware_read_stop_signal() ) { 
            stop_button_procedure(elev_state, queue);
        } 
        
        else {
            queue_get_user_input(queue);  
            if ((p_queue->destination == DEFAULT_DESTINATION || p_queue->destination == DESTINATION_WAITING)
            && p_elev_state->door == DOOR_CLOSED)
            {
                queue_get_next_destination(queue); //set next destination?
                queue_set_preferred_motor_state(queue);
            }
            
            if (queue_check_if_stop_floor(queue)) {
                stop_on_floor(elev_state, queue, door_open_timer); 
                open_door(elev_state); // kanskje legge inn i stop_on_floor
                queue_remove_orders_current_floor(queue);
                p_queue->destination  = DESTINATION_WAITING;
                
            }
            write_to_motor(elev_state, queue);
        }
        
        set_lights(elev_state, queue);
        if (p_elev_state->door == DOOR_OPEN) { 
                try_close_door(elev_state, door_open_timer); 
        }
    }  
}


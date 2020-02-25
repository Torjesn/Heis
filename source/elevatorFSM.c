#include "elevatorFSM.h"
#include "elevatorLogic.h"
#include "elevatorCue.h"
#include <time.h>
#include <stdlib.h>

void elevator_fsm() {
    start_procedure_elevator();
    
    QueueState * p_queue = (QueueState*) malloc(sizeof(*p_queue));
    queue_default_init(p_queue);

    ElevatorState * p_elev_state = (ElevatorState *) malloc(sizeof(*p_elev_state));
    init_elevator_states(p_elev_state);

    get_current_floor_state(p_elev_state, p_queue);

    clock_t * door_open_timer = (clock_t *) malloc(sizeof(*door_open_timer));
    * door_open_timer = clock(); 
    
    
    while (1) {
        get_current_floor_state(p_elev_state, p_queue); // burde denne være her?
        
        if (hardware_read_stop_signal() ) { 
            stop_button_procedure(p_elev_state, p_queue);
        } 
        
        else {
            queue_get_user_input(p_queue);  
            if ((p_queue->destination == DEFAULT_DESTINATION || p_queue->destination == DESTINATION_WAITING)
            && p_elev_state->door == DOOR_CLOSED)
            {
                queue_get_next_destination(p_queue); //set next destination?
                queue_set_preferred_motor_state(p_queue);
            }
            
            if (queue_check_if_stop_floor(p_queue)) {
                stop_on_floor(p_elev_state, p_queue, door_open_timer); 
                open_door(p_elev_state); // kanskje legge inn i stop_on_floor
                queue_remove_orders_current_floor(p_queue);
                p_queue->destination  = DESTINATION_WAITING;
                
            }
            write_to_motor(p_elev_state, p_queue);
        }
        
        set_lights(p_elev_state, p_queue);
        if (p_elev_state->door == DOOR_OPEN) { 
                try_close_door(p_elev_state, door_open_timer); 
        }
    }  
}


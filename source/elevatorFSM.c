#include "elevatorFSM.h"
#include "elevatorLogic.h"
#include "elevatorCue.h"
#include <time.h>
#include <stdlib.h>
//vi burde legge inn headerguards til slutt også



void elevator_fsm() {
    start_procedure_elevator();
    
    queueState * queue = (queueState*) malloc( sizeof(*queue));

    queue_default_init(queue);

    ElevatorState * elev_state = (ElevatorState *) malloc(sizeof(*elev_state));
    init_elevator_states(elev_state);

    get_current_floor_state(elev_state, queue);

    clock_t * real_time = (clock_t *) malloc(sizeof(*real_time));
    * real_time = clock();
    clock_t * door_open_timer = (clock_t *) malloc(sizeof(*door_open_timer));
    * door_open_timer = clock(); // oppsett fungerer 
    while (1) {
        *real_time = clock();
        get_current_floor_state(elev_state, queue); //fungerer 
        if (hardware_read_stop_signal() ) { //fungerer 
            stop_button_procedure(elev_state, queue); //fungerer 
        } else {
            queue_get_user_input(queue); // fungerer 
            if (queue->destination == DEFAULT_DESTINATION) {
                queue_get_next_destination(queue);
                queue_set_preferred_motor_state(queue);
            }
            
            //if (queue->preferred_motor_state != HARDWARE_MOVEMENT_STOP) hardware_command_stop_light(1); //tset
            //else hardware_command_stop_light(0);
            

            if (queue_check_if_stop_floor(queue)) {
                stop_on_floor(elev_state, queue, door_open_timer); 
                open_door(elev_state);
                queue_remove_orders_current_floor(queue);
                queue_get_next_destination(queue);
                queue_set_preferred_motor_state(queue);
            }
            write_to_motor(elev_state, queue);
        }
        
        set_lights(elev_state, queue); //fungerer
        if (elev_state->door == DOOR_OPEN) { //fungerer
                try_close_door(elev_state, real_time, door_open_timer); 
        }
    }  
}


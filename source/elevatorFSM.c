#include "elevatorFSM.h"
#include "elevatorLogic.h"
#include "elevatorCue.h"
#include <time.h>
//vi burde legge inn headerguards til slutt også



void elevator_fsm() {
    start_procedure_elevator();
    
    queueState * queue;
    queue_default_init(queue);

    ElevatorState * elev_state;
    init_elevator_states(elev_state);

    clock_t real_time = clock();
    clock_t door_open_timer = clock();
    
    while (1) {
        real_time = clock();
        
        if (hardware_read_stop_signal() ) { 
            stop_button_procedure(elev_state, queue);
        } else {
        
        queue_get_elevator_input(queue); //Dette er bare funksjoner limt inn, ingen logikk
        queue_get_next_destination(queue);
        queue_set_preferred_motor_state(queue);
        
        if (queue_check_if_stop_floor(queue)) {
            stop_on_floor(elev_state, queue, door_open_timer);
        }
        
        if (elev_state->door == DOOR_OPEN) {
            try_close_door(elev_state, real_time, door_open_timer); 
        }
        write_to_motor(queue, elev_state);
        }
    }  
}


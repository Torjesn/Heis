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

    get_current_floor_state(elev_state, queue);

    clock_t * real_time = clock();
    clock_t * door_open_timer = clock();
    while (1) {
        
        real_time = clock();
        get_current_floor_state(elev_state, queue);
        
        if (hardware_read_stop_signal() ) { 
            stop_button_procedure(elev_state, queue);
        } else {
            
            queue_get_elevator_input(queue);
            if (queue_check_if_stop_floor(queue) || queue->destination == DEFAULT_DESTINATION) {
                stop_on_floor(elev_state, queue, door_open_timer); //kan være at vi burde ha noen flere funkjsoner utenfor for å
            }
            write_to_motor(queue, elev_state);
        }
        
        set_lights(elev_state, queue);
        if (elev_state->door == DOOR_OPEN) {
                try_close_door(elev_state, real_time, door_open_timer); 
        }
    }  
}


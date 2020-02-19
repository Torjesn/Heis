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
        
        if (hardware_read_stop_signal() ) { //Ikke fulstendig gjennomtenkt, men burde fungere?
            stop_button_procedure(elev_state, queue);
        } else {
        
        get_elevator_input(queue); //Dette er bare funksjoner limt inn, ingen logikk
        get_next_destination(queue);
        set_preferred_motor_state(queue);
        hardware_command_movement(queue->preferred_motor_state);
        check_if_stop_floor(queue);
        }
    }  
}


static void stop_on_floor(ElevatorState* elev_state, clock_t* real_time, clock_t* door_open_timer, queueState* queue) {
    *door_open_timer = clock() + 3 * CLOCKS_PER_SEC;
    open_door(elev_state);
    queue_remove_orders_current_floor(queue);
}

static void stop_button_procedure(ElevatorState* elev_state, queueState* queue) {
    elev_state->movement = HARDWARE_MOVEMENT_STOP;
    queue_delete_button(queue);
    if (elev_state->current_floor > 0) {
        elev_state->door = DOOR_OPEN;
    }
}
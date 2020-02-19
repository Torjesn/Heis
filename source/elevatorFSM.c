#include "elevatorFSM.h"
#include "elevatorLogic.h"
#include "elevatorCue.h"
#include <time.h>
//vi burde legge inn headerguards til slutt også

static void stop_on_floor(ElevatorState* elev_state, clock_t* door_open_timer, queueState* queue) {
    *door_open_timer = clock() + 3 * CLOCKS_PER_SEC;
    elev_state->movement = HARDWARE_MOVEMENT_STOP; //kan kanskje gjøres i en funksjon
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
        
        get_elevator_input(queue); //Dette er bare funksjoner limt inn, ingen logikk
        get_next_destination(queue);
        set_preferred_motor_state(queue);
        hardware_command_movement(queue->preferred_motor_state);
        
        if (check_if_stop_floor(queue)) {
            stop_on_floor(elev_state, queue, door_open_timer);
        }
        
        if (elev_state->door == DOOR_OPEN) {
            try_close_door(elev_state, real_time, door_open_timer); 
        }
        }
    }  
}



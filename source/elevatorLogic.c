#include "elevatorCue.h"
#include "hardware.h"
#include "elevatorLogic.h"

static void start_procedure_elevator() {
    int floor_level =  read_floor();
    while (floor_level == -1) {
        int floor_level =  read_floor();
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN); //kan denne være før, så den slipper å settes hele tiden?
    }
}

void set_lights(ElevatorState elevator, queueState * queue){
    //setter stopp-lys: 
    hardware_command_stop_light(hardware_read_stop_signal()); //hardware_read_stop_signal returnerer 0 hvis den er av, 1, hvis på og command skriver med samme verdier
    //setter døra til åpen: 
    if (elevator.door = DOOR_OPEN) {
        hardware_command_door_open(1);
    } else if(elevator.door = DOOR_CLOSED) {
        hardware_command_door_open(0);
    }
    //setter order lights 
    int order_outside_length = 3;
    int order_inside_length = 4;
    //kølys inne
    for (int i = 0; i < order_inside_length; i++)
        {
            hardware_command_order_light(i+1,HARDWARE_ORDER_INSIDE,   queue->order_inside[i]);
        }
    //order opp
    for (int i = 0; i < order_outside_length; i++)
        {
            hardware_command_order_light(i+1,HARDWARE_ORDER_UP, queue->order_up[i]);
        }
    //order ned 
    for (int i = 0; i < order_outside_length; i++)
        {
            hardware_command_order_light(i+2, HARDWARE_ORDER_DOWN, queue->order_down[i+1]);
        }
}

void elevator_fsm() {
    start_procedure_elevator();
    
    queueState * queue;
    queue_default_init(queue);

    ElevatorState * elev_state;
    init_elevator_states(elev_state);
    
    while (1) {
        
        if (hardware_read_stop_signal() ) { //Ikke fulstendig gjennomtenkt, men burde fungere?
           elev_state->movement = HARDWARE_MOVEMENT_STOP;
            queue_delete_button(queue);
            if (elev_state->current_floor > 0) {
                elev_state->door = DOOR_OPEN;
            }

        } else {
        
        get_elevator_input(queue); //Dette er bare funksjoner limt inn, ingen logikk
        get_next_destination(queue);
        set_preferred_motor_state(queue);
        hardware_command_movement(queue->preferred_motor_state);
        check_if_stop_floor(queue);
        }
    }  
}

void init_elevator_states(ElevatorState* elev_state) {
    elev_state->current_floor = -1;
    elev_state->door = DOOR_CLOSED;
    elev_state->movement = HARDWARE_MOVEMENT_STOP;
}


int timer;
static void close_door(ElevatorState* elev_state) {
    if (hardware_read_obstruction_signal() || hardware_read_stop_signal()) {
        timer = 3;
    }
    if (timer == 0) {
        elev_state->door = DOOR_CLOSED;
    }
}

static void open_door(ElevatorState* elev_state) {
    if (
        elev_state->movement == HARDWARE_MOVEMENT_STOP
        && elev_state->current_floor > 0
    )
    elev_state->door = DOOR_OPEN;
}

void change_door(ElevatorState* elev_state) {
    if (elev_state->door == DOOR_OPEN) close_door(elev_state);
    else open_door(elev_state);
}



void write_to_motor(queueState* queue, ElevatorState* elev_state) {
    if ( elev_state->door == DOOR_OPEN) elev_state->movement = HARDWARE_MOVEMENT_STOP;
    else elev_state->movement = elev_state->movement = queue->preferred_motor_state;
    //kunne lagt til en på stopbutton, men dette vil aldri skje
    hardware_command_movement(elev_state->movement);
}


// må lages, skal kalles hvis den skal stoppe.
void stop_on_floor() {

}

#include "elevatorCue.h"
#include "hardware.h"
#include "elevatorLogic.h"

static void start_procedure_elevator() {
    int floor_level =  read_floor();
    while (floor_level == -1) {
        int floor_level =  read_floor();
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
    }
}

void elevator_fsm() {
    start_procedure_elevator();
    
    queueState * queue;
    queue->current_floor = read_floor();
    while (1) {
        get_elevator_input(queue);
        get_next_destination(queue);
        set_motor_state(queue);
        delete_button_queue(queue);
        hardware_command_movement(queue->motor_state);
        int measured_floor = read_floor(); //her er det litt dårlige variabelnavn
        if (measured_floor != queue->current_floor) {
            check_if_stop_floor(queue); //her må det tenkes litt mer
            //her vil dørlogikk osv være, kan legge inn stop/running-bit
        }
        queue->current_floor = measured_floor; // setter 

    }



}



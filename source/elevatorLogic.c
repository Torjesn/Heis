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
    int current_floor = read_floor();
    DoorState * door;
    HardwareMovement * motor_state;//faktisk state til motor
    while (1) {
        get_elevator_input(queue);
        get_next_destination(queue);
        set_preferred_motor_state(queue);
        delete_button_queue(queue);
        hardware_command_movement(queue->preferred_motor_state);
        int measured_floor = read_floor(); //her er det litt dårlige variabelnavn
        if (measured_floor != queue->current_floor) {
            check_if_stop_floor(queue); //her må det tenkes litt mer
            //her vil dørlogikk osv være, kan legge inn stop/running-bit
        }
        queue->current_floor = measured_floor; // setter 

    }



}


int timer;
void close_door(doorstate* door_state){
    if (obstruksjon && stop) {
        timer = 3;
    }
    if (timer == 0) {
        door_state = door_open;
    }
}

void open_dor {
    //skjekker om motor er på
    //skjekker om etage er gyldig
    //sett dør til åpen
    //evt setter lys
}

void write_to_motor() {}

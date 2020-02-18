#include "elevatorCue.h"
#include "hardware.h"
#include "elevatorLogic.h"

int g_number_of_floors = 4;
static void start_procedure_elevator() {
    int floor_level =  read_floor();
    while (floor_level == -1) {
        int floor_level =  read_floor();
        hardware_command_movement(HARDWARE_MOVEMENT_DOWN);
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
    for (int i = 0; i < g_number_of_floors; i++)
    {
        int order_outside_length = 3;
        int order_inside_length = 4;
        //order opp
        for (int j = 0; j < order_outside_length; j++)
        {
            hardware_command_order_light(i+1,HARDWARE_ORDER_UP, queue->order_up[i+1]);
        }
        
    }
}

void elevator_fsm() {
    start_procedure_elevator();
    
    queueState * queue; //Får kanskje ikek default-initialisert dette her?
    int current_floor = read_floor();
    DoorState * door;
    HardwareMovement * motor_state;//faktisk state til motor
    while (1) {
        
        if (hardware_read_stop_signal() ) {
           *motor_state = HARDWARE_MOVEMENT_UP;
            delete_button_queue(queue);
            if (read_floor() > 0) {
                *door = DOOR_OPEN;
            }

        } else {
        
        get_elevator_input(queue);
        get_next_destination(queue);
        set_preferred_motor_state(queue);
        hardware_command_movement(queue->preferred_motor_state);
        int measured_floor = read_floor(); //her er det litt dårlige variabelnavn
        if (measured_floor != queue->current_floor) {
            check_if_stop_floor(queue); //her må det tenkes litt mer
            //her vil dørlogikk osv være, kan legge inn stop/running-bit
        }
        queue->current_floor = measured_floor; // setter 
        }
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

void open_dor(*motor_state) {
    if (*motor)
    //skjekker om motor er på
    //skjekker om etage er gyldig
    //sett dør til åpen
    //evt setter lys
}

void write_to_motor() {}

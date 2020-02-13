#include "hardware.h"

const int g_number_of_floors = 4; //kanskje ikke et g prefix?

//antar floor er 1-indeksert, 
void remove_orders_current_floor(int floor, int cue_order_up[], int cue_order_down[], int cue_order_inside[]) {
    int floor_array = floor-1;
    decrement_array_over_limit(cue_order_up, cue_order_up[floor_array], g_number_of_floors);
    decrement_array_over_limit(cue_order_down, cue_order_down[floor_array], g_number_of_floors);
    decrement_array_over_limit(cue_order_inside, cue_order_inside[floor_array],g_number_of_floors);
    
    cue_order_up[floor_array] = 0;
    cue_order_down[floor_array] = 0;
    cue_order_inside[floor_array] = 0;
}

void decrement_array_over_limit(int array[], int limit, int length) {
    for (int i = 0; i < length; ++i) {
        if(array[i] > limit) --array[i];
    }
}

void get_next_destination(int cue_order_up[],int cue_order_down[], int cue_order_inside[], int* p_destination) {
    
    for(int i = 0; i< g_number_of_floors; ++i) {
        if (cue_order_inside[i] == 1) {
            return i+1;
            
            decrement_array_over_limit(cue_order_inside, 0, g_number_of_floors);
        }
    }
    for(int i = 0; i< g_number_of_floors; ++i) {
        if (cue_order_up[i] == 1) {
            return i+1;
            decrement_array_over_limit(cue_order_up, 0, g_number_of_floors);
            decrement_array_over_limit(cue_order_down, 0, g_number_of_floors);
        }
        if (cue_order_down[i] == 1) {
            return i+1;
            decrement_array_over_limit(cue_order_up, 0, g_number_of_floors);
            decrement_array_over_limit(cue_order_down, 0, g_number_of_floors);
        }
    }
    return 0;
    //return 0 hvis den ikke kan ta 
}


//En potensiell bug er hvordan jeg representerer floor uten 
int check_if_stop_floor(int floor, int cue_order_up[], int cue_order_down[], int cue_order_inside[], HardwareMovement motor_state ) {
    if(
        cue_order_inside[floor-1] 
        || (cue_order_up[floor-1] && motor_state ==  HARDWARE_MOVEMENT_UP) 
        || (cue_order_down[floor-1] && motor_state ==  HARDWARE_MOVEMENT_DOWN)) 
    {       
        remove_orders_current_floor(floor, cue_order_up, cue_order_down, cue_order_inside);
        return 1;
    }
    else return 0;
}

int read_floor() {
    for (int i = 1; i <= g_number_of_floors; ++i ) {
        if(hardware_read_floor_sensor(i)) return i;
    }
    return 0;
}

void get_elevator_input( int *p_cue_count_outside, int *p_cue_count_inside, int cue_order_up[], int cue_order_down[], int cue_order_inside[]) {  
    for (int i = 0; i < g_number_of_floors; ++i) {
        if(hardware_read_order(i+1, HARDWARE_ORDER_UP)) {
            if(cue_order_up[i] == 0) {
                cue_order_up[i] = ++*p_cue_count_outside;
            }
        }

        if(hardware_read_order(i+1, HARDWARE_ORDER_DOWN)) { 
            if(cue_order_down[i] == 0) {
                cue_order_down[i] = ++*p_cue_count_outside;
            }
        }
        
        if(hardware_read_order(i+1, HARDWARE_ORDER_INSIDE)) {
            if(cue_order_inside[i] == 0) {
                cue_order_inside[i] = ++*p_cue_count_inside;
            }
        }
    }
}

typedef

typedef struct {
    int cue_count_outside = 0;
    int cue_count_inside = 0;
    int cue_order_up[] = {0,0,0,0}; //kan eventuelt ha tre her, gjør aksessering enklere med fire
    int cue_order_down[] = {0,0,0,0}; // -||-
    int cue_order_inside[] = {0,0,0,0};
    int destination = 0;
    int current_floor = 0;
    HardwareMovement motor_state = HARDWARE_MOVEMENT_STOP;
} cueState;


void driver() { //ja, den skal hete noe annet enn driver ;)

    


    while(1) {
        get_elevator_input(&cue_count_outside, cue_order_up);

    }
}






void set_state(HardwareMovement * p_motor_state, int destination, int current_floor) { 
    if (current_floor > destination) *p_motor_state = HARDWARE_MOVEMENT_DOWN;
    else if (current_floor < destination) *p_motor_state = HARDWARE_MOVEMENT_UP;
    else *p_motor_state = HARDWARE_MOVEMENT_STOP; //Jeg vet ikke helt hvordan dette vil fungere, hva hvis man trykker på den samme i 
}

void check_stop_signal() {}
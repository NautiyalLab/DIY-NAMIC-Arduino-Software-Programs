 /*--------------------------------------------------------++
||              NEW_P4 (NO OMISSION / INVALID)             ||
++--------------------------------------------------------*/



/*
 * 1/15/2020 Update: Included # of incorrect poke counts during trial window
 * (when ONE response port (Left or Right) light up)
 *
 * PARADIGM:
 *
 * DESCRIPTION: Refer to Logic Flow Diagram for more detailed information
 *
 *              1.
 *              2.
 *              3.
 *              4.
 *              5.
 *              6.
 *              7.
 *
 *
 * KEY CHARACTERISTICS:
 *
 *
 *
 *
 * PUSH BUTTON BEHAVIOR: single short push toggles solenoid valve operation
 *                       push and hold (for 2s) toggles LED operation
 *
 * EVENT CODE STATUS: Compatible
 *
 * EDIT HISTORY:
 *
 *
 * COPYRIGHT: JHL/ Nautiyal Lab
 *            + Open Source Arduino Forum
 *
 *
 *  ## CHANGED IN G5 (group 5)
 * NOTES::: automatically subtracts one from the final count for relevant windows!!
 *     1) incorrect pokes before trial initiation
 *     2) Pokes during the trial window
 * (CODE: xx29)
 * (CODE: xx89) --> currently (2/14/20) not included in analysis
 */

//-- vs P3: forced choice (only L or R port comes on, not both)
#include "globals_main.h"
    //-- same as P3
    //--initiates the variables needed for the other functions
#include "globals_button.h"
#include "globals_time.h"
    //-- same as P3
#include "random_generator.h"
    //-- generates random iti index # to be selected
    //-- generates random port code to be selected (will select LEFT OR RIGHT, not middle like prev. paradigms)
        //-- returns selected port as current_port
#include "IR.h" //-- this is used for the initiation window
    //-- same as P3
    //-- poke in L or R port --> will register as poke if IR turned off
    //-- poke in middle port --> will register as poke out if IR turned off
        //-- count the number of pokes in all ports until the middle port is poked, put them in the CSV file
            //-- by reversing what counts as "valid" during the trial window, we can count the pokes using this program
        //-- end trial initiation window code, start trial window code
    //-- unlike in P2, this function won't change LED status
#include "begin_trial.h"
    //-- same as P3
    //-- after iti window or in the first trial, the parent if will become True
    //-- if it's neither the delay, initiation or trial windows at that time, turn on initiation window
        //-- the info for this will come from the prev iteration of this code + IR.h (initiation = true IF M poke)
        //-- write down when the trial was initiated in the CSV file
    //-- since trial_initiation_window is now true, turn on LED if off, on if off (appears as blinking!)
    //-- once there is a poke in the middle port, valid_trial_window will be true so we will move to the trial part of the code
        //-- all lights will turn off, time will be recorded, initiate delay will be true
    //-- for now, delay_duration is set a 1 ms so it has no practical effect
        //-- everything will be reset afterwards
        //-- get random port will be true to decide which port to activate (L OR R)
#include "push_button.h"
#include "l_port_operate.h"
     //-- activate_left_led:
        //-- if M solenoid is not active: if it's a valid trial and L led is off --> turn on L LED
        //-- if it's a valid trial, L light is on, there was a poke in L and left port counter is not 0
            //-- end valid trial window (make it false), turn of all lights, write down the time
            //-- put the number of in this window L,R, and M pokes in the CSV file; make reward_window true
            //-- reward_type 1: give reward (if it is 2, ITI will begin)
    //-- activate_left_sol:
        //-- if left light is off:
            //-- if it's the reward window and reward type is 1, make them false and 0 respectively
            //-- activate middle solenoid, register time solenoid_on_time (will be reward cue start time)
            //-- reset poke counters (leaves solenoid on!), reward_cue_window is true and M led is turned on
        //-- if middle solenoid is on and it has been more than solenoid_on_duration:
            //-- turn off middle solenoid, register when
        //-- if reward_cue_window, middle light is on and it has been more than solenoid_on_duration:
            //-- make reward_cue_window false, turn off middle LED
            //-- print when this happened, make start_iti_window true (which will make IR.h run)
            //-- put the number of L,R,M pokes that happened during this window, reset counters
        //-- if start_iti_window is true: (which happens after reward has been dispensed)
            //-- if the current time exceeds ITI window time:
                //-- put the number of pokes during ITI window into the CSV file
                //-- get a new ITI length
                //-- reset counters and current port (make it 0) so everything is reset
                //-- make initiate_new_trial true (begin_trial.h will run), iti_window false
#include "r_port_operate.h"
    //-- activate_right_led:
        //-- if M solenoid is not active: if it's a valid trial and R led is off --> turn on R LED
        //-- if it's a valid trial, R light is on, there was a poke in R and right port counter is not 0
            //-- end valid trial window (make it false), turn off all lights, write down the time
            //-- put the number of in this window L,R, and M pokes in the CSV file; make reward_window true
            //-- reward_type 1: give reward (if it is 2, ITI will begin)
    //-- activate_right_sol:
        //-- if right light is off:
            //-- if it's the reward window and reward type is 1, make them false and 0 respectively
            //-- activate middle solenoid, register time solenoid_on_time (will be reward cue start time)
            //-- reset poke counters (leaves solenoid on!), reward_cue_window is true and M led is turned on
        //-- if middle solenoid is on and it has been more than solenoid_on_duration:
            //-- turn off middle solenoid, register when
        //-- if reward_cue_window, middle light is on and it has been more than solenoid_on_duration:
            //-- make reward_cue_window false, turn off middle LED
            //-- print when this happened, make start_iti_window true (which will make IR.h run)
            //-- put the number of L,R,M pokes that happened during this window, reset counters
        //-- if start_iti_window is true: (which happens after reward has been dispensed)
            //-- if the current time exceeds ITI window time:
                //-- put the number of pokes during ITI window into the CSV file
                //-- get a new ITI length
                //-- reset counters and current port (make it 0) so everything is reset
                //-- make initiate_new_trial true (begin_trial.h will run), iti_window false
String paradigm = "G5_P4_v3";

// * * * * * * * * * * * S E T U P * * * * * * * * * * * * * * *

void setup(){

    // randomize the start of the random generator
    randomSeed(analogRead(0));

    pinMode (button_pin, INPUT);

    // solenoids initialization
    for (byte i = 4; i <=6; i++) { pinMode(i, OUTPUT);}

    // LED initialization
    for (byte i = 8; i <=10; i++) { pinMode(i, OUTPUT);}

    // IRs initialization
   for (byte i = 11; i <=13; i++) { pinMode(i, INPUT);}
    // pinMode (9, INPUT);  // modified pin #s
    // pinMode (11, INPUT); // modified pin #s
    // pinMode (13, INPUT); // modified pin #s

    Serial.begin(115200);
    Serial.print("Paradigm:");
    Serial.println(paradigm); //here I should also print if this is left or right larger reward

    // initialize initiate_trial_window to true to start program
    initiate_new_trial = true;

    // initiate random_idx to pick the first idx for Inter-Trial Interval
    random_idx = random_iti();

    // Serial.print("idx: ");
    // Serial.println(random_idx);
    // Serial.println(iti_interval[random_idx]);
}

// * * * * * * * * * * * S E T U P * * * * * * * * * * * * * * *

// * * * * * * * * * * * L O O P * * * * * * * * * * * * * * * *

void loop() {

    // activate_button();
    record_IR();
    begin_new_trial();

    // Once begin_new_trial finishes running, should be right HERE!
    // Next Step is to pick the activation port!!!

    // NEED TO RESET current_port AT ONE POINT IN CODE,
    // SO THAT below if statements don't get evaluated!!!
    // (otherwise, current_port will always be either 1 or 2 --> leading to unexpected results!)

    if (current_port == 1) { //-- the port is chosen in begin_trial.h
      // delay(1);
      activate_left_led();
      activate_left_sol();

      }

    else if (current_port == 2) {
      // delay(1);
      activate_right_led();
      activate_right_sol();

      }

}

// * * * * * * * * * * * L O O P * * * * * * * * * * * * * * * *

 /*--------------------------------------------------------++
||             NEW_P5 (delay_duration = 1ms)              ||
++--------------------------------------------------------*/



/*
 *
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
 * COPYRIGHT: JHL/ Nautiyal Lab
 *            + Open Source Arduino Forum
 *
 *  ## CHANGED IN G5 (group 5)
 * NOTES::: automatically subtracts one from the final count for relevant windows!!
 *     1) incorrect pokes before trial initiation
 *     2) Pokes during the trial window --> Remember that P5 DOES NOT have this (instead we have the omission trial!!)
 *
 * FIX ONLY (CODE: xx29)
 *
 *
 *
 */

//-- vs P4: 5 sec trial window; omission (no poke) + incorrect trials (poke in wrong port) introduced
#include "globals_main.h"
    //-- same as P4
    //--initiates the variables needed for the other functions
#include "globals_button.h"
    //-- same a P4
#include "globals_time.h"
    //-- same as P4
#include "random_generator.h"
    //-- same as P4
    //-- generates random iti index # to be selected
    //-- generates random port code to be selected (will select LEFT OR RIGHT)
        //-- returns selected port as current_port
#include "IR.h"
    //-- same as P4
    //-- poke in L or R port --> will register as poke if IR turned off
    //-- poke in middle port --> will register as poke out if IR turned off
        //-- if it was in the trial initiation phase:
            //-- count the number of pokes in all ports until the middle port is poked, put them in the CSV file
            //-- end trial initiation window code, start trial window code
#include "begin_trial.h"
    //-- same as P4
    //-- after iti window or in the first trial, the parent if will become True
    //-- if it's neither the delay, initiation or trial windows at that time, turn on initiation window
        //-- the info for this will come from the prev iteration of this code + IR.h (initiation = true IF M poke)
        //-- write down when the trial was initiated in the CSV file
    //-- since trial_initiation_window is now true, turn on M LED if off, on if off (appears as blinking!)
    //-- once there is a poke in the middle port, valid_trial_window will be true so we will move to the trial part of the code
        //-- all lights will turn off, time will be recorded, initiate delay will be true
    //-- for now, delay_duration is set to 1 ms so it has no practical effect
        //-- everything will be reset afterwards
        //-- get random port will be true to decide which port to activate (L OR R)
#include "push_button.h"
#include "l_port_operate.h" //-- controls reward cue, reward duration and ITI window
    //-- activate_left_led:
        //-- if M solenoid is not active: if it's a valid trial and L led is off --> turn on L LED
        //-- if it's a valid trial, L light is on, there was a poke in L and left port counter is not 0
            //-- end valid trial window (make it false), turn off all lights, write down the time
            //-- make reward_window true
            //-- reward_type 1: give reward (if it is 2, ITI will begin)
            //-- vs P4: deletes the part that put the number of in this window L,R, and M pokes in the CSV file
        //-- if valid_trial and L LED is on and middle port counter is not 0 OR right port counter isn't 0
                //and there is poke in middle port OR there is a poke in right port (AKA if there is an invalid poke)
            //-- valid trial window is false, turn off all lights, write down time
            //-- make reward_window true but reward_type 2 (which will start the ITI window)
        //-- if valid_trial and L LED is on and led has been on longer than led_trial_duration (AKA omission)
            //-- valid trial window is false, turn off all lights, write down time
            //-- make reward_window true but reward_type 2 (which will start the ITI window)
    //-- activate_left_sol:
        //-- if left light is off:
            //-- if it's the reward window and reward type is 1, make them false and 0 respectively
            //-- activate middle solenoid, register time solenoid_on_time (will be reward cue start time)
            //-- reset poke counters (leaves solenoid on!), reward_cue_window is true and M led is turned on
        //-- if middle solenoid is on and it has been more than solenoid_on_duration:
            //-- turn off middle solenoid, register when
        //-- if reward_cue_window, middle light is on and it has been more than reward_cue_duration:
            //-- make reward_cue_window false, turn off middle LED
            //-- print when this happened, make start_iti_window true (which will make IR.h run)
            //-- put the number of L,R,M pokes that happened during this window, reset counters
        //-- if reward_window and reward type is 2:
            //-- make them false and 0 respectively
            //-- make solenoid_activate false and start_iti true
            //-- reset poke counters
        //-- if start_iti_window is true: (which happens after reward has been dispensed OR reward type is 2)
            //-- if the current time exceeds ITI window time:
                //-- put the number of pokes during ITI window into the CSV file
                //-- get a new ITI length
                //-- reset counters and current port (make it 0) so everything is reset
                //-- make initiate_new_trial true (begin_trial.h will run), iti_window false
#include "r_port_operate.h"
    //-- same as l_port_operate but replace left port with right port
String paradigm = "G5_P5_v2";

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
    Serial.println(paradigm);

    Serial.print("Trial_Duration:"); //-- prints how long each trial can be
    Serial.println(led_trial_duration);
    Serial.print("Delay_Window:");
    Serial.println(delay_duration);

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

    if (current_port == 1) {
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

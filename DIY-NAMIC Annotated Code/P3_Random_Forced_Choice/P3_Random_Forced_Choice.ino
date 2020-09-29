 /*--------------------------------------------------------++
||              NEW_P3 (both lights activation)            ||
++--------------------------------------------------------*/



/*
 * 1/15/2020 Update: Included # of incorrect poke counts during trial window
 * (when reponse ports light up)
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
 *  ## CHANGED IN G5 (group 5)
 * NOTES::: automatically subtracts one from the final count for relevant windows!!
 *     1) incorrect pokes before trial initiation
 *     2) Pokes during the trial window
 * (CODE: xx29)
 * (CODE: xx89) --> currently (2/14/20) not included in analysis
 */


//-- vs P2: add trial initiation
#include "globals_main.h"
    //--initiates the variables needed for the other functions
    //-- added transition_to_reward: used to transition to reward window
#include "globals_button.h"
#include "globals_time.h"
    //-- iti_intervals: gives the range of numbers ITI length can be randomly chosen from
    //-- solenoid_on_duration: determines how much reward will be give [I WILL NEED TO CHANGE THIS]
    //-- reward cue duration: how long the rewards cue will be on [I MIGHT NEED TO CHANGE THIS]
    //-- delay duration = 1 --> I don't know what this refers to
    //-- LED trail duration: determines how long the LED will be on
    //-- m_LED_on/off_time: determines the blinking duration during trial initation
#include "random_generator.h"
     //-- same as P1 & P2
    //-- generates random iti index # to be selected
    //-- generates random port code to be selected
#include "IR.h"
    //-- poke in L or R port --> will register as poke in if light was off, register as poke out if light was on
    //-- poke in middle port --> will register as poke out if light was on; if light was off:
        //-- count the number of pokes in all ports until the middle port is poked
            //-- by reversing what counts as "valid" during the trial window, we can count the pokes using this program
        //-- end trial initiation window code, start trial window code
    //-- unlike in P2, this function won't change LED status
#include "begin_trial.h"
    //-- vs P2: add trial_initiation_window start and end time instead of trial_window_time
    //-- after iti window, the parent if will become True
    //-- if it's neither the delay, initiation or trial windows at that time, turn on initiation window
        //-- the info for this will come from the prev iteration of this code + IR.h (initiation = true IF M poke)
    //-- write down when the trial was initiated in the CSV file
    //-- since trial_initiation_window is now true, turn on LED if off, on if off (appears as blinking!)
    //-- once there is a poke in the middle port, valid_trial_window will be true so we will move to the trial part of the code
    //-- all lights will turn off, time will be recorded, initiate delay will be true
    //-- for now, delay_duration is set a 1 ms so it has no practical effect
    //-- everything will be reset afterwards
    //-- get random port will be true to decide which port to activate (in this paradigm this has no practical effect)
#include "push_button.h"
#include "m_port_operate.h"
    //-- activate_middle_led:
        //-- if solenoid is not active, if it's a valid trial and both L and R LED are off --> turn on L and R LED
        //-- if it's a valid trial, both lights are on, there was a poke in L and left port counter is not 0
            //-- end valid trial window (make it false), turn of all lights, write down the time
            //-- put the number of L,R, and M pokes in the CSV file; make reward_window true
            //-- reward_type 1: give reward (if it is 2, ITI will begin)
        //-- same process is followed if there was a poke in R and right port counter is not 0
    //-- activate_middle_sol:
        //-- if left light is off:
            //-- if it's the reward window and reward type is 1, make them false and 0 respectively
            //-- activate middle solenoid, register time solenoid_on_time (will be reward cue start time)
            //-- reset poke counters (leaves solenoid on!), reward_cue_window is true and M led is turned on
        //-- if middle solenoid is on and it has been more than solenoid_on_duration:
            //-- turn off middle solenoid, register when
        //-- if reward_cue_window, middle light is on and it has been more than solenoid_on_duration:
            //-- make reward_cue_window false, turn of middle LED
            //-- print when this happened, make start_iti_window true (which will make IR.h run)
            //-- put the number of L,R,M pokes that happened during this window, reset counters
        //-- if start_iti_window is true: (which happens after reward has been dispensed)
            //-- if the current time exceeds ITI window time:
                //-- put the number of pokes during ITI window into the CSV file
                //-- get a new ITI length
                //-- reset counters and current port (make it 0) so everything is reset
                //-- make initiate_new_trial true (begin_trial.h will run), iti_window false

String paradigm = "G5_P3_v3";

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

    if (current_port == 55) { //-- this will be true only after a valid poke during the trial window
      // delay(1);
      activate_middle_led();
      activate_middle_sol();

      }

}

// * * * * * * * * * * * L O O P * * * * * * * * * * * * * * * *

/*--------------------------------------------------------++
||                          NEW_P2                        ||
++--------------------------------------------------------*/



/*
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
 *
 * COPYRIGHT: JHL/ Nautiyal Lab
 *            + Open Source Arduino Forum
 *
 *  ## CHANGED IN G5 (group 5)
 * NOTES::: automatically subtracts one from the final count for relevant windows!! (ex: incorrect pokes before trial initiation and etc.)
 * (CODE: xx29)
 * Remember for Group2, subtract from LEFT or RIGHT port
 */


#include "globals_main.h"
     //--initiates the variables needed for the other functions
#include "globals_button.h"
#include "globals_time.h"
    //-- iti_intervals: gives the range of numbers ITI length can be randomly chosen from
    //-- solenoid_on_duration: determines how much reward will be give [I WILL NEED TO CHANGE THIS]
    //-- reward cue duration: how long the rewards cue will be on [I MIGHT NEED TO CHANGE THIS]
#include "random_generator.h"
    //-- same as P1s code
    //-- generates random iti index # to be selected
    //-- generates random port code to be selected
#include "IR.h"
    //-- determines how the pokes get written into the raw files (translate pokes to data points)
  //-- pokes in M port doesn't initiate trial_window (but will be counted)
  //-- pokes in L or R ports will start trial window
      //-- turns off L and R LED, end trial window and starts reward window
      //-- registers when the trial has ended on the CSV file
    //--always gets middle port from the random port generator because that's where the reward will be
#include "begin_trial.h"
    //-- checks if both trial and reward windows are currently off
    //-- if so, starts trial_window & turns on L and R LED
    //-- prints time stamp for trial initiation
    //-- vs P1: turns on both L and R light, not just the middle one
#include "push_button.h"
    //-- same as P1s code
    //-- I am thinking this might actually be for testing the paradigm and boxes purposes
    //-- might be related to the button attached to the arduino hardware
#include "m_port_operate.h"
    //-- checks if L/R LEDs are off, it's not ITI window and reward cue hasn't been initiated yet
    //-- then turns on middle light + activates middle solenoid
    //-- prints when the solenoid was activated (used to count reward amount) + LEDs activated
    //-- once enough reward is dispensed, turns of solenoid and prints the time for that
    //-- once the reward cue has been on long enough, it will
        //-- solenoid and LED on durations are coded separately --> reward cue can be longer than reward duration!!
    //-- starts the ITI window for random interval
    //-- prints every poke during this time to the CSV file
    //-- resets all parameters and variables as needed
    //-- vs P1: checks if L and R LEDs are off (will be done in IR.h) instead of M
String paradigm = "G5_P2";

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
    Serial.print("Paradigm: ");
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

void loop() { //-- this will keep the code running until the program is ended

    // activate_button();
    record_IR(); //-- call IR.h
    begin_new_trial(); //-- call begin_trial.h

    // Once begin_new_trial finishes running, should be right HERE!
    // Next Step is to pick the activation port!!!

    // NEED TO RESET current_port AT ONE POINT IN CODE,
    // SO THAT below if statements don't get evaluated!!!
    // (otherwise, current_port will always be either 1 or 2 --> leading to unexpected results!)

    if (current_port == 55) {
      // delay(1);
      activate_middle_port(); //--call m_port_operate

      }

}

// * * * * * * * * * * * L O O P * * * * * * * * * * * * * * * *

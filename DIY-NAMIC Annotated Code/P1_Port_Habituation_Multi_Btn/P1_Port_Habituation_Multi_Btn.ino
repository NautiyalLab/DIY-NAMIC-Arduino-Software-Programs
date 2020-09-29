/*--------------------------------------------------------++
||                          NEW_P1                        ||
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
 *
 *  ## CHANGED IN G5 (group 5)
 * NOTES::: automatically subtracts one from the final count for relevant windows!! (ex: incorrect pokes before trial initiation and etc.)
 * (CODE: xx29)
 *
 *
 *
 */


#include "globals_main.h"
    /* //--initiates the variables needed for the other functions */
#include "globals_button.h"
    /* //-- not sure about this/*
    /* //-- probably determines how led and solenoid activation works */
#include "globals_time.h"
    /* //-- iti_intervals: gives the range of numbers ITI length can be randomly chosen from */
    /* //-- solenoid_on_duration: determines how much reward will be give [I WILL NEED TO CHANGE THIS] */
    /* //-- reward cue duration: how long the rewards cue will be on [I MIGHT NEED TO CHANGE THIS] */
#include "random_generator.h"
    /* //-- random_port --> selects a random port; 1: Left, 2: Right; 55: middle
    /* //-- random_iti --> will randomize ITI length */
#include "IR.h"
    /* //-- determines how the pokes get written into the raw files (translate pokes to data points)
    /* //-- prints out the number of pokes during the pre-trial phase --> logs in number of pokes in the l and r
   /* //-- pokes in L or R port doesn't initiate trial_window (but will be counted)
  /* //-- pokes in M will start trial window
      /* //-- turns of M LED, end trial window and starts reward window
      /* //-- registers when the trial has ended on the CSV file */
#include "begin_trial.h"
  /* //-- checks if iti ended
  //-- starts trials window by turning on the middle light
    //-- by making led_state_M HIGH */
#include "push_button.h"
  /* //-- NOT SURE ABOUT THIS ONE
  /* //--might be actually turning on and off the LEDs and solenoids */
#include "m_port_operate.h"
  /* //-- turn on middle port light, activate Middle solenoid
  /* //-- logs these into the raw file
  /* //-- then turns off solenoid
  /* //-- then turns off reward cue after reward_cue_duration is reached
  /* //-- then starts ITI window for a random (av. 45s) duration
  /* //-- counts the pokes during the ITI window
  /* //-- resets everything, so when this code is run again it will follow the same steps */

String paradigm = "G5_P1_vF";

// * * * * * * * * * * * S E T U P * * * * * * * * * * * * * * *

void setup(){

    // randomize the start of the random generator
    randomSeed(analogRead(0));

    pinMode (button_pin, INPUT);

    // solenoids initialization
    for (byte i = 4; i <=6; i++) { pinMode(i, OUTPUT);}    //--solenoids start

    // LED initialization                                   //--LEDs start
    for (byte i = 8; i <=10; i++) { pinMode(i, OUTPUT);}

    // IRs initialization                                    //--IRs start
    for (byte i = 11; i <=13; i++) { pinMode(i, INPUT);}
    // pinMode (9, INPUT);  // modified pin #s
    // pinMode (11, INPUT); // modified pin #s
    // pinMode (13, INPUT); // modified pin #s



    Serial.begin(115200);
    Serial.print("Paradigm: "); /* //-- what prints onto the desktop when you start the program */
    Serial.println(paradigm);

    // initialize initiate_trial_window to true to start program  //--this will initiate trial window
    initiate_new_trial = true;

    // initiate random_idx to pick the first idx for Inter-Trial Interval  //--determine how long the ITI window will be
    random_idx = random_iti();

    // Serial.print("idx: ");
    // Serial.println(random_idx);
    // Serial.println(iti_interval[random_idx]);
}

// * * * * * * * * * * * S E T U P * * * * * * * * * * * * * * *

// * * * * * * * * * * * L O O P * * * * * * * * * * * * * * * *

void loop() {

    // activate_button();
    record_IR(); //-- IR numbers are printed --> so show if they are initialized
    begin_new_trial(); //--trial will begin
            //-- from begin trial. h: checks if iti ended + starts trials window by turning on the middle light
    // Once begin_new_trial finishes running, should be right HERE!
    // Next Step is to pick the activation port!!!

    // NEED TO RESET current_port AT ONE POINT IN CODE,
    // SO THAT below if statements don't get evaluated!!!
    // (otherwise, current_port will always be either 1 or 2 --> leading to unexpected results!)

    if (current_port == 55) { //--turn on the middle port light and log in the pokes
      // delay(1); //-- I MIGHT NEED TO CHANGE THIS IN LATER PARADIGMS
      activate_middle_port(); //-- from m_port_operate.h
      }

}

// * * * * * * * * * * * L O O P * * * * * * * * * * * * * * * *

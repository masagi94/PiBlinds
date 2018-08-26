#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
 
 
// The wiringPi pins the stepper motor  is connected to
#define IN1 0
#define IN2 1
#define IN3 2
#define IN4 3
 
// Mode is used to track the current stepper mode.
int mode = 0;


// Helper functions
void configure();
void fullStepMode();
void reverseFullStepMode();
void setStep();


 

// The main execution of the code
int main(int argc, char *argv[]){
    wiringPiSetup();
 
    // Sets up the signals and buttons for use
    configure();
 
    // When initialize wiring failed, print message to screen
    if(wiringPiSetup() == -1){
        printf("setup wiringPi failed !\n");
        return -1;
    }
    
    int result;

    char check[10];
    
    strcpy(check, "close");
    if(argc != 0){
        result = strcmp(check, argv[1]);
    }

    if(result == 0){
        printf("closing...\n");
    }
    else{
        printf("opening...\n");
    }
    
    int i = 0;
    while(i < 2250){
        
        if(result == 0){
            fullStepMode();
        }
        else{
            reverseFullStepMode();
        }

        i++;
    }

    setStep(0,0,0,0);


    printf("done\n");
    
    return 0;
}
 
 
 
// Configures the signals and buttons for use. The buttons are connected
// to pull-up resistors to avoid floating values, since they all connect to ground.
void configure(){
    
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
 
    // use pull-down resistors for the  motor driver
    pullUpDnControl(IN1, PUD_DOWN);
    pullUpDnControl(IN2, PUD_DOWN);
    pullUpDnControl(IN3, PUD_DOWN);
    pullUpDnControl(IN4, PUD_DOWN);

}
  
void fullStepMode(){
 
    int step = 0;
	if (step==0){
        setStep(1,1,0,0);
	   step++;
	}
	if (step==1){
        setStep(0,1,1,0);
        step++;
	}
	if (step==2){
        setStep(0,0,1,1);
        step++;
	}
	if (step==3){
        setStep(1,0,0,1);
        step=0;
    }
}
 
void reverseFullStepMode(){
 
    int step = 0;
    if (step==0){
        setStep(1,1,0,0);
        step++;
    }
    if (step==1){
        setStep(1,0,0,1);
        step++;
    }
    if (step==2){
        setStep(0,0,1,1);
        step++;
    }
    if (step==3){
        setStep(0,1,1,0);
        step=0;
    }
}
 
// This helper method sets the step on the stepper motor
void setStep(int i1, int i2, int i3, int i4){
    digitalWrite(IN1, i1);
    digitalWrite(IN2, i2);
    digitalWrite(IN3, i3);
    digitalWrite(IN4, i4);
    usleep(1750);
}

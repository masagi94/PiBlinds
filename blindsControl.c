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
 
// BlindStatus is used to track the current state of the blinds. 0 is closed, 1 is open.
int blindStatus = 0;


// Helper functions
void configure();
void fullStepMode();
void reverseFullStepMode();
void setStep();
void checkFile();


 

// The main execution of the code
//int main(int argc, char *argv[]){
int main(){
    wiringPiSetup();
 
    // Sets up the signals and buttons for use
    configure();
 
    // When initialize wiring failed, print message to screen
    if(wiringPiSetup() == -1){
        printf("setup wiringPi failed !\n");
        return -1;
    }
    

    checkFile();


    // int result;

    // char check[10];
    
    // strcpy(check, "close");
    // if(argc != 0){
    //     result = strcmp(check, argv[1]);
    // }

    // if(result == 0){
    //     //printf("closing...\n");
    // }
    // else{
    //     //printf("opening...\n");
    // }
    
    int i = 0;


    while(i < 2250){
        
        if(blindStatus == 0){
            //printf("Blinds closed. Opening...\n");
            reverseFullStepMode();
        }
        else if(blindStatus == 1){
            //printf("Blinds open. Closing...\n");
            fullStepMode();
        }

        i++;
    }

    setStep(0,0,0,0);


    
    return 0;
}


/** 
    This method reads and writes to the blindStatus.txt file. The file contains the 
    current status of the blinds; open or closed. If the blinds are open, then the program
    will close them. If the blinds are closed, they will open. 
*/

void checkFile(){
    FILE *fp;

    char buff[50];
    fp = fopen("/home/pi/Desktop/blindStatus.txt", "r");
    fscanf(fp, "%s", buff);
    printf("%s\n", buff);
    fclose(fp);

    fp = fopen("/home/pi/Desktop/blindStatus.txt", "w");
    int result;
    result = strcmp("open", buff);

    if(result == 0){
        blindStatus = 1;
        fputs("close\n",fp);
    }
    else{
        blindStatus = 0;
        fputs("open\n",fp);
    }
    fclose(fp);

    printf("%i\n", blindStatus);

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


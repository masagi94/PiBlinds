#include <wiringPi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
 
 
// The wiringPi pins the stepper motor  is connected to
#define IN1 23
#define IN2 24
#define IN3 25
#define IN4 28

#define IN5 8
#define IN6 9
#define IN7 7
#define IN8 15

#define IN9 0
#define IN10 1
#define IN11 2
#define IN12 3
 
// BlindStatus is used to track the current state of the blinds. 0 is closed, 1 is open.
int blindStatus = 0;
int motorNum = 0;

int blind0State;
int blind1State;
int blind2State;

char stat0[6];
char stat1[6];
char stat2[6];

//FILE *fp1;


// Helper functions
void configure();
void fullStepMode();
void reverseFullStepMode();
void setStep();
void checkFile();
void updateFile();


 

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
    

    int result1;
    int result2;
    int result3;

    char check1[10];
    strcpy(check1, "0");

    char check2[10];
    strcpy(check2, "1");

    
    char check3[10];
    strcpy(check3, "2");

    result1 = strcmp(check1, argv[1]);
    result2 = strcmp(check2, argv[1]);
    result3 = strcmp(check3, argv[1]);

    if(result1 == 0){
        motorNum = 0;
    }
    else if(result2 == 0){
        motorNum = 1;
    }
    else if(result3 == 0){
        motorNum = 2;
    }
    

    checkFile();

    int i = 0;

    printf("Toggling blind\n");
    
    while(i < 2100){
        
        if(blindStatus == 0){
            fullStepMode();
        }
        else if(blindStatus == 1){      
            reverseFullStepMode();
        }

        i++;
    }

    setStep(0,0,0,0);


    updateFile();
    
    return 0;
}


/** 
    This method reads and writes to the blindStatus.txt file. The file contains the 
    current status of the blinds; open or closed. If the blinds are open, then the program
    will close them. If the blinds are closed, they will open. 
*/

void checkFile(){
    FILE *fp, *fp1;
    
    int result0;
    int result1;
    int result2;


    int run0;
    int run1;
    int run2;


    char buff[25];

    fp = fopen("/home/pi/Desktop/blindStatus.txt", "r");
    fscanf(fp, "%s", buff);
    strcpy(stat0, buff);
    fscanf(fp, "%s", buff);
    strcpy(stat1, buff);
    fscanf(fp, "%s", buff);
    strcpy(stat2, buff);


    blind0State = strcmp("closed", stat0);
    blind1State = strcmp("closed", stat1);
    blind2State = strcmp("closed", stat2);

    run0 = strcmp("running", stat0);
    run1 = strcmp("running", stat1);
    run2 = strcmp("running", stat2);

    fclose(fp);

    if (motorNum == 0 && access("b0running.txt", F_OK) != -1){
        printf("MOTOR 0 ALREADY RUNNING... EXITING...\n");
        exit(0);
    }
    else if (motorNum == 1 && access("b1running.txt", F_OK) != -1){
        printf("MOTOR 1 ALREADY RUNNING... EXITING...\n");
        exit(0);
    }
    else if (motorNum == 2 && access("b2running.txt", F_OK) != -1){
        printf("MOTOR 2 ALREADY RUNNING... EXITING...\n");
        exit(0);
    }



    // This will clear the file, so we may write the new status of the blinds to it
    fp = fopen("/home/pi/Desktop/blindStatus.txt", "w");
    
    printf("%i\n", motorNum);

    // If motor 0 is activated
    if(motorNum == 0){
        fp1 = fopen("b0running.txt", "ab+");

        if(blind0State == 0){
            blindStatus = 1;
            fputs("open\n",fp);
        }
        else{
            blindStatus = 0;
            fputs("closed\n",fp);
        }

        fputs(stat1,fp);
        fputs("\n",fp);
        fputs(stat2,fp);
    }
    else if(motorNum == 1){

        fp1 = fopen("b1running.txt", "ab+");

        fputs(stat0,fp);
        fputs("\n",fp);
        
        if(blind1State == 0){
            blindStatus = 1;
            fputs("open\n",fp);
        }
        else{
            blindStatus = 0;
            fputs("closed\n",fp);
        }

        fputs(stat2,fp);
    }
    else{

        fp1 = fopen("b2running.txt", "ab+");

        fputs(stat0,fp);
        fputs("\n",fp);
        fputs(stat1,fp);
        fputs("\n",fp);

        if(blind2State == 0){
            blindStatus = 1;
            fputs("open\n",fp);
        }
        else{
            blindStatus = 0;
            fputs("closed\n",fp);
        }
    }

    fclose(fp1);

    fclose(fp);

}

void updateFile(){
    if (motorNum == 0){
        remove("b0running.txt");
    }
    else if (motorNum == 1){
        remove("b1running.txt");
    }
    else if (motorNum == 2){
        remove("b2running.txt");
    }
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


    pinMode(IN5, OUTPUT);
    pinMode(IN6, OUTPUT);
    pinMode(IN7, OUTPUT);
    pinMode(IN8, OUTPUT);
 
    pullUpDnControl(IN5, PUD_DOWN);
    pullUpDnControl(IN6, PUD_DOWN);
    pullUpDnControl(IN7, PUD_DOWN);
    pullUpDnControl(IN8, PUD_DOWN);

    pinMode(IN9, OUTPUT);
    pinMode(IN10, OUTPUT);
    pinMode(IN11, OUTPUT);
    pinMode(IN12, OUTPUT);
 
    pullUpDnControl(IN9, PUD_DOWN);
    pullUpDnControl(IN10, PUD_DOWN);
    pullUpDnControl(IN11, PUD_DOWN);
    pullUpDnControl(IN12, PUD_DOWN);

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
    if(motorNum == 0){
        digitalWrite(IN1, i1);
        digitalWrite(IN2, i2);
        digitalWrite(IN3, i3);
        digitalWrite(IN4, i4);
    }
    else if(motorNum == 1){
        digitalWrite(IN5, i1);
        digitalWrite(IN6, i2);
        digitalWrite(IN7, i3);
        digitalWrite(IN8, i4);
    }
    else if(motorNum == 2){
        digitalWrite(IN9, i1);
        digitalWrite(IN10, i2);
        digitalWrite(IN11, i3);
        digitalWrite(IN12, i4);
    }


    usleep(3000);
}


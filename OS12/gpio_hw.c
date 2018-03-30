#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define LED_R   3
#define LED_Y   2
#define LED_G   0
#define SW_R    6
#define SW_Y    5
#define SW_G    4
#define SW_W    27

void init(void);
void off(void);
void blink(void);
int chkAnswer(void);
void failure(void);
void allview(void);
void RYG(void);
void input1(void);
void input2(void);
void input3(void);
sem_t sem1;
sem_t sem2;
sem_t sem3;
sem_t sem4;
int a[5] = {0, 0, 0, 0, 0};
int b[5] = {0, 0, 0, 0, 0};
int roundn = 1;
int num=0;

int main(void)
{
    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 1);
    sem_init(&sem3, 0, 1);
    sem_init(&sem4, 0, 1);
    sem_wait(&sem1);
    sem_wait(&sem2);
    sem_wait(&sem3);
    pthread_t   t1;
    pthread_t   t2;
    pthread_t   t3;
    pthread_t   t4;
    srand((int)time(NULL));
    init();
    for(num=0;num<5;num++){
        b[num]=(rand()%3)+1;
    }
    num=0;
    sleep(1);
    allview();
    pthread_create(&t1, NULL, (void *)&input1, NULL);
    pthread_create(&t2, NULL, (void *)&input2, NULL);
    pthread_create(&t3, NULL, (void *)&input3, NULL);
    pthread_create(&t4, NULL, (void *)&RYG, NULL);
    while(roundn<6)
    {
        
        if(digitalRead(SW_W) == 0)
        {
            if(chkAnswer()==1)
            {
                roundn++;
                if(roundn==6){
                    break;
                }
            }else{
                exit(1);
            }
            allview();
            for(num=0;num<5;num++){
                a[num]=0;
            }
            num=0;
            delay(250);
        }
    }
    blink();
    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);
    sem_destroy(&sem4);
    return 0;
}

void init(void)
{
    if(wiringPiSetup() == -1)
    {
        exit(1);
    }

    pinMode(SW_R, INPUT);
    pinMode(SW_Y, INPUT);
    pinMode(SW_G, INPUT);
    pinMode(SW_W, INPUT);
    pinMode(LED_R, OUTPUT);
    pinMode(LED_Y, OUTPUT);
    pinMode(LED_G, OUTPUT);

    off();
    blink();
}

void off(void)
{   
    digitalWrite(LED_R, 0);
    digitalWrite(LED_Y, 0);
    digitalWrite(LED_G, 0);
}

void blink(void)
{   
    int i=0;
    
    while(i<3)
    {
        digitalWrite(LED_R, 1);
        delay(250);
        digitalWrite(LED_R, 0);

        digitalWrite(LED_Y, 1);
        delay(250);
        digitalWrite(LED_Y, 0);

        digitalWrite(LED_G, 1);
        delay(250);
        digitalWrite(LED_G, 0);

        i++;
    }   
}

void failure(void)
{
    int i=0;
    
    while(i<3)
    {
        digitalWrite(LED_R, 1);
        digitalWrite(LED_Y, 1);
        digitalWrite(LED_G, 1);
        delay(250);
        digitalWrite(LED_R, 0);
        digitalWrite(LED_Y, 0);
        digitalWrite(LED_G, 0);
        delay(250);

        i++;
    }
}

int chkAnswer(void)
{
    int i=0;
    while(i<roundn)
    {
        if(a[i]!=b[i])
        {
            failure();
            return 0;
        }
        i++;
    }
    return 1;
}

void allview(void){
    int l=0;
    int i=0;
    while(i<roundn)
    {
        l=b[i];
        if(l==1)
        {
            digitalWrite(LED_R, 1);
        }
        else if(l==2)
        {
            digitalWrite(LED_Y, 1);
        }
        else if(l==3)
        {
            digitalWrite(LED_G, 1);
        }
        delay(250);
        off();
        delay(250);
        i++;
    }
}
void RYG(void){
    while(roundn<6){
        sem_wait(&sem4);
        if(digitalRead(SW_R) == 0)
        {   
            sem_post(&sem1);
            sem_wait(&sem1);
            delay(250);
        }
        else if(digitalRead(SW_Y) == 0)
        {
            sem_post(&sem2);
            sem_wait(&sem2);
            delay(250);
        }
        else if(digitalRead(SW_G) == 0)
        {
            sem_post(&sem3);
            sem_wait(&sem3);
            delay(250);
        }
        sem_post(&sem4);
    }
}
void input1(void){
    while(roundn<6){
        sem_wait(&sem1);
        digitalWrite(LED_R, 1);
        delay(200);
        *(a+num)=1;
        num++;
        off();
        sem_post(&sem1);
        delay(250);
    }
}
void input2(void){
    while(roundn<6){
        sem_wait(&sem2);
        digitalWrite(LED_Y, 1);
        delay(200);
        *(a+num)=2;
        num++;
        off();
        sem_post(&sem2);
        delay(250);
    }
}
void input3(void){
    while(roundn<6){
        sem_wait(&sem3);
        digitalWrite(LED_G, 1);
        delay(200);
        *(a+num)=3;
        num++;
        off();
        sem_post(&sem3);
        delay(250);
    }
}

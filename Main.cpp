
#include <stdio.h>
#include <stdint.h>
#include "PIO_LED.h"
#include "PIO_BUTTON.h"
#include <time.h>
#include "CSpider.h"
#include "CSpiderLeg.h"
#include <iostream>
#include <pthread.h>

void* first_move(void* Sp)//void* first_move(void*)
{
    CSpider* Spider = (CSpider*) Sp;

    Spider->SetSpeed(80);

    //Spider.Reset();

    //Spider.WakeUp();

    std::cout<<"\nMove Forward\n";
    Spider->MoveForward(10);

    Spider->Reset();

    std::cout<<"\nRotate\n";
    Spider->RotatelLeft(11);

    Spider->Reset();

    std::cout<<"\nMove Forward Again\n";
    Spider->MoveForward(10);

    return NULL;
}

void* LEDs(void*)
{
    CPIO_LED LED_PIO;

    uint32_t tickCount = OS_GetTickCount(); 
    int led = 1;
    int frequency = 8;            
        
    while (true)
    {
        if(OS_GetTickCount()-tickCount > OS_TicksPerSecond()/frequency) {
            tickCount = OS_GetTickCount(); 
                led = led << 1;

            if(led > 256) {
                        led = 1;
                }

            LED_PIO.SetLED(led);
                        
        }
    }

    return NULL;
}

int main(int argc, char *argv[]){

    CPIO_LED LED_PIO;
    CPIO_BUTTON BUTTON_PIO;
    CSpider Spider;

    Spider.SetSpeed(100);

    //Spider.Reset();
    std::cout<<"\n\nReset Done\n\n";

    Spider.Abort();
    Spider.Sleep();
    Spider.WakeUp();
    
    printf("Spider Init & Standup\r\n");
    if (!Spider.Init())
    {
        printf("Spilder Init failed\r\n");
    }
    else
    {
        if (!Spider.Standup())
            printf("Spilder Standup failed\r\n");
    }

    printf("Main Funtionality\n");
    while(true)
    {

        if ((BUTTON_PIO.GetBUTTON() & 0x01)== 0) //KEY0 FIRST MOVE
        {
            pthread_t t1, leds;
            pthread_cancel(leds);

            pthread_create(&t1, NULL, &first_move, &Spider); //Move Thread

            pthread_create(&leds, NULL, &LEDs, NULL); //LEDs thread

            //-----Stop LEDs thread
            void* result;
            pthread_join(t1,&result);

            pthread_cancel(leds);
            //---------------------

        }

        if ((BUTTON_PIO.GetBUTTON() & 0x02)== 0) //KEY1
        {
            exit(0); //Key 1 Not implemented yet... Just exit
        }
    }
return 0;
}

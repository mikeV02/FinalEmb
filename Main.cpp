#include <stdio.h>
#include <stdint.h>
#include "PIO_LED.h"
#include "PIO_BUTTON.h"
#include <time.h>
#include "CSpider.h"
#include "CSpiderLeg.h"
#include <iostream>
#include <pthread.h>
#include <unistd.h>

void* first_move(void* Sp)//void* first_move(void*)
{
    CSpider* Spider = (CSpider*) Sp;

    Spider->SetSpeed(80);

    std::cout<<"\nMove Forward\n";
    Spider->MoveForward(20); //10 ft

    Spider->Reset();

    std::cout<<"\nRotate\n"; //MAY VARY ON DIFFERENT SPIDERS.
    Spider->RotatelLeft(8); // Arbitrary value. It makes the spider rorate pretty close 180 degrees.

    Spider->Reset();

    std::cout<<"\nMove Forward Again\n";
    Spider->MoveForward(20); //10 ft
    
    Spider->Reset();

    return NULL;
}

void* second_move(void* Sp)//void* first_move(void*)
{
	bool alternate = true;
	CSpider* Spider = (CSpider*) Sp;
	
	
    Spider->SetSpeed(15);
	Spider->BestDance(1);
	/*
	Spider->Stomp(8);
	std::cout<<"\nReseting\n";
       Spider->Reset();
       //Spider->RotatelRight(2);
       //Spider->RotatelLeft(2);
		for(int i = 0; i < 3; i++){
			Spider->BodyForward();
			Spider->TiltForward();
			Spider->TiltRight();
			Spider->TiltBackward();
			Spider->BodyBackward();
			Spider->TiltLeft();
			Spider->TiltNone();
		}
		Spider->ByeBye(3);
		Spider->Reset();
		
		Spider->SetSpeed(30);
		for(int i=0; i<30; i++){
	
			Spider->RotatelRight(1);
			Spider->MoveParallelL(2);
			//Spider->BodyUpDown(1);
		}		
	   	std::cout<<"\nReseting\n";
		Spider->Reset();
		std::cout<<"\nBefore Wave\n";
		Spider->ByeBye(3);
		std::cout<<"\nReseting\n";
		Spider->Reset();
		std::cout<<"\nAfter Wave\n";
       Spider->usleep(1000000);
       Spider->BodyNone();
       Spider->BodyBackward();
       //Spider->usleep(1000000);
       Spider->BodyNone();
       Spider->DEMO_Rollover();
       Spider->MoveForward(1);
	   std::cout<<"\nReseting\n";
       Spider->Reset();
       Spider->TiltForward();
       Spider->TiltNone();
       Spider->TiltBackward();
       Spider->TiltNone();
       Spider->BodyUpDown(1);
       Spider->MoveBackward(1);
	   std::cout<<"\nReseting\n";
       Spider->Reset();
       Spider->TiltRight();
       Spider->TiltNone();
      Spider->TiltLeft();
       Spider->TiltNone();
       Spider->TiltRight();
       Spider->TiltNone();
       Spider->TiltLeft();
       Spider->TiltNone();
       Spider->ByeBye(3);*/

	return NULL;
}

void* LEDsL(void*)
{

    CPIO_LED LED_PIO;

    uint32_t tickCount = OS_GetTickCount();
    int led = 1;
    int frequency = 8;            
        
    while (true)
    {

        #define handle_error_en(en, msg) \
             do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
        int s, t;

        t = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

        s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        if (s != 0)
            handle_error_en(s, "pthread_setcancelstate");

        if (t != 0)
            handle_error_en(s, "pthread_setcanceltype");

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

void* LEDsR(void*)
{

    CPIO_LED LED_PIO;

    uint32_t tickCount = OS_GetTickCount();
    int led = 1;
    int frequency = 8;            
        
    while (true)
    {

        #define handle_error_en(en, msg) \
             do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
        int s, t;

        t = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

        s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        if (s != 0)
            handle_error_en(s, "pthread_setcancelstate");

        if (t != 0)
            handle_error_en(s, "pthread_setcanceltype");

        if(OS_GetTickCount()-tickCount > OS_TicksPerSecond()/frequency) {
            tickCount = OS_GetTickCount();
                led = led >> 1;

            if(led < 1) {
                        led = 256;
                }

            LED_PIO.SetLED(led);
                        
        }
    }

    return NULL;
}

void* LEDsB(void*)
{

    CPIO_LED LED_PIO;

    uint32_t tickCount = OS_GetTickCount();
    int led;
    int frequency = 16;            
    bool change = true;
    while (true)
    {

        #define handle_error_en(en, msg) \
             do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)
        int s, t;

        t = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

        s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
        if (s != 0)
            handle_error_en(s, "pthread_setcancelstate");

        if (t != 0)
            handle_error_en(s, "pthread_setcanceltype");

        if(OS_GetTickCount()-tickCount > OS_TicksPerSecond()/frequency) {
            tickCount = OS_GetTickCount();
			if(change){
            led = 0;
			change=false;
			}
			else{
				led = 127;
				change=true;
			}
			LED_PIO.SetLED(led);                       
        }
    }
    return NULL;
}

void* exitProg(void* btn)
{
    
    CPIO_BUTTON *BUTTON_PIO = (CPIO_BUTTON*) btn;
    printf("Exit function\r\n");
	sleep(1);
    while((BUTTON_PIO->GetBUTTON() & 0x03) != 0)
    { }
    exit(0);
    return 0;
}

int main(int argc, char *argv[]){

    CPIO_LED LED_PIO;
    CPIO_BUTTON BUTTON_PIO;
    CSpider Spider;
    
    int buttons;
    int prev_buttons;
	unsigned int sleep(int GoToSleep);


    printf("Before exit thread\r\n");
    // Exit Program------------------------------------
    pthread_t Ext;
    pthread_create(&Ext, NULL, &exitProg, &BUTTON_PIO);
    //-------------------------------------------------
    printf("After exit thread\r\n");


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


    Spider.SetSpeed(100);

    pthread_t ledsL;
    pthread_create(&ledsL, NULL, &LEDsL, NULL); //LEDs Left thread
    
    printf("Main Funtionality\n");
    while(true)
    {
        prev_buttons = buttons;
        buttons = BUTTON_PIO.GetBUTTON();

        if ((BUTTON_PIO.GetBUTTON() & 0x01)== 0) //KEY0 FIRST MOVE
        {
            if(buttons != prev_buttons)
            {
                pthread_cancel(ledsL);
                printf("\n\nLEFT LEDs has been Stopped\n\n");
				//sleep(10);
                pthread_t t1, ledsR;

                pthread_create(&t1, NULL, &first_move, &Spider); //Move Thread

                pthread_create(&ledsR, NULL, &LEDsR, NULL); //LEDs Right thread


                //-----Stop LEDs thread
                void* result;
                pthread_join(t1,&result);
                printf("Joining\r\n");
                pthread_cancel(ledsR);
                printf("\n\nLEDs has been Stopped 2\n\n");
                //---------------------
		pthread_create(&ledsL, NULL, &LEDsL, NULL); //LEDs Left thread
            }
        }


        if ((BUTTON_PIO.GetBUTTON() & 0x02)== 0) //KEY1
        {
		if(buttons != prev_buttons)
		    {
			pthread_cancel(ledsL);
			printf("\n\nLEFT LEDs has been Stopped\n\n");
					//sleep(10);
			pthread_t t2, ledsB;

			pthread_create(&t2, NULL, &second_move, &Spider); //Move Thread

			pthread_create(&ledsB, NULL, &LEDsB, NULL); //LEDs Right thread


			//-----Stop LEDs thread
			void* result;
			pthread_join(t2,&result);
			printf("Joining\r\n");
			pthread_cancel(ledsB);
			printf("\n\nLEDs has been Stopped 2\n\n");
			pthread_create(&ledsL, NULL, &LEDsL, NULL); //LEDs Left thread
			//---------------------
		    }
        }
    }
return 0;
}

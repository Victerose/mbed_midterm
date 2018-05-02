#include "mbed.h"
#include "uLCD_4DGL.h"
#include <string> 
#include <sstream>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

Serial pc(USBTX, USBRX);

// vibra
InterruptIn Din(D13);
DigitalOut redLED(LED1);

//uLCD
uLCD_4DGL uLCD(D1, D0, D2);

//**************************************************************************
//Zigbee
//**************************************************************************
Serial xbee(D9, D7);

void reply_messange(char *xbee_reply, char *messange){
    xbee_reply[0] = xbee.getc();
    xbee_reply[1] = xbee.getc();
    xbee_reply[2] = xbee.getc();
    if(xbee_reply[1] == 'O' && xbee_reply[2] == 'K'){
        pc.printf("%s\r\n", messange);
        xbee_reply[0] = '\0';
        xbee_reply[1] = '\0';
        xbee_reply[2] = '\0';
    }
}

void check_addr(char *xbee_reply, char *messenger){
    xbee_reply[0] = xbee.getc();
    xbee_reply[1] = xbee.getc();
    xbee_reply[2] = xbee.getc();
    pc.printf("%s = %c%c\r\n", messenger, xbee_reply[1], xbee_reply[2]);
    xbee_reply[0] = '\0';
    xbee_reply[1] = '\0';
    xbee_reply[2] = '\0';
}

// interrupt when vibra
char sent_word[2] = {'G','G'};

void RLED(){
    xbee.printf("%s", sent_word);
    wait(0.1);
    redLED = 1;
    uLCD.locate(0,2);
    uLCD.printf("Bang~~~~"); //Default Green on black text
    wait(100);
}

//**************************************************************************
// main
//**************************************************************************
int main(){
    pc.baud(9600);

    // interrupt when vibra
    Din.rise(&RLED);

    //*************************************************
    // Zigbee
    //*************************************************
    
    char xbee_reply[3];
    
    // XBee setting
    xbee.baud(9600);
    xbee.printf("+++");
    xbee_reply[0] = xbee.getc();
    xbee_reply[1] = xbee.getc();

    if(xbee_reply[0] == 'O' && xbee_reply[1] == 'K'){
        pc.printf("enter AT mode.\r\n");
        xbee_reply[0] = '\0';
        xbee_reply[1] = '\0';
    }

    xbee.printf("ATMY 0x18\r\n");
    reply_messange(xbee_reply, "setting MY : 0x18");
    
    xbee.printf("ATDL 0x16\r\n");
    reply_messange(xbee_reply, "setting DL : 0x16");

    xbee.printf("ATWR\r\n");
    reply_messange(xbee_reply, "write config");

    xbee.printf("ATMY\r\n");
    check_addr(xbee_reply, "MY");

    xbee.printf("ATDL\r\n");
    check_addr(xbee_reply, "DL");

    xbee.printf("ATCN\r\n");
    reply_messange(xbee_reply, "exit AT mode");

    xbee.getc();
    
    //*************************************************
    int i=0;
    int count_down = 59;

    char buf[100] = {0};

    uLCD.color(RED);
    uLCD.locate(1,2);
    uLCD.printf("\nAre u ready???\n"); //Default Green on black text
    uLCD.text_width(4); //4X size text
    uLCD.text_height(4);


    while(1){
        xbee.getc();
        while (i < 5){
            buf[i] = xbee.getc();
            i++;
            buf[i] = '\0';
        }

        if(buf[0]=='s'){
            redLED = 1;
            wait(0.2);

            while(count_down>0){
                count_down--;
                wait(1);
                uLCD.locate(1,2);
                uLCD.printf("%d",count_down); //Default Green on black text
            }
            uLCD.locate(1,2);
            uLCD.printf("GG~"); //Default Green on black text
        }
        
        redLED = 0;
    }
}

// sudo mbed compile -m K64F -t GCC_ARM -f


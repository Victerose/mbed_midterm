#include "mbed.h"
#include "parallax.h"

Serial pc(USBTX, USBRX);

// constant
int detect_area = 60;

// ping
DigitalInOut pin2(D2); // low ping
DigitalInOut pin3(D3); // high ping
parallax_ping ping_low(pin2); // low ping
parallax_ping ping_high(pin3); // high ping

// moving servo
PwmOut pin13(D13); // pin13 : left conti. servo
PwmOut pin11(D11); // pin11 : right conti. servo

// catcher
PwmOut pin12(D12);

// decide whether see the ball
int ball(float low, float high){
    if (low < 200 && high > 250)
        return 1;
    else   
        return 0;
}


int main() {

    pc.baud(9600);

    servo_ticker.attach(&servo_control, .5);

    parallax_servo catcher(pin12);

    parallax_servo left_servo(pin13);
    parallax_servo right_servo(pin11);
    left_servo.set_ramp(50);
    right_servo.set_ramp(50);

    DigitalOut ping7(D7);
    //printf("Ping = %lf\r\n", (float)ping_low); // see the distance

    while(1) {
        pc.printf("Ping = %lf\r\n", (float)ping_low);
        
        if(ping_low < detect_area){ // see the target
            left_servo = -2; right_servo = 2;
            wait(0.1);
        }
        else{
            left_servo = 20; right_servo = 20;
            wait(0.1);
        }

       // shoter
       ping7 = 0;
       wait(1);
       ping7 = 1;
       wait(1);
       
    }

}

// sudo mbed compile -m K64F -t GCC_ARM -f
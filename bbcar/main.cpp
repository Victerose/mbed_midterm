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

// gun
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

    parallax_servo gun(pin12);
    gun = 500;

    parallax_servo left_servo(pin13);
    left_servo = 0;
    parallax_servo right_servo(pin11);
    right_servo = 0;
    left_servo.set_ramp(50);
    right_servo.set_ramp(50);

    // shotter
    DigitalOut shotter1(D7);
    shotter1 = 1;
    DigitalOut shotter2(D6);
    shotter2 = 1;
    //printf("Ping = %lf\r\n", (float)ping_low); // see the distance

    printf("GOGO");
    wait(1);
    
    while(1) {
        
        printf("Ping = %lf\r\n", (float)ping_low);
        wait(0.2);

        if(ping_low < detect_area){ // see the target
            left_servo = 0; right_servo = 0;
            wait(1);
            left_servo = -10; right_servo = -10;
            wait(0.5);
            shotter1 = 0; shotter2 = 0;
            wait(1);
            gun = 0;
            wait(1);
            printf("finish shotting");
            shotter1 = 1; shotter2 = 1;
            wait(1);
            gun = 500;
            break;
        }
        else{
            left_servo = 20; right_servo = 20;
        }
        printf("looping");
        wait(0.2);
    }

    printf("finish all");
    wait(1);
    return 0;

}

// sudo mbed compile -m K64F -t GCC_ARM -f
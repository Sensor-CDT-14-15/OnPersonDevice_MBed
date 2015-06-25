#include "mbed.h"
#include "GPS.h"


GPS gps(PTE0, PTE1);

DigitalOut led_red(LED_RED);
InterruptIn call(PTD1);
InterruptIn text(PTD3);

Serial pc(USBTX, USBRX);
Serial uart(PTE22, PTE23);

char number[12] = "07962014080";

Idiot test to make sure doesn't get a badge!!!
void call_button(void)
{
    printf("Call button\n");
    uart.printf("ATD07962014080;");   //end of line characters!!!
    wait(0.1);
    printf("start call");
    uart.putc(0x0D);
    wait(20);
    uart.printf("ATH");
    uart.putc(0x0D);
    printf("end call");
}

void text_button(void)
{
    printf("Text Button\n");
    uart.printf("AT+CMGF=1");   //end of line characters!!!
    uart.putc(0x0D);
    wait(0.1);
    while(uart.readable()) {
        pc.putc(uart.getc());
    }
    if(gps.sample()) {
        pc.printf("Seguimi phone notification message sent from GPS location: %f, %f\n",  gps.latitude, gps.longitude);
    } 
else {
        //No GPS lock..pre-determined message with CEB long, lat
        pc.printf("Seguimi phone notification, message sent from GPS location: 52.20, 0.120.\n"); 
    }


    uart.printf("AT+CMGS=\"07962014080\"");   //end of line characters!!!
    uart.putc(0x0D);
    wait(1);
    uart.printf("Hello - message"); //Message string
    wait(1);
    uart.putc(0x0D);
    uart.putc(0x1A);       //Control-Z Send text
    uart.putc(0x0D);
}

int main()
{
    while(1) {
        call.rise(&call_button);
        text.rise(&text_button);
    }

}

#include <SimpleModbusMaster.h>
//#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include <WiFiUdp.h>
//#include <ArduinoOTA.h>
/*
   The example will use packet1 to read a register from address 0 (the adc ch0 value)
   from the arduino slave (id=1). It will then use this value to adjust the brightness
   of an led on pin 9 using PWM.
   It will then use packet2 to write a register (its own adc ch0 value) to address 1 
   on the arduino slave (id=1) adjusting the brightness of an led on pin 9 using PWM.
*/

//////////////////// Port information ///////////////////
#define baud 9600
#define timeout 1000
#define polling 200 // the scan rate
#define retry_count 10

// used to toggle the receive/transmit pin on the driver
#define TxEnablePin 2

#define LED 3
#define LED1 5
#define LED2 6
#define LED3 7
// Khai báo số lượng thanh ghi chứa dữ liệu trong Master
#define TOTAL_NO_OF_REGISTERS 10

enum
{
  READ1,
  READ2,
  WRITE3,
  TOTAL_NO_OF_PACKETS // leave this last entry
};

// Create an array of Packets to be configured
Packet packets[TOTAL_NO_OF_PACKETS];

// Masters register array
unsigned int regs[TOTAL_NO_OF_REGISTERS];

void setup()
{  //Serial.begin(9600);
  pinMode(LED, INPUT);
  pinMode(LED1, INPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
//Serial.begin(115200);
  // Định dạng        tên enum    ,địa chỉ slaver, function  ,Vị trí của enum slaver,số lượng byte đoc, đọc xong lưu vào thanh ghi regs[0]
  modbus_construct(&packets[READ1], 2, READ_HOLDING_REGISTERS, 1, 1, 0);  // Số 0 cuối là dữ liệu được lưu vào thanh ghi regs[0],số 0 đầu là vị trí bắt đầu docdjw,số 1 là 1 ô nhớ
  modbus_construct(&packets[READ2], 2, READ_HOLDING_REGISTERS, 2, 1, 1); 
  modbus_construct(&packets[WRITE3], 2, PRESET_SINGLE_REGISTER , 0, 1, 2);
  
  // Initialize the Modbus Finite State Machine
  modbus_configure(&Serial, baud, SERIAL_8E1, timeout, polling, retry_count, TxEnablePin, packets, TOTAL_NO_OF_PACKETS, regs);

}
int ii=0;
void loop()
{
  modbus_update();
  ii++;
  if (ii>=2000){
  regs[WRITE3] = 255; // update data to be written to arduino slave
  ii=0;
  delay(50);
  }
  else   if (ii==1000){
  regs[WRITE3] = 0; // update data to be written to arduino slave
  delay(50);
  }
  analogWrite(LED2, regs[READ1]>>2); // constrain adc value from the arduino slave to 255
  analogWrite(LED3, regs[READ2]>>2); // constrain adc value from the arduino slave to 255

}

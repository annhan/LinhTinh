#include <SimpleModbusSlave.h>

#define  LED 10  
#define  LED1 11
// Using the enum instruction allows for an easy method for adding and 
// removing registers. Doing it this way saves you #defining the size 
// of your slaves register array each time you want to add more registers
// and at a glimpse informs you of your slaves register layout.

//////////////// registers of your slave ///////////////////
enum 
{     
  IN_VAL,     
  OUT2_VAL,      
  OUT_VAL,     
  HOLDING_REGS_SIZE // leave this one
  // total number of registers for function 3 and 16 share the same register array
  // i.e. the same address space
};

unsigned int holdingRegs[HOLDING_REGS_SIZE]; // function 3 and 16 register array
////////////////////////////////////////////////////////////

void setup()
{
  /* parameters(HardwareSerial* SerialPort,
                long baudrate, 
		unsigned char byteFormat,
                unsigned char ID, 
                unsigned char transmit enable pin, 
                unsigned int holding registers size,
                unsigned int* holding register array)
  */
  
  /* Valid modbus byte formats are:
     SERIAL_8N2: 1 start bit, 8 data bits, 2 stop bits
     SERIAL_8E1: 1 start bit, 8 data bits, 1 Even parity bit, 1 stop bit
     SERIAL_8O1: 1 start bit, 8 data bits, 1 Odd parity bit, 1 stop bit
     
     You can obviously use SERIAL_8N1 but this does not adhere to the
     Modbus specifications. That said, I have tested the SERIAL_8N1 option 
     on various commercial masters and slaves that were suppose to adhere
     to this specification and was always able to communicate... Go figure.
     
     These byte formats are already defined in the Arduino global name space. 
  */
	
  modbus_configure(&Serial, 115200, SERIAL_8N1, 2, 2, HOLDING_REGS_SIZE, holdingRegs);

  // modbus_update_comms(baud, byteFormat, id) is not needed but allows for easy update of the
  // port variables and slave id dynamically in any function.
  modbus_update_comms(9600, SERIAL_8N1, 2);
  pinMode(LED1, OUTPUT);
  pinMode(LED, OUTPUT);
}
int ii=0;
void loop()
{
  // modbus_update() is the only method used in loop(). It returns the total error
  // count s. You don't have to use it but it's usefulince the slave started                                      
  // for fault finding by the modbus master.
  modbus_update_comms(115200, SERIAL_8N1, 2);
  modbus_update();
  
 // holdingRegs[ADC_VAL] = analogRead(A0); // update data to be read by the master to adjust the PWM
  
  analogWrite(LED, holdingRegs[IN_VAL]>>2); // constrain adc value from the arduino master to 255
 // holdingRegs[OUT_VAL] = holdingRegs[IN_VAL]>>2; // update data to be read by the master to adjust the PWM
 // analogWrite(LED1, holdingRegs[PWM_VAL]>>2); // constrain adc value from the arduino master to 255

    ii++;
  if (ii>=2000){
  holdingRegs[OUT_VAL] = 1023; // update data to be written to arduino slave
  holdingRegs[OUT2_VAL] = 1023; // update data to be written to arduino slave
  ii=0;
  delay(50);
  }
  else   if (ii==1000){
  holdingRegs[OUT_VAL] = 0; // update data to be written to arduino slave
  holdingRegs[OUT2_VAL] = 0; // update data to be written to arduino slave
  delay(50);
  }
}


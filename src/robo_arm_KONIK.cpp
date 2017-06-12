#include "mraa.hpp"
#include "SparkFun_pca9685_Edison.h"
#include "Intel_Edison_BT_SPP.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

#define SERVO1 0
#define SERVO2 1
#define SERVO3 2
#define SERVO4 3
#define SERVO5 4
#define SERVO6 5
#define SERVO_NUM 6

void servoTest(pca9685 pwm);
void moveBricks(pca9685 pwm);
void servoMediumPos(pca9685 pwm);

int16_t servoMinAngle = 0, servoMaxAngle = 170;					//that makes 0,540 ms pulse length (120*4,5us)
uint16_t minServoPL = 120, maxServoPL = 511;					//that makes 2,300 ms pulse length (511*4,5 us)


int main() {

  Intel_Edison_BT_SPP SPP = Intel_Edison_BT_SPP();		// create an INTEL_Edison_BT_SPP object for bluetooth connection

  mraa::I2c* pwm_i2c;   //creating an I2c object for the pca9685 constructor
  pwm_i2c = new mraa::I2c(1); // setting the bus for I2C object

  pca9685 pwm(pwm_i2c, 0x40); // 0x40 is the default address for the PCA9685.

  //checking registers value (AI and ALLCALL in MODE1 register / OUTDRV in MODE2 register works for me)
  cout<<"Current mode register values: "<<pwm.readModeRegisters()<<endl;			//should be uint8_t 1028
  cout<<"Current channel 0 LED0_ON register values: "<<pwm.readChl0RegON()<<endl;
  cout<<"Current channel 0 LED0_OFF register values: "<<pwm.readChl0RegOFF()<<endl;
  cout<<"Current prescaler: "<<dec<<static_cast<int16_t>(pwm.getPrescaler())<<endl;


  pwm.setPrescaler(121);														//set prescaler 121 (~50Hz pwm frequency)
  cout<<"Now prescaler: "<<dec<<static_cast<int16_t>(pwm.getPrescaler())<<endl;	//check whether prescaler is set correctly

  //setting pwm duty on every servo to 0 to avoid collision
  for (uint8_t i=0;i<SERVO_NUM;i++) {
	  pwm.setChlDuty(i, 0);
  }



  //values are set experimentally cause servos are acting strangely sometimes (better check the pulse width on oscilloscope)
  pwm.setServoAnglePulseLimits(minServoPL, maxServoPL);
  pwm.setServoAngleLimits(servoMinAngle, servoMaxAngle);

  /* if you want to check the set values
  pwm.getServoAngleLimits(&servoMinAngle, &servoMaxAngle);
  cout<<"Current servo min angle: "<<dec<<servoMinAngle<<endl;
  cout<<"Current servo max angle: "<<dec<<servoMaxAngle<<endl;
  */

  //do the servo test to check whether everything is okay
  //servoTest(pwm);

  servoMediumPos(pwm);

  //moveBricks(pwm);

  //now it's time for bluetoth connection
  printf("I am opening bluetooth SPP and waiting for connection...\n");
  // Call open function to open BT SPP profile
  int spp_return = SPP.open();

  printf("SPP.open() return value is %d\n",spp_return);


  int16_t tempServo1=90, tempServo2=90, tempServo3=90, tempServo4=90, tempServo5=90, tempServo6=90;
  int16_t interval=10;
  int32_t delay_interval=300000;	//in microseconds


  for(;;)
  {

	  //fast sleep to avoid communication disorder
	  sleep(1);

	  printf("I am ready to read an spp buffer...\n");
	  //checking if it is something to read from connected device
	  //if yes - check the size of sent information
	  ssize_t size = SPP.read();
	  if (size > 0 && size < 32)
	  {
		  cout << "I caught a buffer!" << endl;
		  //get buffer by calling getBuf() function and write it to buf array of chars
		  char* buf = SPP.getBuf();
		  //print incoming data in console
		  for (int i=0;i<size;i++) {
			  cout << buf[i];
		  }

		  //check if buffer is equal to written condition
		  //if yes - change pulse width on specific pwm pin to change the brightness to RGB led
		  if (buf[0] == '1') {
			  for (int16_t i=tempServo1; i<=(tempServo1+interval); i++) {
				  pwm.setChlAngle(SERVO1, i);
				  usleep(delay_interval);
			  }
			  tempServo1 += interval;
		  } else if (buf[0] == '2') {
			  for (int16_t i=tempServo1; i>=(tempServo1-interval); i--) {
				  pwm.setChlAngle(SERVO1, i);
				  usleep(delay_interval);
			  }
			  tempServo1 -= interval;
		  } else if (buf[0] == '3') {
			  for (int16_t i=tempServo2; i<=(tempServo2+interval); i++) {
				  pwm.setChlAngle(SERVO2, i);
				  usleep(delay_interval);
			  }
			  tempServo2 += interval;
		  } else if (buf[0] == '4') {
			  for (int16_t i=tempServo2; i>=(tempServo2-interval); i--) {
				  pwm.setChlAngle(SERVO2, i);
				  usleep(delay_interval);
			  }
			  tempServo2 -= interval;
		  }	else if (buf[0] == '5') {
			  for (int16_t i=tempServo3; i<=(tempServo3+interval); i++) {
				  pwm.setChlAngle(SERVO3, i);
				  usleep(delay_interval);
			  }
			  tempServo3 += interval;
		  } else if (buf[0] == '6') {
			  for (int16_t i=tempServo3; i>=(tempServo3-interval); i--) {
				  pwm.setChlAngle(SERVO3, i);
				  usleep(delay_interval);
			  }
			  tempServo3 -= interval;
		  } else if (buf[0] == '7') {
			  for (int16_t i=tempServo4; i<=(tempServo4+interval); i++) {
				  pwm.setChlAngle(SERVO4, i);
				  usleep(delay_interval);
			  }
			  tempServo4 += interval;
		  }	else if (buf[0] == '8') {
			  for (int16_t i=tempServo4; i>=(tempServo4-interval); i--) {
				  pwm.setChlAngle(SERVO4, i);
				  usleep(delay_interval);
			  }
			  tempServo4 -= interval;
		  } else if (buf[0] == '9') {
			  for (int16_t i=tempServo5; i<=(tempServo5+interval); i++) {
				  pwm.setChlAngle(SERVO5, i);
				  usleep(delay_interval);
			  }
			  tempServo5 += interval;
		  } else if (buf[0] == 'a') {
			  for (int16_t i=tempServo5; i>=(tempServo5-interval); i--) {
				  pwm.setChlAngle(SERVO5, i);
				  usleep(delay_interval);
			  }
			  tempServo5 -= interval;
		  }	else if (buf[0] == 'b') {
			  for (int16_t i=tempServo6; i<=(tempServo6+interval); i++) {
				  pwm.setChlAngle(SERVO6, i);
				  usleep(delay_interval);
			  }
			  tempServo6 += interval;
		  } else if (buf[0] == 'c') {
			  for (int16_t i=tempServo6; i>=(tempServo6-interval); i--) {
				  pwm.setChlAngle(SERVO6, i);
				  usleep(delay_interval);
			  }
			  tempServo6 -= interval;
		  }	else {
			  printf("Unknown command for Robo Arm Led\n");
		  }
	  }
	  cout<<"Servo "<<SERVO1+1<<" position: "<<tempServo1<<endl;
	  cout<<"Servo "<<SERVO2+1<<" position: "<<tempServo2<<endl;
	  cout<<"Servo "<<SERVO3+1<<" position: "<<tempServo3<<endl;
	  cout<<"Servo "<<SERVO4+1<<" position: "<<tempServo4<<endl;
	  cout<<"Servo "<<SERVO5+1<<" position: "<<tempServo5<<endl;
	  cout<<"Servo "<<SERVO6+1<<" position: "<<tempServo6<<endl;

  }
  return MRAA_SUCCESS;
}

void servoTest(pca9685 pwm)
{
	for (int j=0; j < SERVO_NUM; j++) {
		for (int16_t i = servoMinAngle; i <= servoMaxAngle; i+=5)
		{
			pwm.setChlAngle(j, i);
		    cout<<"Servo "<<j+1<< " position: "<<i<<endl;

		    usleep(100000);
		}
		for (int16_t i = servoMaxAngle; i >= servoMinAngle; i-=5)
		{
			pwm.setChlAngle(j, i);
			cout<<"Servo " << j << " position: "<<i<<endl;

			usleep(100000);
		}
	}
	cout<<"Servo test complete!"<<endl;
}

void moveBricks(pca9685 pwm)
{
	pwm.setChlAngle(SERVO1, 90);
	sleep(2);
	pwm.setChlAngle(SERVO2, 50);
	sleep(2);
	pwm.setChlAngle(SERVO3, 140);
	sleep(2);
	pwm.setChlAngle(SERVO4, 140);
	sleep(2);
	pwm.setChlAngle(SERVO5, 170);
	sleep(2);
	pwm.setChlAngle(SERVO6, 90);

	sleep(5);

	pwm.setChlAngle(SERVO1, 130);
	sleep(2);
	pwm.setChlAngle(SERVO2, 80);
	sleep(2);
	pwm.setChlAngle(SERVO3, 130);
	sleep(2);
	pwm.setChlAngle(SERVO4, 160);
	sleep(2);
	pwm.setChlAngle(SERVO5, 170);
	sleep(2);
	pwm.setChlAngle(SERVO6, 60);

	sleep(10);
}

void servoMediumPos(pca9685 pwm) {
	for (int i=0;i<SERVO_NUM;i++) {
		pwm.setChlAngle(i, 90);
		cout<<"Servo "<<i+1<<" position 90"<<endl;
		sleep(3);
	}
	cout<<"Setting all servos at 90 position done"<<endl;
}

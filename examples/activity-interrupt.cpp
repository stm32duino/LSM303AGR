/*
	# Activity interrupt
	Demonstration of the activity/inactivity feature
	being used to generate an interrupt
*/

#include <Arduino.h>
#include <Wire.h>
#include <LSM303AGR_ACC_Sensor.h>

#if defined(ARDUINO_SAM_DUE)
#define DEV_I2C Wire1   //Define which I2C bus is used. Wire1 for the Arduino Due
#define SerialPort Serial
#else
#define DEV_I2C Wire    
#define SerialPort Serial
#endif

#define INT2_PIN 6
LSM303AGR_ACC_Sensor *Acc;

volatile bool hit = false;

void ISR()
{
	hit = true;
}

void setup()
{
	DEV_I2C.begin();
	SerialPort.begin(115200);	  
	Acc = new LSM303AGR_ACC_Sensor(&DEV_I2C);
	
	Acc->Init();
	Acc->Enable();

	// Set a threshold of 500mg for 1000ms
	Acc->SetActivityDuration(1000);
	Acc->SetActivityThreshold(500.0f);	

	// Enable the activity interrupt
	// Disable when not needed with DisableActivityInterrupt
	Acc->EnableActivityInterrupt();	
	
	// Attach interrupt connected to INT2 pin of LSM303
	attachInterrupt(digitalPinToInterrupt(INT2_PIN), ISR, RISING);
}
void loop()
{
	// Single hit
	if (hit)
	{
		SerialPort.println("Interrupt Hit!");
		hit = false;
	}
}
/*
	# Wake-up detection

	Demonstrates the inertial wake-up detection feature.
	You can enable detection, switch the device to low-power mode,
	set up a wakeup interrupt for your MCU and go to sleep.

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

#define INT1_PIN 7
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
	
	// Enable inertial wake-up for 500mg
	Acc->EnableWakeUpDetection(500.0f);

	// Set low power mode
	Acc->SetPowerMode(LSM303AGR_ACC_MODE_LOW_POWER);

	// Attach interrupt to pin connected to INT1 of device
	attachInterrupt(digitalPinToInterrupt(INT1_PIN), ISR, RISING);
}
void loop()
{
	if (hit)
	{
		SerialPort.print(millis());
		SerialPort.println(": Inertial Wake Up!");
		hit = false;
		//Go To sleep
	}
}
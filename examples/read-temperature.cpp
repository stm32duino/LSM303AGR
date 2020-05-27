/*
	# Read Temperature
	Demonstration of the onboard temperature sensor.
	
	The temperature sensor can be enabled and read back
	to get an idea of the board temperature.
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

LSM303AGR_ACC_Sensor *Acc;

void setup()
{
	
	DEV_I2C.begin();
	SerialPort.begin(115200);	  
	Acc = new LSM303AGR_ACC_Sensor(&DEV_I2C);

	Acc->Enable();

	// Enable the temp. sensor
	Acc->EnableTemperatureSensor();
	
	// Use Acc->DisableTemperatureSensor();
	// to disable after use
}

void loop()
{
	float temp = 0;
	if (Acc->GetTemperature(&temp) == LSM303AGR_ACC_STATUS_OK) // read successfully
	{
		SerialPort.print("Temperature : ");
		SerialPort.println(temp);
	}
	else
	{
		SerialPort.println("Failed to read temperature");
	}

	delay(500);	
}
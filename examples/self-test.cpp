/*
	# Self-test 
	
	The LSM303 has an onboard self-test feature
	which allows you to check if the MEMS elements are damaged
	by simulating a proof mass with an electrostatic force.

	For details on the steps, and the theory behind it 
	please refer to the application notes.

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


// Get average Linear acceleration in all 3 axes
bool GetAverageAcc(float arr[3])
{
	// Take the average of five readings, 
	// but discard the first reading

	for (int i = 0; i < 6; i++)
	{
		int32_t acc[3] = { 0 };

		// Check for new X,Y,Z data available.
		bool data_available = false;

		// Wait until data is available to do 
		// a read, otherwise you risk reading stale data
		while (!data_available)
		{
			if (Acc->IsDataAvailable(&data_available, LSM303AGR_ACC_AXIS_ALL) != LSM303AGR_ACC_STATUS_OK)
			{
				// failing to read a status probably means the device is unresponsive
			// or dead. Abort the test.
				return false;
			}
		}
		
		// Read all axis values
		if (Acc->GetAxes(acc) != LSM303AGR_ACC_STATUS_OK)
			return false;
		

		// discard the first reading
		if (i == 0)
			continue;
		
		// average out
		for (int k = 0; k < 3; k++)
			arr[k] += (((float)acc[k]) / 5);
	}

	return true;
}

bool SelfTest(uint8_t mode)
{

	float acc_nost[3] = { 0 };
	float acc_st[3] = { 0 };	
	
	// Recommended test conditions of 100 Hz @ 2g
	Acc->SetODR(100.0f);
	Acc->SetFS(2.0f);

	// Take a normal reading
	Acc->DisableSelfTest();
	
	// Wait for readings to stabilize
	delay(90);
	
	// Take no self-test reading
	if (!GetAverageAcc(acc_nost))
		return false;

	// Enable the self-test mode
	Acc->EnableSelfTest(mode);

	// Wait for readings to stabilize
	delay(90);
	
	// Take the self-test reading
	if (!GetAverageAcc(acc_st))
		return false;
	
	// Check that the difference between
	// both sets of readings fits within a threshold. 
	// See AN

	for (int i = 0; i < 3; i++)
	{
		float diff = abs(acc_st[i] - acc_nost[i]);
		if (!(66.3f <= diff && diff <= 1404.0f))
		{
			return false;
		}
	}
	
	return true;
}

void setup()
{
	
	DEV_I2C.begin();
	SerialPort.begin(115200);	  
	Acc = new LSM303AGR_ACC_Sensor(&DEV_I2C);
	Acc->Init();
	Acc->Enable();

	
	// mode = 0 or 1 denotes direction of movement of proof mass
	if (SelfTest(1) && SelfTest(0)) 
	{
		SerialPort.println("Self-Test was successful");	
	}
	else
	{
		SerialPort.println("Self-Test was not successful. Try again.");
	}

	// Disable test mode
	Acc->DisableSelfTest();
}

void loop()
{
	
}
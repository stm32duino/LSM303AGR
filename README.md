# LSM303AGR
Arduino library to support the LSM303AGR 3D accelerometer and 3D magnetometer

## API

This sensor uses I2C to communicate. It is then required to create a TwoWire interface before accessing the sensors:  

    TwoWire dev_i2c(I2C2_SDA, I2C2_SCL);  
    dev_i2c.begin();  

An instance can be created and enabled following the procedure below:  

    LSM303AGR_ACC_Sensor Acc(&dev_i2c);
    Acc.begin();
    Acc.Enable();
    LSM303AGR_MAG_Sensor Mag(&dev_i2c);
    Mag.begin();
    Mag.Enable();

The access to the sensor values is done as explained below:  

  Read accelerometer and magnetometer.  

    int32_t accelerometer[3];
    int32_t magnetometer[3];
    Acc.GetAxes(accelerometer);
    Mag.GetAxes(magnetometer);  

## Documentation

You can find the source files at  
https://github.com/stm32duino/LSM303AGR

The LSM303AGR datasheet is available at  
http://www.st.com/content/st_com/en/products/mems-and-sensors/e-compasses/lsm303agr.html

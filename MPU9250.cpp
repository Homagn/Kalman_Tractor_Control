#include <Wire.h>
#include "MPU9250.h"


void MPU9250::begin(){
	Wire.begin();
}

void MPU9250::initialize() 
{    
  // MPU = Motion Processing Unit
  // Register information source from the document
  // MPU-9250 Register Map and Descriptions Revision 4.2 18/09/2013
  //
  // Accelerometer - 16bit resolution
  // Gyroscope     - 16bit resolution
  // Magnet        - 14bit resolution access via the auxiliary I2C bus
  //
  // The class intentionally reads one register at a time. Reading all registers together
  // is available in other public classes. 
  //

  // This register allows the user to configure the power mode - page 38
  // 0 = false
  // 1 = true. The MPU can be put in low power sleep mode
  updateBits(_REGISTER_PWR_MGMT_1, BIT_SLEEP, ONE_BIT, 0);

  // This same register allows the user to configure the clock source - page 38
  // 0 = Internal 8MHz oscillator
  // 1 = PLL with X axis gyroscope reference
  // 2 = PLL with Y axis gyroscope reference
  // 3 = PLL with Z axis gyroscope reference
  // 4 = PLL with external 32.768kHz reference
  // 5 = PLL with external 19.2MHz reference
  updateBits(_REGISTER_PWR_MGMT_1, BITS_CLKSEL, THREE_BITS, 1);

  // Gyroscope Configuration - page 12
  // 0 =  250 degrees/second <- highest sensitivity
  // 1 =  500 degrees/second
  // 2 = 1000 degrees/second
  // 3 = 2000 degrees/second
  updateBits(_REGISTER_GYRO_CONFIG, BITS_FS_SEL, TWO_BITS, 0);

  // Accelerometer Configuration - page 13
  // 0 = +/-  2g <- highest sensitivity
  // 1 = +/-  4g
  // 2 = +/-  8g
  // 3 = +/- 16g
  updateBits(_REGISTER_ACCEL_CONFIG, BITS_AFS_SEL, TWO_BITS, 0);

  // Initialise the private variables
  _ax = 0; _ay = 0; _az = 0;
  _gx = 0; _gy = 0; _gz = 0;
  _mx = 0; _my = 0; _mz = 0;

}

byte MPU9250::updateBits(uint8_t registerAddress, uint8_t firstbit, uint8_t length, uint8_t data) 
{
  uint8_t oldByte;
  uint8_t newByte;

  // Read the value in the register
  oldByte = readByte(registerAddress);

  // Return if we have a connection problem
  if (oldByte == 0) return false;

  uint8_t change = ((1 << length) - 1) << (firstbit - length + 1);

  // Shift to the correct position
  data <<= firstbit - length + 1;      
   
  // Zero out all non-changing bits
  data = data & change;                  

  // Zero out all changing bits
  newByte = oldByte & ~change;         
   
  // Combine data
  newByte = newByte | data;              

  // Update the value in the register
  return writeByte(registerAddress, newByte);

}

byte MPU9250::readByte(uint8_t registerAddress)
{
  // Begin communication with MPU9250
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write((uint8_t) registerAddress);

  // End the transmission - keep the i2c connection
  int nackCatcher = Wire.endTransmission(false);

  // Return if we have a connection problem	
  if(nackCatcher != 0){return 0;}

  // Request 1 byte from MPU9250
  Wire.requestFrom(_i2cAddress , _ONE_BYTE);

  // Wait for the byte to arrive
  while(Wire.available() < _ONE_BYTE);

  // Read the values
  return Wire.read();
}

byte MPU9250::writeByte(uint8_t registerAddress, uint8_t value)
{
  // Begin communication with MPU9250
  Wire.beginTransmission(_i2cAddress);

  // The register you want
  Wire.write(registerAddress);

  // Send 1 byte to the MPU9250
  Wire.write(value);

  // End the transmission
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem	
  if(nackCatcher != 0){return 0;}

  // All ok
  return true;
}

//-----------------
//|               |
//|  Accelerator  |
//|               |
//-----------------

int MPU9250::readAcceleroXYZ()
{
  // Begin communication with MPU9250
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(_REGISTER_ACCELERO_X);

  // End the transmission - keep the i2c connection
  int nackCatcher = Wire.endTransmission(false);

  // Return if we have a connection problem	
  if(nackCatcher != 0){return 0;}
	
  // Request 2 bytes from MPU9250
  Wire.requestFrom(_i2cAddress, _SIX_BYTES);

  // Wait for the bytes to arrive
  while(Wire.available() < _SIX_BYTES);

  // Read the X value
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();
  _ax = (_byteHigh<<8) + _byteLow;

  // Read the Y value
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();
  _ay = (_byteHigh<<8) + _byteLow;

  // Read the Z value
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();
  _az = (_byteHigh<<8) + _byteLow;

  // End the transmission
  nackCatcher = Wire.endTransmission();

  // Return
  return nackCatcher;
}

double MPU9250::getAcceleroX()
{
  // Return Accelerometer X
  return ((double)_ax/32767)*2;
}

double MPU9250::getAcceleroY()
{
  // Return Accelerometer Y
  return ((double)_ay/32767)*2;
}

double MPU9250::getAcceleroZ()
{
  // Return Accelerometer Z
  return ((double)_az/32767)*2;
}

//-----------------
//|               |
//|     Gyro      |
//|               |
//-----------------

int MPU9250::readGyroXYZ()
{
  // Begin communication with MPU9250
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(_REGISTER_GYRO_X);

  // End the transmission - keep the i2c connection
  int nackCatcher = Wire.endTransmission(false);

  // Return if we have a connection problem	
  if(nackCatcher != 0){return -1;}

  // Request 2 bytes from MPU9250
  Wire.requestFrom(_i2cAddress, _SIX_BYTES);

  // Wait for the bytes to arrive
  while(Wire.available() < _SIX_BYTES);

  // Read the X value
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();
  _gx = (_byteHigh<<8) + _byteLow;

  // Read the Y value
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();
  _gy = (_byteHigh<<8) + _byteLow;

  // Read the Z value
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();
  _gz = (_byteHigh<<8) + _byteLow;

  // End the transmission
  nackCatcher = Wire.endTransmission();

  // Return
  return nackCatcher;
}

double MPU9250::getGyroX()
{
  // Read GyroX
  return ((double)_gx/32767)*250;
}

double MPU9250::getGyroY()
{
  // Read Gyro Y
  return ((double)_gy/32767)*250;
}

double MPU9250::getGyroZ()
{
  // Return Gyro Z
  return ((double)_gz/32767)*250;
}

//-----------------
//|               |
//|     Magnet    |
//|               |
//-----------------

int MPU9250::readMagnetXYZ()
{
  // Enable Bypass Mode on the i2c Master (for the Magnet)
  // When this bit is equal to 1 and I2C_MST_EN is equal to 0, the host application
  // processor will be able to directly access the auxiliary I2C bus on the MPU-9250
  //
  // If the bypass is set one time, the module may disable bypass and then no information
  // from the magnetic. So best to keep setting the bypass.
  
  // Set I2C_BYPASS_EN to true which is the second bit
  writeByte(_REGISTER_INT_PIN_CFG, 0x02);

  // Wait 10ms for the reconfiguration to work
  delay(10);

  // Begin communication with MPU9250
  Wire.beginTransmission(_i2cAddress_Magnet);

  // The register you want
  Wire.write(_REGISTER_MAGNET_CONTROL);

  // Send 1 byte to the MPU9250
  Wire.write(0x01);

  // End the transmission - keep the i2c connection
  int nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem	
  if(nackCatcher != 0){return 0;}

  // This delay of 10ms is essential for the Magnet to wake up
  // Else 0 is returned as you asked too quickly for the reading
  delay(10);

  // Begin communication with MPU9250
  Wire.beginTransmission(_i2cAddress_Magnet);

  // Tell register you want some data
  Wire.write(_REGISTER_MAGNET_X);

  // End the transmission
  nackCatcher = Wire.endTransmission();

  // Return if we have a connection problem	
  if(nackCatcher != 0){return 0;}

  // Begin communication with MPU9250
  //Wire.beginTransmission(_i2cAddress_Magnet);
	
  // Request 2 bytes from MPU9250
  Wire.requestFrom(_i2cAddress_Magnet, _SEVEN_BYTES);

  // Wait for the bytes to arrive
  while(Wire.available() < _SEVEN_BYTES);

  // Read the X value
  _byteLow = Wire.read();
  _byteHigh = Wire.read(); 
  _mx = (_byteHigh<<8) + _byteLow;

  // Read the Y value
  _byteLow = Wire.read();
  _byteHigh = Wire.read(); 
  _my = (_byteHigh<<8) + _byteLow;

  // Read the Z value
  _byteLow = Wire.read();
  _byteHigh = Wire.read(); 
  _mz = (_byteHigh<<8) + _byteLow;

  // Read the ST2 register
  // When any of measurement data is read, read ST2 register at the end.
  Wire.read(); 

  // End the transmission
  nackCatcher = Wire.endTransmission();

  // Return
  return nackCatcher;
}

double MPU9250::getMagnetX()
{
  // Read Magnet X
  return (double(_mx)/8192)*4800;
}

double MPU9250::getMagnetY()
{
  // Read Magnet Y
  return (double(_my)/8192)*4800;
}

double MPU9250::getMagnetZ()
{
  // Read Magnet Z
  return (double(_mz)/8192)*4800; 
}

void MPU9250::readMagnetST2()
{
  // ST2 register has a role as data reading end register. When any of measurement data register is 
  // read in continuous measurement mode or external trigger measurement mode, it means data reading start 
  // and taken as data reading until ST2 register is read. Therefore, when any of measurement data is read, 
  // be sure to read ST2 register at the end.
  
  // Begin communication with MPU9250
  Wire.beginTransmission(_i2cAddress_Magnet);

  // Tell register you want to contact
  Wire.write(_REGISTER_MAGNET_ST2);

  // End the transmission - keep the i2c connection
  int nackCatcher = Wire.endTransmission(false);

  // Return if we have a connection problem	
  if(nackCatcher != 0){return;}

  // Request 2 bytes from MPU9250
  Wire.requestFrom(_i2cAddress_Magnet, _ONE_BYTE);

  // Wait for the bytes to arrive
  while(Wire.available() < _ONE_BYTE);

  // Read the ST2 register. Default is 0 which means 14-bit output
  Wire.read();
}

//-----------------
//|               |
//|  Temperature  |
//|               |
//-----------------

float MPU9250::getTemperature()
{
  // Begin communication with MPU9250
  Wire.beginTransmission(_i2cAddress);

  // Tell register you want some data
  Wire.write(_REGISTER_TEMP);

  // End the transmission - keep the i2c connection
  int nackCatcher = Wire.endTransmission(false);

  // Return if we have a connection problem	
  if(nackCatcher != 0){return 0;}

  // Request 2 bytes from MPU9250
  Wire.requestFrom(_i2cAddress, _TWO_BYTES);

  // Wait for the bytes to arrive
  while(Wire.available() < _TWO_BYTES);

  // Read the values
  _byteHigh = Wire.read(); 
  _byteLow = Wire.read();

  // End the transmission
  Wire.endTransmission();

  // Calculate Temperature
  return ((_byteHigh<<8) + _byteLow);
}
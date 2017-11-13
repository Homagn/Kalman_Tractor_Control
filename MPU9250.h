#ifndef MPU9250_h
  #define MPU9250_h

  #include "Arduino.h"
  
  // Version of the Library
  #define _LIB_VERSION "1.00"

  //Address of the MPU9250 compass on i2C
  #define _i2cAddress        		0x68
  #define _i2cAddress_Magnet 		0x0C

  // MPU9250 setup registers
  #define _REGISTER_USER_CTRL		0x6A
  #define _REGISTER_PWR_MGMT_1   	0x6B
  #define _REGISTER_INT_PIN_CFG  	0x37

  //Acceleromemter Registers
  #define _REGISTER_ACCEL_CONFIG 	0x1C
  #define _REGISTER_ACCELERO_X 		0x3B
  #define _REGISTER_ACCELERO_Y 		0x3D
  #define _REGISTER_ACCELERO_Z 		0x3F

  //Gyroscope Registers
  #define _REGISTER_GYRO_CONFIG  	0x1B
  #define _REGISTER_GYRO_X 		0x43
  #define _REGISTER_GYRO_Y 		0x45
  #define _REGISTER_GYRO_Z 		0x47
  
  //Magnetometer Registers
  #define _REGISTER_MAGNET_CONTROL    	0x0A
  #define _REGISTER_MAGNET_X		0x03
  #define _REGISTER_MAGNET_Y		0x05
  #define _REGISTER_MAGNET_Z		0x07
  #define _REGISTER_MAGNET_ST2		0x09

  //Temperature
  #define _REGISTER_TEMP 		0x41

  #define BIT_SLEEP 6
  #define BITS_CLKSEL 2
  #define BITS_FS_SEL 4
  #define BITS_AFS_SEL 4

  #define ONE_BIT 1
  #define TWO_BITS 2
  #define THREE_BITS 3

  #define _ONE_BYTE 1
  #define _TWO_BYTES 2
  #define _SIX_BYTES 6
  #define _SEVEN_BYTES 7

  class MPU9250{
	public:
		static const char *libraryVersion() { return _LIB_VERSION; }

		void begin();
		void initialize();
		void readMagnetST2();
        	
                byte updateBits(uint8_t, uint8_t, uint8_t, uint8_t);
		byte readByte(uint8_t);
                byte writeByte(uint8_t, uint8_t);

		int readAcceleroXYZ();
		double getAcceleroX();
		double getAcceleroY();
		double getAcceleroZ();

                int readGyroXYZ();
                double getGyroX();
                double getGyroY();
                double getGyroZ();
		
		int readMagnetXYZ();
		double getMagnetX();
		double getMagnetY();
		double getMagnetZ();
		
		float getTemperature();

	private:
		int _ax, _ay, _az;
		int _gx, _gy, _gz;
		int _mx, _my, _mz;
		
		byte _byteHigh;
		byte _byteLow;
  		
  };

#endif
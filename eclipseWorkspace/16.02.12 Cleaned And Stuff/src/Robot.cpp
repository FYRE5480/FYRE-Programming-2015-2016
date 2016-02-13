#include "WPILib.h"

#define I2C_SLAVE_ADR 0xA8 // from this thread: http://www.chiefdelphi.com/forums/showthread.php?t=132572
						  // We think this is the address of the Roborio. 168
//							https://wpilib.screenstepslive.com/s/3120/m/7912/l/175524-sending-data-from-the-crio-to-an-arduino
#define ADXL_REG    0x8 // I am trying a thing.
#define TO_READ     8 // num of bytes we are going to read each time (two bytes for each axis)
#define I2C_CHANNEL 2 // The I2C bus on the upper left-hand side is bus 2
typedef unsigned char byte;

int num = 0;
int holdLen = 0;
int arrayLen = 0;
bool isABool = 0;

class Robot: public IterativeRobot
{
	I2C *i2cChannel2; // I ADDED THISSSSSS;KLADFSJKDSAMKSL
	byte *anArray = new byte[1];
	uint8_t *holderPtr = new uint8_t[1];

	void RobotInit()
	{
		i2cChannel2 = new I2C(I2C::Port::kOnboard, I2C_SLAVE_ADR); // ;kadfslkmads;lkmdsa;lkmf;oewianm;oimeai;
	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		ReadDistance(); //KUFGHLIAEFWJK;M;AEWLKMOMAEW;LKMAEW;LKMC;LKM
	}

	void ReadDistance() { // THIS IS MY FUNCTIONA;FLKDMS;LKM F;DLKMF;LAKDSM;FLKMAD;SLMF;oifkmlads;flkadsg;lkafdsm;km
//		byte buff[TO_READ];
//		i2cChannel2->Read(ADXL_REG, TO_READ, buff);
//		SmartDashboard::PutNumber("i2c is", i2cChannel2->Read(ADXL_REG, TO_READ, buff));
//		i2cChannel2->Write(ADXL_REG, 1);
//		i2cChannel2->Read(ADXL_REG, TO_READ, buff);    // request 1 bytes from slave device #8
//		long n = i2cChannel2 -> Read();
//		i2cChannel2->Transaction(NULL, 0, anArray, TO_READ);
//		i2cChannel2->Read(ADXL_REG, 1, holderPtr);
//		i2cChannel2->ReadOnly(ADXL_REG, holderPtr);
		i2cChannel2->Write(ADXL_REG, 1);
//		holdLen = sizeof(holderPtr);
		SmartDashboard::PutNumber("Connected via i2c?", i2cChannel2->AddressOnly());
//		SmartDashboard::PutNumber("holdLen:", holdLen);
//		SmartDashboard::PutNumber("sizeof(anArray):", sizeof(anArray));
		SmartDashboard::PutNumber("Transaction return is a bool:", i2cChannel2->Transaction(NULL, 0, anArray, TO_READ));
//		SmartDashboard::PutNumber("i2c Transaction return:", anArray[0]);
//		SmartDashboard::PutNumber("i2c Read return:", holderPtr[0]);
//		num++; // if putting 0 doesn't work
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot)

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
private:
	I2C *i2cChannel2; // I ADDED THISSSSSS;KLADFSJKDSAMKSL
	byte *anArray = new byte[1];
	uint8_t *holderPtr = new uint8_t[1];
	LiveWindow *lw = LiveWindow::GetInstance();
	SendableChooser *chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";
	std::string autoSelected;

	void RobotInit()
	{
		i2cChannel2 = new I2C(I2C::Port::kOnboard, I2C_SLAVE_ADR); // ;kadfslkmads;lkmdsa;lkmf;oewianm;oimeai;
		chooser = new SendableChooser();
		chooser->AddDefault(autoNameDefault, (void*)&autoNameDefault);
		chooser->AddObject(autoNameCustom, (void*)&autoNameCustom);
		SmartDashboard::PutData("Auto Modes", chooser);
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
		i2cChannel2->Write(ADXL_REG, 1);
		SmartDashboard::PutNumber("Connected via i2c?", i2cChannel2->AddressOnly());
		SmartDashboard::PutNumber("Transaction return is a bool:", i2cChannel2->Transaction(NULL, 0, anArray, TO_READ));
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot)

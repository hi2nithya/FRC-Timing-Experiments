#include <iostream>
#include <memory>
#include <string>

#include <Joystick.h>
#include <SampleRobot.h>
#include <Notifier.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <RobotDrive.h>
#include <Timer.h>
#include <sys/prctl.h>

class TimeStats {

	double min, max, avg;
	unsigned long count, logcount, initcount;
	double prev;

	public:
	TimeStats(int l, int i) {
		min = 1E6; max = 0; avg = 0; count = 0;
		prev = 0;
		logcount = l; initcount = i;
	}

	void Record (double timer) {
		double duration = timer - prev;
		if (count > initcount) {
			if (duration > max) max = duration;
			if (duration < min) min = duration;
			avg = (avg*(count-initcount) + duration) / (1.0 + count - initcount);

			if ((count % logcount) == 0) {
				std::cout << "max " << max << "; min " << min << "; avg " << avg << "; count " << count << std::endl;
			}
		}
		prev = timer;
		count ++;
	}

};


/**
 * This is a demo program showing the use of the RobotDrive class.
 * The SampleRobot class is the base of a robot application that will
 * automatically call your Autonomous and OperatorControl methods at the right
 * time as controlled by the switches on the driver station or the field
 * controls.
 *
 * WARNING: While it may look like a good choice to use for your code if you're
 * inexperienced, don't. Unless you know what you are doing, complex code will
 * be much more difficult under this system. Use IterativeRobot or Command-Based
 * instead if you're new.
 */
class Robot: public frc::SampleRobot {
	frc::RobotDrive myRobot { 0, 1 }; // robot drive system
	frc::Joystick stick { 0 }; // only joystick
	frc::SendableChooser<std::string> chooser;
	const std::string autoNameDefault = "Default";
	const std::string autoNameCustom = "My Auto";

public:
	Robot() {
		//Note SmartDashboard is not initialized here, wait until RobotInit to make SmartDashboard calls
		myRobot.SetExpiration(0.1);
	}

	void RobotInit() {
		chooser.AddDefault(autoNameDefault, autoNameDefault);
		chooser.AddObject(autoNameCustom, autoNameCustom);
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
		std::cout << "Init done " << std::endl;
	}

	/*
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString line to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional comparisons to the
	 * if-else structure below with additional strings. If using the
	 * SendableChooser make sure to add them to the chooser code above as well.
	 */
	void Autonomous() {
		auto autoSelected = chooser.GetSelected();
		// std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", autoNameDefault);
		std::cout << "Auto selected: " << autoSelected << std::endl;

		if (autoSelected == autoNameCustom) {
			// Custom Auto goes here
			std::cout << "Running custom Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 1.0); // spin at half speed
			frc::Wait(2.0);                // for 2 seconds
			myRobot.Drive(0.0, 0.0);  // stop robot
		} else {
			// Default Auto goes here
			std::cout << "Running default Autonomous" << std::endl;
			myRobot.SetSafetyEnabled(false);
			myRobot.Drive(-0.5, 0.0); // drive forwards half speed
			frc::Wait(2.0);                // for 2 seconds
			myRobot.Drive(0.0, 0.0);  // stop robot
		}
	}

	/*
	 * Runs the motors with arcade steering.
	 */
	void OperatorControl() override {
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled()) {
			// drive with arcade style (use right stick)
			myRobot.ArcadeDrive(stick);

			// wait for a motor update time
			frc::Wait(0.005);
		}
	}

	/*
	 * Runs during test mode
	 */
	void Test() override {
	}

	static void handler () {
		static int i = 0;
		static std::unique_ptr<Timer> tmr = std::make_unique<Timer>();
		static TimeStats t = TimeStats(200, 200);

		if (i == 0) {
			pthread_setname_np(pthread_self(), "2 thread");
			  sched_param sch;
			  int policy;
			  pthread_getschedparam(pthread_self(), &policy, &sch);
			  sch.sched_priority = 2;
			  pthread_setschedparam(pthread_self(), SCHED_FIFO, &sch);
			//pthread_setschedprio(pthread_self(), 2);
				tmr->Start();

		}
		t.Record (tmr->Get());
		i++;
	}

	void Disabled() override {
		std::unique_ptr<Timer> tmr2 = std::make_unique<Timer>();
		pthread_setname_np(pthread_self(), "82 thread");
		pthread_setschedprio(pthread_self(), 82);

		char buf[51];
		pthread_getname_np(pthread_self(), &buf[0], 50);
		std::cout << "Disabled " << std::endl;
		std::cout << buf << std::endl;

		std::unique_ptr<Notifier> n = std::make_unique<Notifier>(&handler);
		n->StartPeriodic(0.005);

		int i=0;
		tmr2->Start();
		while (1) {
			i++;
			//frc::Wait (30);
			//if (i % 10000)
				//std::cout << "Elapsed time " << tmr2->Get() << std::endl;
		}
	}

};


START_ROBOT_CLASS(Robot)

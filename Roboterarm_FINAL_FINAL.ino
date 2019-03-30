#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>

// Constants
int stp_Pins[5][2] = {
			{20, 21},
			{18, 19},
			{7, 6},
			{11, 10},
			{13, 12}, };
const int stp_Speed[5] = { 1500, 1500, 1500, 1500, 1500 };
const int stp_Acc[5] = { 500, 500, 500, 500, 500 };
const int sw1 = 49, sw2 = 51;
int servPin = 53;
char stpSerial;

bool newData = false;
bool demoMode = false;
bool switches = false;

String pos_1, pos_2;
const int pos1 = 3000, pos2 = 0, pos3 = 7500, pos4 = -10000, pos5 = 1900;

// Init Servo Object
Servo myservo;

// Init Stepper Objects
AccelStepper stp1(AccelStepper::DRIVER, stp_Pins[0][0], stp_Pins[0][1]);
AccelStepper stp2(AccelStepper::DRIVER, stp_Pins[1][0], stp_Pins[1][1]);
AccelStepper stp3(AccelStepper::DRIVER, stp_Pins[2][0], stp_Pins[2][1]);
AccelStepper stp4(AccelStepper::DRIVER, stp_Pins[3][0], stp_Pins[3][1]);
AccelStepper stp5(AccelStepper::DRIVER, stp_Pins[4][0], stp_Pins[4][1]);

void setup()
{
	pinMode(sw1, INPUT_PULLUP);
	pinMode(sw2, INPUT_PULLUP);

	// Initialize Steppers
	INIT_STEPPER();
	delay(100);
	// Initialize Servo
	myservo.attach(servPin);
	Serial.begin(9600);
	delay(200);
	Serial.println("Serial initialized");
	Serial.println();

	INIT_DEMO();
}

void loop()
{
	if (demoMode == true)
	{
		while (switches == true)
		{
			if (!digitalRead(sw2))
			{
				delay(2000);
				Serial.println("Fahre in Ausgangsposition");
				INIT_STARTPOS();
				Serial.println("In Ausgangsposition gefahren");
				switches = false;
			}
			if (!digitalRead(sw1))
			{
				delay(2000);
				Serial.println("Setze Startpostion");
				INIT_SETPOS();
				switches = false;
			}
		}
		prog_6();
		delay(2000);
	}
	if (demoMode == false)
	{
		if (Serial.available() > 0)
		{
			stpSerial = Serial.read();

			// STEPPER verfahren
			if (stpSerial == 'y' || stpSerial == 'x' || stpSerial == 'c' || stpSerial == 'v' || stpSerial == 'b')
				moveStepper(stpSerial);

			// Servo
			if (stpSerial == 'n')
				moveServo();

			// In Ausgangsposition fahren, um Initialisierung zu vermeiden
			if (stpSerial == 'm')
				moveStart();

			// Programm 1
			if (stpSerial == 'a')
				prog_1();

			// Programm 2
			if (stpSerial == 's')
				prog_2();

			// Programm 3
			if (stpSerial == 'd')
				prog_3();

			// Programm 4
			if (stpSerial == 'f')
				prog_4();

			// Programm 5
			if (stpSerial == 'g')
				prog_5();

			// Programm Flasche
			if (stpSerial == 'h')
				Flasche();

			// Ausgangspos Initialisieren
			if (stpSerial == 'i')
				INIT_STARTPOS();

			// Set aktuelle Position
			if (stpSerial == 'o')
				INIT_SETPOS();
		}
	}
}

void INIT_STEPPER()
{
	stp1.setMaxSpeed(stp_Speed[0]);
	stp2.setMaxSpeed(stp_Speed[1]);
	stp3.setMaxSpeed(stp_Speed[2]);
	stp4.setMaxSpeed(stp_Speed[3]);
	stp5.setMaxSpeed(stp_Speed[4]);
	stp1.setAcceleration(stp_Acc[0]);
	stp2.setAcceleration(stp_Acc[1]);
	stp3.setAcceleration(stp_Acc[2]);
	stp4.setAcceleration(stp_Acc[3]);
	stp5.setAcceleration(stp_Acc[4]);
}

void INIT_STARTPOS()
{
	Serial.println("Ausgangsposition anfahren...");
	newData = true;

	while (newData == true)
	{
		stp1.moveTo(pos1);
		stp2.moveTo(pos2);
		stp3.moveTo(pos3);
		stp4.moveTo(pos4);
		stp5.moveTo(pos5);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp1.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}
		newData = false;
		Serial.println("Ausgangsposition erreicht");
		Serial.println();
	}
	stpSerial = 'e';
}

void INIT_SETPOS()
{
	stp1.setCurrentPosition(pos1);
	stp2.setCurrentPosition(pos2);
	stp3.setCurrentPosition(pos3);
	stp4.setCurrentPosition(pos4);
	stp5.setCurrentPosition(pos5);

	Serial.print("Neue Position 1: ");
	Serial.println(stp1.currentPosition());
	Serial.print("Neue Position 2: ");
	Serial.println(stp2.currentPosition());
	Serial.print("Neue Position 3: ");
	Serial.println(stp3.currentPosition());
	Serial.print("Neue Position 4: ");
	Serial.println(stp4.currentPosition());
	Serial.print("Neue Position 5: ");
	Serial.println(stp5.currentPosition());
}

void prog_1()
{
	Serial.println("Programm 1");
	newData = true;
	while (newData == true)
	{
		stp1.moveTo(-3000);
		stp2.moveTo(0);
		stp3.moveTo(8000);
		stp4.moveTo(-9000);
		stp5.moveTo(2500);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(2000);

		stp1.moveTo(-3200);
		stp2.moveTo(0);
		stp3.moveTo(10300);
		stp4.moveTo(-10500);
		stp5.moveTo(2460);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(2000);

		stp1.moveTo(-3000);
		stp2.moveTo(0);
		stp3.moveTo(8000);
		stp4.moveTo(-9000);
		stp5.moveTo(2500);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		INIT_STEPPER();
		Serial.println("Programm 1 fertig!");
		newData = false;
	}
	stpSerial = 'e';
}

void prog_2()
{
	Serial.println("Programm 2...");
	newData = true;
	while (newData == true)
	{
		stp1.moveTo(-500);
		stp2.moveTo(0);
		stp3.moveTo(7500);
		stp4.moveTo(-10000);
		stp5.moveTo(2500);
		myservo.write(120);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		stp4.moveTo(-10700);
		stp4.runToPosition();

		delay(5000);
		myservo.write(75);
		delay(500);

		stp4.moveTo(-10000);
		stp4.runToPosition();

		stp1.moveTo(9000);
		stp2.moveTo(-15500);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
		}

		delay(1000);

		stp4.moveTo(-10700);
		stp4.runToPosition();

		delay(500);
		myservo.write(120);

		delay(1000);

		stp4.moveTo(-10000);
		stp4.runToPosition();

		INIT_STEPPER();
		Serial.println("Programm 2 fertig!");
		newData = false;
	}
	stpSerial = 'e';
}

void prog_3()
{
	Serial.println("Programm 3");
	newData = true;
	while (newData == true)
	{
		stp3.moveTo(6900);
		stp4.moveTo(-10200);

		stp3.setMaxSpeed(700);
		stp4.setMaxSpeed(700);

		while (stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0)
		{
			stp3.run();
			stp4.run();
		}

		delay(3000);

		stp3.moveTo(5000);
		stp4.moveTo(-8900);
		stp3.setMaxSpeed(300);
		stp4.setMaxSpeed(205);
		stp3.setAcceleration(5000);
		stp4.setAcceleration(5000);

		while (stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0)
		{
			stp3.run();
			stp4.run();
		}

		delay(2000);

		stp3.moveTo(6900);
		stp4.moveTo(-10200);

		while (stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0)
		{
			stp3.run();
			stp4.run();
		}

		stp3.setMaxSpeed(stp_Speed[2]);
		stp4.setMaxSpeed(stp_Speed[3]);
		stp3.setAcceleration(stp_Acc[2]);
		stp4.setAcceleration(stp_Acc[3]);

		INIT_STEPPER();
		Serial.println("Programm 3 fertig");
		newData = false;

	}
	stpSerial = 'e';
}

void prog_4()
{
	Serial.println("Programm 4");
	newData = true;
	while (newData == true)
	{
		// Ausgangsposition
		stp1.moveTo(-1300);
		stp3.moveTo(6800);
		stp4.moveTo(-10050);
		stp5.moveTo(1900);

		setParam(700, 700, 700, 700, 700, 1000, 1000, 1000, 1000, 1000);

		while (stp1.distanceToGo() != 0 || stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0 ||
			stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		Serial.println("Ausgangsposition Prog 4 erreicht");
		delay(2000);

		// Zweite Position
		stp1.moveTo(-1550);
		stp3.moveTo(5900);
		stp4.moveTo(-9400);
		stp5.moveTo(1900);

		setParam(50, 700, 220, 130, 700, 5000, 5000, 5000, 5000, 5000);

		while (stp1.distanceToGo() != 0 || stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0 || 
			stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(1000);

		// Dritte Position
		stp1.moveTo(-1550);
		stp3.moveTo(5920);
		stp4.moveTo(-9400);
		stp5.moveTo(2120);

		setParam(700, 700, 5, 700, 50, 5000, 5000, 5000, 5000, 5000);

		while (stp1.distanceToGo() != 0 || stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(1000);

		// Vierte Position
		stp1.moveTo(-1050);
		stp3.moveTo(6900);
		stp4.moveTo(-10300);
		stp5.moveTo(2100);

		setParam(50, 700, 102, 90, 2, 5000, 5000, 5000, 5000, 5000);

		while (stp1.distanceToGo() != 0 || stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(1000);

		// Ausgangsposition
		stp1.moveTo(-1300);
		stp3.moveTo(6800);
		stp4.moveTo(-10050);
		stp5.moveTo(1900);

		setParam(50, 700, 20, 50, 40, 5000, 5000, 5000, 5000, 5000);

		while (stp1.distanceToGo() != 0 || stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(1000);

		INIT_STEPPER();

		Serial.println("Programm 4 fertig");
		newData = false;

	}
	stpSerial = 'e';
}

void prog_5()
{
	Serial.println("Programm 5 gestartet");
	newData = true;
	while (newData == true)
	{
		// Ausgangsposition
		stp1.moveTo(-1000);
		stp3.moveTo(7400);
		stp4.moveTo(-10600);
		stp5.moveTo(1900);

		setParam(1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000);

		while (stp1.distanceToGo() != 0 || stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0 || 
			stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(1000);

		// Schliesse Servo
		int n = 70;
		for (int i = 0; i < 5; i++)
		{
			myservo.write(n);
			n -= 5;
			delay(100);
		}

		delay(500);

		// Fahre nach zweite Position
		stp4.moveTo(-10200);
		stp5.moveTo(2500);

		setParam(1000, 1000, 1000, 2000, 3000, 1000, 1000, 1000, 1000, 1000);

		while (stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp4.run();
			stp5.run();
		}

		delay(1000);

		// Setze ab
		stp4.moveTo(-10580);
		stp4.setMaxSpeed(500);
		stp4.setAcceleration(500);
		stp4.runToPosition();

		delay(500);

		//  Oeffne Servo
		n = 50;
		for (int i = 0; i < 5; i++)
		{
			myservo.write(n);
			n += 5;
			delay(100);
		}
		myservo.write(90);

		delay(500);

		// Fahre nach oben
		stp4.moveTo(-9800);
		stp4.setMaxSpeed(500);
		stp4.setAcceleration(500);
		while (stp4.distanceToGo() != 0)
			stp4.run();


		// Fahre neue Position an
		stp1.moveTo(-1300);
		stp3.moveTo(8200);
		stp4.moveTo(-11000);
		stp5.moveTo(1900);

		setParam(1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000);

		while (stp1.distanceToGo() != 0 || stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0 || 
			stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(1000);

		// Schliesse Servo
		n = 70;
		for (int i = 0; i < 5; i++)
		{
			myservo.write(n);
			n -= 5;
			delay(100);
		}

		// Fahre nach zweite Position
		stp4.moveTo(-10200);
		stp5.moveTo(2500);

		setParam(1000, 1000, 1000, 2000, 3000, 1000, 1000, 1000, 1000, 1000);

		while (stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp4.run();
			stp5.run();
		}

		delay(1000);

		// Setze ab
		stp4.moveTo(-11000);
		stp4.setMaxSpeed(500);
		stp4.setAcceleration(500);
		while (stp4.distanceToGo() != 0)
			stp4.run();

		delay(500);

		//  Oeffne Servo
		n = 50;
		for (int i = 0; i < 5; i++)
		{
			myservo.write(n);
			n += 5;
			delay(100);
		}
		myservo.write(90);

		delay(500);

		// Fahre nach oben
		stp4.moveTo(-10000);
		stp4.setMaxSpeed(500);
		stp4.setAcceleration(500);
		while (stp4.distanceToGo() != 0)
			stp4.run();

		delay(1000);

		INIT_STEPPER();

		Serial.println("Programm 5 fertig");
		newData = false;

	}
	stpSerial = 'e';
}

void prog_6()
{
	Serial.println("Programm 6 gestartet");
	newData = true;
	while (newData == true)
	{
		stp1.moveTo(3000);
		stp2.moveTo(0);
		stp3.moveTo(7500);
		stp4.moveTo(-10000);
		stp5.moveTo(1900);

		setParam(358, 1000, 322, 358, 272, 1000, 1000, 1000, 1000, 1000);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(2000);

		stp1.moveTo(-2000);
		stp2.moveTo(-14000);
		stp3.moveTo(12000);
		stp4.moveTo(-6500);
		stp5.moveTo(-1900);

		setParam(358, 1000, 322, 358, 272, 500, 500, 500, 500, 500);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(2000);

		INIT_STEPPER();

		Serial.println("Programm 6 ist fertig");
		newData = false;
	}
	stpSerial = 'e';
}

void Flasche()
{
	Serial.println("Programm Flasche einschenken gestartet");
	newData = true;
	delay(1000);
	while (newData == true)
	{
		// Fahre ueber Flasche
		stp4.moveTo(-7000);
		stp4.runToPosition();

		stp3.moveTo(3000);
		stp3.runToPosition();

		myservo.write(120);
		delay(1000);

		// Fahre Startposition an
		stp1.moveTo(11200);
		stp2.moveTo(0);
		stp3.moveTo(4000);
		stp4.moveTo(-12500);
		stp5.moveTo(1900);

		setParam(2000, 700, 500, 500, 500, 1000, 1000, 1000, 1000, 1000);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(1000);

		// Schliesse Servo 1
		int n = 120;
		for (int i = 0; i < 9; i++)
		{
			myservo.write(n);
			n -= 5;
			delay(100);
		}

		delay(1000);

		// Verfahre bis Einschenkposition
		stp1.moveTo(8500);
		stp4.moveTo(-8000);

		setParam(1000, 1000, 1000, 385, 1000, 500, 500, 500, 500, 500);

		while (stp1.distanceToGo() != 0 || stp4.distanceToGo() != 0)
		{
			stp1.run();
			stp4.run();
		}

		delay(1000);

		stp1.moveTo(5000);
		stp3.moveTo(5500);
		stp4.moveTo(-8500);
		stp5.moveTo(2400);

		setParam(560, 700, 240, 80, 80, 1000, 1000, 1000, 1000, 1000);

		while (stp1.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}

		delay(1000);

		// Schenke ein

		stp2.moveTo(-9500);
		stp2.setMaxSpeed(850);
		stp2.runToPosition();

		delay(5000);

		// Drehe wieder normal und fahre in Ausgangsposition

		stp2.moveTo(0);
		stp2.setMaxSpeed(1000);
		stp2.runToPosition();

		stp5.moveTo(1900);
		stp5.setMaxSpeed(1000);
		stp5.runToPosition();

		// Fahre an Ausgangsposition

		stp1.moveTo(11200);
		stp2.moveTo(0);
		stp3.moveTo(4000);
		stp4.moveTo(-12500);

		setParam(1000, 700, 600, 600, 700, 1000, 1000, 1000, 1000, 1000);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 ||
			stp3.distanceToGo() != 0 || stp4.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
			stp3.run();
			stp4.run();
		}

		delay(1000);

		// Oeffne Servo 1
		n = 90;
		for (int i = 0; i < 9; i++)
		{
			myservo.write(n);
			n += 5;
			delay(100);
		}

		delay(1000);

		// Fahre hoch
		stp4.moveTo(-8000);
		stp4.setMaxSpeed(1000);
		stp4.runToPosition();

		delay(1000);

		INIT_STEPPER();

		Serial.println("Programm 6 ist fertig");
		newData = false;
	}
	stpSerial = 'e';
}

void moveStepper(char stpSerial)
{
	AccelStepper *stepper;
	String stpStr, posStr;
	int posInt, stpInt;

	// Abfrage nach Stepper char
	switch (stpSerial)
	{
		// Stepper 1
	case 'y':
	{
		stepper = &stp1;
		stpInt = 1;
	}
	break;
	// Stepper 2
	case 'x':
	{
		stepper = &stp2;
		stpInt = 2;
	}
	break;
	// Stepper 3
	case 'c':
	{
		stepper = &stp3;
		stpInt = 3;
	}
	break;
	// Stepper 4
	case 'v':
	{
		stepper = &stp4;
		stpInt = 4;
	}
	break;
	// Stepper 5
	case 'b':
	{
		stepper = &stp5;
		stpInt = 5;
	}
	break;
	}

	// Anzeige von Stepper
	Serial.print("Stepper ");
	Serial.println(stpInt);
	Serial.print("Aktuelle Pos: ");
	Serial.println(stepper->currentPosition());
	Serial.print("Neue Position eingeben: ");

	newData = true;
	while (newData == true)
	{
		if (Serial.available() > 0)
		{
			posStr = Serial.readString();
			posInt = posStr.toInt();

			stepper->moveTo(posInt);

			while (stepper->distanceToGo() != 0)
			{
				stepper->run();
			}
			Serial.print("Neue Pos: ");
			Serial.println(stepper->currentPosition());
			Serial.println();
			newData = false;
		}
	}
	stpSerial = 'e';
}

void moveServo()
{
	int servInt;
	String servStr;

	Serial.println("Servo:");
	Serial.print("Aktuelle Pos Servo: ");
	Serial.println(myservo.read());

	newData = true;
	while (newData == true)
	{
		if (Serial.available() > 0)
		{
			servStr = Serial.readString();
			servInt = servStr.toInt();

			myservo.write(servInt);

			Serial.print("Neue Pos Servo: ");
			Serial.println(myservo.read());
			Serial.println();
			newData = false;
		}
	}
	stpSerial = 'e';
}

void moveStart()
{
	Serial.println("In Ausgangsposition fahren");
	newData = true;
	while (newData == true)
	{
		stp1.moveTo(3000);
		stp2.moveTo(0);
		stp3.moveTo(7500);
		stp4.moveTo(-10000);
		stp5.moveTo(1900);

		while (stp1.distanceToGo() != 0 || stp2.distanceToGo() != 0 || stp3.distanceToGo() != 0 ||
			stp4.distanceToGo() != 0 || stp5.distanceToGo() != 0)
		{
			stp1.run();
			stp2.run();
			stp3.run();
			stp4.run();
			stp5.run();
		}
		Serial.println("In Ausgangsposition gefahren");
		Serial.println();
		newData = false;
	}
	stpSerial = 'e';
}

void setParam(int stp1Speed, int stp2Speed, int stp3Speed, int stp4Speed, int stp5Speed,
	int stp1Acc, int stp2Acc, int stp3Acc, int stp4Acc, int stp5Acc)
{
	stp1.setMaxSpeed(stp1Speed);
	stp2.setMaxSpeed(stp2Speed);
	stp3.setMaxSpeed(stp3Speed);
	stp4.setMaxSpeed(stp4Speed);
	stp5.setMaxSpeed(stp5Speed);

	stp1.setAcceleration(stp1Acc);
	stp2.setAcceleration(stp2Acc);
	stp3.setAcceleration(stp3Acc);
	stp4.setAcceleration(stp4Acc);
	stp5.setAcceleration(stp5Acc);
}

void INIT_DEMO()
{
	while (switches == false)
	{
		if (!digitalRead(sw2))
		{
			delay(1000);
			Serial.println("Demo Mode");
			demoMode = true;
			switches = true;
		}

		if (!digitalRead(sw1))
		{
			delay(1000);
			Serial.println("Serial Mode");
			Serial.println();
			// Abfragen fuer Initialisierung Roboter
			Serial.println("Press 'i' for initialize Startposition");
			Serial.println("Press 'o' if startposition is reached and you want to set startposition");
			Serial.println();
			switches = true;
		}
	}
}
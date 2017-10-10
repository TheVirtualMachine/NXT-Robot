#pragma config(Sensor, S1, lightSensor, sensorLightActive)

/*--------------------------------------------------------------------------------------------------------*\
|*    MOTORS & SENSORS:                                                                                   *|
|*    [I/O Port]              [Name]              [Type]              [Description]                       *|
|*    Port B                  motorB              NXT                 Right motor                         *|
|*    Port C                  motorC              NXT                 Left motor                          *|
|*    Port 1                  lightSensor         Light Sensor        Back mounted                        *|
\*---------------------------------------------------------------------------------------------------4246-*/

// Define the directions.
#define LEFT true
#define RIGHT (!LEFT)

// Define the colour ranges.
#define BLACK_END 43
#define MID_START BLACK_END
#define MID_END 50
#define GREEN_START MID_END
#define GREEN_END 60


// Define the motor values.
#define HIGH 60
#define LOW 5
#define ROTATE 10

// Define other constants.
#define GREEN_TICKS_TRIGGER 1400

// The value of the light sensor on this tick.
int light;

// How many ticks we have been detecting green for.
int greenTicks = 0;

// How many green stops we have passed.
int stopsPassed = 0;

// Have we caught the chair yet
bool chairCaught = false;

// Have we caught the chair yet
bool chairPushed = false;

// Return whether or not we are currently on black.
bool onBlack() {
	return (light < BLACK_END);
}

// Return whether or not we are currently on green.
bool onGreen() {
	return (light >= GREEN_START && light < GREEN_END);
}

// Check if we are currently between black and green.
bool onMid() {
	return (light >= MID_START && light < MID_END);
}

void goLeft() {
	motor[motorB] = HIGH; // Set right motor to high.
	motor[motorC] = LOW; // Set left motor to low.
}

void goRight() {
	motor[motorB] = LOW; // Set right motor to low.
	motor[motorC] = HIGH; // Set left motor to high.
}

void sharpLeft() {
	motor[motorB] = HIGH; // Set right motor to high.
	motor[motorC] = -ROTATE; // Set left motor to low.
}

void sharpRight() {
	motor[motorB] = -ROTATE; // Set right motor to low.
	motor[motorC] = HIGH; // Set left motor to high.
}

// Go straight.
void goStraight() {
	motor[motorB] = HIGH / 2;
	motor[motorC] = HIGH / 2;
}

// Go straight.
void fastStraight() {
	motor[motorB] = HIGH;
	motor[motorC] = HIGH;
}

// Double check if we are on green.
bool checkGreen() {
	motor[motorB] = 0;
	motor[motorC] = 0;
	//wait1Msec(1000);

	bool checkOne = false;
	bool checkTwo = false;

	motor[motorB] = ROTATE;
	motor[motorC] = -ROTATE;
	//original: 1000
	wait1Msec(500);
	motor[motorB] = 0;
	motor[motorC] = 0;
	light = SensorValue[lightSensor];
	checkOne = onGreen();

	motor[motorB] = -ROTATE;
	motor[motorC] = ROTATE;
	//original: 2000;
	wait1Msec(1000);
	motor[motorB] = 0;
	motor[motorC] = 0;
	light = SensorValue[lightSensor];
	checkTwo = onGreen();

	motor[motorB] = ROTATE;
	motor[motorC] = -ROTATE;
	//original: 1000;
	wait1Msec(500);
	motor[motorB] = 0;
	motor[motorC] = 0;

	//wait1Msec(1000);

	return (checkOne || checkTwo);
}

// Handle green checking.
bool handleGreen() {
	if ((onGreen() || onMid()) && greenTicks < GREEN_TICKS_TRIGGER) {
		greenTicks++;
		if (greenTicks >= GREEN_TICKS_TRIGGER && onGreen()) {
			if (checkGreen()) {
				stopsPassed++;
				return (true);
			}
		}
	}
	return (false);
}

void catchChair()
{
	motor[motorB] = ROTATE;
	motor[motorC] = -ROTATE;
	wait1Msec(1300);
	goStraight();
	wait1Msec(2000);
	sharpLeft();
	wait1Msec(1000);
	goStraight();
	wait1Msec(2500);
	chairCaught = true;
}

void pushChair()
{
	motor[motorB] = ROTATE;
	motor[motorC] = -ROTATE;
	wait1Msec(1300);
	goStraight();
	wait1Msec(2700);
	motor[motorB] = -(HIGH/2);
	motor[motorC] = -(HIGH/2);
	wait1Msec(4000);
	sharpLeft();
	wait1Msec(800);
	chairPushed = true;
}

task main() {
	wait1Msec(50);
	motor[motorA] = 100;
	while (nNxtButtonPressed != 3){light = SensorValue[lightSensor];}
	goStraight();
	wait1Msec(1000);
	while (true) {
		light = SensorValue[lightSensor];
		if (stopsPassed == 2 && !chairCaught)
		{
			catchChair();
		}
		else if (stopsPassed == 5 && !chairPushed)
		{
			pushChair();
		}
		if (handleGreen() || onGreen()) {
			goStraight();
		}
		else if (onMid()){
			fastStraight();
		} else if (onBlack()) {
			goRight();
		} else {
			goLeft();
		}
		if (!(onGreen() || onMid())) {
			greenTicks = 0;
		}

	}
}

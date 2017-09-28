#pragma config(Sensor, S1, lightSensor, sensorLightActive)

/*--------------------------------------------------------------------------------------------------------*\
|*    MOTORS & SENSORS:                                                                                   *|
|*    [I/O Port]              [Name]              [Type]              [Description]                       *|
|*    Port A                  motorA              NXT                 Shooter                             *|
|*    Port B                  motorB              NXT                 Right motor                         *|
|*    Port C                  motorC              NXT                 Left motor                          *|
|*    Port 1                  lightSensor         Light Sensor        Back mounted                        *|
\*---------------------------------------------------------------------------------------------------4246-*/

// Define the directions.
#define LEFT true
#define RIGHT (!LEFT)

// Define the colour ranges.
#define BLACK_END 50
#define GREEN_START BLACK_END
#define GREEN_END 60

// Define the motor values.
#define HIGH 30
#define LOW 2

// The value of the light sensor on this tick.
int light;

// If we are currently on green.
bool onGreenNow = false;

// Return whether or not we are currently on black.
bool onBlack() {
	return (light < BLACK_END);
}

// Return whether or not we are currently on green.
bool onGreen() {
	return (light >= GREEN_START && light < GREEN_END);
}

void goLeft() {
	motor[motorB] = HIGH; // Set right motor to low.
	motor[motorC] = LOW; // Set left motor to high.
}

void goRight() {
	motor[motorB] = LOW; // Set right motor to low.
	motor[motorC] = HIGH; // Set left motor to high.
}

// Set the direction to turn.
void setDirection(bool newDir) {
	if (newDir == LEFT) {
		goLeft();
		} else {
		goRight();
	}
}

// Stop moving.
void stop() {
	motor[motorB] = 0;
	motor[motorC] = 0;
}

// Shoot the projectile.
void shoot() {
	stop();
	motor[motorA] = 100;
	wait10Msec(9);
	motor[motorA] = 0;
	wait1Msec(3000);
}

//Track the Line
void runBot() {
	setDirection(RIGHT);
	wait1Msec(500);
	while (true) {
		light = SensorValue[lightSensor];
		if (onGreen()) {
			motor[motorB] = HIGH;
			motor[motorC] = HIGH;
			} else if (onBlack()) {
			onGreenNow = false;
			setDirection(RIGHT);
			} else {
			onGreenNow = false;
			setDirection(LEFT);
		}
	}
	stop();
}

//Driver method which ties together all functions
task main() {
	while (true) {
		light = SensorValue[lightSensor];
		if (nNxtButtonPressed == 3) {
			motor[motorB] = HIGH;
			motor[motorC] = HIGH + 1;
			wait1Msec(3000);
			shoot();
			runBot();
		}
	}
}

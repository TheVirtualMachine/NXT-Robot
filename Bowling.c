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
#define BLACK_END 40
#define GREEN_START BLACK_END
#define GREEN_END 50
#define WHITE_START GREEN_END

// Define the motor values.
#define HIGH 30
#define LOW 2

// The value of the light sensor on this tick.
int light;

// The number of green stops that have been passed.
int stopsPassed = 0;

// If we are currently on green.
bool onGreenNow = false;

// If we have already shot.
bool hasShot = false;

// Return whether or not we are currently on black.
bool onBlack() {
	return (light < BLACK_END);
}

// Return whether or not we are currently on green.
bool onGreen() {
	return (light >= GREEN_START && light < GREEN_END);
}

// Return whether or not we are currently on white.
bool onWhite() {
	return (light >= WHITE_START);
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
	wait1Msec(1000);
}

void runBot() {
	setDirection(RIGHT);
	while (true) {
		light = SensorValue[lightSensor];
		if (onBlack()) {
			onGreenNow = false;
			setDirection(RIGHT);
		} else if (onWhite()) {
			onGreenNow = false;
			setDirection(LEFT);
		} else if (onGreen()) {
			onGreenNow = true;
			if (!onGreenNow) {
				stopsPassed += 1;
			}
			if (stopsPassed == 1) {
				setDirection(RIGHT)
			} else if (stopsPassed == 2 && !hasShot) {
				shoot();
				hasShot = true;
			}
		}
	}
	stop();
}

task main() {
	while (true) {
		if (nNxtButtonPressed == 1 || nNxtButtonPressed == 2) {
			runBot();
		}
	}
}

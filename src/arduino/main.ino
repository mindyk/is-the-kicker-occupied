#include <SPI.h>
#include <Ethernet.h>

// this is the mac address of YOUR arduino
byte mac[] = {0x90, 0xA2, 0xDA, 0x0C, 0x4C, 0x61};

EthernetClient client;

// this is the (web)server ip you want to push your data to
IPAddress server(127, 0, 0, 1);

boolean lastConnected = false;

// runs once
void setup() {
	// start the Ethernet connection:
	if (Ethernet.begin(mac) == 0) {}
	// wait for it
	delay(1000);
}

// runs forever
void loop() {

  long currentDistance;
	// if there's no net connection, but there was one last time
	// through the loop, then stop the client:
	if (!client.connected() && lastConnected) {
		client.stop();
	}

	currentDistance = getCurrentDistance();
	if (currentDistance != 0) {
		if(!client.connected()) {
			sendData(currentDistance);
		}
	}

	// store the state of the connection for next time through
	// the loop:
	lastConnected = client.connected();

	// wait for it
	delay(1000);
}

// this method makes a HTTP connection to the server:
void sendData(long data) {

	// if there's a successful connection:
	if (client.connect(server, 80)) {
		// send the HTTP GET request:
		client.print("GET /add.php?distance=");
		client.print(data);
		client.println(" HTTP/1.0");
		client.println();
		client.stop();
	} else {
		// if you couldn't make a connection:
    	client.stop();
	}
}

//+++ pin setup you may change this according to YOUR arduino wire setup
int pingPin = 5;
int inPin = 6;
//--- pin setup

// reading fromt he ping sensor
long getCurrentDistance() {
	long duration, inches, cm;

	// The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
	// Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
	pinMode(pingPin, OUTPUT);
	digitalWrite(pingPin, LOW);
	delayMicroseconds(2);
	digitalWrite(pingPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(pingPin, LOW);

	pinMode(inPin, INPUT);
	duration = pulseIn(inPin, HIGH);
	cm = microsecondsToCentimeters(duration);

	return cm;
}

// math
long microsecondsToCentimeters(long microseconds) {
	// The speed of sound is 340 m/s or 29 microseconds per centimeter.
	// The ping travels out and back, so to find the distance of the
	// object we take half of the distance travelled.
	return microseconds / 29 / 2;
}

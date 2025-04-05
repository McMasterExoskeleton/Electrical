// Arduino code for HLSR-20P Current Sensor
// Connect sensor Uout to A3 pin of Arduino

const int analogPin = A3;          // Pin connected to Uout of HLSR-20P
const float referenceVoltage = 2.29; // Uref in volts (typically 2.5V)[3]
const float sensitivity = 40.0;     // Sensitivity in mV/A[4]
const int numReadings = 10;        // Number of readings for averaging

int readings[numReadings];          // Array for moving average filter
int readIndex = 0;                  // Current position in array
int total = 0;                      // Running total
int average = 0;                    // Average value

void setup() {
  Serial.begin(9600);
  Serial.println("HLSR-20P Current Sensor Monitor");
  Serial.println("--------------------------------");
  
  // Initialize all readings to 0
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

void loop() {
  // Subtract the last reading
  total = total - readings[readIndex];
  
  // Read from the sensor
  readings[readIndex] = analogRead(analogPin);
  
  // Add the reading to the total
  total = total + readings[readIndex];
  
  // Advance to the next position in the array
  readIndex = (readIndex + 1) % numReadings;
  
  // Calculate the average
  average = total / numReadings;
  
  // Convert analog value (0-1023) to voltage (0-5V)
  float voltage = average * (5.0 / 1023.0);
  
  // Calculate current using formula: Current = (Uout - Uref) / Sensitivity
  // The HLSR-20P has ±50A measurement range with Uout-Uref ranging from -2V to +2V[3]
  float current = (voltage - referenceVoltage) / (sensitivity / 1000.0);
  
  // Display the results
  Serial.print("Analog value: ");
  Serial.print(average);
  Serial.print(", Voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V, Current: ");
  Serial.print(current, 3);
  Serial.println(" A");
  
  // Check if current is within the sensor's range
  if (abs(current) > 50.0) {
    Serial.println("WARNING: Current exceeds sensor range (±50A)!");
  }
  
  delay(200); // Update 5 times per second
}

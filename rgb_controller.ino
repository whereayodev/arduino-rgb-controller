#define RED_PIN 3
#define GREEN_PIN 6
#define BLUE_PIN 5

// Variable to determine strip type
// true - for common cathode strips (regular)
// false - for common anode strips (inverted logic)
const bool COMMON_CATHODE = true;

// RGB values and brightness
int currentR = 255;
int currentG = 0;
int currentB = 0;
int brightness = 100;  // Brightness in percentage (0-100)
bool powerState = false;

void setup() {
  Serial.begin(115200);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // Initialize with off state
  setColor(0, 0, 0);

  Serial.println("{\"status\":\"ready\",\"device\":\"Arduino RGB Controller\"}");
}

// Function to set color based on strip type
void setColor(int r, int g, int b) {
  if (COMMON_CATHODE) {
    analogWrite(RED_PIN, r);
    analogWrite(GREEN_PIN, g);
    analogWrite(BLUE_PIN, b);
  } else {
    // Invert values for common anode strips
    analogWrite(RED_PIN, 255 - r);
    analogWrite(GREEN_PIN, 255 - g);
    analogWrite(BLUE_PIN, 255 - b);
  }
}

// Applies current RGB values considering brightness
void applyColor() {
  if (!powerState) {
    setColor(0, 0, 0);
    return;
  }

  // Improved algorithm for brightness calculation
  // Using a curve that works better at low levels

  // For very low brightness (1-10%) use linear scale
  // For medium and high brightness (11-100%) use a slightly
  // more intensive curve
  float brightnessFactor;

  if (brightness <= 10) {
    // For low brightness - linear scale from 0.5% to 10%
    brightnessFactor = map(brightness, 0, 10, 0, 25) / 255.0;
  } else {
    // For medium and high brightness - exponential curve
    brightnessFactor = map(brightness, 10, 100, 25, 255) / 255.0;
  }

  // Calculate values using brightness factor
  int r = round(currentR * brightnessFactor);
  int g = round(currentG * brightnessFactor);
  int b = round(currentB * brightnessFactor);

  // Log what we're sending to pins
  Serial.print("Brightness factor: ");
  Serial.print(brightnessFactor, 4);
  Serial.print(" | Sending to pins - R:");
  Serial.print(r);
  Serial.print(" G:");
  Serial.print(g);
  Serial.print(" B:");
  Serial.println(b);

  // Set the color
  setColor(r, g, b);

  // Send current state
  Serial.print("{\"state\":");
  Serial.print(powerState ? "true" : "false");
  Serial.print(",\"color\":{\"r\":");
  Serial.print(currentR);
  Serial.print(",\"g\":");
  Serial.print(currentG);
  Serial.print(",\"b\":");
  Serial.print(currentB);
  Serial.print("},\"brightness\":");
  Serial.print(brightness);
  Serial.print(",\"applied\":{\"r\":");
  Serial.print(r);
  Serial.print(",\"g\":");
  Serial.print(g);
  Serial.print(",\"b\":");
  Serial.print(b);
  Serial.print("}}");
  Serial.println();
}

void smoothTransition(int targetR, int targetG, int targetB, int steps, int delayMs) {
  for (int i = 0; i <= steps; i++) {
    int r = currentR + (targetR - currentR) * i / steps;
    int g = currentG + (targetG - currentG) * i / steps;
    int b = currentB + (targetB - currentB) * i / steps;

    setColor(r, g, b);
    delay(delayMs);  // small delay for smoothness
  }

  // Set final value
  currentR = targetR;
  currentG = targetG;
  currentB = targetB;
  setColor(currentR, currentG, currentB);
}

// New function for smooth power transition
void smoothPowerTransition(bool targetState, int steps = 30, int delayMs = 10) {
  if (targetState == powerState) return;
  
  if (targetState) {
    // Smooth power on
    for (int i = 0; i <= steps; i++) {
      float factor = (float)i / steps;
      int r = round(currentR * factor);
      int g = round(currentG * factor);
      int b = round(currentB * factor);
      setColor(r, g, b);
      delay(delayMs);
    }
  } else {
    // Smooth power off
    for (int i = steps; i >= 0; i--) {
      float factor = (float)i / steps;
      int r = round(currentR * factor);
      int g = round(currentG * factor);
      int b = round(currentB * factor);
      setColor(r, g, b);
      delay(delayMs);
    }
  }
  
  powerState = targetState;
  applyColor();
}

void processCommand(String command) {
  // RGB color setting command: rgb 255,0,0
  if (command.startsWith("rgb ")) {
    String values = command.substring(4);
    int firstComma = values.indexOf(',');
    int secondComma = values.lastIndexOf(',');

    if (firstComma > 0 && secondComma > firstComma) {
      int newR = constrain(values.substring(0, firstComma).toInt(), 0, 255);
      int newG = constrain(values.substring(firstComma + 1, secondComma).toInt(), 0, 255);
      int newB = constrain(values.substring(secondComma + 1).toInt(), 0, 255);

      if (!powerState) powerState = true;

      smoothTransition(newR, newG, newB, 30, 10);  // 30 steps, 10ms delay
    }
  }
  // Brightness setting command: brightness 50
  else if (command.startsWith("brightness ")) {
    brightness = command.substring(11).toInt();
    brightness = constrain(brightness, 0, 100);
    applyColor();
  }
  
  // Power on/off command: power on/off
  else if (command.startsWith("power ")) {
    String state = command.substring(6);
    if (state == "on") {
      smoothPowerTransition(true);
    } else if (state == "off") {
      smoothPowerTransition(false);
    }
  }

  // Current state request: status
  else if (command == "status") {
    applyColor();  // Sends current state
  }
  // Strip type toggle
  else if (command == "toggle_strip_type") {
    const bool oldType = COMMON_CATHODE;
    *((bool*)&COMMON_CATHODE) = !COMMON_CATHODE;  // Workaround to change const
    Serial.print("Strip type changed from ");
    Serial.print(oldType ? "COMMON_CATHODE" : "COMMON_ANODE");
    Serial.print(" to ");
    Serial.println(COMMON_CATHODE ? "COMMON_CATHODE" : "COMMON_ANODE");
    applyColor();
  }
  // Unknown command
  else {
    Serial.println("{\"error\":\"Unknown command\"}");
  }
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // Remove extra spaces
    processCommand(command);
  }
}
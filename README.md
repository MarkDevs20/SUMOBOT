## Developer
Markanthonydevs

# Arduino Sumobot with Line Detection

A competitive sumobot robot built with Arduino that uses ultrasonic sensors for enemy detection and infrared line sensors for ring boundary detection.

## Features

- **Ultrasonic Enemy Detection**: Detects opponents within 40cm and attacks
- **5-Sensor Line Detection**: Prevents the robot from leaving the ring
- **IR Remote Control**: Easy operation with remote control buttons
- **Two-Step Calibration**: Simple calibration process for line sensors
- **Attack & Search Mode**: Automatically searches and attacks enemies

## Hardware Requirements

### Components
- Arduino Uno
- Adafruit Motor Shield V1
- 2x DC Motors
- HC-SR04 Ultrasonic Sensor
- 5x IR Line Sensors (Analog)
- IR Receiver Module
- IR Remote Control

### Pin Configuration
```
IR Receiver    → A0
Line Sensor 1  → A1
Line Sensor 2  → A2
Line Sensor 3  → A3
Line Sensor 4  → A4
Line Sensor 5  → A5
Ultrasonic Trig → Pin 10
Ultrasonic Echo → Pin 9
Left Motor     → Motor Port 3
Right Motor    → Motor Port 4
```

## Software Requirements

### Arduino Libraries
- `IRremote` - For IR remote control
- `AFMotor` - For Adafruit Motor Shield control

Install via Arduino Library Manager:
```
Sketch → Include Library → Manage Libraries
Search and install: "IRremote" and "Adafruit Motor Shield library"
```

## Setup Instructions

### 1. Upload Code
1. Connect Arduino to your computer
2. Open the `.ino` file in Arduino IDE
3. Select your board: `Tools → Board → Arduino Uno`
4. Select your port: `Tools → Port → [Your Arduino Port]`
5. Click Upload

### 2. IR Remote Configuration
Configure your IR remote button codes in the code:
```cpp
#define POWER_BTN 0x57A8FF00  
#define SUMO_BTN  0x22DDFF00
#define CALIB_BTN 0xBE41FF00
```

To find your remote codes:
1. Open Serial Monitor (9600 baud)
2. Press buttons on your remote
3. Copy the hex codes displayed
4. Update the `#define` values in the code

### 3. Calibrate Line Sensors

**IMPORTANT: Calibration must be done before first use!**

1. Open Serial Monitor (9600 baud)
2. Place robot on **BLACK LINE**
3. Press **CALIB** button on remote
4. Wait for confirmation message
5. Place robot on **WHITE surface**
6. Press **CALIB** button again
7. Calibration complete!

## Usage

### Operating the Sumobot

1. **Power On**: Press `POWER` button
2. **Start Fighting**: Press `SUMO` button
3. **Stop Fighting**: Press `SUMO` button again
4. **Power Off**: Press `POWER` button

### Robot Behavior

The sumobot operates in three modes:

**Line Detection (Priority 1)**
- When any sensor detects the black boundary line
- Action: Backs up for 500ms to stay in ring

**Enemy Detected (Priority 2)**
- When ultrasonic detects object < 40cm
- Action: Charges forward at full speed (255)

**Search Mode (Priority 3)**
- When no enemy is detected
- Action: Spins in place to search (speed 200)

## Troubleshooting

### Line Sensors Not Working
- Check all 5 sensors are connected to A1-A5
- Re-run calibration process
- Ensure good contrast between black line and white surface
- Check Serial Monitor for sensor values during calibration

### Robot Not Detecting Enemy
- Check ultrasonic sensor connections (Trig=10, Echo=9)
- Verify enemy is within 40cm range
- Check Serial Monitor for distance readings

### IR Remote Not Responding
- Check IR receiver is connected to A0
- Verify IR codes match your remote
- Ensure fresh batteries in remote
- Check Serial Monitor for received codes

### Motors Not Moving
- Check motor shield connections
- Verify motors are in ports 3 and 4
- Check motor power supply
- Test motors individually

## Customization

### Adjust Detection Range
Change enemy detection distance (default 40cm):
```cpp
if (dist > 0 && dist < 40) {  // Change 40 to desired distance
```

### Adjust Motor Speed
Modify attack and search speeds:
```cpp
motorLeft.setSpeed(255);   // Attack speed (0-255)
motorRight.setSpeed(255);

// Search mode speed
motorLeft.setSpeed(200);   // Search speed (0-255)
motorRight.setSpeed(200);
```

### Adjust Backup Duration
Change how long robot backs up when hitting line:
```cpp
delay(500); // Change 500ms to desired duration
```

## Competition Tips

1. **Calibrate Before Each Match**: Lighting conditions affect sensors
2. **Test Detection Range**: Verify 40cm detection works in your arena
3. **Check Battery Levels**: Low battery affects motor speed
4. **Clean Sensors**: Dust can affect line detection
5. **Secure Connections**: Check all wires before competing

## License

MIT License - Feel free to modify and use for your projects

## Contributing

Pull requests are welcome! For major changes, please open an issue first to discuss proposed changes.



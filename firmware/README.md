# ShotBot Firmware

This directory contains the ESP32 firmware used to control **ShotBot**, a stair-climbing delivery robot developed as part of the Stanford Moonshot Club.

The firmware provides Bluetooth gamepad control through the Bluepad32 library and directly controls three BTS7960 motor-driver channels:

- Left drive motor
- Right drive motor
- Front / stair-climbing motor

The firmware contained in this directory was used on the working ShotBot prototype.

---

## Firmware Structure

The ShotBot firmware is intentionally compact and is implemented as a single Arduino sketch.

```text
firmware/
├── README.md
└── ShotBotCode/
    └── ShotBotCode.ino
```

The complete ESP32 control firmware is located at:

`ShotBotCode/ShotBotCode.ino`

The sketch contains the Bluetooth gamepad interface, motor-driver configuration, PWM generation, drivetrain control, stair-climbing motor control, and controller-disconnect safety behavior.

---

## Hardware Interface

The firmware runs on an **ESP32 DevKit** mounted to the custom ShotBot interconnect PCB.

The ESP32 controls three BTS7960 H-bridge motor drivers:

| Motor | Function |
| --- | --- |
| M1 | Left drive motor |
| M2 | Front / stair-climbing motor |
| M3 | Right drive motor |

Each motor driver receives independent forward and reverse PWM signals from the ESP32.

The electrical connections between the ESP32 and the motor drivers are defined by the custom interconnect PCB.

For additional electrical documentation, see:

[`../electrical/`](../electrical/)

---

## ESP32 Pin Assignments

### Motor Enable Pins

| Motor | ESP32 GPIO |
| --- | ---: |
| M1 - Left Drive | GPIO 23 |
| M2 - Front / Stair Climber | GPIO 16 |
| M3 - Right Drive | GPIO 17 |

### Motor PWM Pins

| Motor | LPWM | RPWM |
| --- | ---: | ---: |
| M1 - Left Drive | GPIO 25 | GPIO 26 |
| M2 - Front / Stair Climber | GPIO 27 | GPIO 14 |
| M3 - Right Drive | GPIO 33 | GPIO 32 |

The current firmware uses one shared enable signal per BTS7960 module and independent forward/reverse PWM channels for motor direction and speed control.

---

## Gamepad Control

ShotBot is operated using a Bluetooth-compatible gamepad connected to the ESP32 through the **Bluepad32** library.

The firmware continuously reads the connected controller and converts joystick and button inputs into motor commands.

### Drive Controls

The rear drivetrain uses tank-style control.

| Controller Input | Function |
| --- | --- |
| Left joystick Y-axis | Left drive motor |
| Right joystick Y-axis | Right drive motor |

Each side of the drivetrain is controlled independently, allowing forward motion, reverse motion, and differential turning.

### Stair-Climbing Motor

The front motor controls the stair-climbing mechanism.

| Controller Input | Function |
| --- | --- |
| R1 | Front motor forward |
| L1 | Front motor reverse |
| R1 + L1 | Stop front motor |
| Neither pressed | Stop front motor |

---

## PWM Configuration

The ESP32 LEDC peripheral generates PWM signals for the three BTS7960 motor drivers.

| Parameter | Value |
| --- | ---: |
| PWM frequency | 20 kHz |
| PWM resolution | 8-bit |
| Full PWM range | 0-255 |
| Maximum drive-motor output | 102 / 255 |
| Maximum front-motor output | 255 / 255 |
| Joystick deadzone | 8% |
| Main loop delay | 10 ms |

The two rear drive motors are intentionally limited to approximately **40% of the available PWM range**.

The front stair-climbing motor is permitted to use the full PWM range.

An 8% joystick deadzone is applied around the neutral position to prevent small joystick offsets from causing unintended motor motion.

---

## Motor Control

Motor direction is controlled by driving one BTS7960 PWM channel while holding the opposite channel at zero.

Conceptually:

```text
Positive command  -> Forward PWM active, reverse PWM = 0
Negative command  -> Reverse PWM active, forward PWM = 0
Zero command      -> Both PWM channels = 0
```

All requested motor commands are constrained to the configured maximum PWM value before being written to the motor drivers.

This allows the drivetrain and stair-climbing mechanism to use different output limits while sharing the same motor-control function.

---

## Safety Behavior

The firmware includes several basic fail-safe behaviors intended to prevent unintended motor operation.

### Startup

All BTS7960 enable outputs are disabled during ESP32 initialization.

The motors therefore remain inactive while the controller interface and PWM hardware are being configured.

### Controller Connection

When a supported gamepad connects successfully, the firmware stores the active controller and enables the motor drivers.

### Controller Disconnect

If the Bluetooth controller disconnects:

1. All motor PWM commands are set to zero.
2. All BTS7960 enable outputs are disabled.
3. The robot remains stopped until a controller reconnects.

The main control loop also continuously verifies that a controller remains connected. If no valid controller is available, the firmware repeatedly stops the motors and disables the drivers.

---

## Bluepad32

ShotBot uses **Bluepad32** to provide Bluetooth gamepad support on the ESP32.

The firmware registers callbacks for:

- Gamepad connection
- Gamepad disconnection

Once a controller is connected, joystick axes and shoulder-button states are read continuously in the main loop.

Bluepad32 handles the underlying Bluetooth controller interface, allowing the ShotBot firmware to focus on interpreting commands and controlling the motors.

---

## IMU Development

The ShotBot electrical system also includes an **MPU9250 IMU** connected to the ESP32 over I²C.

The IMU interface was tested during development and produced valid sensor feedback.

The sensor was originally intended to support more advanced feedback and future robot-state estimation. However, because the completed ShotBot prototype was operated remotely, IMU feedback was not required by the final teleoperation firmware.

The final controller sketch therefore focuses on gamepad and motor control rather than closed-loop IMU-based behavior.

Additional IMU hardware information is documented in the electrical section of the repository.

---

## Motor Driver Current Feedback

The BTS7960 current-sense connections were included in the ShotBot electrical hardware and routed through the custom interconnect PCB.

These signals were available for potential current monitoring and future protection or diagnostic functionality, but they were not incorporated into the final motor-control logic used by the working teleoperated prototype.

---

## Development Testing

During development, temporary test sketches were used to validate individual parts of the electrical and control system before the final firmware was integrated.

Testing included:

- Individual BTS7960 motor-driver operation
- Motor direction control
- PWM speed control
- Three-motor integration
- Bluetooth gamepad communication
- Controller disconnect behavior
- MPU9250 I²C communication
- Final integration with the custom interconnect PCB

These development-only test sketches were not retained.

The repository therefore contains the final integrated firmware used on the working ShotBot prototype rather than the temporary subsystem test programs.

---

## Building and Uploading

The firmware was developed for an ESP32 using the Arduino framework.

### Requirements

- ESP32-compatible Arduino development environment
- ESP32 board support package
- Bluepad32 library
- USB connection to the ESP32 DevKit

### Upload Procedure

1. Install an Arduino-compatible ESP32 development environment.
2. Install the required ESP32 board support package.
3. Install Bluepad32 and its required ESP32 support.
4. Open:

   `ShotBotCode/ShotBotCode.ino`

5. Select the appropriate ESP32 DevKit target.
6. Select the correct serial port.
7. Compile the sketch.
8. Upload the firmware to the ESP32.
9. Open the serial monitor at **115200 baud** if connection-status messages are desired.
10. Pair a compatible Bluetooth gamepad with the ESP32.

Once a controller connects successfully, the motor drivers are enabled and ShotBot can be operated using the controls described above.

Exact development-tool and library versions were not retained from the original development environment and are therefore not specified here.

---

## Serial Output

The firmware uses a serial baud rate of:

```text
115200 baud
```

Serial output provides basic controller-status information, including:

```text
Bluepad32 ready. Pair a controller to begin.
Controller connected.
Controller disconnected.
```

The serial connection is not required during normal robot operation.

---

## Firmware Scope

The final ShotBot firmware was designed for **reliable remote operation of the completed prototype**.

The implemented firmware provides:

- Bluetooth gamepad control
- Independent left and right drivetrain control
- Bidirectional stair-climbing motor control
- PWM output limiting
- Joystick deadzone handling
- Motor-driver enable control
- Controller-disconnect fail-safe behavior

More advanced features considered during development, including IMU-based feedback and motor-current monitoring, were electrically supported but were not required for the final teleoperated implementation.



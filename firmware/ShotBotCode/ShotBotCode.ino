#include <Bluepad32.h>

// ============================================================================
// ShotBot ESP32 Motor Controller
// ============================================================================
// Controls:
//   - Left drive motor (M1):  Left joystick Y-axis
//   - Right drive motor (M3): Right joystick Y-axis
//   - Front/flipper motor (M2):
//       R1 -> forward
//       L1 -> reverse
//
// Hardware:
//   - ESP32 DevKit
//   - 3x BTS7960 motor drivers
//   - Bluetooth gamepad via Bluepad32
//
// Safety behavior:
//   - Motor drivers remain disabled until a controller is connected.
//   - All motors stop and drivers are disabled if the controller disconnects.
// ============================================================================


// ============================================================================
// Motor Enable Pins
// ============================================================================

constexpr int EN_M1 = 23;  // Left drive motor
constexpr int EN_M2 = 16;  // Front / flipper motor
constexpr int EN_M3 = 17;  // Right drive motor


// ============================================================================
// Motor PWM Pins
// ============================================================================

// M1 - Left drive motor
constexpr int M1_LPWM = 25;
constexpr int M1_RPWM = 26;

// M2 - Front / flipper motor
constexpr int M2_LPWM = 27;
constexpr int M2_RPWM = 14;

// M3 - Right drive motor
constexpr int M3_LPWM = 33;
constexpr int M3_RPWM = 32;


// ============================================================================
// PWM Configuration
// ============================================================================

constexpr int PWM_FREQ = 20000;  // 20 kHz
constexpr int PWM_RES  = 8;      // 8-bit resolution: 0-255

constexpr int MAX_PWM_DRIVE   = 102;  // 40% maximum drive output
constexpr int MAX_PWM_FLIPPER = 255;  // 100% maximum flipper output

// LEDC PWM channels
constexpr int CH_M1_FWD = 0;
constexpr int CH_M1_REV = 1;

constexpr int CH_M3_FWD = 2;
constexpr int CH_M3_REV = 3;

constexpr int CH_M2_FWD = 4;
constexpr int CH_M2_REV = 5;


// ============================================================================
// Controller Configuration
// ============================================================================

constexpr float DRIVE_DEADZONE = 0.08f;
constexpr float GAMEPAD_AXIS_MAX = 512.0f;

constexpr unsigned long LOOP_DELAY_MS = 10;

GamepadPtr pad = nullptr;


// ============================================================================
// Motor Control Functions
// ============================================================================

/**
 * Enable or disable all BTS7960 motor drivers.
 */
void setEnableAll(bool enabled) {
    digitalWrite(EN_M1, enabled ? HIGH : LOW);
    digitalWrite(EN_M2, enabled ? HIGH : LOW);
    digitalWrite(EN_M3, enabled ? HIGH : LOW);
}


/**
 * Command a motor using its forward and reverse LEDC channels.
 *
 * Positive value -> forward
 * Negative value -> reverse
 * Zero           -> stop
 */
void setMotor(int channelForward,
              int channelReverse,
              int value,
              int maxPwm) {

    value = constrain(value, -maxPwm, maxPwm);

    if (value > 0) {
        ledcWrite(channelForward, value);
        ledcWrite(channelReverse, 0);

    } else if (value < 0) {
        ledcWrite(channelForward, 0);
        ledcWrite(channelReverse, -value);

    } else {
        ledcWrite(channelForward, 0);
        ledcWrite(channelReverse, 0);
    }
}


/**
 * Stop all three motors.
 */
void stopAllMotors() {
    setMotor(CH_M1_FWD, CH_M1_REV, 0, MAX_PWM_DRIVE);
    setMotor(CH_M3_FWD, CH_M3_REV, 0, MAX_PWM_DRIVE);
    setMotor(CH_M2_FWD, CH_M2_REV, 0, MAX_PWM_FLIPPER);
}


/**
 * Remove small joystick inputs around the neutral position.
 */
float applyDeadzone(float value, float deadzone) {
    if (fabs(value) < deadzone) {
        return 0.0f;
    }

    return value;
}


// ============================================================================
// Bluepad32 Callbacks
// ============================================================================

void onConnectedGamepad(GamepadPtr gamepad) {
    pad = gamepad;

    Serial.println("Controller connected.");

    setEnableAll(true);
}


void onDisconnectedGamepad(GamepadPtr gamepad) {
    if (pad == gamepad) {
        pad = nullptr;
    }

    Serial.println("Controller disconnected.");

    stopAllMotors();
    setEnableAll(false);
}


// ============================================================================
// Setup
// ============================================================================

void setup() {
    Serial.begin(115200);

    // Configure motor-driver enable pins.
    pinMode(EN_M1, OUTPUT);
    pinMode(EN_M2, OUTPUT);
    pinMode(EN_M3, OUTPUT);

    // Keep motor drivers disabled during startup.
    setEnableAll(false);

    // Configure LEDC PWM channels.
    ledcSetup(CH_M1_FWD, PWM_FREQ, PWM_RES);
    ledcSetup(CH_M1_REV, PWM_FREQ, PWM_RES);

    ledcSetup(CH_M3_FWD, PWM_FREQ, PWM_RES);
    ledcSetup(CH_M3_REV, PWM_FREQ, PWM_RES);

    ledcSetup(CH_M2_FWD, PWM_FREQ, PWM_RES);
    ledcSetup(CH_M2_REV, PWM_FREQ, PWM_RES);

    // Attach ESP32 GPIO pins to their respective PWM channels.
    ledcAttachPin(M1_LPWM, CH_M1_FWD);
    ledcAttachPin(M1_RPWM, CH_M1_REV);

    ledcAttachPin(M3_LPWM, CH_M3_FWD);
    ledcAttachPin(M3_RPWM, CH_M3_REV);

    ledcAttachPin(M2_LPWM, CH_M2_FWD);
    ledcAttachPin(M2_RPWM, CH_M2_REV);

    stopAllMotors();

    // Initialize Bluetooth controller support.
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    Serial.println("Bluepad32 ready. Pair a controller to begin.");
}


// ============================================================================
// Main Loop
// ============================================================================

void loop() {
    BP32.update();

    // Fail-safe: stop and disable all motors when no controller is connected.
    if (!pad || !pad->isConnected()) {
        stopAllMotors();
        setEnableAll(false);

        delay(LOOP_DELAY_MS);
        return;
    }

    setEnableAll(true);


    // ------------------------------------------------------------------------
    // Drive Control
    //
    // Tank drive:
    //   Left joystick Y-axis  -> M1
    //   Right joystick Y-axis -> M3
    // ------------------------------------------------------------------------

    const int leftAxis  = pad->axisY();
    const int rightAxis = pad->axisRY();

    float leftCommand  = leftAxis / GAMEPAD_AXIS_MAX;
    float rightCommand = rightAxis / GAMEPAD_AXIS_MAX;

    leftCommand  = applyDeadzone(leftCommand, DRIVE_DEADZONE);
    rightCommand = applyDeadzone(rightCommand, DRIVE_DEADZONE);

    leftCommand  = constrain(leftCommand, -1.0f, 1.0f);
    rightCommand = constrain(rightCommand, -1.0f, 1.0f);

    const int m1Pwm = static_cast<int>(leftCommand * MAX_PWM_DRIVE);
    const int m3Pwm = static_cast<int>(rightCommand * MAX_PWM_DRIVE);

    setMotor(CH_M1_FWD, CH_M1_REV, m1Pwm, MAX_PWM_DRIVE);
    setMotor(CH_M3_FWD, CH_M3_REV, m3Pwm, MAX_PWM_DRIVE);


    // ------------------------------------------------------------------------
    // Front / Flipper Control
    //
    // R1 -> forward at full PWM
    // L1 -> reverse at full PWM
    // Both or neither -> stop
    // ------------------------------------------------------------------------

    const bool r1Pressed = pad->r1();
    const bool l1Pressed = pad->l1();

    int m2Pwm = 0;

    if (r1Pressed && !l1Pressed) {
        m2Pwm = MAX_PWM_FLIPPER;

    } else if (l1Pressed && !r1Pressed) {
        m2Pwm = -MAX_PWM_FLIPPER;
    }

    setMotor(CH_M2_FWD, CH_M2_REV, m2Pwm, MAX_PWM_FLIPPER);

    delay(LOOP_DELAY_MS);
}
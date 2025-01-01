#include "TensorFlowLite.h" // TensorFlow Lite library for microcontrollers
#include "model_data.h" // Include the converted model file as a C array
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include <Stepper.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// Define constants
constexpr int kTensorArenaSize = 2 * 1024; // Adjust based on available memory
uint8_t tensor_arena[kTensorArenaSize]; // Tensor arena for model data

// Model setup
const tflite::Model* model = tflite::GetModel(model_data); // Load the TFLite model

// TensorFlow Lite interpreter
tflite::MicroInterpreter* interpreter = nullptr;

// Define the sensor pins (adjust as needed)
const int irSensorPin = 2;
const int metalSensorPin = 3;
const int moistureSensorPin = A0;
const int moist_threshold = 350;
const int servoMotorPin = 5;
const int setpositionPin = 7;

Stepper stepper(2048, 8, 10, 9, 11);
Servo servoMotor;
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns and 2 rows

void setposition();

void setup() {
  Serial.begin(9600);
  
  // Initialize the sensor pins
  pinMode(irSensorPin, INPUT);
  pinMode(metalSensorPin, INPUT);
  pinMode(moistureSensorPin, INPUT);
  pinMode(setpositionPin, INPUT_PULLUP);

  // Initialize stepper motor
  stepper.setSpeed(15);

  // Initialize servo motor
  servoMotor.attach(servoMotorPin);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Waste Detection:");

  // Initialize TensorFlow Lite interpreter
  static tflite::AllOpsResolver resolver;
  interpreter = new tflite::MicroInterpreter(model, resolver, tensor_arena, kTensorArenaSize, nullptr);

  // Allocate memory for input/output tensors
  interpreter->AllocateTensors();

  Serial.println("Model loaded and interpreter initialized.");
}

void loop() {
  stepper.step(0);
  setposition();

  // Read actual sensor data
  int irValue = digitalRead(irSensorPin); // Read the IR sensor (digital)
  int metalValue = digitalRead(metalSensorPin); // Read the metal detector (digital)
  int moistureValue = analogRead(moistureSensorPin); // Read the moisture sensor (analog)

  Serial.print("IR: ");
  Serial.print(irValue);
  Serial.print(" | Metal: ");
  Serial.print(metalValue);
  Serial.print(" | Moisture: ");
  Serial.println(moistureValue);

  lcd.setCursor(0, 1);

  // Normalize the data if needed (you may need to scale or transform values)
  // For simplicity, we are using the raw values directly. Consider scaling or normalization if required.
  float input_data[3] = {float(irValue), float(metalValue), float(moistureValue)};

  // Fill the input tensor with the data
  TfLiteTensor* input_tensor = interpreter->input(0);
  for (int i = 0; i < 3; i++) {
    input_tensor->data.f[i] = input_data[i];
  }

  // Run inference
  interpreter->Invoke();

  // Get the output
  TfLiteTensor* output_tensor = interpreter->output(0);
  float output_data[3];
  for (int i = 0; i < 3; i++) {
    output_data[i] = output_tensor->data.f[i];
  }

  // Determine the waste category (based on output)
  int category = -1;
  float max_value = -1.0;
  for (int i = 0; i < 3; i++) {
    if (output_data[i] > max_value) {
      max_value = output_data[i];
      category = i;
    }
  }

  // Display waste category on LCD and serial monitor
  if (category == 0) {
    Serial.println("Dry Waste Detected");
    lcd.print("Dry Waste Detected");
    segregateDryWaste();
  } else if (category == 1) {
    Serial.println("Metal Waste Detected");
    lcd.print("Metal Waste Detected");
    segregateMetalWaste();
  } else if (category == 2) {
    Serial.println("Wet Waste Detected");
    lcd.print("Wet Waste Detected");
    segregateWetWaste();
  }

  delay(1000); // Wait before next loop
}

void segregateDryWaste() {
  Serial.println("Segregating Dry Waste");
  rotateToBin(1);
}

void segregateMetalWaste() {
  Serial.println("Segregating Metal Waste");
  rotateToBin(2);
}

void segregateWetWaste() {
  Serial.println("Segregating Wet Waste");
  rotateToBin(3);
}

void rotateToBin(int binIndex) {
  switch (binIndex) {
    case 1:
      stepper.step(0);
      delay(500);
      openBinCover();
      delay(4000);
      closeBinCovers();
      break;

    case 2:
      stepper.step(682);
      delay(500);
      openBinCover();
      delay(4000);
      closeBinCovers();
      stepper.step(-682);
      break;

    case 3:
      stepper.step(1364);
      delay(500);
      openBinCover();
      delay(4000);
      closeBinCovers();
      stepper.step(-1364);
      break;
  }
}

void openBinCover() {
  servoMotor.write(0);
}

void closeBinCovers() {
  servoMotor.write(90);
}

void setposition() {
  if (digitalRead(setpositionPin) == LOW) {
    stepper.step(50);
  }
}

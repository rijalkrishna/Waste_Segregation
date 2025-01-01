# Waste Segregation System with TensorFlow Lite

This project uses TensorFlow Lite for Microcontrollers to classify waste into three categories: **Dry**, **Metal**, and **Wet** based on data from various sensors. The system integrates an IR sensor, metal sensor, and moisture sensor, and controls a stepper motor and servo to segregate waste into different bins. The classification is performed using a pre-trained machine learning model converted to TensorFlow Lite format.

## Components Used

- **IR Sensor**: Detects the presence of waste.
- **Metal Sensor**: Detects metal waste.
- **Moisture Sensor**: Detects wet waste based on moisture content.
- **Stepper Motor**: Controls the movement of the bin covers.
- **Servo Motor**: Opens and closes the bin covers for waste segregation.
- **LCD Display**: Displays the detected waste category.
- **Arduino**: Controls the sensors, motors, and inference process.

## Libraries Used

- **TensorFlow Lite for Microcontrollers**: For running the machine learning model on the microcontroller.
- **Stepper**: Controls the stepper motor for bin rotation.
- **Servo**: Controls the servo motor for opening/closing the bin covers.
- **LiquidCrystal_I2C**: Manages the LCD display.

## Circuit Setup

1. **IR Sensor**: Connect to pin 2.
2. **Metal Sensor**: Connect to pin 3.
3. **Moisture Sensor**: Connect to analog pin A0.
4. **Servo Motor**: Connect to pin 5.
5. **Stepper Motor**: Connect to pins 8, 9, 10, and 11.
6. **Set Position Pin**: Connect to pin 7 (used to adjust stepper position).

## Code Overview

### Setup
- The model is loaded from the `model_data.h` file.
- Sensor pins are initialized.
- The stepper motor and servo are configured.
- The LCD is initialized to display "Waste Detection".

### Loop
1. **Sensor Readings**: The system reads values from the IR, metal, and moisture sensors.
2. **TensorFlow Lite Inference**: The sensor data is passed to the TensorFlow Lite model for classification. The model determines if the waste is dry, metal, or wet.
3. **Waste Segregation**: Based on the classification, the system uses the stepper motor to rotate to the appropriate bin and the servo motor to open the bin cover.
4. **Display**: The detected waste category is displayed on the LCD and printed to the serial monitor.

### Waste Segregation Logic
- **Dry Waste**: If the waste is detected as dry, the system will rotate to the dry waste bin.
- **Metal Waste**: If metal waste is detected, the system will rotate to the metal waste bin.
- **Wet Waste**: If wet waste is detected, the system will rotate to the wet waste bin.

### TensorFlow Lite Model
The model used for classification is a pre-trained TensorFlow Lite model, which is included in the `model_data.h` file. The model is loaded into the system, and its output is used to determine the waste category.

## File Overview

### `model.tflite`
This is the TensorFlow Lite model file that has been trained to classify waste into three categories: **Dry**, **Metal**, and **Wet**. It is optimized for use with microcontrollers and is loaded in the Arduino code using the `model_data.h` header file.

### `training.ipynb`
This is a Jupyter notebook used to train the machine learning model. It contains the steps to:
1. Load and preprocess the `waste_classification_dataset.csv`.
2. Train a machine learning model (likely a neural network or decision tree) on the dataset.
3. Convert the trained model to the TensorFlow Lite format using the TensorFlow Lite converter.
4. Save the resulting model to `model.tflite`, which can then be used in the Arduino project.

### `model_data.h`
This header file contains the converted TensorFlow Lite model as a C array. It is included in the Arduino code to allow the microcontroller to load and run the model for waste classification. The model is stored as an array to be embedded directly into the Arduino program.

### `waste_classification_dataset.csv`
This CSV file contains the data used for training the machine learning model. It includes features from various sensors (such as IR, metal, and moisture sensor readings) and labels that represent the waste category (dry, metal, or wet). The dataset is loaded and preprocessed in the `training.ipynb` notebook for model training.

### `scaler.pkl`
This file contains the scaler used to normalize the sensor data before it is fed into the model. The scaler was trained on the dataset and is used to scale sensor values to a similar range, ensuring that the model performs accurately. This scaler can be used on the Arduino to preprocess sensor readings in the same way as during training.

### `waste_detection.ino`
This is the main Arduino sketch file that runs on the Arduino board. It reads data from the IR, metal, and moisture sensors, passes the data to the TensorFlow Lite model, and then uses the output to control the stepper motor and servo motor to segregate waste. The waste category is also displayed on the LCD screen.

## Requirements

- Arduino IDE
- TensorFlow Lite for Microcontrollers library
- Stepper library
- Servo library
- LiquidCrystal_I2C library

## How to Use

1. **Set up the circuit** as described above.
2. **Install the required libraries** in Arduino IDE (Stepper, Servo, LiquidCrystal_I2C, TensorFlow Lite for Microcontrollers).
3. **Upload the `waste_detection.ino` code** to your Arduino board.
4. The system will begin reading sensor data and classifying waste. The detected waste category will be displayed on the LCD, and the waste will be segregated accordingly.

## Notes

- The system uses a pre-trained TensorFlow Lite model to classify waste. If you'd like to train your own model, you can use the `training.ipynb` notebook to create and convert a new model, then update the `model_data.h` file with the new model.
- The waste segregation logic is based on sensor values and model predictions. Make sure to calibrate your sensors as needed.



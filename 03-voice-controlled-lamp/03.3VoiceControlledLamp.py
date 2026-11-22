import serial
import speech_recognition as sr

# Serial to ESP32
ser = serial.Serial('COM3', 9600, timeout=1)

recognizer = sr.Recognizer()
print("Voice control system started...")

try:
    while True:
        try:
            with sr.Microphone() as source:
                recognizer.adjust_for_ambient_noise(source, duration=0.5)
                print("Say a command...")
                audio = recognizer.listen(source, phrase_time_limit=2)

            print("Processing...")
            text = recognizer.recognize_google(audio, language='ar-EG')
            print(f'You said: {text}')

            # ----------- Turning on the Lamb-----------
            if "turn on" in text or "turn on" in text:
                ser.write(b'1')
                print("→ Lamp turned ON")

            # ----------- Turning off the Lamb-----------
            elif "turn off" in text:
                ser.write(b'0')
                print("→ Lamp turned OFF")

        except sr.UnknownValueError:
            print("Could not understand the audio.")
        except sr.RequestError:
            print("Google API request failed.")

except KeyboardInterrupt:
    print("\nExiting...")
    ser.close()

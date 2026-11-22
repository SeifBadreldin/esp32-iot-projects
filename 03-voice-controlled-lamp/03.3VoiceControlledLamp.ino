int lamp = 32; // GPIO 5
char data;

void setup() {
  Serial.begin(9600);
  pinMode(lamp, OUTPUT);
  digitalWrite(lamp, LOW); // البداية مطفي
}

void loop() {
  if (Serial.available()) {
    data = Serial.read();
    if (data == '1') digitalWrite(lamp, HIGH);
    else if (data == '0') digitalWrite(lamp, LOW);
  }
}

int battery = A4;
int battery_Level;

void setup() {
  // put your setup code here, to run once:
  pinMode(battery, INPUT);
  Serial.begin(9600);
}

void loop() {
  battery_Level = analogRead(battery);
  int data = map(battery_Level, 10, 930, 0, 100);
  Serial.println(data);
  // put your main code here, to run repeatedly:
}

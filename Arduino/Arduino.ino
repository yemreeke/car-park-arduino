int IRpin = 7;  // IR sensörün bağlı olduğu data pini
void setup() {
  pinMode(PIRpin,INPUT);  
  Serial.begin(9600);  // Seri portu başlat
}
void loop() {
  Serial.println(digitalRead(IRpin));  // IR sensörden okunan veriyi seri port ekranına yaz
  delay(500);  // Yarım saniye bekle
}
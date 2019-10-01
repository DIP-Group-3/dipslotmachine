int buzzer = 4; // default pin 4

void CoinInsert() {
  tone(buzzer, 1000);
  delay(50);
  tone(buzzer, 1500);
  delay(50);
  noTone(buzzer);
  delay(2000);
}

void BetIncrement() {
  tone(buzzer, 500);
  delay(100);
  noTone(buzzer);
  delay(2000);
}

void SpinActivate() {
  tone(buzzer, 600);
  delay(50);
  tone(buzzer, 800);
  delay(50);
  tone(buzzer, 600);
  delay(50);
  tone(buzzer, 800);
  delay(50);
  tone(buzzer, 600);
  delay(50);
  tone(buzzer, 800);
  delay(150);
  noTone(buzzer);
  delay(2000);
}

void WinCondition() {
  tone(buzzer, 800);
  delay(80);
  tone(buzzer, 600);
  delay(80);
  tone(buzzer, 1000);
  delay(80);
  tone(buzzer, 800);
  delay(200);
  noTone(buzzer);
  delay(2000);
}

void Jackpot() {
  tone(buzzer, 800);
  delay(50);
  tone(buzzer, 1000);
  delay(50);
  tone(buzzer, 800);
  delay(50);
  tone(buzzer, 1000);
  delay(50);
  tone(buzzer, 800);
  delay(50);
  tone(buzzer, 1000);
  delay(50);
  tone(buzzer, 800);
  delay(50);
  tone(buzzer, 1000);
  delay(50);
  tone(buzzer, 800);
  delay(50);
  tone(buzzer, 1600);
  delay(100);
  noTone(buzzer);
  delay(2000);
}

void setup() {
  //CoinInsert();
  //BetIncrement();
  //SpinActivate();
  //WinCondition();
  Jackpot();
}

void loop() {

}

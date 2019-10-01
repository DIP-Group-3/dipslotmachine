int buzzer = 4; // default pin 4

void CoinInsertSFX() {
  tone(buzzer, 1000);
  delay(50);
  tone(buzzer, 1500);
  delay(50);
  noTone(buzzer);
  delay(2000);
}

void BetIncrementSFX() {
  tone(buzzer, 500);
  delay(100);
  noTone(buzzer);
  delay(2000);
}

void SpinActivateSFX() {
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

void WinConditionSFX() {
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

void JackpotSFX() {
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
  //CoinInsertSFX();
  //BetIncrementSFX();
  //SpinActivateSFX();
  //WinConditionSFX();
  JackpotSFX();
}

void loop() {

}

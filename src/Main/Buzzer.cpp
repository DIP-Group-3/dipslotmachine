int buzzer = 4; // default pin 4

void StartUpSFX() {
  tone(buzzer, 260);
  delay(250);
  tone(buzzer, 330);
  delay(120);
  tone(buzzer, 349);
  delay(120);
  tone(buzzer, 392);
  delay(250);
  tone(buzzer, 490);
  delay(250);
  tone(buzzer, 523);
  delay(200);
  noTone(buzzer);
  delay(2000);
}

void CoinInsertSFX() {
  tone(buzzer, 1000);
  delay(70);
  tone(buzzer, 1500);
  delay(70);
  noTone(buzzer);
  delay(2000);
}

void BetIncrementSFX() {
  tone(buzzer, 1319);
  delay(90);
  tone(buzzer, 1047);
  delay(90);
  tone(buzzer, 1319);
  delay(90);
  tone(buzzer, 1047);
  delay(200);
  noTone(buzzer);
  delay(2000);
}

void SpinActivateSFX() {
  for (int count = 0; count < 8; count++) {
    tone(buzzer, 400);
    delay(70);
    tone(buzzer, 600);
    delay(70);
  }
  noTone(buzzer);
  delay(2000);
}

void LoseSFX() {
  tone(buzzer, 1500);
  delay(200);
  tone(buzzer, 1250);
  delay(200);
  tone(buzzer, 1000);
  delay(200);
  tone(buzzer, 1100);
  delay(250);
  noTone(buzzer);
  delay(2000);
}

void WinConditionSFX() {
  tone(buzzer, 900);
  delay(80);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 900);
  delay(250);
  noTone(buzzer);
  delay(50);
  tone(buzzer, 900);
  delay(100);
  tone(buzzer, 1150);
  delay(100);
  tone(buzzer, 1350);
  delay(250);
  tone(buzzer, 1150);
  delay(100);
  tone(buzzer, 1350);
  delay(300);
  noTone(buzzer);
  delay(2000);
}

void JackpotSFX() {
  tone(buzzer, 196);
  delay(150);
  tone(buzzer, 262);
  delay(150);
  tone(buzzer, 330);
  delay(150);
  tone(buzzer, 392);
  delay(150);
  tone(buzzer, 523);
  delay(150);
  tone(buzzer, 659);
  delay(150);
  tone(buzzer, 784);
  delay(450);
  tone(buzzer, 659);
  delay(450);

  noTone(buzzer);
  delay(50);

  tone(buzzer, 116);
  delay(150);
  tone(buzzer, 294);
  delay(150);
  tone(buzzer, 311);
  delay(150);
  tone(buzzer, 415);
  delay(150);
  tone(buzzer, 523);
  delay(150);
  tone(buzzer, 621);
  delay(150);
  tone(buzzer, 830);
  delay(450);
  tone(buzzer, 621);
  delay(450);

  noTone(buzzer);
  delay(50);

  tone(buzzer, 207);
  delay(150);
  tone(buzzer, 262);
  delay(150);
  tone(buzzer, 349);
  delay(150);
  tone(buzzer, 233);
  delay(150);
  tone(buzzer, 587);
  delay(150);
  tone(buzzer, 698);
  delay(150);
  tone(buzzer, 932);
  delay(400);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 932);
  delay(150);
  noTone(buzzer);
  delay(10);
  tone(buzzer, 932);
  delay(150);
  noTone(buzzer);
  delay(10);
  tone(buzzer, 932);
  delay(150);
  noTone(buzzer);
  delay(10);
  tone(buzzer, 1047);
  delay(550);  
  noTone(buzzer);
  delay(2000);
}

void DispenseCoinsSFX() {
  for (int count = 0; count < 7; count++) {     //set limit of count to number of coins dispensing?
    tone(buzzer, 800);
    delay(90);
    tone(buzzer, 1000);
    delay(90);
  }

  tone(buzzer, 1600);
  delay(100);
  noTone(buzzer);
  delay(2000);
}

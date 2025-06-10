#include <Wire.h>
#include "RTClib.h"
#include <LiquidCrystal_I2C.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD のアドレスが 0x27 の場合

const int relayPin = 8;
bool alreadyActivated = false;
const int duration = 15; // アクチュエータ ON 時間（秒）

DateTime nextRun;

void setup() {
  delay(500); // 電源起動安定のため
  Serial.begin(9600);
  rtc.begin();
  lcd.init();
  lcd.backlight();

  digitalWrite(relayPin, LOW);
  pinMode(relayPin, OUTPUT);
  
  //rtc.adjust(DateTime(2025,6,9,12,39,00));
  //if (rtc.lostPower()) {
  //  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //}

  //nextRun = getNextSundayNoon();
  nextRun = getNextDailyNoon();
  //nextRun = DateTime(2025, 6, 2, 14, 58, 0);  // 2025年6月1日 15:30:00 に実行
  lcd.clear();
}

void loop() {
  DateTime now = rtc.now();

  // 1行目：現在時刻の表示
  lcd.setCursor(0, 0);
  lcd.print("Now: ");
  printMMDD_HHMM(now);

  // 2行目：次回実行時刻の表示
  lcd.setCursor(0, 1);
  lcd.print("Next:");
  printMMDD_HHMM(nextRun);

  // 実行時刻になったら作動
  if (now.unixtime() >= nextRun.unixtime() && !alreadyActivated) {
    digitalWrite(relayPin, HIGH);
    delay(duration * 1000); // 指定秒数 ON
    digitalWrite(relayPin, LOW);
    alreadyActivated = true;
    //nextRun = getNextSundayNoon(); // 次の実行時刻を再設定
    nextRun = getNextDailyNoon(); // 次の実行時刻を再設定
  }

  // 過ぎていなければリセット
  if (now.unixtime() < nextRun.unixtime()) {
    alreadyActivated = false;
  }

  delay(1000);
}

DateTime getNextSundayNoon() {
  DateTime now = rtc.now();
  int currentDayOfWeek = now.dayOfTheWeek(); // 0 = Sunday
  int daysUntilSunday = (7 - currentDayOfWeek) % 7;
  if (daysUntilSunday == 0 && now.hour() >= 12) {
    daysUntilSunday = 7;
  }
  DateTime targetDate = now + TimeSpan(daysUntilSunday, 0, 0, 0);
  return DateTime(targetDate.year(), targetDate.month(), targetDate.day(), 12, 0, 0);
}

DateTime getNextDailyNoon() {
  DateTime now = rtc.now();
  if (now.hour() < 12) {
    // 今日の12:00が未来
    return DateTime(now.year(), now.month(), now.day(), 12, 0, 0);
  } else {
    // 翌日の12:00を設定
    DateTime tomorrow = now + TimeSpan(1, 0, 0, 0);
    return DateTime(tomorrow.year(), tomorrow.month(), tomorrow.day(), 12, 0, 0);
  }
}

// MM/DD HH:MM 表示用の関数（LCD横幅16文字に合わせる）
void printMMDD_HHMM(DateTime dt) {
  if (dt.month() < 10) lcd.print("0");
  lcd.print(dt.month());
  lcd.print("/");
  if (dt.day() < 10) lcd.print("0");
  lcd.print(dt.day());
  lcd.print(" ");
  if (dt.hour() < 10) lcd.print("0");
  lcd.print(dt.hour());
  lcd.print(":");
  if (dt.minute() < 10) lcd.print("0");
  lcd.print(dt.minute());
}

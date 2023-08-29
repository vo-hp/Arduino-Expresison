// SDA A4
// SCL A5


#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

String sohang1 = "";
String sohang2 = "";
String sohang3 = "";
String dapan = "";
String bieu_thuc = "";
String nhap = "nhap bieu thuc: ";
String nhaplai = "nhap lai: ";

int viTriKt1;
int viTriKt2;
int kt1;
int kt2;
int countkt;
char kt;
char;
float result;
bool areThere2operator = false;
bool isThere1operator = false;
bool wasNotExpression = false;

bool containOnlyDigits(const String& str) {
  for (int c = 0; c < str.length(); c++) {
    char d = str[c];
    if (!isdigit(d)) {
      if (d == '.') {
      } else {
        return false;
      }
    }
  }
  return true;
}

void nhapBieuThuc() {
  if (not wasNotExpression) {
    Serial.println(nhap);
    lcd.setCursor(0, 0);
    lcd.print(nhap);
  } else {
    Serial.println(nhaplai);
    lcd.setCursor(0, 0);
    lcd.print(nhaplai);
  }
  while (Serial.available() == 0) {
    countkt = 0;
    result = 0;
    dapan = "";
    bieu_thuc = "";
    sohang1 = "";
    sohang2 = "";
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
}

void readBieuThuc3sh() {
  sohang1 = bieu_thuc.substring(0, viTriKt1);
  sohang2 = bieu_thuc.substring(viTriKt1 + 1, viTriKt2);
  sohang3 = bieu_thuc.substring(viTriKt2 + 1, bieu_thuc.length());
  sohang1.trim();
  sohang2.trim();
  sohang3.trim();
  if (containOnlyDigits(sohang1) && containOnlyDigits(sohang2) && containOnlyDigits(sohang3)) {
    wasNotExpression = false;
    Serial.println(sohang1.toFloat());
    Serial.println(sohang2.toFloat());
    Serial.println(sohang3.toFloat());
  } else {
    wasNotExpression = true;
  }
}

void readBieuThuc2sh() {
  sohang1 = bieu_thuc.substring(0, viTriKt1);
  sohang2 = bieu_thuc.substring(viTriKt1 + 1, bieu_thuc.length());
  sohang1.trim();
  sohang2.trim();
  if (containOnlyDigits(sohang1) && containOnlyDigits(sohang2)) {
    wasNotExpression = false;
    Serial.println(sohang1.toFloat());
    Serial.println(sohang2.toFloat());
  } else {
    wasNotExpression = true;
  }
}

void analyzeExpression() {
  if (Serial.available() > 0) {
    clearLCD();
    bieu_thuc = Serial.readString();
    bieu_thuc.trim();
    Serial.println(bieu_thuc);
    lcd.setCursor(0, 0);
    lcd.print(bieu_thuc);
    checkOperator();
    doExpression1operator();
    doExpression2operator();
  }
}

void checkOperator() {
  for (int i = 0; i < bieu_thuc.length(); i++) {
    kt = bieu_thuc.charAt(i);
    if (kt == '+' || kt == '-' || kt == '*' || kt == '/') {
      Serial.println("vi tri tim thay dau " + String(i));
      countkt++;
      Serial.println("so ki tu = " + String(countkt));
      if (countkt == 1) {
        viTriKt1 = i;
        kt1 = char(bieu_thuc.charAt(i));
        Serial.println(String("ki tu 1 la: ") + char(kt1));
        isThere1operator = true;
      }
      if (countkt == 2) {
        viTriKt2 = i;
        kt2 = char(bieu_thuc.charAt(i));
        Serial.println(String("ki tu 2 la: ") + char(kt2));
        areThere2operator = true;
        isThere1operator = false;
      }
    }
    if (i == (bieu_thuc.length() - 1)) {
      countkt = 0;
    }
  }
}

void doExpression1operator() {
  if (isThere1operator) {
    readBieuThuc2sh();
    if (not wasNotExpression) {
      lcd.setCursor(0, 0);
      lcd.print(bieu_thuc);  // sohang1.toInt() + String(" ") + char(kt1) + String(" ") + sohang2.toInt()
      xetTruongHop1KiTu();
      isThere1operator = false;
    } 
    else {
    }
  }
}

void doExpression2operator() {
  if (areThere2operator) {
    readBieuThuc3sh();
    if (not wasNotExpression) {
      lcd.setCursor(0, 0);
      lcd.print(bieu_thuc);  // sohang1.toInt() + String(" ") + char(kt1) + String(" ") + sohang2.toInt() + char(kt2) + sohang3.toInt();
      xetTruongHop2KiTu();
      areThere2operator = false;
    } else {
    }
  }
}

void printResult() {
  Serial.print("= ");
  Serial.println(result);
  dapan.concat(result);
  lcd.setCursor((16 - dapan.length()), 1);
  lcd.print(result);
}

void clearLCD() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
}

float phep_cong(String sohang1, String sohang2) {
  return sohang1.toFloat() + sohang2.toFloat();
}

float phep_tru(String sohang1, String sohang2) {
  return sohang1.toFloat() - sohang2.toFloat();
}

float phep_nhan(String sohang1, String sohang2) {
  return sohang1.toFloat() * sohang2.toFloat();
}

float phep_chia(String sohang1, String sohang2) {
  return sohang1.toFloat() / sohang2.toFloat();
}

void xetTruongHop1KiTu() {
  if (kt1 == '+') {
    result = sohang1.toFloat() + sohang2.toFloat();
  }
  if (kt1 == '-') {
    result = sohang1.toFloat() - sohang2.toFloat();
  }
  if (kt1 == '*') {
    result = sohang1.toFloat() * sohang2.toFloat();
  }
  if (kt1 == '/') {
    result = sohang1.toFloat() / sohang2.toFloat();
  }
  printResult();
}

void truongHop2KiTuCungDau() {
  if (kt1 == '+' && kt2 == '+') {
    result = sohang1.toFloat() + sohang2.toFloat() + sohang3.toFloat();
  }
  if (kt1 == '-' && kt2 == '-') {
    result = sohang1.toFloat() - sohang2.toFloat() - sohang3.toFloat();
  }
  if (kt1 == '*' && kt2 == '*') {
    result = sohang1.toFloat() * sohang2.toFloat() * sohang2.toFloat();
  }
  if (kt1 == '/' && kt2 == '/') {
    result = sohang1.toFloat() / sohang2.toFloat() / sohang2.toFloat();
  }
}

void xetTruongHop2KiTu() {
  truongHop2KiTuCungDau();
  if (kt1 == '+' && kt2 == '*' || kt1 == '*' and kt2 == '+') {
    if (kt1 == '+' && kt2 == '*') {
      result = sohang1.toFloat() + phep_nhan(sohang2, sohang3);
    } else {
      result = phep_nhan(sohang1, sohang2) + sohang3.toFloat();
    }
  }
  if (kt1 == '+' && kt2 == '-' || kt1 == '-' and kt2 == '+') {
    if (kt1 == '+' && kt2 == '-') {
      result = sohang1.toFloat() + phep_tru(sohang2, sohang3);
    } else {
      result = phep_tru(sohang1, sohang2) + sohang3.toFloat();
    }
  }
  if (kt1 == '+' && kt2 == '/' || kt1 == '/' and kt2 == '+') {
    if (kt1 == '+' && kt2 == '/') {
      result = sohang1.toFloat() + phep_chia(sohang2, sohang3);
    } else {
      result = phep_chia(sohang1, sohang2) + sohang3.toFloat();
    }
  }
  if (kt1 == '-' && kt2 == '*' || kt1 == '*' and kt2 == '-') {
    if (kt1 == '-' && kt2 == '*') {
      result = sohang1.toFloat() - phep_nhan(sohang2, sohang3);
    } else {
      result = phep_nhan(sohang1, sohang2) - sohang3.toFloat();
    }
  }
  if (kt1 == '-' && kt2 == '/' || kt1 == '/' and kt2 == '-') {
    if (kt1 == '-' && kt2 == '/') {
      result = sohang1.toFloat() - phep_chia(sohang2, sohang3);
    } else {
      result = phep_chia(sohang1, sohang2) - sohang3.toFloat();
    }
  }
  if (kt1 == '/' && kt2 == '*' || kt1 == '*' and kt2 == '/') {
    if (kt1 == '/' && kt2 == '*') {
      result = sohang1.toFloat() / sohang2.toFloat() * sohang3.toFloat();
    } else {
      result = phep_nhan(sohang1, sohang2) / sohang3.toFloat();
    }
  }
  printResult();
}


void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
}

void loop() {
  nhapBieuThuc();
  analyzeExpression();
  delay(1000);
  clearLCD();
}
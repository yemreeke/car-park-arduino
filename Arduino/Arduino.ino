#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <IRremote.h>

LiquidCrystal_I2C LCD_EKRAN(0x27, 16, 2);
IRrecv irrecv(3);  //IR alıcının data pini
decode_results results;

// Araba var mı yok mu sensörleri pinleri
#define A_BLOK_ARABA_1 38
#define A_BLOK_ARABA_2 40
#define A_BLOK_ARABA_3 42
#define B_BLOK_ARABA_1 46
#define B_BLOK_ARABA_2 48
#define B_BLOK_ARABA_3 50

#define MAKSIMUM_ARABA_SAYISI 6

#define YANGIN_PIN 24
#define BUZZER 26
#define LDR_PIN 30
#define LED_PIN 32

#define GIRIS_SERVO_PIN 7
#define CIKIS_SERVO_PIN 8
#define GIRIS_KAPI 10
#define CIKIS_KAPI 11
int mevcutArabaSayisi = 0;

Servo girisServoMotor;
Servo cikisServoMotor;

String ISIK_DURUM = "OTOMATIK";  // OTOMATIK AÇIK KAPALI


void setup() {
  Serial.begin(9600);   // Serial haberleşme başlat
  LCD_EKRAN.begin();    // LCD ekran başlat
  irrecv.enableIRIn();  //Aktif ediyoruz

  // Araba var mı yok mu sensörlerini giriş olarak ayarla
  pinMode(A_BLOK_ARABA_1, INPUT);
  pinMode(A_BLOK_ARABA_2, INPUT);
  pinMode(A_BLOK_ARABA_3, INPUT);
  pinMode(B_BLOK_ARABA_1, INPUT);
  pinMode(B_BLOK_ARABA_2, INPUT);
  pinMode(B_BLOK_ARABA_3, INPUT);
  // Giriş ve çıkış kapıları sensörlerini giriş olarak ayarla
  pinMode(GIRIS_KAPI, INPUT);
  pinMode(CIKIS_KAPI, INPUT);

  // Buzzer Çıkış olarak tanımla
  pinMode(BUZZER, OUTPUT);

  // Led Pin
  pinMode(LED_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  pinMode(YANGIN_PIN, INPUT);

  // Giriş ve çıkış kapıları servo motorlarını pinleri tanımlanmıştır.
  girisServoMotor.attach(GIRIS_SERVO_PIN);
  cikisServoMotor.attach(CIKIS_SERVO_PIN);
  // Başlangıçta servolar kapalı konumda olsun.
  girisServoMotor.write(0);
  cikisServoMotor.write(0);
}
void loop() {
  IsikKontrol();
  IR_Oku();
  YanginKontrol();
  ArabaSayisiHesapla();
  if (mevcutArabaSayisi == MAKSIMUM_ARABA_SAYISI) {
    // Otopark da boş yer yoktur, doludur.
    EkranDoluYaz();

    // Araba giriş yapamaz, araba çıkış yapabilir.
    CikisKontrol();

    // Serial.println("Park yeri dolu!");
  } else {
    // Otoparkta boş yer vardır.
    EkranBosYaz();

    // Araba giriş yapabilir.
    GirisKontrol();

    // Araba çıkış yapabilir.
    CikisKontrol();

    // Serial.println("Park yeri boş!");
  }
}

void ArabaSayisiHesapla() {
  // Bu fonksiyon ile park yerlerindeki sensörlerden
  // okunan verilere göre mevcut araba sayısını hesaplar.
  int geciciSayi = 0;
  if (digitalRead(A_BLOK_ARABA_1) == LOW) {
    geciciSayi++;
  }
  if (digitalRead(A_BLOK_ARABA_2) == LOW) {
    geciciSayi++;
  }
  if (digitalRead(A_BLOK_ARABA_3) == LOW) {
    geciciSayi++;
  }
  if (digitalRead(B_BLOK_ARABA_1) == LOW) {
    geciciSayi++;
  }
  if (digitalRead(B_BLOK_ARABA_2) == LOW) {
    geciciSayi++;
  }
  if (digitalRead(B_BLOK_ARABA_3) == LOW) {
    geciciSayi++;
  }
  mevcutArabaSayisi = geciciSayi;
}


void IR_Oku() {
  if (irrecv.decode(&results)) {
    //Eğer tuşa basıldığı algılandıysa
    if (results.value == 0xFFA25D) { //1 Tuşu
      ISIK_DURUM="ACIK";
      Serial.println("1");
    } else if (results.value == 0xFF629D) {//2 Tuşu
      ISIK_DURUM="KAPALI";
      Serial.println("2");
    } else if (results.value == 0xFFE21D) {//3 Tuşu
      ISIK_DURUM="OTOMATIK";
      Serial.println("3");
    } else if (results.value == 0xFF22DD) {//4 Tuşu
      Serial.println("4");
    } else if (results.value == 0xFF02FD) {//5 Tuşu
      Serial.println("5");
    } else if (results.value == 0xFFC23D) {//6 Tuşu
      Serial.println("6");
    } else if (results.value == 0xFFE01F) {//7 Tuşu
      Serial.println("7");
    } else if (results.value == 0xFFA857) {//8 Tuşu
      Serial.println("8");
    } else if (results.value == 0xFF906F) {//9 Tuşu
      Serial.println("9");
    } else if (results.value == 0xFF6897) {//* Tuşu
      Serial.println("*");
    } else if (results.value == 0xFF9867) {//0 Tuşu
      Serial.println("0");
    } else if (results.value == 0xFFB04F) {//# Tuşu
      Serial.println("#");
    } else if (results.value == 0xFF18E7) {//Yukarı Tuşu
      Serial.println("Yukarı");
      GirisKapiAc();
    } else if (results.value == 0xFF10EF) {//Sol Tuşu
      Serial.println("Sol");
      CikisKapiAc();
    } else if (results.value == 0xFF38C7) {//Ok Tuşu
      Serial.println("OK");
    } else if (results.value == 0xFF5AA5) {//Sağ Tuşu
      Serial.println("Sağ");
      CikisKapiKapat();
    } else if (results.value == 0xFF4AB5) { // Aşağı Tuşu
      Serial.println("Aşağı");
      GirisKapiKapat();
    }
    // Serial.println(results.value, HEX);
    //Seri ekranda hex formatta bu adresi yazdır.
    irrecv.resume();  // Daha sonraki değer için bekle.
  }
}
void GirisKontrol() {
  // Giriş kapısındaki sensörden okunan veriyi alır.
  int girisDeger = digitalRead(GIRIS_KAPI);
  if (girisDeger == LOW) {
    // girisDeger LOW ise araba vardır ve giriş yapacaktır.
    GirisKapiAc();     // Giriş kapısını aç.
    delay(3000);       // 3 saniye bekle
    GirisKapiKapat();  // Giriş kapısını kapat.
  }
}
void CikisKontrol() {
  // Çıkış kapısındaki sensörden okunan veriyi alır.
  int cikisDeger = digitalRead(CIKIS_KAPI);
  if (cikisDeger == LOW) {
    // cikisDeger LOW ise araba vardır ve çıkış yapacaktır.
    CikisKapiAc();     // Çıkış kapısını aç.
    delay(3000);       // 3 saniye bekle
    CikisKapiKapat();  // Çıkış kapısını kapat.
  }
}

void GirisKapiAc() {
  // açık konumdan kapalı konuma doğru servo motoru hareket ettirir.
  // 0->90 'a doğru hareket
  for (int i = 0; i <= 90; i++) {
    girisServoMotor.write(i);
    delay(15);
  }
}
void GirisKapiKapat() {
  // kapalı konumdan açık konuma doğru servo motoru hareket ettirir.
  // 90->0 'a doğru hareket
  for (int i = 90; i >= 0; i--) {
    girisServoMotor.write(i);
    delay(15);
  }
}
void CikisKapiAc() {
  // açık konumdan kapalı konuma doğru servo motoru hareket ettirir.
  // 0->90 'a doğru hareket
  for (int i = 0; i <= 90; i++) {
    cikisServoMotor.write(i);
    delay(15);
  }
}
void CikisKapiKapat() {
  // kapalı konumdan açık konuma doğru servo motoru hareket ettirir.
  // 90->0 'a doğru hareket
  for (int i = 90; i >= 0; i--) {
    cikisServoMotor.write(i);
    delay(15);
  }
}

void EkranDoluYaz() {
  LCD_EKRAN.setCursor(0, 0);
  LCD_EKRAN.print("  OTOPARK DOLU  ");
  LCD_EKRAN.setCursor(0, 1);
  LCD_EKRAN.print("GIRIS YAPILAMAZ!");
}
void EkranBosYaz() {
  LCD_EKRAN.setCursor(0, 0);
  LCD_EKRAN.print("   HOSGELDINIZ  ");
  LCD_EKRAN.setCursor(0, 1);
  int bosYerSayisi = MAKSIMUM_ARABA_SAYISI - mevcutArabaSayisi;
  LCD_EKRAN.print(bosYerSayisi);
  LCD_EKRAN.print(" ARAC GIREBILIR");
}
void YanginKontrol() {
  // ILK ONCE YANGIN SENSORUNDEN DEGER OKUYORUZ
  // OKUDUĞUMUZ DEĞER 0 OLDUĞUNDA YANGIN VAR DEMEKTİR
  int yanginSensor = digitalRead(YANGIN_PIN);
  if (yanginSensor == 0) { // YANGIN VARSA
    
    // LCD Ekrana ALARM YAZILARINI YAZ
    LCD_EKRAN.setCursor(0, 0);
    LCD_EKRAN.print("!!!  ALARM  !!!!");
    LCD_EKRAN.setCursor(0, 1);
    LCD_EKRAN.print("!! YANGIN VAR !!");

    digitalWrite(BUZZER, 1);  // Buzzer çalıştır

    // Her iki kapıyı da aynı anda aç
    for (int i = 0; i <= 90; i++) {
      cikisServoMotor.write(i);
      girisServoMotor.write(i);
      delay(15);
    }

    // YANGIN SENSORU 0 VERDİĞİ SÜRECE BEKLE
    
    while (digitalRead(YANGIN_PIN) == 0) {
      delay(100); // YANGIN OLDUĞU SÜRECE KOD BURADA SONSUZ DÖNGÜDE KALIYOR
    }
    // YANGIN OLMAZSA ILERLE
    
    // BUZZER KAPAT
    digitalWrite(BUZZER, 0);

    // Kapıları kapat
    for (int i = 90; i >= 0; i--) {
      cikisServoMotor.write(i);
      girisServoMotor.write(i);
      delay(15);
    }
  }
}
void IsikKontrol() {
  // ISIK DURUM AÇIKSA LEDİ AÇ
  // ISIK DURUM KAPALIYSA LEDİ KAPAT
  // ISIK DURUM OTOMATIK İSE LDR DEN DEĞER OKU
  // OKUDUĞUN DEĞERE BAK KARANLIKTA LEDİ AÇ
  // AYDINLIKTA KAPAT
  if (ISIK_DURUM == "ACIK") {
    digitalWrite(LED_PIN, 1);
  } else if (ISIK_DURUM == "KAPALI") {
    digitalWrite(LED_PIN, 0);
  } else if (ISIK_DURUM == "OTOMATIK") {
    int ldr = digitalRead(LDR_PIN);
    if (ldr == 1) { // KARANLIK
      digitalWrite(LED_PIN, 1);
    } else { // AYDINLIK
      digitalWrite(LED_PIN, 0);
    }
  }
}

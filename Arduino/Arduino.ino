#include<Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C LCD_EKRAN(0x27, 16, 2);

// Araba var mı yok mu sensörleri pinleri
#DEFINE A_BLOK_ARABA_1 0
#DEFINE A_BLOK_ARABA_2 2
#DEFINE A_BLOK_ARABA_3 4
#DEFINE B_BLOK_ARABA_1 6
#DEFINE B_BLOK_ARABA_2 8
#DEFINE B_BLOK_ARABA_3 10
#DEFINE MAKSIMUM_ARABA_SAYISI 6

#DEFINE GIRIS_KAPI 12
#DEFINE CIKIS_KAPI 13
#DEFINE GIRIS_SERVO_PIN 11
#DEFINE CIKIS_SERVO_PIN 9

int mevcutArabaSayisi = 0;

Servo girisServoMotor;
Servo cikisServoMotor;







void setup() {
    Serial.begin(9600); // Serial haberleşme başlat
    LCD_EKRAN.begin(); // LCD ekran başlat

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

    // Giriş ve çıkış kapıları servo motorlarını pinleri tanımlanmıştır.
    girisServoMotor.attach(GIRIS_SERVO_PIN);
    cikisServoMotor.attach(CIKIS_SERVO_PIN);
    // Başlangıçta servolar kapalı konumda olsun.
    girisServoMotor.write(0);
    cikisServoMotor.write(0);
}
void loop() {
    ArabaSayisiHesapla();
    if(mevcutArabaSayisi == MAKSIMUM_ARABA_SAYISI){
        // Otopark da boş yer yoktur, doludur.
        EkranDoluYaz();
        
        // Araba giriş yapamaz, araba çıkış yapabilir.
        CikisKontrol();
        
        Serial.println("Park yeri dolu!");
    }
    else{
        // Otoparkta boş yer vardır.
        EkranBosYaz();
        
        // Araba giriş yapabilir.
        GirisKontrol();

        // Araba çıkış yapabilir.
        CikisKontrol();

        Serial.println("Park yeri boş!");
    }
}

void ArabaSayisiHesapla(){
    // Bu fonksiyon ile park yerlerindeki sensörlerden
    // okunan verilere göre mevcut araba sayısını hesaplar.
    int geciciSayi = 0;
    if(digitalRead(A_BLOK_ARABA_1) == HIGH){
        geciciSayi++;
    }
    if(digitalRead(A_BLOK_ARABA_2) == HIGH){
        geciciSayi++;
    }
    if(digitalRead(A_BLOK_ARABA_3) == HIGH){
        geciciSayi++;
    }
    if(digitalRead(B_BLOK_ARABA_1) == HIGH){
        geciciSayi++;
    }
    if(digitalRead(B_BLOK_ARABA_2) == HIGH){
        geciciSayi++;
    }
    if(digitalRead(B_BLOK_ARABA_3) == HIGH){
        geciciSayi++;
    }
    mevcutArabaSayisi = geciciSayi;
}

void GirisKontrol(){
    // Giriş kapısındaki sensörden okunan veriyi alır.
    int girisDeger = digitalRead(GIRIS_KAPI);
    if(girisDeger == HIGH){
        // girisDeger HIGH ise araba vardır ve giriş yapacaktır.
        GirisKapiAc(); // Giriş kapısını aç.
        delay(3000); // 3 saniye bekle
        GirisKapiKapat(); // Giriş kapısını kapat.
    }
    
}
void CikisKontrol(){
    // Çıkış kapısındaki sensörden okunan veriyi alır.
    int cikisDeger = digitalRead(CIKIS_KAPI);
    if(cikisDeger == HIGH){
        // cikisDeger HIGH ise araba vardır ve çıkış yapacaktır.
        CikisKapiAc(); // Çıkış kapısını aç.
        delay(3000); // 3 saniye bekle
        CikisKapiKapat(); // Çıkış kapısını kapat.
    }

}

void GirisKapiAc(){
    // açık konumdan kapalı konuma doğru servo motoru hareket ettirir.
    // 0->90 'a doğru hareket
    for (int i = 0; i >= 90 ; i++)
    {
        girisServoMotor.write(i);
        delay(15);
    }
}
void GirisKapiKapat(){
    // kapalı konumdan açık konuma doğru servo motoru hareket ettirir.
    // 90->0 'a doğru hareket
    for (int i = 90; i >= 0 ; i--)
    {
        girisServoMotor.write(i);
        delay(15);
    }
}
void CikisKapiAc(){
    // açık konumdan kapalı konuma doğru servo motoru hareket ettirir.
    // 0->90 'a doğru hareket
    for (int i = 0; i >= 90 ; i++)
    {
        cikisServoMotor.write(i);
        delay(15);
    }
}
void CikisKapiKapat(){
    // kapalı konumdan açık konuma doğru servo motoru hareket ettirir.
    // 90->0 'a doğru hareket
    for (int i = 90; i >= 0 ; i--)
    {
        cikisServoMotor.write(i);
        delay(15);
    }
}

void EkranDoluYaz(){
    LCD_EKRAN.setCursor(0,0);
    LCD_EKRAN.print("  OTOPARK DOLU  ");
    LCD_EKRAN.setCursor(0,1);
    LCD_EKRAN.print("GIRIS YAPILAMAZ!");
}
void EkranBosYaz(){
    LCD_EKRAN.setCursor(0,0);
    LCD_EKRAN.print("   HOSGELDINIZ  ");
    LCD_EKRAN.setCursor(0,1);
    int bosYerSayisi = MAKSIMUM_ARABA_SAYISI - mevcutArabaSayisi;
    LCD_EKRAN.print(bosYerSayisi);
    LCD_EKRAN.print(" ARAC GIREBILIR");
}
```
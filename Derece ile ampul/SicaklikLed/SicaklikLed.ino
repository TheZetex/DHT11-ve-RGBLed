#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT11.h>

#define DHT11PIN 7
#define kirmiziPin 9   
#define yesilPin 10    // Eskiden turuncuPin olarak adlandırılmıştı, aslında yeşil LED pini
#define maviPin 11     
#define WIRE Wire

// OLED ekran boyutları
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &WIRE, -1);
DHT11 DHT(DHT11PIN);

// LED tipini belirtin. True ise common anode, false ise common cathode
const bool commonAnode = false;

void setup() {
  Serial.begin(9600);
  pinMode(kirmiziPin, OUTPUT);
  pinMode(yesilPin, OUTPUT);
  pinMode(maviPin, OUTPUT);
  
  // OLED ekranı başlat
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  display.clearDisplay(); // Ekranı temizle
  display.display(); // Ekranı güncelle

  // İlk ekran metni
  showText("Sicaklik Gosterici");
}

void loop() {
  Serial.println();
  
  float nem = DHT.readHumidity();
  float sicaklik = DHT.readTemperature();

  // DHT11 sensöründen okunan değerleri kontrol et
  if (nem < 0 || sicaklik < 0) {
    Serial.println("Sensör hatası!");
    displayError("Sensör hatası!");
  } else {
    Serial.print("NEM (%): ");
    Serial.println(nem, 2);
    Serial.print("SICAKLIK (C): ");
    Serial.println(sicaklik, 2);

    // Ekranda sıcaklık ve nem değerlerini göster
    displayTemperatureAndHumidity(sicaklik, nem);

    // LED renk ayarları
    if (sicaklik < 20) {
      // Soğuk (Mavi)
      renkAyarla(0, 0, 255); // mavi
    } 
    else if (sicaklik <= 25) {
      // Ilık (Turuncu = Kırmızı + Yeşil karışımı)
      renkAyarla(255, 165, 0); // turuncu
    } 
    else {
      // Sıcak (Kırmızı)
      renkAyarla(255, 0, 0); 
    }
  }

  delay(2000); // Her 2 saniyede bir güncelle
}

void showText(String text) {
  display.clearDisplay(); // Ekranı temizle
  display.setTextSize(2); // Yazı boyutunu ayarla
  display.setTextColor(SSD1306_WHITE); // Yazı rengini beyaz yap
  display.setCursor(0, 0); // Başlangıç konumu
  display.println(text);
  display.display(); // Ekranda göster
  delay(500);  // Titreşimleri önlemek için kısa bekleme
}

void displayTemperatureAndHumidity(float temp, float hum) {
  display.clearDisplay(); // Ekranı temizle
  display.setTextSize(2); // Yazı boyutunu ayarla
  display.setTextColor(SSD1306_WHITE); // Yazı rengini beyaz yap
  display.setCursor(0, 0); // Başlangıç konumu
  display.print("S: ");
  display.print(temp);
  display.println(" C");
  display.print("N: ");
  display.print(hum);
  display.println(" %");
  display.display(); // Ekranda göster
}

void displayError(String errorMessage) {
  display.clearDisplay(); // Ekranı temizle
  display.setTextSize(1); // Yazı boyutunu ayarla
  display.setTextColor(SSD1306_WHITE); // Yazı rengini beyaz yap
  display.setCursor(0, 0); // Başlangıç konumu
  display.println(errorMessage);
  display.display(); // Ekranda göster
}

void renkAyarla(int kirmizi, int yesil, int mavi){ 
  if(commonAnode){
    // Common anode LED için invert et
    kirmizi = 255 - kirmizi;
    yesil = 255 - yesil;
    mavi = 255 - mavi;
  }
  analogWrite(kirmiziPin, kirmizi);
  analogWrite(yesilPin, yesil);
  analogWrite(maviPin, mavi);
}

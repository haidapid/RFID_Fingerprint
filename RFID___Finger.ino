#include <SPI.h>
#include <WiFi.h>
#include <MFRC522.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>

#define SS_PIN        5   // Pin SDA RFID ke Pin 5 ESP32
#define RST_PIN       4   // Pin RST RFID ke Pin 27 ESP32
#define ON_Board_LED  2   // ESP32 pin Led
#define buzzer        32
#define relay         26

// Define 16x2 LCD
int screenWidth = 16;
int screenHeight = 2;

// Define two line
String line1 = " SILAHKAN ABSEN "; // scroll this line

int stringStart, stringStop = 0;
int scrollCursor = screenWidth;

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);
int i = 0;
uint8_t id;
int readsuccess;
byte readcard[4];
char str[32] = "";
String StrUID, nama, serverPath, serverName, datarekap;
String rfid_uid = "";
String api_key = "rahasia"; //Password Api Server
const char* ssid = "Hippo"; //SSID WiFi
String serverku = "172.20.10.3"; //IP komputer
const char* password = "123456788"; //Password Wifi

//================================================= setup =================================================
void setup() {
  Serial.begin(115200);
  Serial2.begin(9600);
  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  pinMode(relay, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ON_Board_LED, OUTPUT);

  SPI.begin();
  rfid.PCD_Init();
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Sensor Fingerprint Terbaca");
  } else {
    Serial.println("Sensor Fingerprint Tidak Tidak Terbaca");
    while (1) {
      delay(1);
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  Serial.print("Menghubungkan ke ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Terhubung. IP : ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  lcd.clear();
  delay(500);
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" SELAMAT DATANG ");
  delay(100);
  lcd.setCursor(scrollCursor, 1);
  lcd.print(line1.substring(stringStart, stringStop));
  delay(100);
  lcd.print("    ");
  //lcd.clear();
  if (stringStart == 0 && scrollCursor > 0) {
    scrollCursor--;
    stringStop++;
  } else if (stringStart == stringStop) {
    stringStart = stringStop = 0;
    scrollCursor = screenWidth;
  } else if (stringStop == line1.length() && scrollCursor == 0) {
    stringStart++;
  } else {
    stringStart++;
    stringStop++;
  }

  getFingerprintIDez();
  delay(50);
  readsuccess = getid();
  if (readsuccess) {// jika rfid membaca ada kartu
    Serial.println("=================================== BATAS =========================================");
    Serial.println("Identifikasi Kartu!");
    Serial.println(StrUID);
    serverPath = "http://" + serverku + "/RFID_finger/cek_rfid.php?id_rfid=" + StrUID;
    cek_absen();
    serverName = "http://" + serverku + "/RFID_finger/rekap_rfid.php";
    datarekap = "api_key=" + api_key +
                "&id_rfid=" + StrUID +
                "&nama=" + nama +
                + "";
    kirimdata();
  }
  delay(200);
}

//================================================= getid =================================================
int getid() {
  if (!rfid.PICC_IsNewCardPresent()) {
    return 0;
  }
  if (!rfid.PICC_ReadCardSerial()) {
    return 0;
  }
  for (int i = 0; i < 4; i++) {
    readcard[i] = rfid.uid.uidByte[i]; //storing the UID of the tag in readcard
    array_to_string(readcard, 4, str);
    StrUID = str;
  }
  rfid.PICC_HaltA();
  return 1;
}

void array_to_string(byte array[], unsigned int len, char buffer[]) {
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[i] >> 4) & 0x0F;
    byte nib2 = (array[i] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}
//================================================= getid =================================================

//================================================= cek_absen =================================================
void cek_absen() {
  HTTPClient http;
  http.begin(serverPath.c_str());
  int httpResponseCode = http.GET();
  if (httpResponseCode > 0) {
    nama = http.getString();
    nama.remove(0, 1);
    unsigned int panjangrespon = nama.length();
    if (panjangrespon < 5) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("   COBA LAGI    ");
      Serial.println(" TIDAK TERDAFTAR");
      Serial.println("=================================== BATAS =========================================");
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
      delay(200);
      digitalWrite(relay, LOW);
      delay(2000);
     digitalWrite(relay, LOW);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  TERIMA KASIH  ");
      lcd.setCursor(0, 1);
      lcd.print(  nama  );

      Serial.print("Terima Kasih");
      Serial.println(nama);
      digitalWrite(ON_Board_LED, HIGH);
      delay(200);
      digitalWrite(ON_Board_LED, LOW);
      delay(200);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(buzzer, HIGH);
      delay(100);
      digitalWrite(buzzer, LOW);
      delay(100);
      digitalWrite(relay, HIGH);
      delay(5000);
      digitalWrite(relay, LOW);
    }
  }
  serverPath = "";
}
//================================================= cek_absen =================================================

//================================================= kirimdata =================================================
void kirimdata() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  TERIMA KASIH  ");
  WiFiClient client;
  HTTPClient http;
  http.begin(serverName);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");// Specify content-type header
  int httpResponseCode2 = http.POST(datarekap);

  if (httpResponseCode2 == 200) {
    
    String responya = http.getString();
    Serial.println(responya);
    Serial.println("=================================== BATAS =========================================");
    nama = "";
  }
  else {
    Serial.println(httpResponseCode2);
    Serial.println();
  }
  nama = "";
  serverName = "";
}
//================================================= kirimdata =================================================



//================================================= Finger =================================================
uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Finger ditemukan!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Finger tidak terdaftar !!!");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }
  

  Serial.print("ID Finger"); Serial.print(finger.fingerID);
  Serial.print(" dari "); Serial.println(finger.confidence);
  return finger.fingerID;
}


int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  {
    return -1;
  }
  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
  }
  // found a match!
  Serial.print("ID Finger "); Serial.print(finger.fingerID);
  Serial.print(" dari "); Serial.println(finger.confidence);

  Serial.println("Identifikasi Fingerprint");
  serverPath = "http://" + serverku + "/RFID_finger/cek_finger.php?id_finger=" + finger.fingerID;
  cek_absen();
  serverName = "http://" + serverku + "/RFID_finger/rekap_finger.php";
  datarekap = "api_key=" + api_key +
              "&id_finger=" + finger.fingerID +
              "&nama=" + nama +
              + "";
  kirimdata();
  delay(1000);
  return finger.fingerID;
}
//================================================= Finger =================================================

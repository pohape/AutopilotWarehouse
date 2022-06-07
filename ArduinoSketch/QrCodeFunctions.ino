void processQrCodeScanner() {
  if (QRSerial.available()) {
    Serial.write(QRSerial.read());
  }
}

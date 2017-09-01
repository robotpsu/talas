void setup() {
  pinMode(30, OUTPUT);
  digitalWrite(30, HIGH);
  Serial.begin(9600);
  Serial.flush();
  Serial1.begin(38400);
  Serial1.flush();

/*
  if (unsigned long rate = inCmdMode()) {
    Serial.println(rate);
    Serial.println("\nBluetooth module info:");
    printBtInfo();
    Serial.println("\nEnter commands:");
    return;
  }
*/
}

void loop() {
  if (Serial1.available())
    Serial.write(Serial1.read());
  if (Serial.available()) {
    int b = Serial.read();
    Serial.write(b);
    Serial1.write(b);
  }
}

const unsigned long rates[] = {
  4800, 9600, 19200, 38400, 57600, 115200, 23400, 460800, 921600, 1382400};
const size_t numRates = sizeof(rates) / sizeof(unsigned long);

const char *cmds[] = {
  "NAME", "VERSION", "UART", "ADDR", "PSWD", "ROLE", "CMODE", "CLASS", "IAC"};
const size_t numCmds = sizeof(cmds) / sizeof(char*);

unsigned long inCmdMode() {
  char buf[32];

  for(size_t i = 0; i < numRates; i++) {
    Serial1.begin(rates[i]);
    Serial1.setTimeout(100);
    Serial1.flush();
    Serial1.write("AT\r\n");
    if (Serial1.readBytes(buf, 32) > 0)
      return rates[i];
    Serial1.end();
  }
}

void printBtInfo() {
  for (size_t i = 0; i < numCmds; i++) {
    Serial1.write("AT+"); Serial1.write(cmds[i]); Serial1.write("?\r\n");
    String s = Serial1.readString();
    if (!s.length()) continue;
    if (s.endsWith("OK\r\n")) {
      s.remove(s.indexOf('\r'));
      Serial.println(s);
    } else {
      Serial.write('+');
      Serial.print(cmds[i]);
      Serial.print(":FAIL");
    }
  }
}


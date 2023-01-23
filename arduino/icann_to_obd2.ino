#include <Arduino.h>
#include <SoftwareSerial.h>

class ICANNToOBD2
{
public:
  ICANNToOBD2(int rxPin, int txPin) : serial(rxPin, txPin)
  {
    serial.begin(9600);
  }

  std::string translate()
  {
    std::vector<uint8_t> data;
    // Read data from the ICANN bus
    while (serial.available() > 0)
    {
      data.push_back(serial.read());
    }
    // Parse the raw ICANN bus data to extract the DTC code
    std::string dtc = parse(data);
    // Lookup the corresponding OBD-II code using the Obd2Dictionary
    return Obd2Dictionary<std::string, std::string>::lookup(dtc);
  }

private:
  std::string parse(const std::vector<uint8_t> &data)
  {
    // implementation to parse the raw data and extract the DTC code
    // ...
  }
  SoftwareSerial serial;
};

ICANNToOBD2 translator(2, 3); // rx, tx

void setup()
{
  // put your setup code here, to run once:
}

void loop()
{
  std::string obd2_code = translator.translate();
  Serial.print(obd2_code);
  delay(1000);
}

//
//    FILE: AGS02MA_calibrate_manual.ino
//  AUTHOR: Rob Tillaart, Beanow
// VERSION: 0.2.0
// PURPOSE: test application
//    DATE: 2022-04-22
//     URL: https://github.com/RobTillaart/AGS02MA
//

#include "AGS02MA.h"

// The zero calibration value we'll (temporarily) set in the example.
#define ZC_VALUE 700

#define READS 10
#define INTERVAL 3000


AGS02MA AGS(26);

AGS02MA::ZeroCalibration initialValue;
u_int8_t version;


void setup()
{
  // ESP devices typically mis the first serial log lines after flashing.
  // Delay somewhat to include all output.
  delay(1000);

  Serial.begin(115200);
  Serial.println(__FILE__);

  Serial.print("AGS02MA_LIB_VERSION: ");
  Serial.println(AGS02MA_LIB_VERSION);
  Serial.println();

  Serial.print("READS:\t\t");
  Serial.println(READS);
  Serial.print("INTERVAL:\t");
  Serial.println(INTERVAL);

  Wire.begin();

  bool b = AGS.begin();
  Serial.print("BEGIN:\t\t");
  Serial.println(b);

  Serial.print("VERSION:\t");
  version = AGS.getSensorVersion();
  Serial.println(version);

  if (version != 118)
  {
    Serial.println();
    Serial.println("Only v118 sensors support manual zero calibration. For other versions, you can use the 'AGS02MA_calibrate' example instead.");
  }
  else
  {
    b = AGS.setPPBMode();
    uint8_t m = AGS.getMode();
    Serial.print("MODE:\t\t");
    Serial.print(b);
    Serial.print("\t");
    Serial.println(m);

    Serial.println();
    Serial.println("Your initial zero calibration is:");
    initialValue = AGS.getZeroCalibration();
    printZeroCalibration(initialValue);
    Serial.println();

    Serial.println("Showing sample data before changing.");
    for (size_t i = 0; i < READS; i++)
    {
      delay(INTERVAL);
      printPPB();
    }

    Serial.println();
    Serial.println("Manually setting zero calibration:");
    b = AGS.manualZeroCalibration(ZC_VALUE);
    Serial.print("CALIB:\t");
    Serial.println(b);
    auto newValue = AGS.getZeroCalibration();
    printZeroCalibration(newValue);
    Serial.println();

    Serial.println("Showing sample data.");
    for (size_t i = 0; i < READS; i++)
    {
      delay(INTERVAL);
      printPPB();
    }

    Serial.println();
    Serial.println("Restoring initial zero calibration:");
    b = AGS.manualZeroCalibration(initialValue.value);
    Serial.print("CALIB:\t");
    Serial.println(b);
    auto restoredValue = AGS.getZeroCalibration();
    printZeroCalibration(restoredValue);
    Serial.println();

  }

}

void loop()
{
  delay(INTERVAL);
  printPPB();
}

void printZeroCalibration(AGS02MA::ZeroCalibration &zc) {
  Serial.print("Status:\t");
  Serial.println(zc.status);
  Serial.print("Value:\t");
  Serial.println(zc.value);
}

void printPPB()
{
  uint32_t value = AGS.readPPB();
  Serial.print("PPB:\t");
  Serial.print(value);
  Serial.print("\t");
  Serial.print(AGS.lastStatus(), HEX);
  Serial.print("\t");
  Serial.print(AGS.lastError(), HEX);
  Serial.println();
}

// -- END OF FILE --

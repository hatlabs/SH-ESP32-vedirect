#include <Arduino.h>

#include "sensesp_app.h"
#include "sensesp_app_builder.h"
#include "sensors/vedirect.h"
#include "signalk/signalk_output.h"

// SDA and SCL pins on SH-ESP32
#define SDA_PIN 16
#define SCL_PIN 17

#define TX_PIN SCL_PIN
#define RX_PIN SDA_PIN

ReactESP app([]() {
// Some initialization boilerplate when in debug mode...
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  SensESPAppBuilder builder;

  sensesp_app = builder.set_hostname("vedirect-test")
                    ->set_standard_sensors(NONE)
                    ->get_app();

  // initialize Serial1 on the opto_in pin

  Serial1.begin(19200, SERIAL_8N1, RX_PIN, TX_PIN, false);

  // app.onAvailable(Serial1, []() {
  //   Serial.write(Serial1.read());
  // });

  VEDirectInput* vedi = new VEDirectInput(&Serial1);

  vedi->parser.data.channel_1_battery_voltage.connect_to(new SKOutputNumber(
      "electrical.batteries.house.voltage", "/battery/1/voltage"));
  vedi->parser.data.channel_1_battery_current.connect_to(new SKOutputNumber(
      "electrical.batteries.house.current", "/battery/1/current"));
  vedi->parser.data.state_of_charge.connect_to(new SKOutputNumber(
      "electrical.batteries.house.capacity.stateOfCharge", "/battery/1/stateOfCharge"));

  sensesp_app->enable();
});

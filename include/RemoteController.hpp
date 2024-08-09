#ifndef REMOTECONTROLLER
#define REMOTECONTROLLER

#include <IRremote.hpp>

#define IRRECEIVEPIN 0

static constexpr IRRawDataType button_asterisk = 0xBD42FF00;
static constexpr IRRawDataType button_hash     = 0xB54AFF00;
static constexpr IRRawDataType button_ok       = 0xBF40FF00;
static constexpr IRRawDataType button_up       = 0xB946FF00;
static constexpr IRRawDataType button_down     = 0xEA15FF00;

IRRawDataType getIrReceiverCommand() {
  IRRawDataType rawCommand = 0;
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol != UNKNOWN) {
      if (IrReceiver.decodedIRData.command != 0) {
        rawCommand = IrReceiver.decodedIRData.decodedRawData;
      }
      IrReceiver.resume();
    }
  }
  return rawCommand;
}

#endif
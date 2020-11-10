#include "../gcode/gcode.h"
#include "../gcode/queue.h"
#include "../core/macros.h"
#include "../module/PeriphDevice.h"
#include "../module/CanModule.h"
#include "../module/CanBus.h"

/*
* Disable /Enable chamber door event
* S0: disable door event
* S1: enable door event
* S2: release hold of UART for receiving other PC gcodes
*/


void GcodeSuite::M1010() {
  uint8_t s;

  if (!parser.seen('S')) {
    Periph.ReportStatus();
    return;
  }

  // if chamber is not exist, not allow other operation
  if (!Periph.IsOnline(PERIPH_IOSW_DOOR)) {
    SERIAL_ECHOLN("Chamber is not exist.");
    return;
  }

  s = parser.byteval('S', 12);

  switch (s)
  {
  case 0:
    Periph.SetDoorCheck(false);
    break;

  case 1:
    Periph.SetDoorCheck(true);
    break;

  case 2:
    Periph.SetUartLock(false);
    break;

  case 10:
    Periph.TriggerDoorEvent(true);
    SERIAL_ECHOLN("triggered door open");
    break;

  case 11:
    Periph.TriggerDoorEvent(false);
    SERIAL_ECHOLN("triggered door close");
    break;

  case 12:
    CanModules.SetFunctionValue(EXTEND_CAN_NUM, FUNC_REPORT_ENCLOSURE, NULL, 0);
    break;

  default:
    break;
  }
}

#include "mouse.h"
#include "mouse_usb_config.h"

#include <stdint.h>

#include <stdbool.h>

#include <stdio.h>

#include "inc/hw_types.h"

#include "usblib/usblib.h"

#include "usblib/usbhid.h"

#include "usblib/usb-ids.h"

#include "usblib/device/usbdevice.h"

#include "usblib/device/usbdcomp.h"

#include "usblib/device/usbdhid.h"

#include "usblib/device/usbdhidgamepad.h"

#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/usb.h"

#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"

#include "debug_utils/swo_segger.h"
#include "utils/uartstdio.h"

/* HID Device */

int main(void) {
  ROM_FPULazyStackingEnable();

  // Set the clocking to run from the PLL at 50MHz
  ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  mouseInit();

  uint8_t hidRep[HID_REPORT_BUF_LEN_BYTE] = {0};
  makeHidReport(hidRep);

  for (;;) {
    for (uint32_t counter = 0; counter < 5000000; ++counter) {
      // wait
    }
    USBDHIDReportWrite(&mouseDevice, hidRep, HID_REPORT_BUF_LEN_BYTE, false);
  }
  return 0;
}

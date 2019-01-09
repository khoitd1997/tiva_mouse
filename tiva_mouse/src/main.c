#include "mouse.h"
#include "mouse_usb_config.h"

#include "mma8451.h"

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

#include "debug_utils/swo_segger.h"
#include "utils/uartstdio.h"

#define MMA8451_INT_PORT GPIO_PORTD_BASE
#define MMA8451_INT_PERIPH_CLK SYSCTL_PERIPH_GPIOD
#define MMA8451_INT_PIN GPIO_PIN_2

int main(void) {
  SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

  IntMasterDisable();

  uint8_t hidRep[HID_REPORT_BUF_LEN_BYTE] = {0};

  mma8451Init();

  mma8451Configure();
  mma8451InterruptInit();

  int32_t currData[ACCEL_TOTAL_AXIS] = {0};

  mouseInit();
  IntMasterEnable();
  mouseEnable();

  for (;;) {
    IntMasterDisable();
    mma8451ReadAccelData(currData);
    IntMasterEnable();
    makeHidReport(hidRep, currData);
    USBDHIDReportWrite(&mouseDevice, hidRep, HID_REPORT_BUF_LEN_BYTE, false);
    // for (uint32_t counter = 0; counter < 5000; ++counter) {
    //   // wait
    // }
  }
  return 0;
}

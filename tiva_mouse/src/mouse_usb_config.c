#include "mouse_usb_config.h"
#include "mouse.h"

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

// // #include <stdbool.h>
// // #include <stdint.h>

#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_nvic.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_uart.h"

#define USB_EP_TO_INDEX(x) ((x) >> 4)
#define INT_IN_ENDPOINT USB_EP_3

#define TOTAL_STRING_DESCRIPTOR 4
#define LANG_DESCRIPTOR_LEN 4

#define VENDOR_ID 0xe000          // currently unused vendor ID
#define PRODUCT_ID USB_PID_MOUSE  // mouse ID
#define MAX_PWR_MA 125

/**********************STRING DESCRIPTOR******************************/

static const uint8_t mouseLanguageString[] = {
    LANG_DESCRIPTOR_LEN, USB_DTYPE_STRING, USBShort(USB_LANG_EN_US)};

static const uint8_t mouseManufactureString[] = {
    (10 + 1) * 2, USB_DTYPE_STRING,
    'K',          0,
    'h',          0,
    'o',          0,
    'i',          0,
    ' ',          0,
    'T',          0,
    'r',          0,
    'i',          0,
    'n',          0,
    'h',          0,
};

static const uint8_t mouseProductString[] = {
    (10 + 1) * 2, USB_DTYPE_STRING,
    'K',          0,
    'h',          0,
    'o',          0,
    'i',          0,
    ' ',          0,
    'M',          0,
    'o',          0,
    'u',          0,
    's',          0,
    'e',          0,
};

static const uint8_t mouseSerialString[] = {
    (8 + 1) * 2, USB_DTYPE_STRING, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0};

static const uint8_t *const mouseStringDescriptorList[] = {
    mouseLanguageString, mouseManufactureString, mouseProductString, mouseSerialString};

/*******************HID report descriptor********************************/
static const uint8_t mouseReportDescriptor[] = {
    0x05, 0x01,               // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,               // USAGE (Mouse)
    0xa1, 0x01,               // COLLECTION (Application)
    0x09, 0x01,               //   USAGE (Pointer)
    0xa1, 0x00,               //   COLLECTION (Physical)
    0x05, 0x09,               //     USAGE_PAGE (Button)
    0x19, 0x01,               //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,               //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,               //     LOGICAL_MINIMUM (0)
    0x25, 0x01,               //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,               //     REPORT_COUNT (3)
    0x75, 0x01,               //     REPORT_SIZE (1)
    0x81, 0x02,               //     INPUT (Data,Var,Abs)
    0x95, 0x01,               //     REPORT_COUNT (1)
    0x75, 0x05,               //     REPORT_SIZE (5)
    0x81, 0x03,               //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,               //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,               //     USAGE (X)
    0x09, 0x31,               //     USAGE (Y)
    0x15, MOUSE_LOGICAL_MIN,  //     LOGICAL_MINIMUM (-127)
    0x25, MOUSE_LOGICAL_MAX,  //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,               //     REPORT_SIZE (8)
    0x95, 0x02,               //     REPORT_COUNT (2)
    0x81, 0x06,               //     INPUT (Data,Var,Rel)
    0xc0,                     //   END_COLLECTION
    0xc0                      // END_COLLECTION
};

static const uint8_t *const mouseClassDescriptor[] = {mouseReportDescriptor};

/********************USB General Descriptor*********************/

static const tHIDDescriptor hidDescriptorList[] = {{
    9,                     // bLength
    USB_HID_DTYPE_HID,     // bDescriptorType
    0x111,                 // bcdHID (version 1.11 compliant)
    0,                     // bCountryCode (not localized)
    1,                     // bNumDescriptors
    USB_HID_DTYPE_REPORT,  // Report descriptor
    136                    // Size of report descriptor
}};

// config descriptor
static const uint8_t mouseConfigDescriptor[] = {
    9,                        // size of the descriptor
    USB_DTYPE_CONFIGURATION,  // type of this structure
    USBShort(34),             // total size of structure
    1,                        // total number of interface
    1,                        // unique value for this config
    5,                        // index of string descriptor
    USB_CONF_ATTR_BUS_PWR,    // powering mode
    MAX_PWR_MA,               // max power in 2 mA increment
};

// interface descriptor
static const uint8_t mouseHIDDescriptor[] = {
    9,                       // Size of the interface descriptor.
    USB_DTYPE_INTERFACE,     // Type of this descriptor.
    0,                       // The index for this interface.
    0,                       // The alternate setting for this interface.
    1,                       // The number of endpoints used by this interface.
    USB_CLASS_HID,           // The interface class
    USB_HID_SCLASS_BOOT,     // The interface sub-class.
    USB_HID_PROTOCOL_MOUSE,  // The interface protocol for the sub-class specified above.
    4,                       // The string index for this interface.
    9,                       // Size of this HID descriptor.
    USB_HID_DTYPE_HID,       // HID descriptor type.
    USBShort(0x101),         // Version is 1.1.
    0,                       // Country code is not specified.
    1,                       // Number of descriptors.
    USB_HID_DTYPE_REPORT,    // Type of this descriptor.
    USBShort(sizeof(mouseReportDescriptor)),  // Length of the Descriptor.
};

// endpoint descriptors
static const uint8_t mouseEndPointDescriptor[] = {
    7,                   // The size of the endpoint descriptor.
    USB_DTYPE_ENDPOINT,  // Descriptor type is an endpoint.
    USB_EP_DESC_IN | USB_EP_TO_INDEX(INT_IN_ENDPOINT),
    USB_EP_ATTR_INT,  // Endpoint is an interrupt endpoint.
    USBShort(64),     // The maximum packet size.
    16,               // The polling interval for this endpoint.
};

/* Combining all of the above and be ready to send to host */

static const tConfigSection mouseConfigPartialSection = {sizeof(mouseConfigDescriptor),
                                                         mouseConfigDescriptor};

static const tConfigSection g_sHIDInterfaceSection = {sizeof(mouseHIDDescriptor),
                                                      mouseHIDDescriptor};

static const tConfigSection g_sHIDInEndpointSection = {sizeof(mouseEndPointDescriptor),
                                                       mouseEndPointDescriptor};

// the final array that has everything
static const tConfigSection *mouseConfigFullSection[] = {
    &mouseConfigPartialSection, &g_sHIDInterfaceSection, &g_sHIDInEndpointSection};

#define NUM_KEYBOARD_SECTIONS (sizeof(mouseConfigFullSection) / sizeof(tConfigSection *))

static const tConfigHeader mouseConfigHeader = {NUM_KEYBOARD_SECTIONS, mouseConfigFullSection};

static const tConfigHeader *const configHeaderList[] = {&mouseConfigHeader};

tHIDReportIdle mouseReportIdle[] = {
    {125, 1, 0, 0},  // Report 1 polled every 500mS (4 * 125).
};

tUSBDHIDDevice mouseDevice = {.ui16VID                  = VENDOR_ID,
                              .ui16PID                  = PRODUCT_ID,
                              .ui16MaxPowermA           = MAX_PWR_MA,
                              .ui8PwrAttributes         = USB_CONF_ATTR_BUS_PWR,
                              .ui8Subclass              = USB_SUBCLASS_UNDEFINED,
                              .ui8Protocol              = USB_HID_PROTOCOL_MOUSE,
                              .ui8NumInputReports       = 1,
                              .psReportIdle             = mouseReportIdle,
                              .pfnRxCallback            = mouseRxCallBack,
                              .pvRxCBData               = (void *)(&mouseDevice),
                              .pfnTxCallback            = mouseTxCallBack,
                              .pvTxCBData               = NULL,
                              .bUseOutEndpoint          = false,
                              .psHIDDescriptor          = hidDescriptorList,
                              .ppui8ClassDescriptors    = mouseClassDescriptor,
                              .ppui8StringDescriptors   = mouseStringDescriptorList,
                              .ui32NumStringDescriptors = TOTAL_STRING_DESCRIPTOR,
                              .ppsConfigDescriptor      = configHeaderList,
                              .sPrivateData             = 0};
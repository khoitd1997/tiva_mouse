#ifndef _MOUSE_USB_CONFIG_H
#define _MOUSE_USB_CONFIG_H

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

/**********************STRING DESCRIPTOR******************************/
#define TOTAL_STRING_DESCRIPTOR 4
#define LANG_DESCRIPTOR_LEN 4

const uint8_t mouseLanguageString[] = {
    LANG_DESCRIPTOR_LEN, USB_DTYPE_STRING, USBShort(USB_LANG_EN_US)};

const uint8_t mouseManufactureString[] = {
    (17 + 1) * 2, USB_DTYPE_STRING,
    'K',          0,
    'h',          0,
    'o',          0,
    'i',          0,
    'T',          0,
    ' ',          0,
    'I',          0,
    'n',          0,
    's',          0,
    't',          0,
    'r',          0,
    'u',          0,
    'm',          0,
    'e',          0,
    'n',          0,
    't',          0,
    's',          0,
};

const uint8_t mouseProductString[] = {(16 + 1) * 2, USB_DTYPE_STRING,
                                      'K',          0,
                                      'e',          0,
                                      'y',          0,
                                      'b',          0,
                                      'o',          0,
                                      'a',          0,
                                      'r',          0,
                                      'd',          0,
                                      ' ',          0,
                                      'E',          0,
                                      'x',          0,
                                      'a',          0,
                                      'm',          0,
                                      'p',          0,
                                      'l',          0,
                                      'e',          0};

const uint8_t mouseSerialString[] = {
    (8 + 1) * 2, USB_DTYPE_STRING, '1', 0, '2', 0, '3', 0, '4', 0, '5', 0, '6', 0, '7', 0, '8', 0};

const uint8_t *const mouseStringDescriptorList[] = {
    mouseLanguageString, mouseManufactureString, mouseProductString, mouseSerialString};

/*******************HID report descriptor********************************/
const uint8_t mouseReportDescriptor[] = {
    0x05,
    0x01,  //     Usage Page (Generic Desktop)
    0x09,
    0x02,  //     Usage (Mouse)

    0xA1,
    0x01,  //     Collection (Application)

    // Report ID 1: Mouse buttons + scroll/pan
    0x85,
    0x01,  //     Report Id 1
    0x09,
    0x01,  //     Usage (Pointer)
    0xA1,
    0x00,  //     Collection (Physical)
    0x95,
    0x05,  //     Report Count (3)
    0x75,
    0x01,  //     Report Size (1)
    0x05,
    0x09,  //     Usage Page (Buttons)
    0x19,
    0x01,  //     Usage Minimum (01)
    0x29,
    0x05,  //     Usage Maximum (05)
    0x15,
    0x00,  //     Logical Minimum (0)
    0x25,
    0x01,  //     Logical Maximum (1)
    0x81,
    0x02,  //     Input (Data, Variable, Absolute)
    0x95,
    0x01,  //     Report Count (1)
    0x75,
    0x03,  //     Report Size (3)
    0x81,
    0x01,  //     Input (Constant) for padding
    0x75,
    0x08,  //     Report Size (8)
    0x95,
    0x01,  //     Report Count (1)
    0x05,
    0x01,  //     Usage Page (Generic Desktop)
    0x09,
    0x38,  //     USAGE (Wheel)
    0x15,
    0x81,  //     Logical Minimum (-127)
    0x25,
    0x7F,  //     Logical Maximum (127)
    0x81,
    0x06,  //     Input (Data, Variable, Relative)
    0x05,
    0x0C,  //     Usage Page (Consumer)
    0x0A,
    0x38,
    0x02,  //     Usage (AC Pan)
    0x95,
    0x01,  //     Report Count (1)
    0x81,
    0x06,  //     Input (Data,Value,Relative,Bit Field)
    0xC0,  //     End Collection (Physical)

    // Report ID 2: Mouse motion
    0x85,
    0x02,  //     Report Id 2
    0x09,
    0x01,  //     Usage (Pointer)
    0xA1,
    0x00,  //     Collection (Physical)
    0x75,
    0x0C,  //     Report Size (12)
    0x95,
    0x02,  //     Report Count (2)
    0x05,
    0x01,  //     Usage Page (Generic Desktop)
    0x09,
    0x30,  //     Usage (X)
    0x09,
    0x31,  //     Usage (Y)
    0x16,
    0x01,
    0xF8,  //     Logical maximum (2047)
    0x26,
    0xFF,
    0x07,  //     Logical minimum (-2047)
    0x81,
    0x06,  //     Input (Data, Variable, Relative)
    0xC0,  //   End Collection (Physical)
    0xC0,  // End Collection (Application)

    // Report ID 3: Advanced buttons
    0x05,
    0x0C,  // Usage Page (Consumer)
    0x09,
    0x01,  // Usage (Consumer Control)
    0xA1,
    0x01,  // Collection (Application)
    0x85,
    0x03,  // Report Id (3)
    0x15,
    0x00,  // Logical minimum (0)
    0x25,
    0x01,  // Logical maximum (1)
    0x75,
    0x01,  // Report Size (1)
    0x95,
    0x01,  // Report Count (1)

    0x09,
    0xCD,  // Usage (Play/Pause)
    0x81,
    0x06,  // Input (Data,Value,Relative,Bit Field)
    0x0A,
    0x83,
    0x01,  // Usage (AL Consumer Control Configuration)
    0x81,
    0x06,  // Input (Data,Value,Relative,Bit Field)
    0x09,
    0xB5,  // Usage (Scan Next Track)
    0x81,
    0x06,  // Input (Data,Value,Relative,Bit Field)
    0x09,
    0xB6,  // Usage (Scan Previous Track)
    0x81,
    0x06,  // Input (Data,Value,Relative,Bit Field)

    0x09,
    0xEA,  // Usage (Volume Down)
    0x81,
    0x06,  // Input (Data,Value,Relative,Bit Field)
    0x09,
    0xE9,  // Usage (Volume Up)
    0x81,
    0x06,  // Input (Data,Value,Relative,Bit Field)
    0x0A,
    0x25,
    0x02,  // Usage (AC Forward)
    0x81,
    0x06,  // Input (Data,Value,Relative,Bit Field)
    0x0A,
    0x24,
    0x02,  // Usage (AC Back)
    0x81,
    0x06,  // Input (Data,Value,Relative,Bit Field)
    0xC0   // End Collection
};

const uint8_t *const mouseClassDescriptor[] = {mouseReportDescriptor};

/********************USB General Descriptor*********************/

const tHIDDescriptor hidDescriptorList[] = {{
    9,                     // bLength
    USB_HID_DTYPE_HID,     // bDescriptorType
    0x111,                 // bcdHID (version 1.11 compliant)
    0,                     // bCountryCode (not localized)
    1,                     // bNumDescriptors
    USB_HID_DTYPE_REPORT,  // Report descriptor
    136                    // Size of report descriptor
}};

// config descriptor
const uint8_t mouseConfigDescriptor[] = {
    9,                        // size of the descriptor
    USB_DTYPE_CONFIGURATION,  // type of this structure
    USBShort(34),             // total size of structure
    1,                        // total number of interface
    1,                        // unique value for this config
    5,                        // index of string descriptor
    USB_CONF_ATTR_BUS_PWR,    // powering mode
    125,                      // max power in 2 mA increment
};

// interface descriptor
uint8_t mouseHIDDescriptor[] = {
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
const uint8_t mouseEndPointDescriptor[] = {
    7,                   // The size of the endpoint descriptor.
    USB_DTYPE_ENDPOINT,  // Descriptor type is an endpoint.
    USB_EP_DESC_IN | USB_EP_TO_INDEX(INT_IN_ENDPOINT),
    USB_EP_ATTR_INT,  // Endpoint is an interrupt endpoint.
    USBShort(64),     // The maximum packet size.
    16,               // The polling interval for this endpoint.
};

/* Combining all of the above and be ready to send to host */

const tConfigSection mouseConfigPartialSection = {sizeof(mouseConfigDescriptor),
                                                  mouseConfigDescriptor};

const tConfigSection g_sHIDInterfaceSection = {sizeof(mouseHIDDescriptor), mouseHIDDescriptor};

const tConfigSection g_sHIDInEndpointSection = {sizeof(mouseEndPointDescriptor),
                                                mouseEndPointDescriptor};

// the final array that has everything
const tConfigSection *mouseConfigFullSection[] = {
    &mouseConfigPartialSection, &g_sHIDInterfaceSection, &g_sHIDInEndpointSection};

#define NUM_KEYBOARD_SECTIONS (sizeof(mouseConfigFullSection) / sizeof(tConfigSection *))

const tConfigHeader mouseConfigHeader = {NUM_KEYBOARD_SECTIONS, mouseConfigFullSection};

const tConfigHeader *const configHeaderList[] = {&mouseConfigHeader};

tHIDReportIdle g_psReportIdle[] = {
    {125, 1, 0, 0},  // Report 1 polled every 500mS (4 * 125).
};

#endif
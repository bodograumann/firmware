#include "usb_api.h"
#include "usb_composite_device.h"
#include "usb_descriptor_mouse_report.h"
#include "usb_descriptor_generic_hid_report.h"
#include "usb_descriptor_gamepad_report.h"

#define USB_CONFIGURATION_DESCRIPTOR_TOTAL_LENGTH \
    (USB_DESCRIPTOR_LENGTH_CONFIGURE + \
    2 * (USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_HID + (2 * USB_DESCRIPTOR_LENGTH_ENDPOINT)) + \
    4 * (USB_DESCRIPTOR_LENGTH_INTERFACE + USB_DESCRIPTOR_LENGTH_HID + USB_DESCRIPTOR_LENGTH_ENDPOINT))

#define USB_GENERIC_HID_DESCRIPTOR_INDEX \
    (USB_DESCRIPTOR_LENGTH_CONFIGURE + USB_DESCRIPTOR_LENGTH_INTERFACE)

#define USB_BASIC_KEYBOARD_HID_DESCRIPTOR_INDEX \
    (USB_GENERIC_HID_DESCRIPTOR_INDEX + USB_DESCRIPTOR_LENGTH_HID + \
    2 * USB_DESCRIPTOR_LENGTH_ENDPOINT + USB_DESCRIPTOR_LENGTH_INTERFACE)

#define USB_MEDIA_KEYBOARD_HID_DESCRIPTOR_INDEX \
    (USB_BASIC_KEYBOARD_HID_DESCRIPTOR_INDEX + USB_DESCRIPTOR_LENGTH_HID + \
    USB_DESCRIPTOR_LENGTH_ENDPOINT + USB_DESCRIPTOR_LENGTH_INTERFACE)

#define USB_SYSTEM_KEYBOARD_HID_DESCRIPTOR_INDEX \
    (USB_MEDIA_KEYBOARD_HID_DESCRIPTOR_INDEX + USB_DESCRIPTOR_LENGTH_HID + \
    USB_DESCRIPTOR_LENGTH_ENDPOINT + USB_DESCRIPTOR_LENGTH_INTERFACE)

#define USB_MOUSE_HID_DESCRIPTOR_INDEX \
    (USB_SYSTEM_KEYBOARD_HID_DESCRIPTOR_INDEX + USB_DESCRIPTOR_LENGTH_HID + \
    USB_DESCRIPTOR_LENGTH_ENDPOINT + USB_DESCRIPTOR_LENGTH_INTERFACE)

#define USB_GAMEPAD_HID_DESCRIPTOR_INDEX \
    (USB_MOUSE_HID_DESCRIPTOR_INDEX + USB_DESCRIPTOR_LENGTH_HID + \
    USB_DESCRIPTOR_LENGTH_ENDPOINT + USB_DESCRIPTOR_LENGTH_INTERFACE)

static USB_DESC_STORAGE_TYPE(uint8_t) UsbConfigurationDescriptor[USB_CONFIGURATION_DESCRIPTOR_TOTAL_LENGTH] = {

    // Configuration descriptor
    USB_DESCRIPTOR_LENGTH_CONFIGURE,
    USB_DESCRIPTOR_TYPE_CONFIGURE,
    USB_SHORT_GET_LOW(USB_CONFIGURATION_DESCRIPTOR_TOTAL_LENGTH),
    USB_SHORT_GET_HIGH(USB_CONFIGURATION_DESCRIPTOR_TOTAL_LENGTH),
    USB_DEVICE_CONFIG_HID,
    USB_COMPOSITE_CONFIGURATION_INDEX,
    USB_STRING_DESCRIPTOR_NONE,
    (USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_D7_MASK) |
        (USB_DEVICE_CONFIG_REMOTE_WAKEUP << USB_DESCRIPTOR_CONFIGURE_ATTRIBUTE_REMOTE_WAKEUP_SHIFT),
    USB_DEVICE_MAX_POWER,

    // Generic HID interface descriptor
    USB_DESCRIPTOR_LENGTH_INTERFACE,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    USB_GENERIC_HID_INTERFACE_INDEX,
    USB_INTERFACE_ALTERNATE_SETTING_NONE,
    USB_GENERIC_HID_ENDPOINT_COUNT,
    USB_CLASS_HID,
    USB_HID_SUBCLASS_NONE,
    USB_HID_PROTOCOL_NONE,
    USB_STRING_DESCRIPTOR_NONE,

    // Generic HID descriptor
    USB_DESCRIPTOR_LENGTH_HID,
    USB_DESCRIPTOR_TYPE_HID,
    USB_SHORT_GET_LOW(USB_HID_VERSION),
    USB_SHORT_GET_HIGH(USB_HID_VERSION),
    USB_HID_COUNTRY_CODE_NOT_SUPPORTED,
    USB_REPORT_DESCRIPTOR_COUNT_PER_HID_DEVICE,
    USB_DESCRIPTOR_TYPE_HID_REPORT,
    USB_SHORT_GET_LOW(USB_GENERIC_HID_REPORT_DESCRIPTOR_LENGTH),
    USB_SHORT_GET_HIGH(USB_GENERIC_HID_REPORT_DESCRIPTOR_LENGTH),

    // Generic HID IN endpoint descriptor
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_GENERIC_HID_ENDPOINT_IN_INDEX | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(USB_GENERIC_HID_INTERRUPT_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(USB_GENERIC_HID_INTERRUPT_IN_PACKET_SIZE),
    USB_GENERIC_HID_INTERRUPT_IN_INTERVAL,

    // Generic HID OUT endpoint descriptor
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_GENERIC_HID_ENDPOINT_OUT_INDEX | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(USB_GENERIC_HID_INTERRUPT_OUT_PACKET_SIZE),
    USB_SHORT_GET_HIGH(USB_GENERIC_HID_INTERRUPT_OUT_PACKET_SIZE),
    USB_GENERIC_HID_INTERRUPT_IN_INTERVAL,

    // Basic keyboard interface descriptor
    USB_DESCRIPTOR_LENGTH_INTERFACE,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    USB_BASIC_KEYBOARD_INTERFACE_INDEX,
    USB_INTERFACE_ALTERNATE_SETTING_NONE,
    USB_BASIC_KEYBOARD_ENDPOINT_COUNT,
    USB_CLASS_HID,
    USB_HID_SUBCLASS_BOOT,
    USB_HID_PROTOCOL_KEYBOARD,
    USB_STRING_DESCRIPTOR_NONE,

    // Basic keyboard HID descriptor
    USB_DESCRIPTOR_LENGTH_HID,
    USB_DESCRIPTOR_TYPE_HID,
    USB_SHORT_GET_LOW(USB_HID_VERSION),
    USB_SHORT_GET_HIGH(USB_HID_VERSION),
    USB_HID_COUNTRY_CODE_NOT_SUPPORTED,
    USB_REPORT_DESCRIPTOR_COUNT_PER_HID_DEVICE,
    USB_DESCRIPTOR_TYPE_HID_REPORT,
    USB_SHORT_GET_LOW(USB_BASIC_KEYBOARD_REPORT_DESCRIPTOR_LENGTH),
    USB_SHORT_GET_HIGH(USB_BASIC_KEYBOARD_REPORT_DESCRIPTOR_LENGTH),

    // Basic keyboard endpoint descriptor
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_BASIC_KEYBOARD_ENDPOINT_INDEX | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(USB_BASIC_KEYBOARD_INTERRUPT_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(USB_BASIC_KEYBOARD_INTERRUPT_IN_PACKET_SIZE),
    USB_BASIC_KEYBOARD_INTERRUPT_IN_INTERVAL,

    // Media keyboard interface descriptor
    USB_DESCRIPTOR_LENGTH_INTERFACE,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    USB_MEDIA_KEYBOARD_INTERFACE_INDEX,
    USB_INTERFACE_ALTERNATE_SETTING_NONE,
    USB_MEDIA_KEYBOARD_ENDPOINT_COUNT,
    USB_CLASS_HID,
    USB_HID_SUBCLASS_NONE,
    USB_HID_PROTOCOL_NONE,
    USB_STRING_DESCRIPTOR_NONE,

    // Media keyboard HID descriptor
    USB_DESCRIPTOR_LENGTH_HID,
    USB_DESCRIPTOR_TYPE_HID,
    USB_SHORT_GET_LOW(USB_HID_VERSION),
    USB_SHORT_GET_HIGH(USB_HID_VERSION),
    USB_HID_COUNTRY_CODE_NOT_SUPPORTED,
    USB_REPORT_DESCRIPTOR_COUNT_PER_HID_DEVICE,
    USB_DESCRIPTOR_TYPE_HID_REPORT,
    USB_SHORT_GET_LOW(USB_MEDIA_KEYBOARD_REPORT_DESCRIPTOR_LENGTH),
    USB_SHORT_GET_HIGH(USB_MEDIA_KEYBOARD_REPORT_DESCRIPTOR_LENGTH),

    // Media keyboard endpoint descriptor
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_MEDIA_KEYBOARD_ENDPOINT_INDEX | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(USB_MEDIA_KEYBOARD_INTERRUPT_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(USB_MEDIA_KEYBOARD_INTERRUPT_IN_PACKET_SIZE),
    USB_MEDIA_KEYBOARD_INTERRUPT_IN_INTERVAL,

    // System keyboard interface descriptor
    USB_DESCRIPTOR_LENGTH_INTERFACE,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    USB_SYSTEM_KEYBOARD_INTERFACE_INDEX,
    USB_INTERFACE_ALTERNATE_SETTING_NONE,
    USB_SYSTEM_KEYBOARD_ENDPOINT_COUNT,
    USB_CLASS_HID,
    USB_HID_SUBCLASS_NONE,
    USB_HID_PROTOCOL_NONE,
    USB_STRING_DESCRIPTOR_NONE,

    // System keyboard HID descriptor
    USB_DESCRIPTOR_LENGTH_HID,
    USB_DESCRIPTOR_TYPE_HID,
    USB_SHORT_GET_LOW(USB_HID_VERSION),
    USB_SHORT_GET_HIGH(USB_HID_VERSION),
    USB_HID_COUNTRY_CODE_NOT_SUPPORTED,
    USB_REPORT_DESCRIPTOR_COUNT_PER_HID_DEVICE,
    USB_DESCRIPTOR_TYPE_HID_REPORT,
    USB_SHORT_GET_LOW(USB_SYSTEM_KEYBOARD_REPORT_DESCRIPTOR_LENGTH),
    USB_SHORT_GET_HIGH(USB_SYSTEM_KEYBOARD_REPORT_DESCRIPTOR_LENGTH),

    // System keyboard endpoint descriptor
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_SYSTEM_KEYBOARD_ENDPOINT_INDEX | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(USB_SYSTEM_KEYBOARD_INTERRUPT_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(USB_SYSTEM_KEYBOARD_INTERRUPT_IN_PACKET_SIZE),
    USB_SYSTEM_KEYBOARD_INTERRUPT_IN_INTERVAL,

    // Mouse interface descriptor
    USB_DESCRIPTOR_LENGTH_INTERFACE,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    USB_MOUSE_INTERFACE_INDEX,
    USB_INTERFACE_ALTERNATE_SETTING_NONE,
    USB_MOUSE_ENDPOINT_COUNT,
    USB_CLASS_HID,
    USB_HID_SUBCLASS_BOOT,
    USB_HID_PROTOCOL_MOUSE,
    USB_STRING_DESCRIPTOR_NONE,

    // Mouse HID descriptor
    USB_DESCRIPTOR_LENGTH_HID,
    USB_DESCRIPTOR_TYPE_HID,
    USB_SHORT_GET_LOW(USB_HID_VERSION),
    USB_SHORT_GET_HIGH(USB_HID_VERSION),
    USB_HID_COUNTRY_CODE_NOT_SUPPORTED,
    USB_REPORT_DESCRIPTOR_COUNT_PER_HID_DEVICE,
    USB_DESCRIPTOR_TYPE_HID_REPORT,
    USB_SHORT_GET_LOW(USB_MOUSE_REPORT_DESCRIPTOR_LENGTH),
    USB_SHORT_GET_HIGH(USB_MOUSE_REPORT_DESCRIPTOR_LENGTH),

    // Mouse endpoint descriptor
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_MOUSE_ENDPOINT_INDEX | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(USB_MOUSE_INTERRUPT_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(USB_MOUSE_INTERRUPT_IN_PACKET_SIZE),
    USB_MOUSE_INTERRUPT_IN_INTERVAL,

    // Gamepad HID interface descriptor
    USB_DESCRIPTOR_LENGTH_INTERFACE,
    USB_DESCRIPTOR_TYPE_INTERFACE,
    USB_GAMEPAD_INTERFACE_INDEX,
    USB_INTERFACE_ALTERNATE_SETTING_NONE,
    USB_GAMEPAD_ENDPOINT_COUNT,
    USB_CLASS_HID,
    USB_HID_SUBCLASS_NONE,
    USB_HID_PROTOCOL_NONE,
    USB_STRING_DESCRIPTOR_NONE,

    // Gamepad HID descriptor
    USB_DESCRIPTOR_LENGTH_HID,
    USB_DESCRIPTOR_TYPE_HID,
    USB_SHORT_GET_LOW(USB_HID_VERSION),
    USB_SHORT_GET_HIGH(USB_HID_VERSION),
    USB_HID_COUNTRY_CODE_NOT_SUPPORTED,
    USB_REPORT_DESCRIPTOR_COUNT_PER_HID_DEVICE,
    USB_DESCRIPTOR_TYPE_HID_REPORT,
    USB_SHORT_GET_LOW(USB_GAMEPAD_REPORT_DESCRIPTOR_LENGTH),
    USB_SHORT_GET_HIGH(USB_GAMEPAD_REPORT_DESCRIPTOR_LENGTH),

    // Gamepad HID IN endpoint descriptor
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_GAMEPAD_ENDPOINT_INDEX | (USB_IN << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(USB_GAMEPAD_INTERRUPT_IN_PACKET_SIZE),
    USB_SHORT_GET_HIGH(USB_GAMEPAD_INTERRUPT_IN_PACKET_SIZE),
    USB_GAMEPAD_INTERRUPT_IN_INTERVAL,

    // Gamepad HID OUT endpoint descriptor
    USB_DESCRIPTOR_LENGTH_ENDPOINT,
    USB_DESCRIPTOR_TYPE_ENDPOINT,
    USB_GAMEPAD_ENDPOINT_INDEX | (USB_OUT << USB_DESCRIPTOR_ENDPOINT_ADDRESS_DIRECTION_SHIFT),
    USB_ENDPOINT_INTERRUPT,
    USB_SHORT_GET_LOW(USB_GAMEPAD_INTERRUPT_OUT_PACKET_SIZE),
    USB_SHORT_GET_HIGH(USB_GAMEPAD_INTERRUPT_OUT_PACKET_SIZE),
    USB_GAMEPAD_INTERRUPT_OUT_INTERVAL,
};

usb_status_t USB_DeviceGetHidDescriptor(
    usb_device_handle handle, usb_device_get_hid_descriptor_struct_t *hidDescriptor)
{
    hidDescriptor->length = USB_DESCRIPTOR_LENGTH_HID;

    switch (hidDescriptor->interfaceNumber) {
        case USB_GENERIC_HID_INTERFACE_INDEX:
            hidDescriptor->buffer = (uint8_t*)&UsbConfigurationDescriptor[USB_GENERIC_HID_DESCRIPTOR_INDEX];
            break;
        case USB_BASIC_KEYBOARD_INTERFACE_INDEX:
            hidDescriptor->buffer = (uint8_t*)&UsbConfigurationDescriptor[USB_BASIC_KEYBOARD_HID_DESCRIPTOR_INDEX];
            break;
        case USB_MEDIA_KEYBOARD_INTERFACE_INDEX:
            hidDescriptor->buffer = (uint8_t*)&UsbConfigurationDescriptor[USB_MEDIA_KEYBOARD_HID_DESCRIPTOR_INDEX];
            break;
        case USB_SYSTEM_KEYBOARD_INTERFACE_INDEX:
            hidDescriptor->buffer = (uint8_t*)&UsbConfigurationDescriptor[USB_SYSTEM_KEYBOARD_HID_DESCRIPTOR_INDEX];
            break;
        case USB_MOUSE_INTERFACE_INDEX:
            hidDescriptor->buffer = (uint8_t*)&UsbConfigurationDescriptor[USB_MOUSE_HID_DESCRIPTOR_INDEX];
            break;
        case USB_GAMEPAD_INTERFACE_INDEX:
            hidDescriptor->buffer = (uint8_t*)&UsbConfigurationDescriptor[USB_GAMEPAD_HID_DESCRIPTOR_INDEX];
            break;
        default:
            return kStatus_USB_InvalidRequest;
    }
    return kStatus_USB_Success;
}

usb_status_t USB_DeviceGetConfigurationDescriptor(
    usb_device_handle handle, usb_device_get_configuration_descriptor_struct_t *configurationDescriptor)
{
    if (USB_COMPOSITE_CONFIGURATION_INDEX > configurationDescriptor->configuration) {
        configurationDescriptor->buffer = (uint8_t*)UsbConfigurationDescriptor;
        configurationDescriptor->length = USB_CONFIGURATION_DESCRIPTOR_TOTAL_LENGTH;
        return kStatus_USB_Success;
    }
    return kStatus_USB_InvalidRequest;
}

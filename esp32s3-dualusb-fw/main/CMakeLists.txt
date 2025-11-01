idf_component_register(SRCS
    "main.c"
    "usb_device.c"
    "usb_host.c"
    "usb_mode.c"
    "filesystem.c"
    "led_control.c"
    "led_autoprobe.c"
INCLUDE_DIRS "."
REQUIRES esp_tinyusb usb)

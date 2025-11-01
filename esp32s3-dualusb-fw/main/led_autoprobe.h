/**
 * @file led_autoprobe.h
 * @brief LED autoprobe header - detects which GPIO has active LED
 *
 * Author: A.R. Ansari <ansarirahim1@gmail.com>
 * Date: 2025-11-01
 */

#ifndef LED_AUTOPROBE_H
#define LED_AUTOPROBE_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize LED autoprobe
 * 
 * Starts autoprobe task that tests GPIO 6 and 38 with staggered blink pattern.
 * User should observe physical LED to determine which GPIO is active.
 */
void led_autoprobe_init(void);

/**
 * @brief Set the active LED GPIO (called after manual detection)
 * 
 * @param gpio GPIO number (6 or 38)
 */
void led_autoprobe_set_active(int gpio);

/**
 * @brief Get the active LED GPIO
 * 
 * @return GPIO number or -1 if not set
 */
int led_autoprobe_get_active(void);

#ifdef __cplusplus
}
#endif

#endif /* LED_AUTOPROBE_H */


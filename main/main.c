/*
 *
 *
 */

#include <string.h>
#include <errno.h>

#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_event.h"
#include "portmacro.h"

#include "sdkconfig.h"

// ---- Local components ---- //
// #include "code_snippets/general.h"

// ---- Local includes ---- //

// ---- End of includes

__attribute__((unused))
static const char* TAG = "app_main";

// ---- Private function declarations ---- //


//! Program entry point
void app_main(void)
{
    // Make everything 'INFO' by default
    esp_log_level_set("*", ESP_LOG_INFO);

    // Increase verbosity of some components, being tested
    // esp_log_level_set("console", ESP_LOG_DEBUG);

    // ---- log level verbosity set ---- //

    // Lots of system API's use this, and will crash if it's not setup
    // No harm in starting it early
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    vTaskDelete(NULL);      // Delete this task (main task)
    return;
}


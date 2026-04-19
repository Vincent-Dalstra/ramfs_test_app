/* Example test application for ramfs.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "unity.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "esp_err.h"
#include "esp_check.h"

__attribute__((unused))
static const char* TAG = "app_main";

static void print_banner(const char* text);

// Easier to adjust than having to rebuild sdkconfig each time, to adjust main() stack size
#define UNITY_TASK_STACKMEM (3584 + 2056)

// Task that handles the unit-tests - makes it easy to adjust stack memory etc.
TaskHandle_t unity_main_task_handle;
void unity_main_task(void *pvParameter);

// Start the other task, then immediately kill the task (and recover its stack memory)
void app_main(void)
{
    esp_log_level_set("*", ESP_LOG_INFO);

    xTaskCreate(&unity_main_task, "unity", UNITY_TASK_STACKMEM, NULL, 10, &unity_main_task_handle);

    vTaskDelete(NULL);      // Delete this task (main task)
    __unreachable();
}

void unity_main_task(void *pvParameter)
{
    esp_log_level_set("*", ESP_LOG_DEBUG);      // Print more messages for tests under development

    print_banner("Running tests currently in development");
    UNITY_BEGIN();
    unity_run_tests_by_tag("[TDD-dev]", false);
    UNITY_END();

    // Give a bit of time to see it
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    // Reduce serial monitor clutter
    esp_log_level_set("*", ESP_LOG_WARN);       // Turn off log output from non-unity stuff

    /* These are the different ways of running registered tests.
     * In practice, only one of them is usually needed.
     *
     * UNITY_BEGIN() and UNITY_END() calls tell Unity to print a summary
     * (number of tests executed/failed/ignored) of tests executed between these calls.
     */
//    print_banner("Executing one test by its name");
//    UNITY_BEGIN();
//    unity_run_test_by_name("Mean of an empty array is zero");
//    UNITY_END();
//
//    print_banner("Running tests with [mean] tag");
//    UNITY_BEGIN();
//    unity_run_tests_by_tag("[mean]", false);
//    UNITY_END();

    print_banner("Run spuriously-failing tests: [fails-sometimes] tag");
    UNITY_BEGIN();
    unity_run_tests_by_tag("[fails-sometimes]", false);
    UNITY_END();

    vTaskDelay(500 / portTICK_PERIOD_MS);
    print_banner("Running the tests that have the [fails] tag");
    UNITY_BEGIN();
    unity_run_tests_by_tag("[fails]", false);
    UNITY_END();

    vTaskDelay(500 / portTICK_PERIOD_MS);

    print_banner("Running tests without any kind of [fails tag");
    UNITY_BEGIN();
    unity_run_tests_by_tag("[fails", true);
    UNITY_END();

    // Give a bit of time to see it
    vTaskDelay(2000 / portTICK_PERIOD_MS);

    esp_log_level_set("*", ESP_LOG_DEBUG);

    print_banner("Running tests currently in development");
    UNITY_BEGIN();
    unity_run_tests_by_tag("[TDD-dev]", false);
    UNITY_END();

//    print_banner("Running all the registered tests");
//    UNITY_BEGIN();
//    unity_run_all_tests();
//    UNITY_END();

    while (1) {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    print_banner("Starting interactive test menu");
    /* This function will not return, and will be busy waiting for UART input.
     * Make sure that task watchdog is disabled if you use this function.
     */
    unity_run_menu();

}

static void print_banner(const char* text)
{
    printf("\n#### %s #####\n\n", text);
}

/*
 * See the commit message (reproduced below) which explains why this code
 * exists (Note: there were about ~100 test-cases at this time).
 *
 * TL;DR    (1) unity creates a 'constructor' for EVERY SINGLE TEST_CASE() !!!
 *          (2) Really weird race conditions involving constructors being called at boot time
 *          (3) constructors take 0.3s each when LOGD is enabled during boot
 */
__attribute__((constructor))        // This attribute causes the compiler to run it at boot time (before app_main())
void workaround_somehow_prevents_startup_stuck()
{
    // This prevents the bug from happening, but why?
    ESP_LOGW("strange_fix", "Why does this work?, n = %d", 0);

    // ---- The following also work ---- //

//    ESP_EARLY_LOGW("strange_fix", "Why does this work?, n = %d", 0);

//    ESP_LOGW("strange_fix", "multiple numbers? n = %d, a = %d, b = %d, c = %d", 0, 22, 456, -1);

    // ----  All of the ones below do not work! ---- //

//    ESP_LOGW("strange_fix", "short: n = %d", 0);

//    ESP_LOGW("strange_fix", "So the number is important somehow?");

    // Multiple times does not work!
//    ESP_EARLY_LOGW("strange_fix", "Why does this work?, n = %d", 0);
//    ESP_EARLY_LOGW("strange_fix", "Why does this work?, n = %d", 0);
//    ESP_EARLY_LOGW("strange_fix", "Why does this work?, n = %d", 0);

    // None of these work!
//    vTaskDelay(1 / portTICK_PERIOD_MS);
//    vTaskDelay(5 / portTICK_PERIOD_MS);
//    vTaskDelay(50 / portTICK_PERIOD_MS);
//    vTaskDelay(100 / portTICK_PERIOD_MS);
//    vTaskDelay(500 / portTICK_PERIOD_MS);

    return;
}

/**
commit ee58b7b017c52b2f583f56adbbd554c97a843ab6
Author: Vincent Dalstra <vincent.dalstra@outlook.com>
Date:   Fri Feb 7 17:59:43 2025 +0800

    Add strange fix to prevent hanging during startup
    
    There's a problem that's been happening, where after adding a unity
    TEST_CASE, the esp32-s3 will get stuck at boot, not progressing beyond
    the message "app_start: Starting scheduler on CPU1". This is well before
    it even executes ANY test case! Just to confirm that it's a race
    condition:
    - Removing ANY of the TEST_CASE's would make it work again
    - Setting default log level to INFO, so it doesn't print a DEBUG message
      for each and every "calling init function: ", thus speeding up the
      boot times, would also stop the issues.
    - Turning off "Run memory test on SPI SRAM initialisation" also stopped
      the issue! (reduces boot time by a fair bit as well).
    
    Digging deeper, I found out that for every TEST_CASE, unity will create
    a 'constructor' function that runs at boot, which is why there's so many
    of the damn things! But here's where it gets weird. When I tried to
    simulate the problem by creating my own function, it worked again! I
    experimented a bit and yeah... just look at the diff. For some reason
    having it print %d (of a constant 0) is vital for it to work?
    
    And it really does fix it - when I enabled the other test-components,
    (bringing the total tests up to 125!), it would still work!
    
	I don't even know what's going on. Once I have more time I'll file an
    issue on the esp-idf github, and maybe someone there will know how the
    hell this all works.
*/

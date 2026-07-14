#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "nvs_flash.h"       
#include "mqtt-aws.h"
#include "movement-driver.h"
#include "wifi-driver.h"

typedef void (*generic_func_ptr)(void);

// Global Queue Handle
QueueHandle_t myQueue;

void app_main(void) {
    
    
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
        
    myQueue = xQueueCreate(5, sizeof(void*));
    
    generic_func_ptr wifi_fn   = (generic_func_ptr)mi_wifi_inicializar; 
    generic_func_ptr sensor_fn = (generic_func_ptr)init_movement_sensor; 

    xQueueSend(myQueue, &wifi_fn, portMAX_DELAY);
    xQueueSend(myQueue, &sensor_fn, portMAX_DELAY);
    
    vTaskDelay(10000 / portTICK_PERIOD_MS);
    
    generic_func_ptr funcion_a_ejecutar;
    if (xQueueReceive(myQueue, &funcion_a_ejecutar, portMAX_DELAY) == pdPASS) {
        funcion_a_ejecutar(); // Corre mi_wifi_inicializar
    }

    vTaskDelay(pdMS_TO_TICKS(500));

    if (xQueueReceive(myQueue, &funcion_a_ejecutar, portMAX_DELAY) == pdPASS) {
        funcion_a_ejecutar();
    }
        
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
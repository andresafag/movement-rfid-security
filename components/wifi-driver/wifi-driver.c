#include "wifi-driver.h"
#include <stdio.h>
#include "esp_err.h"        
#include "esp_wifi.h"       
#include "esp_event.h"      
#include "esp_netif.h"      
#include "freertos/idf_additions.h"
#include "nvs_flash.h"      
#include "esp_log.h"        

static const char *TAG = "WIFI_DRIVER";

int mi_wifi_inicializar() 
{
    ESP_LOGI(TAG, "Iniciando configuracion de Wi-Fi...");

    
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "brigeth",          
            .password = "12345678",  
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_wifi_connect());

    ESP_LOGI(TAG, "Wi-Fi iniciado y conectando a %s...", wifi_config.sta.ssid);
	
	if (ESP_ERR_WIFI_NOT_INIT){
		return 1;
	}
	else if (ESP_ERR_WIFI_NOT_STARTED){
		return 1;
	}
	
	return ESP_OK;
} 

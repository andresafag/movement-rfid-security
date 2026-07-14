#include <stdio.h>
#include "esp_event.h"
#include "esp_err.h"
#include "esp_log.h"
#include "mqtt_client.h"  // CRITICAL: Added missing core MQTT header
#include "mqtt-aws.h"

static const char *TAG = "AWS_MQTT";

// Reference the embedded certificates declared in your CMakeLists.txt
extern const uint8_t aws_root_ca_pem_start[]   asm("_binary_aws_root_ca_pem_start");
extern const uint8_t certificate_pem_crt_start[] asm("_binary_certificate_pem_crt_start");
extern const uint8_t private_pem_key_start[]   asm("_binary_private_pem_key_start");

// Added the event handler function before it gets used below
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Successfully Connected to AWS IoT Core!");
            // Optional: Subscribe to a topic immediately upon connection
            esp_mqtt_client_subscribe(client, "esp32/sub_topic", 0);
            break;
            
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGW(TAG, "Disconnected from AWS IoT Core");
            break;
            
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Message received!");
            printf("Topic: %.*s\n", event->topic_len, event->topic);
            printf("Payload: %.*s\n", event->data_len, event->data);
            break;
            
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT Event Error encountered");
            break;
            
        default:
            break;
    }
}

void init_mqtt(void) {
    const esp_mqtt_client_config_t mqtt_cfg = {

        .broker.address.uri = "mqtts://://amazonaws.com",
        
        // Security configuration credentials
        .credentials.authentication.certificate = (const char *)certificate_pem_crt_start,
        .credentials.authentication.key = (const char *)private_pem_key_start,
        .broker.verification.certificate = (const char *)aws_root_ca_pem_start,
    };

    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    
    // Registers the handler function created above
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    
    // Starts the internal MQTT client background task
    esp_mqtt_client_start(client);
	
	char payload[128];
	    
	    // Construct your custom JSON keys and values dynamically
	snprintf(payload, sizeof(payload), 
	"{\"device_id\":\"esp32_sensor_1\",\"movement\":\"%s\"}", 
	         "detected"); 

	    // Publish to AWS. The -1 tells the driver to calculate string length automatically.
	int msg_id = esp_mqtt_client_publish(client, "device/telemetry", payload, -1, 1, 0);
	ESP_LOGI("MQTT_PUB", "Sent JSON payload, msg_id=%d", msg_id);
}

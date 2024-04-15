#include <stdio.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "esp_ota_ops.h" // Thêm thư viện quản lý OTA

#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASS "your_wifi_password"
#define OTA_URL   "https://example.com/firmware.bin"
#define CHUNK_SIZE 1024
#define OTA_DONE_FLAG_ADDR 0x0 // Địa chỉ lưu trữ cờ OTA trong Flash

static const char *TAG = "ota_update";
static int ota_retry_count = 0;
static const int OTA_RETRY_LIMIT = 3;

static esp_err_t event_handler(void *ctx, system_event_t *event) {
    return ESP_OK;
}

void wifi_init_sta() {
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS
        }
    };
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
}

void ota_task(void *pvParameter) {
    ESP_LOGI(TAG, "Starting OTA update");
    esp_http_client_config_t config = {
        .url = OTA_URL,
        .cert_pem = NULL,
    };
    esp_err_t ret = esp_https_ota(&config);
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA update successful");
        // Kiểm tra tính nguyên vẹn của firmware
        esp_ota_handle_t handle = 0;
        const esp_partition_t *update_partition = esp_ota_get_next_update_partition(NULL);
        if (update_partition != NULL) {
            ret = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &handle);
            if (ret == ESP_OK) {
                // Firmware đã được tải xuống và được chứng thực
                esp_ota_end(handle);
                esp_restart();
            } else {
                ESP_LOGE(TAG, "OTA verify failed %d", ret);
            }
        }
    } else {
        ESP_LOGE(TAG, "OTA update failed %d", ret);
        if (++ota_retry_count <= OTA_RETRY_LIMIT) {
            ESP_LOGI(TAG, "Retrying OTA update (attempt %d/%d)...", ota_retry_count, OTA_RETRY_LIMIT);
            vTaskDelay(pdMS_TO_TICKS(5000)); // Đợi 5 giây trước khi thử lại
            xTaskCreate(&ota_task, "ota_task", 8192, NULL, 5, NULL);
        } else {
            ESP_LOGE(TAG, "Reached OTA retry limit. Switching back to factory firmware...");

            // Rollback to previous firmware
            esp_ota_mark_app_invalid_rollback_and_reboot();
        }
    }
    vTaskDelete(NULL);
}

// Hàm kiểm tra xem quá trình OTA đã hoàn thành hay chưa
bool is_ota_done() {
    esp_app_desc_t running_app_info;
    esp_ota_get_partition_description(esp_ota_get_running_partition(), &running_app_info);
    return (strcmp(running_app_info.project_name, "factory") != 0); // Trả về true nếu không phải là factory firmware
}

void update_ota_flag() {
    nvs_handle_t nvs_handle;
    esp_err_t err = nvs_open("ota", NVS_READWRITE, &nvs_handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return;
    }
    // Ghi giá trị cờ vào NVS
    uint8_t ota_done_flag = 1; // Đánh dấu rằng quá trình OTA đã hoàn thành
    err = nvs_set_u8(nvs_handle, "ota_done", ota_done_flag);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) writing ota_done flag to NVS!", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "OTA flag updated successfully!");
    }
    
    // Đóng NVS handle
    nvs_close(nvs_handle);
}

void app_main() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_init(event_handler, NULL));
    wifi_init_sta();
    // Kiểm tra xem quá trình OTA đã hoàn thành chưa
    if (!is_ota_done()) {
        // Nếu quá trình OTA chưa hoàn thành, thực hiện OTA
        xTaskCreate(&ota_task, "ota_task", 8192, NULL, 5, NULL);
    } else {
        //any task
    }
}

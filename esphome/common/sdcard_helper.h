#pragma once

#include "esphome/core/log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "sdmmc_cmd.h"

namespace watch_sdcard {

static const char *const TAG = "watch_sdcard";
static sdmmc_card_t *card_handle = nullptr;
static bool is_mounted = false;

inline bool mount_sdcard() {
  if (is_mounted && card_handle != nullptr) {
    return true;
  }

  esp_vfs_fat_sdmmc_mount_config_t mount_config = {
      .format_if_mount_failed = false,
      .max_files = 5,
      .allocation_unit_size = 16 * 1024,
      .disk_status_check_enable = false,
  };

  sdmmc_host_t host = SDMMC_HOST_DEFAULT();
  sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
  slot_config.clk = GPIO_NUM_2;
  slot_config.cmd = GPIO_NUM_1;
  slot_config.d0 = GPIO_NUM_3;
  slot_config.d1 = GPIO_NUM_NC;
  slot_config.d2 = GPIO_NUM_NC;
  slot_config.d3 = GPIO_NUM_NC;
  slot_config.d4 = GPIO_NUM_NC;
  slot_config.d5 = GPIO_NUM_NC;
  slot_config.d6 = GPIO_NUM_NC;
  slot_config.d7 = GPIO_NUM_NC;
  slot_config.cd = SDMMC_SLOT_NO_CD;
  slot_config.wp = SDMMC_SLOT_NO_WP;
  slot_config.width = 1;
  slot_config.flags = 0;

  esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card_handle);
  if (ret != ESP_OK) {
    ESP_LOGW(TAG, "SD card mount failed (%s)", esp_err_to_name(ret));
    is_mounted = false;
    card_handle = nullptr;
    return false;
  }

  is_mounted = true;
  ESP_LOGI(TAG, "SD card mounted at /sdcard: %s", card_handle->cid.name);
  return true;
}

inline float get_total_mb() {
  if (!is_mounted || card_handle == nullptr)
    return 0.0f;
  uint64_t total_bytes = 0, free_bytes = 0;
  if (esp_vfs_fat_info("/sdcard", &total_bytes, &free_bytes) == ESP_OK && total_bytes > 0) {
    return (float) (total_bytes / (1024ULL * 1024ULL));
  }
  return (float) (((uint64_t) card_handle->csd.capacity * card_handle->csd.sector_size) / (1024ULL * 1024ULL));
}

inline float get_free_mb() {
  if (!is_mounted || card_handle == nullptr)
    return 0.0f;
  uint64_t total_bytes = 0, free_bytes = 0;
  if (esp_vfs_fat_info("/sdcard", &total_bytes, &free_bytes) == ESP_OK) {
    return (float) (free_bytes / (1024ULL * 1024ULL));
  }
  return 0.0f;
}

}  // namespace watch_sdcard

#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/i2s.h"
#include "wav_object.h"

#define ESP_WAV_PLAYER_DEFAULT_CONFIG() \
    {               \
    .i2s_num = I2S_NUM_0,                               \
    .i2s_pin_config = {                                 \
        .bck_o_en = 1,                                  \
        .ws_o_en = 1,                                   \
        .data_out_en = 1,                               \
    },                                                  \
    .base_cfg = {                                       \
        .mode = I2S_MODE_MASTER | I2S_MODE_TX,         \
        .sample_rate = 22050,                           \
        .bits_per_sample = 16,                          \
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,   \
        .communication_format = I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB, \
        .dma_buf_count = 4,                             \
        .dma_buf_len = 256,                             \
        .tx_desc_auto_clear = true                       \
    },                                                  \
    .queue_len = 4                                      \
}

typedef void *esp_wav_player_t;
typedef void (*esp_wav_player_cb_t)(esp_wav_player_t wav_player, void *arg);

typedef enum {
    ESP_WAV_PLAYER_STOPPED,
    ESP_WAV_PLAYER_PLAYING,
    ESP_WAV_PLAYER_PAUSED
} esp_wav_player_state_t;

typedef struct {
    int              i2s_num;
    i2s_pin_config_t i2s_pin_config;
    i2s_config_t     base_cfg;
    size_t           queue_len;
} esp_wav_player_config_t;

esp_err_t esp_wav_player_init(esp_wav_player_t *player, const esp_wav_player_config_t *config);
esp_err_t esp_wav_player_deinit(esp_wav_player_t hdl);

esp_err_t esp_wav_player_play(esp_wav_player_t player, const wav_obj_t *src);
esp_err_t esp_wav_player_stop(esp_wav_player_t player);
esp_err_t esp_wav_player_pause(esp_wav_player_t player);

esp_err_t esp_wav_player_get_state(esp_wav_player_t player, esp_wav_player_state_t *st);
esp_err_t esp_wav_player_set_volume(esp_wav_player_t player, uint8_t v);
esp_err_t esp_wav_player_get_volume(esp_wav_player_t hdl, uint8_t *vol);
esp_err_t esp_wav_player_get_queued(esp_wav_player_t hdl, size_t *qlen);

void esp_wav_player_set_start_cb(esp_wav_player_t player, esp_wav_player_cb_t cb, void *arg);
void esp_wav_player_set_end_cb(esp_wav_player_t player, esp_wav_player_cb_t cb, void *arg);

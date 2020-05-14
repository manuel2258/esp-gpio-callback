#include "input_types.h"

#include "esp_log.h"
#include "esp_timer.h"

namespace gpio_cb {

RawButtonInput::RawButtonInput(uint64_t pin, bool pullup,
                               std::function<void()> callback)
    : callback(callback) {
  mask = 1ULL << pin;

  gpio_config.intr_type = pullup ? GPIO_INTR_NEGEDGE : GPIO_INTR_POSEDGE;
  gpio_config.mode = GPIO_MODE_INPUT;
  gpio_config.pin_bit_mask = mask;
  gpio_config.pull_up_en = pullup ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE;
  gpio_config.pull_down_en =
      !pullup ? GPIO_PULLDOWN_ENABLE : GPIO_PULLDOWN_DISABLE;
}

BlockedButtonInput::BlockedButtonInput(uint64_t pin, bool pullup,
                                       uint32_t block_time,
                                       std::function<void()> callback)
    : RawButtonInput(pin, pullup, callback), block_time(block_time * 1000) {}

void BlockedButtonInput::invoke_callback(uint64_t pin) {
  auto cur_time = esp_timer_get_time();
  if (last_time + block_time <= cur_time) {
    callback();
    last_time = cur_time;
  }
}

} // namespace gpio_cb
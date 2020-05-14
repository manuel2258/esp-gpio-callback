#include "gpio_cb/input_manager.h"

#include "esp_log.h"

namespace gpio_cb {

InputManager *InputManager::instance;

static void IRAM_ATTR gpio_isr_handler(void *arg) {
  uint64_t gpio_num = (uint64_t)arg;
  xQueueSendFromISR(InputManager::get_instance()->gpio_queue, &gpio_num, NULL);
}

static void input_manager_task(void *arg) {
  InputManager::get_instance()->input_task();
}

InputManager::InputManager(std::vector<IInput *> *inputs, uint32_t stack_size)
    : inputs(inputs), gpio_queue(xQueueCreate(10, sizeof(uint64_t))) {
  if (instance != nullptr) {
    delete instance;
  }
  instance = this;

  total_mask = 0;
  for (auto input : *inputs) {
    gpio_config(input->get_gpio_config());
    total_mask |= input->get_mask();
  }

  ESP_LOGD(LOG_TAG, "Created inputmask %llu", total_mask);

  xTaskCreate(input_manager_task, "input_manager_task", stack_size, NULL, 10,
              &task_handle);

  ESP_ERROR_CHECK(
      gpio_install_isr_service(ESP_INTR_FLAG_IRAM | ESP_INTR_FLAG_EDGE));

  apply_to_inputs([this](uint64_t pin) {
    ESP_LOGD(LOG_TAG, "Installed ISR for %llu", pin);
    ESP_ERROR_CHECK(
        gpio_isr_handler_add((gpio_num_t)pin, gpio_isr_handler, (void *)pin));
  });
}

InputManager::~InputManager() {
  apply_to_inputs([](uint64_t pin) {
    ESP_ERROR_CHECK(gpio_isr_handler_remove((gpio_num_t)pin));
  });

  gpio_uninstall_isr_service();

  for (auto input : *inputs) {
    if (input->is_owned()) {
      delete input;
    }
  }
  delete inputs;
  vTaskDelete(task_handle);
  instance = nullptr;
}

void InputManager::input_task() {
  uint64_t io_num;
  for (;;) {
    if (xQueueReceive(gpio_queue, &io_num, portMAX_DELAY)) {
      ESP_LOGV(LOG_TAG, "Pressed pin %llu", io_num);
      uint64_t mask = 1ULL << io_num;
      for (auto input : *inputs) {
        if ((input->get_mask() & mask) == 0) {
          continue;
        }
        input->invoke_callback(io_num);
      }
    }
  }
}

void InputManager::apply_to_inputs(std::function<void(uint64_t)> func) {
  for (uint32_t i = 0; i < 64; ++i) {
    auto mask = 1ULL << i;
    // Check if input i is included in the mask
    if ((mask & total_mask) == 0) {
      continue;
    }
    func(i);
  }
}

} // namespace gpio_cb


#ifndef GPIOCALLBACKMANAGER_H
#define GPIOCALLBACKMANAGER_H

#include <functional>
#include <vector>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "input_types.h"

namespace gpio_cb {

/**
 * @brief Manages Inputs and theirs IRS Routine.
 */
class InputManager {
private:
  const char *LOG_TAG = "InputManager";

  static InputManager *instance;

  std::vector<IInput *> *inputs;
  uint64_t total_mask;

  TaskHandle_t task_handle;

  /**
   * @brief Applies a function to each active pin of the total_mask.
   */
  void apply_to_inputs(std::function<void(uint64_t)> func);

public:
  const xQueueHandle gpio_queue;

  /**
   * @brief Construct a new InputManager object.
   * Does also asign newly created instance to singleton.
   * Use Builder for easier way of creating the InputManager.
   * @param inputs The to register Inputs.
   * @param stack_size The Tasks stacksize (Depends on callbacks complexity).
   */
  InputManager(std::vector<IInput *> *inputs, uint32_t stack_size);
  ~InputManager();

  static InputManager *get_instance() { return instance; }

  /**
   * @brief Main Task of the InputManager.
   * Will be called from extra task created in ctor.
   */
  void input_task();
};
} // namespace gpio_cb

#endif // GPIOCALLBACKMANAGER_H

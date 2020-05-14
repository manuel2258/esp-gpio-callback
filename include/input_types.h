#ifndef IINPUT_H
#define IINPUT_H

#include <functional>
#include <vector>

#include "driver/gpio.h"

namespace input {

/**
 * @brief Base Interface for all Inputs.
 *
 */
class IInput {
public:
  virtual ~IInput() = default;

  /**
   * @brief Gets the inputs gpio_config.
   * @return gpio_config_t* The Inputs gpio_config
   */
  virtual gpio_config_t *get_gpio_config() = 0;

  /**
   * @brief Gets the inputs bit mask.
   * @return unsigned char The Inputs mask
   */
  virtual uint64_t get_mask() = 0;

  /**
   * @brief Invokes the inputs callback.
   * Should be called when the input's pin changes.
   */
  virtual void invoke_callback(uint64_t pin) = 0;

  /**
   * @brief Whether the input should be owned by the manager.
   * If so then it should be cleaned up by the manager.
   */
  virtual bool is_owned() = 0;
};

/**
 * @brief Simple baseclass for Inputs.
 */
class BaseCallbackInput : public IInput {
protected:
  uint64_t mask;
  gpio_config_t gpio_config;

public:
  BaseCallbackInput() = default;
  virtual ~BaseCallbackInput() = default;

  uint64_t get_mask() override { return mask; }

  gpio_config_t *get_gpio_config() override { return &gpio_config; }
};

/**
 * @brief A unfiltered single pin Input.
 * Does not privide any software solution to deal with debouncing.
 */
class RawButtonInput : public BaseCallbackInput {
protected:
  std::function<void()> callback;

public:
  /**
   * @brief Construct a new RawButtonInput object.
   * @param pin The Buttons pin.
   * @param pullup Whether the Button is pullduped, or pulldowned.
   * @param callback The to call callback when triggered.
   */
  RawButtonInput(uint64_t pin, bool pullup, std::function<void()> callback);

  ~RawButtonInput() {}

  void invoke_callback(uint64_t pin) override { callback(); }

  bool is_owned() override { return true; }
};

/**
 * @brief A simple filtered single pin Input.
 * After recieving a signal will block for a set amount to deal with debouncing.
 */
class BlockedButtonInput : public RawButtonInput {
private:
  const uint32_t block_time;
  uint64_t last_time = 0;

public:
  /**
   * @brief Construct a new BlockedButtonInput object
   * @param pin The Buttons pin.
   * @param pullup Whether the Button is pullduped, or pulldowned.
   * @param block_time The time in milliseconds, for which the input remains
   * blocked.
   * @param callback The to call callback when triggered.
   */
  BlockedButtonInput(uint64_t pin, bool pullup, uint32_t block_time,
                     std::function<void()> callback);

  void invoke_callback(uint64_t pin) override;
};

} // namespace input

#endif // IINPUT_H

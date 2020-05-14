#ifndef GPIOCALLBACKMANAGERBUILDER_H
#define GPIOCALLBACKMANAGERBUILDER_H

#include <vector>

#include "input_manager.h"
#include "input_types.h"

namespace input {

class InputManagerBuilder {
private:
  std::vector<IInput *> *inputs = nullptr;
  uint32_t stack_size;

public:
  InputManagerBuilder(uint32_t stack_size);
  ~InputManagerBuilder();

  InputManager *build();

  InputManagerBuilder &add_input(IInput *input);
};
} // namespace input

#endif // GPIOCALLBACKMANAGERBUILDER_H

#include "input_manager_builder.h"

namespace input {

InputManagerBuilder::InputManagerBuilder(uint32_t stack_size)
    : stack_size(stack_size) {
  inputs = new std::vector<IInput *>();
}

InputManagerBuilder &InputManagerBuilder::add_input(IInput *input) {
  inputs->push_back(input);
  return *this;
}

InputManager *InputManagerBuilder::build() {
  auto instance = new InputManager(inputs, stack_size);
  inputs = nullptr;
  return instance;
}

InputManagerBuilder::~InputManagerBuilder() {
  if (inputs != nullptr) {
    delete inputs;
  }
}

} // namespace input

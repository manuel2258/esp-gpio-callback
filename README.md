# GPIO-Callback  
  
A small and easy way to convert GPIO Buttons into Callbacks.  
Originally developed for my own projects but it might be useful for others as well.  
  
## Requirements  
  
To need to use the ESP-IDF and a compatible board. Personally I tested it with an ESP-WROOM 32 Development board.  
Also your project has to be written in C++, not C.

## How to use

Its simple to use. Create a InputManager Object (Builder is available) and you are ready to go.

```C++
gpio_cb::InputManagerBuilder builder(2048);
builder
    .add_input(new gpio_cb::RawButtonInput(
        14, true, []() { ESP_LOGI("Button14", "Pressed"); }))
    .build();
```

This will print out "Button14: Pressed" when pressed.  
There are multiple Inputs available just as the interface IInput which lets your create your own customs Input Devices.
Everything class is in the gpio_cb namespace.  

### Notes
  
New InputManager should always be created on the stack. Calling its ctor will set its InputManager::instance to the newly created instance. You can delete InputManager::instance to unhook and delets the inputs, if there are set to be owned (look at the IInput interface). Therefor if the Input is owned by Manager it should as well be created on the stack.

## Include in your own projects

Simple clone this repo into ```ESP_IDF_PATH/components/``` for a global installation or into ```PROJECT_ROOT/components/``` for a projects installation only. Note that a projects installation might have to somehow be included.  
  
That should mostly be it. If you want to add some additionally inputs or found a critical bug contact me or create a PR.
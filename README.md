# ESPHome Coffee

ESPHome Coffee is a collection of configuration snippets, scripts, and custom
C++ code for DIY IOT coffee makers and espresso machines, powered by
[ESPHome](https://esphome.io).


## Features

Some highlights:
* PID temperature control via ESPHome's climate component
* 4-digit 7-segment display
* ergobled status indicator
* temperature gradient based brewing detection
* powersaving mode


## Documentation
*TODO*: documentation.


## Configuration

There's an example configuration, `coffee-maker.yaml`, which can be a starting
point for custom configurations.
Most features have to be configured and hooked up manually; this is to keep
flexibility as close to native ESPHome as possible.
That also means that some knowledge of how to configure and flash ESPHome is
required for a custom setup.


## Disclaimer

This project and it's code quality is still in an early stage and should be
considered experimental, or less euphemistic: "a bodge for fun".
Not all features have to make sense -- it's a firmware for home automation
integrated coffee makers, after all.
Feedback and contributions are welcome.

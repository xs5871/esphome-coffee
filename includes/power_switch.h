void power_switch_on_press(void) {
        auto call = id(controller).make_call();
        if (id(controller).mode == CLIMATE_MODE_OFF) {
          call.set_mode("HEAT");
        } else {
          call.set_mode("OFF");
        }
        call.perform();
}

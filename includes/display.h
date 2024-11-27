const char* animation = " _-*:%";
static uint8_t i = 0;

void display_timer(esphome::tm1637::TM1637Display &it) {
    const int dt = id(homeassistant_time).now().timestamp
                - id(display_timer_start_time) + 1;
    it.printf("%%%3d", dt);
}

void display_temperature(esphome::tm1637::TM1637Display &it) {
    const float ep = id(controller).get_proportional_term();
    uint8_t idx = 4;

    if (id(controller_mode) == CONTROLLER_MODE_LOAD) {
        idx = 5;
    } else if (ep > .01) {
        idx = i % 4;
    } else if (ep < -.01) {
        idx = 3 - (i % 4);
    } else {
        idx = 4 * (i % 2);
    }

    it.printf("%c%2.1f", animation[idx], id(controller).current_temperature);
    if (i % 16 == 1) it.set_intensity(7);
}

void display_target_temperature(esphome::tm1637::TM1637Display &it) {
    it.printf("t%2.1f", id(controller).target_temperature);
    it.set_intensity(4);
}

void display_clock(esphome::tm1637::TM1637Display &it) {
    if (i % 2) {
        it.strftime("%H%M", id(homeassistant_time).now());
    } else {
        it.strftime("%H.%M", id(homeassistant_time).now());
    }
}

void display_lambda(esphome::tm1637::TM1637Display &it) {
    static bool _timer = false;
    i++;

    // display mode: target temperature
    if (id(display_mode_ttemperature)) {
        display_target_temperature(it);
        return;
    }

    // display mode: clock
    if (id(controller).mode != CLIMATE_MODE_HEAT) {
        display_clock(it);
        return;
    }

    // display mode: timer
    const bool _load = (id(controller_mode) == CONTROLLER_MODE_LOAD)
                    && (id(controller_load).state);

    if ((! id(display_timer_start_time)) && (_load)) {
        id(display_timer_start).execute();
    } else if ((id(display_timer_start_time)) && (! _load)) {
        id(display_timer_stop).execute();
    }

    if (id(display_timer_start_time)) {
        display_timer(it);
        return;
    }

    // display mode: target temperature
    if (!(i % 16) || id(display_mode_ttemperature)) {
        display_target_temperature(it);
    }
    // display mode: current temperature
    else {
        display_temperature(it);
    }

}

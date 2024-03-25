enum controller_mode {
    CONTROLLER_MODE_LOAD,
    CONTROLLER_MODE_PID,
    CONTROLLER_MODE_IDLE,
};

void controller_on_state_(void) {
    const float error = id(controller).get_error_value();
    const int mode = id(controller_mode);

    if (id(controller).mode != CLIMATE_MODE_HEAT) {
        if (mode != CONTROLLER_MODE_IDLE) {
            id(controller_mode_idle).execute();
        }
    }
    else
    if ((mode != CONTROLLER_MODE_IDLE) &&
        (error < -id(controller_pid_threshold_high))) {
        id(controller_mode_idle).execute();
    }
    else
    if ((mode != CONTROLLER_MODE_LOAD) &&
        ((id(controller_on_load)) ||
         (error > -id(controller_pid_threshold_low)))) {
        id(controller_mode_load).execute();
    }
    else
    if ((mode != CONTROLLER_MODE_PID) &&
        (! id(controller_on_load)) &&
        (error > -id(controller_pid_threshold_high)) &&
        (error < -id(controller_pid_threshold_low))) {
        id(controller_mode_pid).execute();
    }
}

void controller_load_detection(float threshold_p, float threshold_d) {
    static float last_error = 0.;
    static uint32_t last_time = millis();
    const float error = id(controller).get_error_value();
    const uint32_t time = millis();
    float derivative = 0.;
    float &derivative_avg = id(controller_derivative);

    derivative = (error - last_error) / (time - last_time) * 1000.;
    derivative_avg = (derivative_avg + derivative) / 2.;
    if (std::isnan(derivative_avg)) derivative_avg = .0;
    last_error = error;
    last_time = time;

    ESP_LOGD("load_detection:",  "derivative: %2.1f, average: %2.1f", derivative, derivative_avg);

    if (id(controller_on_load)) {
        if ((id(controller).mode != CLIMATE_MODE_HEAT) ||
            (derivative_avg < 0.)) {
            id(controller_on_load) = false;
        }
    } else {
        if ((id(controller).mode == CLIMATE_MODE_HEAT) &&
            (derivative_avg > -threshold_d) &&
            (error > -threshold_p)) {
            ESP_LOGD("load_detection:", "on_load");
            id(controller_on_load) = true;
        }
    }
}

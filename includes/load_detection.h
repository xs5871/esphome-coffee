void load_detection_(float threshold_p, float threshold_d) {
    static float last_error = NAN;
    static uint32_t last_time = millis();
    const float error = id(controller).get_error_value();
    const uint32_t time = millis();
    float derivative = 0.;
    float &derivative_avg = id(controller_derivative);

    // bodgy skip of the first cycles until everything is initialized
    if (isnan(last_error)) {
        if (error != 0.) {
            last_error = error;
        }
        return;
    }

    // error is positive for current_temperature < target_temperature, that
    // means falling temperatures result in a positive derivative.
    derivative = (error - last_error) / (time - last_time) * 1000.;

    derivative_avg = (derivative_avg + derivative) / 2.;
    if (std::isnan(derivative_avg)) derivative_avg = .0;
    last_error = error;
    last_time = time;

    ESP_LOGD("load_detection:",  "derivative: %2.1f, average: %2.1f", derivative, derivative_avg);

    if (id(controller_load).state) {
        if ((id(controller).mode != CLIMATE_MODE_HEAT) ||
            (derivative_avg < -.3) ||
            (error < -threshold_p)) {
            id(controller_load).publish_state(false);
        }
    } else {
        if ((id(controller).mode == CLIMATE_MODE_HEAT) &&
            (derivative_avg > -threshold_d) &&
            (error > -threshold_p)) {
            id(controller_load).publish_state(true);
        }
    }
}

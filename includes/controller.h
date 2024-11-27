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
        id(controller_load).publish_state(false);
        id(controller_mode_idle).execute();
    }
    else
    if ((mode != CONTROLLER_MODE_LOAD) &&
        ((id(controller_load).state) ||
         (error > -id(controller_pid_threshold_low)))) {
        id(controller_mode_load).execute();
    }
    else
    if ((mode != CONTROLLER_MODE_PID) &&
        (! id(controller_load).state) &&
        (error > -id(controller_pid_threshold_high)) &&
        (error < -id(controller_pid_threshold_low))) {
        id(controller_mode_pid).execute();
    }
}

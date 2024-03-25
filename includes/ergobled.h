void ergobled_mode_gradient_(void) {
    float error = id(controller).get_error_value();
    error = error < -1 ? -1 : (error > 1 ? 1 : error);
    if (error < 0) {
        id(ergobled).turn_on().set_rgb(-error, 1. + error, 0).perform();
    } else {
        id(ergobled).turn_on().set_rgb(0, 1. - error, error).perform();
    }
}

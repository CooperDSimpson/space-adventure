#pragma once

#include "color.hpp"
#include "ship.hpp"
#include <cmath>

class AimCursor {
private:
    double aim_distance = 30.0;  // Distance ahead of the ship
    wchar_t graphic = L'+';      // Cursor appearance
    COLOR color = FG_RED;        // Cursor color

public:
    // Calculate cursor position based on ship position and rotation
    void get_position(const Ship& ship, double& x_out, double& y_out) const {
        x_out = ship.ship_center_x + sin(ship.rotation_angle) * aim_distance;
        y_out = ship.ship_center_y - cos(ship.rotation_angle) * aim_distance;
    }

    // Get cursor appearance
    wchar_t get_graphic() const { return graphic; }

    // Get cursor color
    COLOR get_color() const { return color; }

    // Allow adjusting the distance of the cursor
    void set_distance(double new_distance) {
        aim_distance = new_distance;
    }
};
#pragma once

#include <vector>
#include <cmath>
#include "color.hpp"

class Projectile {
public:
    double x_pos;
    double y_pos;
    double direction; // angle in radians
    double speed = 100.0f; // Units per second
    double lifetime = 2.0; // Seconds before disappearing
    wchar_t graphic = L'*';
    COLOR color = FG_YELLOW;

    Projectile(double start_x, double start_y, double dir)
        : x_pos(start_x), y_pos(start_y), direction(dir) {
    }

    bool update(double delta_time) {
        // Move the projectile based on direction and speed
        x_pos += sin(direction) * speed * delta_time;
        y_pos -= cos(direction) * speed * delta_time;

        // Reduce lifetime
        lifetime -= delta_time;

        // Return true if still alive, false if expired
        return lifetime > 0;
    }
};

class ProjectileManager {
private:
    std::vector<Projectile> projectiles;
    double cooldown = 0.0; // Cooldown timer for shooting
    double cooldown_time = 0.2; // Time between shots

public:
    void update(double delta_time) {
        // Update cooldown timer
        if (cooldown > 0) {
            cooldown -= delta_time;
        }

        // Update all projectiles and remove expired ones
        auto it = projectiles.begin();
        while (it != projectiles.end()) {
            if (!it->update(delta_time)) {
                it = projectiles.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    bool fire(double x, double y, double direction) {
        // Check if we can fire (cooldown expired)
        if (cooldown <= 0) {
            projectiles.emplace_back(x, y, direction);
            cooldown = cooldown_time;
            return true;
        }
        return false;
    }

    const std::vector<Projectile>& get_projectiles() const {
        return projectiles;
    }
};
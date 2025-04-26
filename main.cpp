#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>

#include "galaxy.hpp"
#include "color.hpp"
#include "ship.hpp"
#include "camera.hpp"
#include "projectile.hpp"
#include "renderer.hpp"
#include "cursor.hpp"

Ship ship;
Camera camera;
World world;
ProjectileManager projectile_manager;

int main() {
    double current_distance = 10;
    world.build();
    CHAR_INFO* screen = new CHAR_INFO[camera.screen_width * camera.screen_height];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    Renderer renderer(camera, world, ship, projectile_manager, screen);

    std::vector<std::vector<wchar_t>> ship_design = {
    { L' ', L' ', L' ', L'#', L' ', L' ', L' ' },
    { L' ', L' ', L'#', L'#', L'#', L' ', L' ' },
    { L' ', L'#', L'#', L'#', L'#', L'#', L' ' },
    { L'#', L'#', L'.', L'#', L'.', L'#', L'#' }
    };

    camera.mode = CHASE;

    ship.parts = ship.transpile_ship_map(ship_design, 3, 2, FG_GREEN);

    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();
    auto currentTime = clock::now();

    bool running = 1;
    bool space_pressed_last_frame = false;
    bool plus_pressed_last_frame = false;
    bool minus_pressed_last_frame = false;

    ship.update_camera(camera.relative_x_pos, camera.relative_y_pos);

    while (running) {
        currentTime = clock::now();
        std::chrono::duration<double> delta_time = currentTime - lastTime;
        lastTime = currentTime;

        // Movement controls
        if (GetAsyncKeyState('W') & 0x8000) {
            ship.ship_center_x += sin(ship.rotation_angle) * ship.movement_speed * delta_time.count();
            ship.ship_center_y -= cos(ship.rotation_angle) * ship.movement_speed * delta_time.count();
        }
        else if (GetAsyncKeyState('S') & 0x8000) {
            ship.ship_center_x -= sin(ship.rotation_angle) * ship.movement_speed * delta_time.count();
            ship.ship_center_y += cos(ship.rotation_angle) * ship.movement_speed * delta_time.count();
        }
        if (GetAsyncKeyState('Q') & 0x8000) {
            // Strafe left = 90 degrees counterclockwise from forward
            ship.ship_center_x -= cos(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
            ship.ship_center_y -= sin(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
        }
        else if (GetAsyncKeyState('E') & 0x8000) {
            // Strafe right = 90 degrees clockwise from forward
            ship.ship_center_x += cos(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
            ship.ship_center_y += sin(ship.rotation_angle) * ship.strafe_speed * delta_time.count();
        }
        if (GetAsyncKeyState('A') & 0x8000) {
            ship.rotation_angle -= ship.rotation_speed * delta_time.count();
        }
        else if (GetAsyncKeyState('D') & 0x8000) {
            ship.rotation_angle += ship.rotation_speed * delta_time.count();
        }

        // Fire projectile with spacebar
        bool space_pressed_current_frame = (GetAsyncKeyState(VK_SPACE) & 0x8000) != 0;
        if (space_pressed_current_frame && !space_pressed_last_frame) {
            // Fire a projectile from the ship's position
            projectile_manager.fire(ship.ship_center_x, ship.ship_center_y, ship.rotation_angle);
        }
        space_pressed_last_frame = space_pressed_current_frame;

        // Adjust cursor distance with + and - keys
        bool plus_pressed_current_frame = (GetAsyncKeyState(VK_OEM_PLUS) & 0x8000) != 0;
        if (plus_pressed_current_frame && !plus_pressed_last_frame) {
            // Get current distance and increase it
            AimCursor& cursor = renderer.get_aim_cursor();
            double dummy;
            cursor.get_position(ship, dummy, dummy);
            cursor.set_distance(current_distance + 5.0);
        }
        plus_pressed_last_frame = plus_pressed_current_frame;

        bool minus_pressed_current_frame = (GetAsyncKeyState(VK_OEM_MINUS) & 0x8000) != 0;
        if (minus_pressed_current_frame && !minus_pressed_last_frame) {
            // Get current distance and decrease it
            AimCursor& cursor = renderer.get_aim_cursor();
            double dummy;
            cursor.get_position(ship, dummy, dummy);
            cursor.set_distance(max(10.0, current_distance - 5.0)); // Don't go below 10
        }
        minus_pressed_last_frame = minus_pressed_current_frame;

        // Toggle camera mode

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            running = 0;
        }

        // Update projectiles
        projectile_manager.update(delta_time.count());

        // Render everything
        renderer.render_screen();

        COORD bufferSize = { camera.screen_width, camera.screen_height };
        COORD bufferCoord = { 0, 0 };
        SMALL_RECT writeRegion = { 0, 0, camera.screen_width - 1, camera.screen_height - 1 };
        WriteConsoleOutput(hConsole, screen, bufferSize, bufferCoord, &writeRegion);
    }

    delete[] screen;
    return 0;
}
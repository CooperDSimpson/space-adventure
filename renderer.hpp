#pragma once

#include "ship.hpp"
#include "galaxy.hpp"
#include "camera.hpp"
#include "projectile.hpp"
#include "cursor.hpp"

class Renderer {
private:
    Camera& camera;
    Ship& ship;
    World& world;
    CHAR_INFO*& screen;
    ProjectileManager& projectile_manager;
    AimCursor aim_cursor;
public:

    Renderer(Camera& new_camera, World& new_world, Ship& new_ship, ProjectileManager& new_pm, CHAR_INFO*& new_screen)
        : camera(new_camera), world(new_world), ship(new_ship), projectile_manager(new_pm), screen(new_screen) {
    }

    //void render_screen() {
    //    // Render background world tiles
    //    for (int x = 0; x < camera.screen_width; x++) {
    //        for (int y = 0; y < camera.screen_height; y++) {
    //            int world_x = x;
    //            int world_y = y;

    //            // Adjust world coordinates based on camera mode
    //            if (camera.mode == LOCKED) {
    //                world_x += static_cast<int>(ship.ship_center_x - camera.screen_width / 2);
    //                world_y += static_cast<int>(ship.ship_center_y - camera.screen_height / 2);
    //            }
    //            else if (camera.mode == CHASE) {
    //                world_x += static_cast<int>(ship.ship_center_x - camera.screen_width / 2);
    //                world_y += static_cast<int>(ship.ship_center_y - camera.screen_height / 2);
    //            }

    //            if (world_x < 0 || world_x >= world.tile_map.size() ||
    //                world_y < 0 || world_y >= world.tile_map[0].size()) {
    //                // Out of bounds
    //                screen[y * camera.screen_width + x].Attributes = FG_DARK_GRAY;
    //                screen[y * camera.screen_width + x].Char.UnicodeChar = L'#';
    //            }
    //            else {
    //                // In bounds
    //                screen[y * camera.screen_width + x].Attributes = world.tile_map[world_x][world_y].color;
    //                screen[y * camera.screen_width + x].Char.UnicodeChar = world.tile_map[world_x][world_y].graphic;
    //            }
    //        }
    //    }

    //    // Render projectiles
    //    const auto& projectiles = projectile_manager.get_projectiles();
    //    for (const auto& proj : projectiles) {
    //        int screen_x, screen_y;

    //        if (camera.mode == LOCKED) {
    //            screen_x = camera.screen_width / 2 + static_cast<int>(round(proj.x_pos - ship.ship_center_x));
    //            screen_y = camera.screen_height / 2 + static_cast<int>(round(proj.y_pos - ship.ship_center_y));
    //        }
    //        else if (camera.mode == CHASE) {
    //            screen_x = camera.screen_width / 2 + static_cast<int>(round(proj.x_pos - ship.ship_center_x));
    //            screen_y = camera.screen_height / 2 + static_cast<int>(round(proj.y_pos - ship.ship_center_y));
    //        }

    //        // Check if projectile is on screen
    //        if (screen_x >= 0 && screen_x < camera.screen_width && screen_y >= 0 && screen_y < camera.screen_height) {
    //            screen[screen_y * camera.screen_width + screen_x].Attributes = proj.color;
    //            screen[screen_y * camera.screen_width + screen_x].Char.UnicodeChar = proj.graphic;
    //        }
    //    }

    //    // Render aim cursor
    //    double cursor_world_x, cursor_world_y;
    //    aim_cursor.get_position(ship, cursor_world_x, cursor_world_y);

    //    int cursor_screen_x, cursor_screen_y;
    //    if (camera.mode == LOCKED) {
    //        cursor_screen_x = camera.screen_width / 2 + static_cast<int>(round(cursor_world_x - ship.ship_center_x));
    //        cursor_screen_y = camera.screen_height / 2 + static_cast<int>(round(cursor_world_y - ship.ship_center_y));
    //    }
    //    else if (camera.mode == CHASE) {
    //        cursor_screen_x = camera.screen_width / 2 + static_cast<int>(round(cursor_world_x - ship.ship_center_x));
    //        cursor_screen_y = camera.screen_height / 2 + static_cast<int>(round(cursor_world_y - ship.ship_center_y));
    //    }

    //    // Check if cursor is on screen and render it
    //    if (cursor_screen_x >= 0 && cursor_screen_x < camera.screen_width &&
    //        cursor_screen_y >= 0 && cursor_screen_y < camera.screen_height) {
    //        screen[cursor_screen_y * camera.screen_width + cursor_screen_x].Attributes = aim_cursor.get_color();
    //        screen[cursor_screen_y * camera.screen_width + cursor_screen_x].Char.UnicodeChar = aim_cursor.get_graphic();
    //    }

    //    // Render the ship
    //    ship.print_to_screen(screen, camera.screen_width, camera.screen_height, camera.mode);
    //}


    void render_screen() {
        // Render background world tiles
        for (int x = 0; x < camera.screen_width; x++) {
            for (int y = 0; y < camera.screen_height; y++) {
                int world_x = x;
                int world_y = y;

                // Adjust world coordinates based on camera mode
                if (camera.mode == LOCKED) {
                    world_x += static_cast<int>(ship.ship_center_x - camera.screen_width / 2);
                    world_y += static_cast<int>(ship.ship_center_y - camera.screen_height / 2);
                }
                else if (camera.mode == CHASE) {
                    world_x += static_cast<int>(ship.ship_center_x - camera.screen_width / 2);
                    world_y += static_cast<int>(ship.ship_center_y - camera.screen_height / 2);
                }

                // Get tile from infinite world
                Tile world_tile = world.get_tile(world_x, world_y);

                // Render tile
                screen[y * camera.screen_width + x].Attributes = world_tile.color;
                screen[y * camera.screen_width + x].Char.UnicodeChar = world_tile.graphic;
            }
        }

        // Render projectiles (unchanged)
        const auto& projectiles = projectile_manager.get_projectiles();
        for (const auto& proj : projectiles) {
            int screen_x, screen_y;

            if (camera.mode == LOCKED) {
                screen_x = camera.screen_width / 2 + static_cast<int>(round(proj.x_pos - ship.ship_center_x));
                screen_y = camera.screen_height / 2 + static_cast<int>(round(proj.y_pos - ship.ship_center_y));
            }
            else if (camera.mode == CHASE) {
                screen_x = camera.screen_width / 2 + static_cast<int>(round(proj.x_pos - ship.ship_center_x));
                screen_y = camera.screen_height / 2 + static_cast<int>(round(proj.y_pos - ship.ship_center_y));
            }

            // Check if projectile is on screen
            if (screen_x >= 0 && screen_x < camera.screen_width && screen_y >= 0 && screen_y < camera.screen_height) {
                screen[screen_y * camera.screen_width + screen_x].Attributes = proj.color;
                screen[screen_y * camera.screen_width + screen_x].Char.UnicodeChar = proj.graphic;
            }
        }
        
        
        

        // Render aim cursor (unchanged)
        // Render aim cursor
        double cursor_world_x, cursor_world_y;
        aim_cursor.get_position(ship, cursor_world_x, cursor_world_y);

        int cursor_screen_x, cursor_screen_y;
        if (camera.mode == LOCKED) {
            cursor_screen_x = camera.screen_width / 2 + static_cast<int>(round(cursor_world_x - ship.ship_center_x));
            cursor_screen_y = camera.screen_height / 2 + static_cast<int>(round(cursor_world_y - ship.ship_center_y));
        }
        else if (camera.mode == CHASE) {
            cursor_screen_x = camera.screen_width / 2 + static_cast<int>(round(cursor_world_x - ship.ship_center_x));
            cursor_screen_y = camera.screen_height / 2 + static_cast<int>(round(cursor_world_y - ship.ship_center_y));
        }

        // Check if cursor is on screen and render it
        if (cursor_screen_x >= 0 && cursor_screen_x < camera.screen_width &&
            cursor_screen_y >= 0 && cursor_screen_y < camera.screen_height) {
            screen[cursor_screen_y * camera.screen_width + cursor_screen_x].Attributes = aim_cursor.get_color();
            screen[cursor_screen_y * camera.screen_width + cursor_screen_x].Char.UnicodeChar = aim_cursor.get_graphic();
        }

        // Render the ship (unchanged)
        ship.print_to_screen(screen, camera.screen_width, camera.screen_height, camera.mode);

        // Memory optimization: Unload distant chunks periodically
        static int frame_counter = 0;
        if (frame_counter++ % 100 == 0) {  // Every 100 frames
            world.unload_distant_chunks(
                static_cast<int>(ship.ship_center_x),
                static_cast<int>(ship.ship_center_y),
                10  // Keep chunks within 10 chunk radius
            );
        }
    }

    // Allow access to the cursor for adjustments
    AimCursor& get_aim_cursor() {
        return aim_cursor;
    }
};
#include "raylib.h"
int main() {
    // Initialize a window
    int width{900}, height{600};
    InitWindow(width, height, "Chase Game");

    // Circle Coordinates
    int circle_x{450}, circle_y{300}, circle_radius{25};
    // Circle Edges
    int l_circle_x{circle_x - circle_radius};
    int r_circle_x{circle_x + circle_radius};
    int u_circle_y{circle_y - circle_radius};
    int d_circle_y{circle_y + circle_radius};

    // Axe Coordinates
    int axe_x{0}, axe_y{0}, axe_length{50};
    // Axe Edges
    int l_axe_x{axe_x};
    int r_axe_x{axe_x + axe_length};
    int u_axe_y{axe_y};
    int d_axe_y{axe_y + axe_length};

    int direction{10};

    bool collision_with_axe =
        (d_axe_y >= u_circle_y) &&
        (u_axe_y <= d_circle_y) &&
        (l_axe_x <= r_circle_x) &&
        (r_axe_x >= l_circle_x);

    // WindowShouldClose returns true if esc is clicked
    SetTargetFPS(60);                    // Sets the highest FPS
    while (WindowShouldClose() == false) // Keep the Window open
    {
        // Setup Canvas
        BeginDrawing();
        // Clear canvas to a specific color to avoid flicker
        ClearBackground(WHITE);

        if (collision_with_axe) {
            DrawText("Game Over", 400, 200, 20, RED);
        }
        else {
            // ****Game Logic Begin
            // Update Edges
            l_circle_x = circle_x - circle_radius;
            r_circle_x = circle_x + circle_radius;
            u_circle_y = circle_y - circle_radius;
            d_circle_y = circle_y + circle_radius;
            l_axe_x = axe_x;
            r_axe_x = axe_x + axe_length;
            u_axe_y = axe_y;
            d_axe_y = axe_y + axe_length;

            // Update Collition Condition
            collision_with_axe =
                (d_axe_y >= u_circle_y) &&
                (u_axe_y <= d_circle_y) &&
                (l_axe_x <= r_circle_x) &&
                (r_axe_x >= l_circle_x);

            // Draws a Circle in the Canvas(X, Y, R, C)
            DrawCircle(circle_x, circle_y, circle_radius, BLUE);
            // Draws a Rectangle in the Canvas(X, Y, W, H, C)
            DrawRectangle(axe_x, axe_y, axe_length, axe_length, RED);

            // Circle Movement
            if (IsKeyDown(KEY_D) && circle_x < width - circle_radius)
                circle_x += 10;
            if (IsKeyDown(KEY_A) && circle_x > circle_radius)
                circle_x -= 10;
            if (IsKeyDown(KEY_W) && circle_y > circle_radius)
                circle_y -= 10;
            if (IsKeyDown(KEY_S) && circle_y < height - circle_radius)
                circle_y += 10;

            // Move the Axe
            if (circle_x > axe_x)
                axe_x += 5;
            if (circle_x < axe_x)
                axe_x -= 5;
            if (circle_y < axe_y)
                axe_y -= 5;
            if (circle_y > axe_y)
                axe_y += 5;
        }
        EndDrawing();
    };
}
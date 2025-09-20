#include "raylib.h"
#include <vector>
#include <string>
#include "miku.h"
#include "soundManager.h"

Rectangle TextureRectFromPoint(Texture2D texture, Vector2 pos, Vector2 pivotPoint, float scale) {
    Rectangle dest_rect = {pos.x, pos.y, (float)texture.width * scale, (float)texture.height * scale};
    float x = dest_rect.x - pivotPoint.x;
    float y = dest_rect.y - pivotPoint.y;
    return {x, y, dest_rect.width, dest_rect.height};
}

void DrawTextureFromPoint(Texture2D texture, Vector2 pos, Vector2 pivotPoint, float scale, float rotation, Color color) {
    Rectangle texture_rect = {0, 0, (float)texture.width, (float)texture.height};
    Rectangle dest_rect = {pos.x, pos.y, (float)texture.width * scale, (float)texture.height * scale};
    DrawTexturePro(texture, texture_rect, dest_rect, pivotPoint, rotation, color);
}

void DrawTextureFromCentre(Texture2D texture, Vector2 pos, float scale, float rotation, Color color) {
    Vector2 centre_point = {(float)texture.width * scale / 2, (float)texture.height * scale / 2};
    DrawTextureFromPoint(texture, pos, centre_point, scale, rotation, color);
}

Rectangle TextureRectFromCentre(Texture2D texture, Vector2 pos, float scale) {
    Vector2 centre_point = {(float)texture.width * scale / 2, (float)texture.height * scale / 2};
    return TextureRectFromPoint(texture, pos, centre_point, scale);
}

int main() {
    // Initialize a window
    int width{1280}, height{720};
    InitWindow(width, height, "Whack-A-Miku");
    SetTargetFPS(60);
    HideCursor();

    Texture2D background = LoadTexture("assets/bg.png");
    Texture2D crosshair = LoadTexture("assets/thwacker.png");

    std::vector<Miku*> miku = {};
    miku.push_back(new Miku("a", "Hatsune Miku"));
    miku.push_back(new Miku("b", "Miku Nakano"));
    miku.push_back(new Miku("c", "Miku Izayoi"));
    miku.push_back(new Miku("d", "Miku Hinasaki"));
    miku.push_back(new Miku("e", "Miku (390)"));


    int selectedMikuIndex = 0;
    Miku* selectedMiku = miku[selectedMikuIndex];

    bool isDebug = false;

    int highScore = 0;
    int score = 0;
    float gameTime = 30.0f;
    float timeLeft = gameTime;

    bool isGameRunning = false;

    //Animates the start text size
    float startFontSize = 20;
    int startFontSizeDirection = 1;

    std::vector<Vector2*> spawnLocations = {
        new Vector2{475.0f, 290.0f},
        new Vector2{742.0f, 302.0f},
        new Vector2{309.0f, 433.0f},
        new Vector2{609.0f, 448.0f},
        new Vector2{906.0f, 456.0f},
    };

    std::vector<std::vector<int>> spawns = {
        {0},
        {1},
        {2},
        {3},
        {4},
        {0, 1},
        {2, 4},
        {2,3,4},
        {0,1,3},
        {0,2,3},
        {1,3,4},
        {0,2,3,4},
        {1,2,3,4},
        {0,1,2,3,4},
    };

    // 0 means miku despawned
    // >0 means miku present, value is time left until despawn
    std::vector<float> mikuAtPositions = {0,0,0,0,0};
    // isMikuThwacked tracks whether the miku at each position has been hit
    std::vector<bool> isMikuThwacked = {false, false, false, false, false};

    std::vector<Vector2> LightBulbPositions = {
        {140.0f, 32.0f},
        {147.0f, 124.0f},
        {156.0f, 216.0f},
        {1108.0f, 28.0f},
        {1114.0f, 130.0f},
        {1113.0f, 214.0f},
    };

    std::vector<std::vector<bool>> LightBulbPatterns = {
        {true, false, true, true, false, true,},
        {false, true, false, false, true, false},
        {true, false, true, true, false, true,},
        {true, false, false, true, false, false},
        {false, true, false, false, true, false},
        {false, false, true, false, false, true},
        {false, true, false, false, true, false},
        {true, false, false, true, false, false},
    };

    int currentLightBulbPattern = 0;
    float timePerLightBulbPattern = 0.5f;

    SoundManager::GetInstance()->playMusic(MUSIC_BGM1); // Initialize sound manager and start playing music

    //run game loop
    while (WindowShouldClose() == false)
    {
        SoundManager::GetInstance()->update();

        BeginDrawing();
        DrawTextureFromCentre(background, {(float)width/2, (float)height/2}, 1.0f, 0.0f, WHITE);

        //Draw Miku info at the centre of the screen
        DrawText(selectedMiku->getName().c_str(), GetScreenWidth()/2 - MeasureText(selectedMiku->getName().c_str(), 35)/2, 10, 35, BLACK);

        DrawText(TextFormat("%i", highScore), 450, 102, 20, WHITE);
        DrawText(TextFormat("%.2f", timeLeft), 600, 99, 20, WHITE);
        DrawText(TextFormat("%i", score), 725, 99, 20, WHITE);

        //Draw start button while increasing and decreasing font size using delta time
        if (!isGameRunning) {
            startFontSize += startFontSizeDirection * 20 * GetFrameTime();
            if (startFontSize >= 25) {
                startFontSizeDirection = -1;
            } else if (startFontSize <= 20) {
                startFontSizeDirection = 1;
            }
        } else {
            startFontSize = 20;
            startFontSizeDirection = 1;
        }
        DrawText("START", 1110, 515, startFontSize, BLACK);

        //Draw light bulbs
        for (size_t i = 0; i < LightBulbPositions.size(); i++) {
            if (LightBulbPatterns[currentLightBulbPattern][i]) {
                DrawCircleV(LightBulbPositions[i], 35, { 253, 249, 0, 80 });
            }
        }

        //Update light bulb pattern
        timePerLightBulbPattern -= GetFrameTime();
        if (timePerLightBulbPattern <= 0) {
            currentLightBulbPattern = (currentLightBulbPattern + 1) % LightBulbPatterns.size();
            timePerLightBulbPattern = 0.5f;
        }

        //Draw texture at mouse position
        Vector2 mousePos = GetMousePosition();
        Rectangle mouseRect = TextureRectFromCentre(crosshair, mousePos, 0.3f);

        //Draw Selected Miku
        Vector2 mikuPos = {633.0f, 198.0f};
        DrawTextureFromCentre(selectedMiku->getMikuTexture(), mikuPos, 0.3f, 0.0f, WHITE);

        //Handle click on an next miku arrow
        Rectangle nextMikuRect = {700, 175, 70, 30};
        if (isDebug) {
            //Draw a rectangle around the next miku arrow
            DrawRectangleLinesEx(nextMikuRect, 2, DARKGREEN);
        }
        if (CheckCollisionRecs(mouseRect, nextMikuRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedMikuIndex = (selectedMikuIndex + 1) % miku.size();
            selectedMiku = miku[selectedMikuIndex];
            SoundManager::GetInstance()->playSound(SFX_HIT);
        }

        //Handle click on an previous miku arrow
        Rectangle prevMikuRect = {500, 175, 70, 30};
        if (isDebug) {
            //Draw a rectangle around the previous miku arrow
            DrawRectangleLinesEx(prevMikuRect, 2, DARKGREEN);
        }
        if (CheckCollisionRecs(mouseRect, prevMikuRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedMikuIndex = (selectedMikuIndex - 1 + miku.size()) % miku.size();
            selectedMiku = miku[selectedMikuIndex];
            SoundManager::GetInstance()->playSound(SFX_HIT);
        }

        //Handle click on start button
        Rectangle startButtonRect = {1115, 515, 75, 30};
        if (isDebug) {
            //Draw a rectangle around the start button
            DrawRectangleLinesEx(startButtonRect, 2, DARKGREEN);
        }
        if (CheckCollisionRecs(mouseRect, startButtonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            isGameRunning = true;
            score = 0;
            timeLeft = gameTime;
            SoundManager::GetInstance()->playSound(SFX_HIT);
        }

        bool isAnyMikuPresent = false;
        for (float t : mikuAtPositions) {
            if (t > 0) {
                isAnyMikuPresent = true;
                break;
            }
        }

        // If no mikus are present, randomly select a spawn pattern
        if (!isAnyMikuPresent && isGameRunning) {
            int spawnPatternIndex = GetRandomValue(0, spawns.size() - 1);
            for (int i : spawns[spawnPatternIndex]) {
                mikuAtPositions[i] = (float)(GetRandomValue(120, 300)) / 60.0f;
                isMikuThwacked[i] = false;
            }
        }

        //Draw mikus at spawn locations
        for (size_t i = 0; i < spawnLocations.size(); i++) {
            if (isGameRunning) {
                if (mikuAtPositions[i] > 0) {
                    mikuAtPositions[i] -= GetFrameTime();
                    if (mikuAtPositions[i] <= 0) {
                        mikuAtPositions[i] = 0;
                    }
                }
            } else {
                mikuAtPositions[i] = -1;
            }

            if (mikuAtPositions[i] == -1 || mikuAtPositions[i] == 0) {
                continue;
            }

            Texture2D mikuTexture = isMikuThwacked[i] ? selectedMiku->getThwackTexture() : selectedMiku->getMikuTexture();
            Rectangle mikuRect = TextureRectFromCentre(mikuTexture, *spawnLocations[i], 0.5f);

            DrawTextureFromCentre(mikuTexture, *spawnLocations[i], 0.7f, 0.0f, WHITE);
            if (isDebug) {
                //Draw a rectangle around the miku
                DrawRectangleLinesEx(mikuRect, 2, DARKGREEN);
            }

            if (isGameRunning && CheckCollisionRecs(mouseRect, mikuRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                score += 10;
                isMikuThwacked[i] = true;
                mikuAtPositions[i] = 0.5f; // Despawn the miku after half a second
                SoundManager::GetInstance()->playSound(SFX_HIT);
            }
        }

        if (score > highScore) highScore = score;

        if (timeLeft > 0 && isGameRunning) {
            timeLeft -= GetFrameTime();
            if (timeLeft < 0) timeLeft = 0;
        } else {
            if (score > highScore) highScore = score;
            timeLeft = gameTime;
            isGameRunning = false;
        }


        DrawTextureFromCentre(crosshair, mousePos, 0.7f, IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? -45 : 0, WHITE);
        //Draw position at mouse position
        if (isDebug)
        {
            DrawText(TextFormat("(%.2f, %.2f)", mousePos.x, mousePos.y), mousePos.x, mousePos.y, 20, BLACK);
            //Draw dot on mouse position
            DrawPixelV(mousePos, WHITE);
            //Draw rectangle around mouse position
            DrawRectangleLinesEx(mouseRect, 2, DARKGREEN);
        }

        EndDrawing();
    };
}
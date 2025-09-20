#pragma once
#include "raylib.h"
#include <string>

class Miku
{
private:
    std::string id;
    std::string name;

    Texture2D miku;
    Texture2D thwack;
public:
    Miku(std::string id, std::string name);
    ~Miku();

    std::string getID();
    std::string getName();

    Texture2D getMikuTexture();
    Texture2D getThwackTexture();
};
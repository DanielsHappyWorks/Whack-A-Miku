#include "miku.h"

Miku::Miku(std::string id, std::string name)
    : id{id}, name{name}
{
    miku = LoadTexture(("assets/" + id + "/miku-no-bg.png").c_str());
    thwack = LoadTexture(("assets/" + id + "/miku_whacked-no-bg.png").c_str());
}

Miku::~Miku()
{
    UnloadTexture(miku);
    UnloadTexture(thwack);
}

//getters
std::string Miku::getID() { return id; }
std::string Miku::getName() { return name; }
Texture2D Miku::getMikuTexture() { return miku; }
Texture2D Miku::getThwackTexture() { return thwack; }

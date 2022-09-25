#include "TextureManager.h"

unordered_map<string, sf::Texture> TextureManager::textures;

void TextureManager::LoadTexture(string fileName)
{
    string path = "images/";
    path += fileName + ".png";

    textures[fileName].loadFromFile(path);
}
sf::Texture& TextureManager::GetTexture(string textureName)
{
    //makes sure the texture exists
    if(textures.find(textureName) == textures.end())
    {
        LoadTexture(textureName);
    }
    return textures[textureName];
}
//clears all stored textures
void TextureManager::Clear()
{
    textures.clear();
}




#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;
using std::unordered_map;
using std::string;
class TextureManager
{
    static unordered_map<string, sf::Texture> textures;
    static void LoadTexture(string textureName);
public:
    static sf::Texture& GetTexture(string textureName);
    static void Clear();

};
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;
//board class
class Board
{
    //class representing each individual tile of the board
    struct Tiles
    {
        bool mine;
        bool hasFlag;
        bool hadFlag;
        bool protectRecursion;
        bool clicked;
        bool tileFound;
        bool revealTile;
        float xMin;
        float xMax;
        float yMin;
        float yMax;
        vector<Tiles*> adjacentBombTiles;
        vector<Tiles*> adjacentTiles;
        sf::Sprite tileSprite;
        sf::Sprite flag;
        sf::Sprite tileSpriteRevealed;
        sf::Sprite mineRevealed;
        sf::Sprite one;
        sf::Sprite two;
        sf::Sprite three;
        sf::Sprite four;
        sf::Sprite five;
        sf::Sprite six;
        sf::Sprite seven;
        sf::Sprite eight;

        //constructor
        Tiles();
        //adds a flag or removes from this tile
        void AddRemoveFlag();
        //reveals the tile
        void RevealTile();
        //prints this individual tile
        void PrintIndividualTile(sf::RenderWindow& window);
        //adds a bomb to the tiles vector of adjacent bombs
        void AddAdjacentBombs(Tiles* currentTile);
        //adds a tile to the tile's vector of adjacent tiles
        void AddAdjacentTile(Tiles* currentTile);
        void ReactivateRecursion();

    };
public:
    //declares the boards variables
    int width;
    int widthPixels;
    int height;
    int heightPixels;
    int minesConfig;
    int mines;
    int flagMineCounter;
    int numOfTiles;
    bool debug;
    bool win;
    bool lose;
    float debugXMin;
    float debugXMax;
    float debugYMin;
    float debugYMax;
    float test1XMin;
    float test1XMax;
    float test1YMin;
    float test1YMax;
    float test2XMin;
    float test2XMax;
    float test2YMin;
    float test2YMax;
    float test3XMin;
    float test3XMax;
    float test3YMin;
    float test3YMax;
    float faceXMin;
    float faceXMax;
    float faceYMin;
    float faceYMax;
    sf::Sprite happyFace;
    sf::Sprite deadFace;
    sf::Sprite winFace;
    sf::Sprite debugButton;
    sf::Sprite test1Button;
    sf::Sprite test2Button;
    sf::Sprite test3Button;
    sf::Sprite digits;

    //2D vector of tiles
    vector<vector<Tiles>> tiles;
    //board constructor
    Board(int width_, int height_, int mines_);
    //board methods
    void DrawWindow();   //draws the windows
    void InitializeTiles();
    void PrintAllTiles(sf::RenderWindow& window);
    void PrintFace(sf::RenderWindow& window);
    void PrintDigits(sf::RenderWindow& window);
    void PrintButtons(sf::RenderWindow& window);
    void PlaceBombs();
    void PrintBombs();
    Tiles& LocateTile(float x, float y);
    void LocateAdjacentBombsAndTiles(int row, int column);
    void DebugPress();
    void ShowBomb(sf::RenderWindow& window);
    void LoadTest(string filePath);
    bool CheckVictory();
};
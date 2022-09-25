#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;
#include "Board.h"
#include "TextureManager.h"
#include "Random.h"

//constructor for a board
Board::Board(int width_, int height_, int mines_)
{
    width = width_;
    widthPixels = width_ * 32;
    height = height_;
    heightPixels = height_ * 32 + 100;
    mines = mines_;
    flagMineCounter = mines_;
    minesConfig = mines_;
    debug = false;
    win = false;
    lose = false;
    debugXMin = 0;
    debugXMax = 0;
    debugYMin = 0;
    debugYMax = 0;
    test1XMin = 0;
    test1XMax = 0;
    test1YMin = 0;
    test1YMax = 0;
    test2XMin = 0;
    test2XMax = 0;
    test2YMin = 0;
    test2YMax = 0;
    test3XMin = 0;
    test3XMax = 0;
    test3YMin = 0;
    test3YMax = 0;
    faceXMin = 0;
    faceXMax = 0;
    faceYMin = 0;
    faceYMax = 0;
    numOfTiles = width_ * height_;
    happyFace = sf::Sprite(TextureManager::GetTexture("face_happy"));
    deadFace = sf::Sprite(TextureManager::GetTexture("face_lose"));
    winFace = sf::Sprite(TextureManager::GetTexture("face_win"));
    debugButton = sf::Sprite(TextureManager::GetTexture("debug"));
    test1Button = sf::Sprite(TextureManager::GetTexture("test_1"));
    test2Button = sf::Sprite(TextureManager::GetTexture("test_2"));
    test3Button = sf::Sprite(TextureManager::GetTexture("test_3"));
    digits = sf::Sprite(TextureManager::GetTexture("digits"));
}
//draws the window
void Board::DrawWindow()
{
    sf::RenderWindow window(sf::VideoMode(widthPixels, heightPixels), "Window");
    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
            if(event.type == sf::Event::MouseButtonPressed)
            {
                if(event.mouseButton.button == sf::Mouse::Right)
                {
                    //makes sure the user has not lost or won the game yet
                    if(!lose)
                    {
                        auto x = (float) event.mouseButton.x;
                        auto y = (float) event.mouseButton.y;
                        if(!LocateTile(x, y).revealTile)
                        {
                            if(LocateTile(x,y).hasFlag)
                            {
                                flagMineCounter++;
                            }
                            else if(!LocateTile(x,y).hasFlag)
                            {
                                flagMineCounter--;
                            }
                            LocateTile(x, y).AddRemoveFlag();
                        }
                    }
                }
                if(event.mouseButton.button == sf::Mouse::Left)
                {
                    auto x = (float)event.mouseButton.x;
                    auto y = (float)event.mouseButton.y;
                    if((x >= debugXMin && x <= debugXMax) && (y >= debugYMin && y <= debugYMax))
                    {
                        if(!lose && !win)
                        {
                            DebugPress();
                        }
                    }
                    else if((x >= test1XMin && x <= test1XMax) && (y >= test1YMin && y <= test1YMax))
                    {
                        LoadTest("testboard1.brd");
                    }
                    else if((x >= test2XMin && x <= test2XMax) && (y >= test2YMin && y <= test2YMax))
                    {
                        LoadTest("testboard2.brd");
                    }
                    else if((x >= test3XMin && x <= test3XMax) && (y >= test3YMin && y <= test3YMax))
                    {
                        LoadTest("testboard3.brd");
                    }
                    //user clicks the face to reset the game
                    else if((x >= faceXMin && x <= faceXMax) && (y >= faceYMin && y <= faceYMax))
                    {
                        tiles.clear();
                        InitializeTiles();
                        PlaceBombs();
                        lose = false;
                        win = false;
                    }
                    else
                    {
                        //makes sure the user has not won or lost the game yet
                        if(!lose && !win)
                        {
                            //makes sure the tile has no flag on it before revealing
                            if(!LocateTile(x,y).hasFlag)
                            {
                                int counter = 0;
                                int counter2 = 0;
                                for(int i = 0; i < LocateTile(x,y).adjacentTiles.size(); i++)
                                {
                                    if(LocateTile(x,y).adjacentTiles[i]->revealTile)
                                    {
                                        counter++;
                                    }
                                    if(LocateTile(x,y).adjacentTiles[i]->hadFlag)
                                    {
                                        counter2++;
                                    }
                                }
                                if(LocateTile(x,y).adjacentBombTiles.empty() && LocateTile(x,y).hadFlag && counter > 0)
                                {
                                    LocateTile(x,y).ReactivateRecursion();
                                }
                                else if(LocateTile(x,y).adjacentBombTiles.empty() && LocateTile(x,y).hadFlag &&
                                counter2 == LocateTile(x, y).adjacentTiles.size())
                                {
                                    LocateTile(x,y).ReactivateRecursion();
                                }
                                LocateTile(x, y).RevealTile();
                                LocateTile(x,y).clicked = true;
                            }
                            //checks to see if this tile contains a mine
                            if(!LocateTile(x,y).hasFlag)
                            {
                                if (LocateTile(x, y).mine)
                                {
                                    lose = true;
                                    LocateTile(x,y).hadFlag = false;
                                }
                            }
                        }
                    }
                }
            }
        }
        window.clear(sf::Color::White);
        PrintAllTiles(window);
        PrintFace(window);
        PrintButtons(window);
        PrintDigits(window);
        window.display();
    }
}

//creates each tile and adds it to the 2D vector. Establishes its location as well
void Board::InitializeTiles()
{
    mines = minesConfig;
    digits = sf::Sprite(TextureManager::GetTexture("digits"));
    flagMineCounter = mines;
    //fills the 2D vector with tile objects
    for(int i = 0; i < height; i++)
    {
        vector<Tiles> row;
        for(int n = 0; n < width; n++)
        {
            Tiles tile;
            row.push_back(tile);
        }
        tiles.push_back(row);
    }
    //establishes the location data of each
    float x;
    float y = 0;
    for(int i = 0; i < height; i++)
    {
        x = 0;
        for(int n = 0; n < width; n++)
        {
            tiles[i][n].xMin = x;
            tiles[i][n].xMax = x + 32.f;
            tiles[i][n].yMin = y;
            tiles[i][n].yMax = y + 32.f;
            x += 32.f;
        }
        y += 32.f;
    }
}

//function to print out the tiles
void Board::PrintAllTiles(sf::RenderWindow& window)
{
    for (int i = 0; i < tiles.size(); i++)
    {
        for (int n = 0; n < tiles[i].size(); n++)
        {
            tiles[i][n].PrintIndividualTile(window);
        }
    }
    if(debug)
    {
        ShowBomb(window);
    }
    if(lose)
    {
        ShowBomb(window);
    }
    if(CheckVictory())
    {
        win = true;
        debug = false;
        flagMineCounter = 0;
        ShowBomb(window);
    }
}

//prints the face at the bottom of the board
void Board::PrintFace(sf::RenderWindow& window)
{

    float x = ((float)widthPixels / 2.f) - 32.f;
    float y = float(heightPixels) - 100.f;
    faceXMin = x;
    faceYMin = y;
    faceXMax = x + 64.f;
    faceYMax = y + 64.f;
    happyFace.setPosition(x, y);
    deadFace.setPosition(x,y);
    winFace.setPosition(x,y);
    if(!lose && !win)
    {
        window.draw(happyFace);
    }
    else if(lose)
    {
        window.draw(deadFace);
    }
    else if(win)
    {
        window.draw(winFace);
    }
}
//prints the debug button and the test buttons at the bottom
void Board::PrintButtons(sf::RenderWindow& window)
{
    float x = ((float)widthPixels / 2.f) + 96.f;
    float y = float(heightPixels) - 100.f;
    debugXMin = x;
    debugYMin = y;
    debugXMax = x + 64.f;
    debugYMax = y + 64.f;
    debugButton.setPosition(x,y);
    window.draw(debugButton);
    x += 64.f;
    test1XMin = x;
    test1YMin = y;
    test1XMax = x + 64.f;
    test1YMax = y + 64.f;
    test1Button.setPosition(x,y);
    window.draw(test1Button);
    x += 64.f;
    test2XMin = x;
    test2YMin = y;
    test2XMax = x + 64.f;
    test2YMax = y + 64.f;
    test2Button.setPosition(x,y);
    window.draw(test2Button);
    x += 64.f;
    test3XMin = x;
    test3YMin = y;
    test3XMax = x + 64.f;
    test3YMax = y + 64.f;
    test3Button.setPosition(x,y);
    window.draw(test3Button);
}
//method to randomly place bombs in the tiles. Once done with that it determines how many bomb tiles are next
//to each tile
void Board::PlaceBombs()
{
    //randomly places bombs
    for(int i = 0; i < mines; i++)
    {
        int random = Random::Int(0, numOfTiles - 1);
        int row = random / width;
        int column = random % width;

        if(!tiles[row][column].mine)
        {
            tiles[row][column].mine = true;
        }
        //There was already a bomb here
        else if (tiles[row][column].mine)
        {
            i--;
            continue;
        }
    }

    //determines the number of bombs next to each tile
    for(int i = 0; i < tiles.size(); i++)
    {
        for(int n = 0; n < tiles[i].size(); n++)
        {
            LocateAdjacentBombsAndTiles(i, n);
        }
    }
}

void Board::PrintBombs()
{
    for(int i = 0; i < tiles.size(); i++)
    {
        for(int n = 0; n < tiles[i].size(); n++)
        {
            cout << tiles[i][n].mine << endl;
        }
    }
}

//locates the specific tile where the mouse was clicked
Board::Tiles& Board::LocateTile(float x, float y)
{
    for(int i = 0; i < tiles.size(); i++)
    {
        for(int n = 0; n < tiles[i].size(); n++)
        {
            if((x >= tiles[i][n].xMin && x <= tiles[i][n].xMax) && (y >= tiles[i][n].yMin && y <= tiles[i][n].yMax))
            {
                tiles[i][n].tileFound = true;
                return tiles[i][n];
            }
        }
    }
    //User did not click a valid spot
    Tiles noTile;
    return noTile;
}

//locates how many tiles adjacent to the calling one has a bomb also how many tiles are next to the current tile
void Board::LocateAdjacentBombsAndTiles(int row, int column)
{
    int tempRow = row + 1;
    int tempColumn = column;
    if((tempRow >= 0 && tempRow < height) && (tempColumn >= 0 && tempColumn < width))
    {
        tiles[row][column].AddAdjacentTile(&tiles[tempRow][tempColumn]);
        if(tiles[tempRow][tempColumn].mine)
        {
            tiles[row][column].AddAdjacentBombs(&tiles[tempRow][tempColumn]);
        }
    }
    tempColumn += 1;
    if((tempRow >= 0 && tempRow < height) && (tempColumn >= 0 && tempColumn < width))
    {
        tiles[row][column].AddAdjacentTile(&tiles[tempRow][tempColumn]);
        if(tiles[tempRow][tempColumn].mine)
        {
            tiles[row][column].AddAdjacentBombs(&tiles[tempRow][tempColumn]);
        }
    }
    tempColumn -= 2;
    if((tempRow >= 0 && tempRow < height) && (tempColumn >= 0 && tempColumn < width))
    {
        tiles[row][column].AddAdjacentTile(&tiles[tempRow][tempColumn]);
        if(tiles[tempRow][tempColumn].mine)
        {
            tiles[row][column].AddAdjacentBombs(&tiles[tempRow][tempColumn]);
        }
    }
    tempRow -= 1;
    if((tempRow >= 0 && tempRow < height) && (tempColumn >= 0 && tempColumn < width))
    {
        tiles[row][column].AddAdjacentTile(&tiles[tempRow][tempColumn]);
        if(tiles[tempRow][tempColumn].mine)
        {
            tiles[row][column].AddAdjacentBombs(&tiles[tempRow][tempColumn]);
        }
    }
    tempColumn += 2;
    if((tempRow >= 0 && tempRow < height) && (tempColumn >= 0 && tempColumn < width))
    {
        tiles[row][column].AddAdjacentTile(&tiles[tempRow][tempColumn]);
        if(tiles[tempRow][tempColumn].mine)
        {
            tiles[row][column].AddAdjacentBombs(&tiles[tempRow][tempColumn]);
        }
    }
    tempRow -= 1;
    if((tempRow >= 0 && tempRow < height) && (tempColumn >= 0 && tempColumn < width))
    {
        tiles[row][column].AddAdjacentTile(&tiles[tempRow][tempColumn]);
        if(tiles[tempRow][tempColumn].mine)
        {
            tiles[row][column].AddAdjacentBombs(&tiles[tempRow][tempColumn]);
        }
    }
    tempColumn -= 1;
    if((tempRow >= 0 && tempRow < height) && (tempColumn >= 0 && tempColumn < width))
    {
        tiles[row][column].AddAdjacentTile(&tiles[tempRow][tempColumn]);
        if(tiles[tempRow][tempColumn].mine)
        {
            tiles[row][column].AddAdjacentBombs(&tiles[tempRow][tempColumn]);
        }
    }
    tempColumn -= 1;
    if((tempRow >= 0 && tempRow < height) && (tempColumn >= 0 && tempColumn < width))
    {
        tiles[row][column].AddAdjacentTile(&tiles[tempRow][tempColumn]);
        if(tiles[tempRow][tempColumn].mine)
        {
            tiles[row][column].AddAdjacentBombs(&tiles[tempRow][tempColumn]);
        }
    }
}

//the debug button has been pressed
void Board::DebugPress()
{
    if(!debug)
    {
        debug = true;
    }
    else if(debug)
    {
        debug = false;
    }
}

//function to show all the bombs on the board
void Board::ShowBomb(sf::RenderWindow& window)
{
    //marks the bombs with flags if the user wins
    if(win)
    {
        for (int i = 0; i < tiles.size(); i++)
        {
            for (int n = 0; n < tiles[i].size(); n++)
            {
                if(tiles[i][n].mine)
                {
                    tiles[i][n].tileSprite.setPosition(tiles[i][n].xMin, tiles[i][n].yMin);
                    tiles[i][n].flag.setPosition(tiles[i][n].xMin, tiles[i][n].yMin);
                    window.draw(tiles[i][n].tileSprite);
                    window.draw(tiles[i][n].flag);
                }
            }
        }
    }
    if(debug || lose)
    {
        for (int i = 0; i < tiles.size(); i++)
        {
            for (int n = 0; n < tiles[i].size(); n++)
            {
                if(tiles[i][n].mine)
                {
                    tiles[i][n].tileSprite.setPosition(tiles[i][n].xMin, tiles[i][n].yMin);
                    tiles[i][n].mineRevealed.setPosition(tiles[i][n].xMin, tiles[i][n].yMin);
                    window.draw(tiles[i][n].tileSprite);
                    window.draw(tiles[i][n].mineRevealed);
                }
            }
        }
    }
}
//loads a test
void Board::LoadTest(string filePath)
{
    //clears the 2d vector of tiles
    tiles.clear();
    InitializeTiles();
    mines = 0;
    flagMineCounter = 0;

    string path = "boards/";
    filePath = path + filePath;
    ifstream inFile(filePath);
    int count = 0;
    lose = false;
    win = false;

    if(inFile.is_open())
    {
        string line;
        //retrieves every line from the file
        while(getline(inFile, line))
        {
            for(int j = 0; j < line.length(); j++)
            {
                if(line.at(j) == '1')
                {
                    mines++;
                    flagMineCounter++;
                    int row = count / width;
                    int column = count % width;
                    tiles[row][column].mine = true;
                }
                count++;
            }
        }
    }

    //determines the adjacent bombs and tiles next to each tile
    for(int i = 0; i < tiles.size(); i++)
    {
        for(int n = 0; n < tiles[i].size(); n++)
        {
            LocateAdjacentBombsAndTiles(i, n);
        }
    }
}

//checks to see if all the non tiles have been revealed
bool Board::CheckVictory()
{
    int count = 0;
    for(int i = 0; i < tiles.size(); i++)
    {
        for(int n = 0; n < tiles[i].size(); n++)
        {
            if(!tiles[i][n].mine && tiles[i][n].revealTile)
            {
                count++;
            }
        }
    }

    if(count == (numOfTiles - mines))
    {
        return true;
    }
    else
    {
        return false;
    }
}

//prints out the mine counter
void Board::PrintDigits(sf::RenderWindow &window)
{
    digits = sf::Sprite(TextureManager::GetTexture("digits"));
    int digitOne = flagMineCounter / 100;
    int digitOneLocation;
    int digitTwo = 0;
    if(flagMineCounter >= 0 && flagMineCounter <= 99)
    {
        digitTwo = flagMineCounter / 10;
    }
    else if(flagMineCounter >= -99 && flagMineCounter <= 0)
    {
        digitTwo = flagMineCounter / 10;
    }
    else if(flagMineCounter >= 100 && flagMineCounter <= 999)
    {
        digitTwo = (flagMineCounter / 10) % 10;
    }
    else if(flagMineCounter >= -999 && flagMineCounter <= -100)
    {
        digitTwo = (flagMineCounter / 10) % 10;
    }
    int digitTwoLocation;
    int digitThree = flagMineCounter % 10;
    int digitThreeLocation;
    float xFirst = 21.f;
    float xSecond = 42.f;
    float xThird = 63.f;
    float y = float(heightPixels) - 100.f;
    int negLocation = 21 * 10;
    digitOne = abs(digitOne);
    digitTwo = abs(digitTwo);
    digitThree = abs(digitThree);

    if (digitOne == 0 || digitTwo == 0 || digitThree == 0)
    {
        if (digitOne == 0) {
            digitOneLocation = 0;
        }
        if (digitTwo == 0) {
            digitTwoLocation = 0;
        }
        if (digitThree == 0) {
            digitThreeLocation = 0;
        }
    }
    if (digitOne == 1 || digitTwo == 1 || digitThree == 1)
    {
        if (digitOne == 1) {
            digitOneLocation = 21;
        }
        if (digitTwo == 1) {
            digitTwoLocation = 21;
        }
        if (digitThree == 1) {
            digitThreeLocation = 21;
        }
    }
    if (digitOne == 2 || digitTwo == 2 || digitThree == 2)
    {
        if (digitOne == 2) {
            digitOneLocation = 21 * 2;
        }
        if (digitTwo == 2) {
            digitTwoLocation = 21 * 2;
        }
        if (digitThree == 2) {
            digitThreeLocation = 21 * 2;
        }
    }
    if (digitOne == 3 || digitTwo == 3 || digitThree == 3)
    {
        if (digitOne == 3) {
            digitOneLocation = 21 * 3;
        }
        if (digitTwo == 3) {
            digitTwoLocation = 21 * 3;
        }
        if (digitThree == 3) {
            digitThreeLocation = 21 * 3;
        }
    }
    if (digitOne == 4 || digitTwo == 4 || digitThree == 4)
    {
        if (digitOne == 4) {
            digitOneLocation = 21 * 4;
        }
        if (digitTwo == 4) {
            digitTwoLocation = 21 * 4;
        }
        if (digitThree == 4) {
            digitThreeLocation = 21 * 4;
        }
    }
    if (digitOne == 5 || digitTwo == 5 || digitThree == 5)
    {
        if (digitOne == 5) {
            digitOneLocation = 21 * 5;
        }
        if (digitTwo == 5) {
            digitTwoLocation = 21 * 5;
        }
        if (digitThree == 5) {
            digitThreeLocation = 21 * 5;
        }
    }
    if (digitOne == 6 || digitTwo == 6 || digitThree == 6)
    {
        if (digitOne == 6) {
            digitOneLocation = 21 * 6;
        }
        if (digitTwo == 6) {
            digitTwoLocation = 21 * 6;
        }
        if (digitThree == 6) {
            digitThreeLocation = 21 * 6;
        }
    }
    if (digitOne == 7 || digitTwo == 7 || digitThree == 7)
    {
        if (digitOne == 7) {
            digitOneLocation = 21 * 7;
        }
        if (digitTwo == 7) {
            digitTwoLocation = 21 * 7;
        }
        if (digitThree == 7) {
            digitThreeLocation = 21 * 7;
        }
    }
    if (digitOne == 8 || digitTwo == 8 || digitThree == 8)
    {
        if (digitOne == 8) {
            digitOneLocation = 21 * 8;
        }
        if (digitTwo == 8) {
            digitTwoLocation = 21 * 8;
        }
        if (digitThree == 8) {
            digitThreeLocation = 21 * 8;
        }
    }
    if (digitOne == 9 || digitTwo == 9 || digitThree == 9)
    {
        if (digitOne == 9) {
            digitOneLocation = 21 * 9;
        }
        if (digitTwo == 9) {
            digitTwoLocation = 21 * 9;
        }
        if (digitThree == 9) {
            digitThreeLocation = 21 * 9;
        }
    }

    if ((flagMineCounter < 10) && (flagMineCounter >= 0))
    {
        if(flagMineCounter < 0)
        {
            digits = sf::Sprite(TextureManager::GetTexture("digits"));
            digits.setPosition(0, y);
            digits.setTextureRect(sf::IntRect(negLocation, 0, 21, 32));
            window.draw(digits);
        }
        digits = sf::Sprite(TextureManager::GetTexture("digits"));
        digits.setPosition(xFirst, y);
        digits.setTextureRect(sf::IntRect(0, 0, 21, 32));
        window.draw(digits);
        digits = sf::Sprite(TextureManager::GetTexture("digits"));
        digits.setPosition(xSecond, y);
        digits.setTextureRect(sf::IntRect(0, 0, 21, 32));
        window.draw(digits);
        digits = sf::Sprite(TextureManager::GetTexture("digits"));
        digits.setPosition(xThird, y);
        digits.setTextureRect(sf::IntRect(digitThreeLocation, 0, 21, 32));
        window.draw(digits);
    }
    else if((flagMineCounter >= 10 && flagMineCounter <= 99) || (flagMineCounter >= -99 && flagMineCounter <= 0))
    {
        if(flagMineCounter < 0)
        {
            digits = sf::Sprite(TextureManager::GetTexture("digits"));
            digits.setPosition(0, y);
            digits.setTextureRect(sf::IntRect(negLocation, 0, 21, 32));
            window.draw(digits);
        }
        digits = sf::Sprite(TextureManager::GetTexture("digits"));
        digits.setPosition(xFirst, y);
        digits.setTextureRect(sf::IntRect(0, 0, 21, 32));
        window.draw(digits);
        digits = sf::Sprite(TextureManager::GetTexture("digits"));
        digits.setPosition(xSecond, y);
        digits.setTextureRect(sf::IntRect(digitTwoLocation, 0, 21, 32));
        window.draw(digits);
        digits = sf::Sprite(TextureManager::GetTexture("digits"));
        digits.setPosition(xThird, y);
        digits.setTextureRect(sf::IntRect(digitThreeLocation, 0, 21, 32));
        window.draw(digits);
    }
    else if((flagMineCounter >= 100 && flagMineCounter <= 999) || (flagMineCounter >= -999 && flagMineCounter <= -100))
    {
        if(flagMineCounter < 0)
        {
            digits = sf::Sprite(TextureManager::GetTexture("digits"));
            digits.setPosition(0, y);
            digits.setTextureRect(sf::IntRect(negLocation, 0, 21, 32));
            window.draw(digits);
        }
        digits = sf::Sprite(TextureManager::GetTexture("digits"));
        digits.setPosition(xFirst, y);
        digits.setTextureRect(sf::IntRect(digitOneLocation, 0, 21, 32));
        window.draw(digits);
        digits = sf::Sprite(TextureManager::GetTexture("digits"));
        digits.setPosition(xSecond, y);
        digits.setTextureRect(sf::IntRect(digitTwoLocation, 0, 21, 32));
        window.draw(digits);
        digits = sf::Sprite(TextureManager::GetTexture("digits"));
        digits.setPosition(xThird, y);
        digits.setTextureRect(sf::IntRect(digitThreeLocation, 0, 21, 32));
        window.draw(digits);
    }
}


//constructor for a tile
Board::Tiles::Tiles()
{
    mine = false;
    hasFlag = false;
    revealTile = false;
    tileFound = false;
    hadFlag = false;
    protectRecursion = false;
    clicked = true;
    tileSprite = sf::Sprite(TextureManager::GetTexture("tile_hidden"));
    tileSpriteRevealed = sf::Sprite(TextureManager::GetTexture("tile_revealed"));
    flag = sf::Sprite(TextureManager::GetTexture("flag"));
    mineRevealed = sf::Sprite(TextureManager::GetTexture("mine"));
    one = sf::Sprite(TextureManager::GetTexture("number_1"));
    two = sf::Sprite(TextureManager::GetTexture("number_2"));
    three = sf::Sprite(TextureManager::GetTexture("number_3"));
    four = sf::Sprite(TextureManager::GetTexture("number_4"));
    five = sf::Sprite(TextureManager::GetTexture("number_5"));
    six = sf::Sprite(TextureManager::GetTexture("number_6"));
    seven = sf::Sprite(TextureManager::GetTexture("number_7"));
    eight = sf::Sprite(TextureManager::GetTexture("number_8"));
    xMin = 0;
    xMax = 0;
    yMin = 0;
    yMax = 0;
}

//adds a flag or removes from this tile
void Board::Tiles::AddRemoveFlag()
{
    if(!hasFlag)
    {
        hasFlag = true;
        clicked = true;
    }
    else if(hasFlag)
    {
        hasFlag = false;
        hadFlag = true;
        int counter = 0;
        for(int n = 0; n < adjacentTiles.size(); n++)
        {
            if(adjacentTiles[n]->revealTile)
            {
                counter++;
            }
        }
        if(counter > 0)
        {
            protectRecursion = true;
        }
    }
}

//Prints the individual tile
void Board::Tiles::PrintIndividualTile(sf::RenderWindow& window)
{
    if(revealTile && mine)
    {
        tileSpriteRevealed.setPosition(xMin, yMin);
        mineRevealed.setPosition(xMin, yMin);
        window.draw(tileSpriteRevealed);
        window.draw(mineRevealed);
    }
    else if(revealTile)
    {
        if(adjacentBombTiles.empty())
        {
            tileSpriteRevealed.setPosition(xMin, yMin);
            window.draw(tileSpriteRevealed);
            for (int i = 0; i < adjacentTiles.size(); i++)
            {
                if (!adjacentTiles[i]->revealTile && !adjacentTiles[i]->hasFlag && !adjacentTiles[i]->protectRecursion
                && adjacentTiles[i]->clicked)
                {
                    adjacentTiles[i]->revealTile = true;
                    adjacentTiles[i]->protectRecursion = false;
                    adjacentTiles[i]->clicked = true;
                    adjacentTiles[i]->PrintIndividualTile(window);
                }
            }
        }
        if(adjacentBombTiles.size() == 1)
        {
            tileSpriteRevealed.setPosition(xMin, yMin);
            one.setPosition(xMin,yMin);
            window.draw(tileSpriteRevealed);
            window.draw(one);
        }
        if(adjacentBombTiles.size() == 2)
        {
            tileSpriteRevealed.setPosition(xMin, yMin);
            two.setPosition(xMin,yMin);
            window.draw(tileSpriteRevealed);
            window.draw(two);
        }
        if(adjacentBombTiles.size() == 3)
        {
            tileSpriteRevealed.setPosition(xMin, yMin);
            three.setPosition(xMin,yMin);
            window.draw(tileSpriteRevealed);
            window.draw(three);
        }
        if(adjacentBombTiles.size() == 4)
        {
            tileSpriteRevealed.setPosition(xMin, yMin);
            four.setPosition(xMin,yMin);
            window.draw(tileSpriteRevealed);
            window.draw(four);
        }
        if(adjacentBombTiles.size() == 5)
        {
            tileSpriteRevealed.setPosition(xMin, yMin);
            five.setPosition(xMin,yMin);
            window.draw(tileSpriteRevealed);
            window.draw(five);
        }
        if(adjacentBombTiles.size() == 6)
        {
            tileSpriteRevealed.setPosition(xMin, yMin);
            six.setPosition(xMin,yMin);
            window.draw(tileSpriteRevealed);
            window.draw(six);
        }
        if(adjacentBombTiles.size() == 7)
        {
            tileSpriteRevealed.setPosition(xMin, yMin);
            seven.setPosition(xMin,yMin);
            window.draw(tileSpriteRevealed);
            window.draw(seven);
        }
        if(adjacentBombTiles.size() == 8)
        {
            tileSpriteRevealed.setPosition(xMin, yMin);
            eight.setPosition(xMin,yMin);
            window.draw(tileSpriteRevealed);
            window.draw(eight);
        }
    }
    else if(hasFlag)
    {
        tileSprite.setPosition(xMin, yMin);
        flag.setPosition(xMin, yMin);
        window.draw(tileSprite);
        window.draw(flag);
    }
    else if(!hasFlag)
    {
        tileSprite.setPosition(xMin, yMin);
        window.draw(tileSprite);
    }
}
//reveals this tile
void Board::Tiles::RevealTile()
{
    if(!hasFlag)
    {
        revealTile = true;
    }
}
//adds an adjacent bombs in the tiles vector
void Board::Tiles::AddAdjacentBombs(Tiles* currentTile)
{
    adjacentBombTiles.push_back(currentTile);
}
//adds an adjacent tile to the tile's vector of adjacent tiles
void Board::Tiles::AddAdjacentTile(Tiles* currentTile)
{
    adjacentTiles.push_back(currentTile);
}

void Board::Tiles::ReactivateRecursion()
{
    protectRecursion = true;
    bool keep = false;
    if(adjacentBombTiles.empty())
    {
        keep = true;
    }

    for (int i = 0; i < adjacentTiles.size(); i++)
    {
        if (!adjacentTiles[i]->revealTile && !adjacentTiles[i]->mine && !adjacentTiles[i]->hasFlag && keep)
        {
            adjacentTiles[i]->protectRecursion = false;
            adjacentTiles[i]->revealTile = true;
            adjacentTiles[i]->clicked = true;
            adjacentTiles[i]->ReactivateRecursion();
        }
    }
}







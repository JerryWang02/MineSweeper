#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <unordered_map>
#include "TextureManager.h"
#include "Board.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "Random.h"
using namespace std;

//gets the board data from the config file
Board GetDataFromFile(string filePath)
{
    string path = "boards/";
    filePath = path + filePath;
    ifstream inFile(filePath);
    int width;
    int height;
    int mines;
    if(inFile.is_open())
    {
        inFile >> width;
        inFile >> height;
        inFile >> mines;
    }
    //creates an object that represents the board
    Board board(width, height, mines);
    return board;
}

int main()
{
    //creates a window and displays it to the screen
    Board board = GetDataFromFile("config.cfg");
    board.InitializeTiles();
    board.PlaceBombs();
    board.DrawWindow();

    return 0;
}
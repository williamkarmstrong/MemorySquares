//
//  game.h
//  Memory Squares
//
//  Created by William Armstrong on 09/04/2024.
//

#ifndef game_h
#define game_h

#include <stdio.h>
#include <iostream>
#include <ctime>
#include <vector>
#include <sstream>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Network.hpp"
#include "SFML/System.hpp"

// window
sf::RenderWindow window(sf::VideoMode(500, 500), "Memory Squares");

// event
sf::Event event;

// time
sf::Time delay(sf::Clock, sf::Time);

// text and fonts
sf::Font font;
sf::Text scoreText;
sf::Text gameOver;

// retry
sf::Texture texture;
sf::Sprite retry;

// sound
sf::SoundBuffer Cbuffer;
sf::SoundBuffer Wbuffer;
sf::Sound correct;
sf::Sound wrong;

// game variables
bool stopped = false;
bool newNum = true;
bool generate = false;
bool answer = false;
bool pause = true;

int current = 0;
int prev = 0;
int score = 0;


// pattern
std::vector<int> pattern;
std::vector<int> sq;


// grid
sf::RectangleShape grid[3][3];


// build the grid of squares
void createGrid();

// initialise sound and text
void initText();
void initSound();

// changing squares functions
void changeSquare(std::vector<int>, bool);
void clearSquare(int);

// add block to pattern
void addPattern();

// show pattern
void genPattern();

// answer functions
void getSquare(int, int);
bool checkAnswer(std::vector<int>);
bool fullSequence();

// show game over text
void stopGame();

// reset game
void restart();

// update and render game objects
void update();



void createGrid() {

    float outline = 3.0;
    float size = 97.0;
    float space = outline+size;
    
    sf::RectangleShape rect;
    rect.setSize(sf::Vector2f(size,size));
    rect.setFillColor(sf::Color(243,245,249));
    rect.setOutlineThickness(outline);
    rect.setOutlineColor(sf::Color::Black);

    for (int i = 0; i<3; i++) {
        for (int j = 0; j<3; j++) {
            grid[i][j] = rect;
            grid[i][j].setPosition(space*(i+1), space*(j+1));
            window.draw(grid[i][j]);
        }
    }
}


void initText() {
    
    if (!font.loadFromFile("Resources/franklin-gothic/FranklinGothic.ttf")) {
        std::cout << "Font failed to load!" << std::endl;
    }
        
    scoreText.setFont(font);
    scoreText.setCharacterSize(28);
    scoreText.setFillColor(sf::Color::Black);
    scoreText.setPosition(200, 50);
    
    gameOver.setFont(font);
    gameOver.setCharacterSize(28);
    gameOver.setFillColor(sf::Color::Black);
    gameOver.setPosition(180, 405);
}


void initSound() {
    if (!Cbuffer.loadFromFile("Resources/click.wav")) {
        std::cout << "Sound failed to load!" << std::endl;
    }
    correct.setBuffer(Cbuffer);
    
    if (!Wbuffer.loadFromFile("Resources/wrong.wav")) {
        std::cout << "Sound failed to load!" << std::endl;
    }
    wrong.setBuffer(Wbuffer);
}


void initSprite() {
    if (!texture.loadFromFile("Resources/retry.png")) {
        std::cout << "Image failed to load!" << std::endl;
    }
    
    retry.setTexture(texture);
    retry.setPosition(230, 420);
}


void changeSquare(std::vector<int> square, bool green) {

    if (green) {
        grid[square[0]][square[1]].setFillColor(sf::Color::Green);
    }
    else {
        grid[square[0]][square[1]].setFillColor(sf::Color::Red);
    }
    
    window.draw(grid[square[0]][square[1]]);
}


void clearSquare(int i) {
    grid[pattern[i]][pattern[i+1]].setFillColor(sf::Color(243,245,249));
    window.draw(grid[pattern[i]][pattern[i+1]]);
}
    

sf::Time delay(sf::Clock clock, sf::Time elapsed) {
    pause = true;
    elapsed = clock.getElapsedTime();
    return elapsed;
}


void addPattern() {
    std::srand(static_cast<unsigned>(time(NULL)));
    
    unsigned long len = pattern.size();
    
    int i = rand() % 3;
    int j = rand() % 3;
    
    if (len > 0) {
        while (pattern[len-2] == i && pattern[len-1] == j) {
            i = rand() % 3;
            j = rand() % 3;
        }
    }
    
    pattern.push_back(i);
    pattern.push_back(j);
    
    newNum = false;
    generate = true;
}


void genPattern() {
    prev = current;
    if (current == pattern.size()) {
        generate = false;
        answer = true;
        current = 0;
        prev = 0;
    }
    
    else {
        changeSquare(std::vector<int> {pattern[current], pattern[current+1]}, true);
        current += 2;
    }
}


std::vector<int> getSquare() {
    std::vector<int> square;
    
    for (int i = 0; i<3; i++) {
        for (int j = 0; j<3; j++) {
            if (grid[i][j].getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
                square.push_back(i);
                square.push_back(j);
                return square;
            }
        }
    }
    
    return square;
}


bool checkAnswer(std::vector<int> square) {
    if (pattern[current] == square[0] and pattern[current+1] == square[1]) {
        return true;
    }
    
    return false;
}


bool fullSequence() {
    prev = current;
    
    if (current+2 == pattern.size()) {
        current = 0;
        answer = false;
        newNum = true;
        pause = true;
        score += 1;
        return true;
    }
    
    else {
        current += 2;
        return false;
    }
}


void stopGame() {
    stopped = true;
    newNum = false;
    generate = false;
    answer = false;
    pause = false;
}


void restart() {
    if (retry.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) {
        stopped = false;
        newNum = true;
        current = 0;
        prev = 0;
        score = 0;
        pattern.clear();
    }
}


void update() {
    std::stringstream ss;
    
    ss << "Score: " << score << "\n";
    scoreText.setString(ss.str());
    
    window.draw(scoreText);
    
    if (stopped) {
        window.draw(retry);
    }
    
    for (int i = 0; i<3; i++) {
        for (int j = 0; j<3; j++) {
            window.draw(grid[i][j]);
        }
    }
    
    window.display();
}


#endif /* game_h */

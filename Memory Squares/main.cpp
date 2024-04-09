//
//  main.cpp
//  Memory Squares
//
//  Created by William Armstrong on 09/04/2024.
//

#include "game.h"
#include <iostream>

int main(int argc, const char * argv[]) {
    
    // build the grid
    createGrid();
    
    // text and sound
    initText();
    initSound();
    initSprite();
    
    // clock
    sf::Clock clock;
    sf::Time elapsed;
    
    // initial delay
    delay(clock, elapsed);
    
    // avoids black flash at start of game
    window.clear(sf::Color::White);
    
    // game loop
    while (window.isOpen()) {
    
        // event handling
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (answer) {
                    sq = getSquare();
                }
                else if (stopped) {
                    restart();
                }
            }
        }
        
        
        if (not pause) {
            
            // add a new block to pattern
            if (newNum) {
                addPattern();
            }
            
            // show pattern
            else if (generate) {
                clearSquare(prev);
                genPattern();
                
                if (generate) {
                    elapsed = delay(clock, elapsed);
                }
            }
            
            // check answer
            else if (answer and not sq.empty()) {
                
                correct.stop();
                clearSquare(prev);
                
                if (checkAnswer(sq)) {
                    if (fullSequence()) {
                        elapsed = delay(clock, elapsed);
                    }
                    
                    changeSquare(sq, false);
                    correct.play();
                }
                
                else {
                    wrong.play();
                    stopGame();
                }
                
                sq.clear();
            }
        }

        // check if game has paused for enough time before displaying next block
        else if (pause and elapsed.asSeconds() + 1 < clock.getElapsedTime().asSeconds()) {
            pause = false;
        }
        
        update();
        window.clear(sf::Color::White);
    }
}

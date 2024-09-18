#pragma once
#include <SFML/Graphics.hpp>
#include "NetworkManager.h"
#include "HTMLParser.h"
#include "Renderer.h"

class Browser {
public:
    Browser();
    void run();

private:
    sf::RenderWindow window;
    NetworkManager networkManager;
    HTMLParser htmlParser;
    Renderer renderer;
    
    void handleEvents();
    void loadPage(const std::string& url);
    
    const std::string DEFAULT_URL = "http://example.com";
    /*const std::string DEFAULT_URL = "http://info.cern.ch/hypertext/WWW/TheProject.html";*/
};

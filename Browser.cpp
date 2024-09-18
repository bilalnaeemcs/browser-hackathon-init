#include "Browser.h"
#include <iostream>


Browser::Browser() : window(sf::VideoMode(800, 600), "Simple Browser") {
    renderer.setHTMLParser(&htmlParser);  // Add this line
    loadPage(DEFAULT_URL);
}

// ... (rest of the file remains the same)
void Browser::run() {
    while (window.isOpen()) {
        handleEvents();
        window.clear(sf::Color::White);
        renderer.render(window);
        window.display();
    }
}

void Browser::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Browser::loadPage(const std::string& url) {
    std::cout << "Loading page: " << url << std::endl;
    std::string html = networkManager.fetchPage(url);
    htmlParser.parse(html);
    renderer.update(htmlParser.getDOM());
}

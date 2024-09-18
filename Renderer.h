#pragma once
#include <SFML/Graphics.hpp>
#include <gumbo.h>
#include <vector>
#include "HTMLParser.h"

class Renderer {
public:
    Renderer();
    void setHTMLParser(HTMLParser* parser);
    void update(const GumboNode* dom);
    void render(sf::RenderWindow& window);

private:
    sf::Font font;
    std::vector<sf::Text> textElements;
    HTMLParser* htmlParser;

    void renderNode(const GumboNode* node, sf::Vector2f& position, unsigned int fontSize, bool isLink, int depth);
    void applyStyles(const GumboNode* node, unsigned int& fontSize, bool& isLink, sf::Color& color);
    void applyCSSProperties(const std::string& style, unsigned int& fontSize, bool& isLink, sf::Color& color);
    void applyCSSProperty(const std::string& name, const std::string& value, unsigned int& fontSize, bool& isLink, sf::Color& color);
};

#include "Renderer.h"
#include <gumbo.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

Renderer::Renderer() : htmlParser(nullptr) {
    if (!font.loadFromFile("/System/Library/Fonts/Helvetica.ttc")) {
        std::cerr << "Failed to load font" << std::endl;
    }
}

void Renderer::setHTMLParser(HTMLParser* parser) {
    htmlParser = parser;
}

void Renderer::update(const GumboNode* dom) {
    textElements.clear();
    sf::Vector2f position(10, 10);  // Start with a small padding
    renderNode(dom, position, 16, false, 0);
}

void Renderer::renderNode(const GumboNode* node, sf::Vector2f& position, unsigned int fontSize, bool isLink, int depth) {
    std::string indent(depth * 2, ' ');

    // Debug output
    switch (node->type) {
        case GUMBO_NODE_DOCUMENT:
            std::cout << indent << "Document" << std::endl;
            break;
        case GUMBO_NODE_ELEMENT:
            std::cout << indent << "Element: <" << gumbo_normalized_tagname(node->v.element.tag) 
                      << "> at SFML position (" << position.x << ", " << position.y << ")" << std::endl;
            break;
        case GUMBO_NODE_TEXT:
            std::cout << indent << "Text: \"" << node->v.text.text 
                      << "\" at SFML position (" << position.x << ", " << position.y << ")" << std::endl;
            break;
        case GUMBO_NODE_COMMENT:
            std::cout << indent << "Comment: \"" << node->v.text.text 
                      << "\" at SFML position (" << position.x << ", " << position.y << ")" << std::endl;
            return;  // We don't render comments
        case GUMBO_NODE_WHITESPACE:
            std::cout << indent << "Whitespace at SFML position (" << position.x << ", " << position.y << ")" << std::endl;
            break;  // Don't return, as we might need to handle whitespace
        default:
            std::cout << indent << "Unknown node type at SFML position (" << position.x << ", " << position.y << ")" << std::endl;
            return;
    }

    if (node->type == GUMBO_NODE_ELEMENT) {
        unsigned int currentFontSize = fontSize;
        bool currentIsLink = isLink;
        sf::Color currentColor = sf::Color::Black;

        applyStyles(node, currentFontSize, currentIsLink, currentColor);

        switch (node->v.element.tag) {
            case GUMBO_TAG_H1:
                if (currentFontSize == fontSize) currentFontSize = 24;
                position.y += 10;
                break;
            case GUMBO_TAG_P:
                position.y += 5;
                break;
            case GUMBO_TAG_A:
                if (!currentIsLink) currentIsLink = true;
                break;
            case GUMBO_TAG_STYLE:
                return;  // Ignore style tags
            default:
                break;
        }

        const GumboVector* children = &node->v.element.children;
        for (unsigned int i = 0; i < children->length; ++i) {
            renderNode(static_cast<const GumboNode*>(children->data[i]), position, currentFontSize, currentIsLink, depth + 1);
        }

        if (node->v.element.tag == GUMBO_TAG_P || node->v.element.tag == GUMBO_TAG_H1) {
            position.y += 10;
        }
    } else if (node->type == GUMBO_NODE_TEXT || node->type == GUMBO_NODE_WHITESPACE) {
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(isLink ? sf::Color::Blue : sf::Color::Black);
        text.setPosition(position);

        std::string str = node->v.text.text;
        std::istringstream words(str);
        std::string word;
        float lineWidth = 0;
        while (std::getline(words, word, ' ')) {
            sf::Text wordText = text;
            wordText.setString(word + " ");
            if (lineWidth + wordText.getLocalBounds().width > 780) {
                position.y += fontSize + 5;
                lineWidth = 0;
            }
            wordText.setPosition(position.x + lineWidth, position.y);
            textElements.push_back(wordText);
            lineWidth += wordText.getLocalBounds().width;
        }
        position.y += fontSize + 5;
    }
}
void Renderer::applyStyles(const GumboNode* node, unsigned int& fontSize, bool& isLink, sf::Color& color) {
    if (!htmlParser) return;

    // Apply element-specific styles
    GumboAttribute* styleAttr = gumbo_get_attribute(&node->v.element.attributes, "style");
    if (styleAttr) {
        applyCSSProperties(styleAttr->value, fontSize, isLink, color);
    }

    // Apply global styles (very basic implementation)
    for (const auto& rule : htmlParser->cssParser.rules) {
        if (rule.selector == gumbo_normalized_tagname(node->v.element.tag)) {
            for (const auto& prop : rule.properties) {
                applyCSSProperty(prop.first, prop.second, fontSize, isLink, color);
            }
        }
    }
}

void Renderer::applyCSSProperties(const std::string& style, unsigned int& fontSize, bool& isLink, sf::Color& color) {
    std::istringstream ss(style);
    std::string property;
    while (std::getline(ss, property, ';')) {
        size_t colonPos = property.find(':');
        if (colonPos != std::string::npos) {
            std::string name = property.substr(0, colonPos);
            std::string value = property.substr(colonPos + 1);
            // Trim whitespace
            name.erase(0, name.find_first_not_of(" \t"));
            name.erase(name.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            
            applyCSSProperty(name, value, fontSize, isLink, color);
        }
    }
}

void Renderer::applyCSSProperty(const std::string& name, const std::string& value, unsigned int& fontSize, bool& isLink, sf::Color& color) {
    if (name == "color") {
        if (value == "blue") {
            color = sf::Color::Blue;
            isLink = true;
        } else if (value == "red") {
            color = sf::Color::Red;
        }
        // Add more color handlers as needed
    } else if (name == "font-size") {
        if (value.find("px") != std::string::npos) {
            fontSize = std::stoi(value.substr(0, value.find("px")));
        }
        // Add handlers for other units (em, rem, etc.) as needed
    }
    // Add more property handlers as needed
}

void Renderer::render(sf::RenderWindow& window) {
    for (const auto& text : textElements) {
        window.draw(text);
    }
}

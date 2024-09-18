#include "CSSParser.h"
#include <sstream>
#include <iostream>

void CSSParser::parseInlineStyle(const std::string& style) {
    CSSRule rule;
    std::istringstream stream(style);
    std::string property, value;
    
    while (std::getline(stream, property, ':') && std::getline(stream, value, ';')) {
        // Trim whitespace
        property.erase(0, property.find_first_not_of(" \t"));
        property.erase(property.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);
        
        rule.properties[property] = value;
    }
    
    rules.push_back(rule);
}

void CSSParser::parseStylesheet(const std::string& css) {
    std::istringstream stream(css);
    std::string line;
    CSSRule currentRule;
    bool inRule = false;

    while (std::getline(stream, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        if (line.empty()) continue;

        if (line.back() == '{') {
            // Start of a new rule
            currentRule.selector = line.substr(0, line.length() - 1);
            inRule = true;
        } else if (line == "}") {
            // End of a rule
            rules.push_back(currentRule);
            currentRule = CSSRule();
            inRule = false;
        } else if (inRule) {
            // Property within a rule
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string property = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);
                // Trim whitespace
                property.erase(0, property.find_first_not_of(" \t"));
                property.erase(property.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                if (value.back() == ';') {
                    value.pop_back();
                }
                currentRule.properties[property] = value;
            }
        }
    }
}

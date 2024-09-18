#pragma once
#include <string>
#include <unordered_map>
#include <vector>

struct CSSRule {
    std::string selector;
    std::unordered_map<std::string, std::string> properties;
};

class CSSParser {
public:
    void parseInlineStyle(const std::string& style);
    void parseStylesheet(const std::string& css);
    std::vector<CSSRule> rules;
};

#pragma once
#include <string>
#include <gumbo.h>
#include "CSSParser.h"

class HTMLParser {
public:
    void parse(const std::string& html);
    const GumboNode* getDOM() const;
    CSSParser cssParser;

private:
    GumboOutput* output;
    void extractStyles(const GumboNode* node);
};

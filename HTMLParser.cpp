#include "HTMLParser.h"
#include <iostream>

void HTMLParser::parse(const std::string& html) {
    if (output) {
        gumbo_destroy_output(&kGumboDefaultOptions, output);
    }
    output = gumbo_parse(html.c_str());
    extractStyles(output->root);
}

const GumboNode* HTMLParser::getDOM() const {
    return output ? output->root : nullptr;
}

void HTMLParser::extractStyles(const GumboNode* node) {
    if (node->type != GUMBO_NODE_ELEMENT) {
        return;
    }

    GumboAttribute* style = gumbo_get_attribute(&node->v.element.attributes, "style");
    if (style) {
        cssParser.parseInlineStyle(style->value);
    }

    if (node->v.element.tag == GUMBO_TAG_STYLE) {
        const GumboVector* children = &node->v.element.children;
        if (children->length > 0) {
            const GumboNode* textNode = static_cast<const GumboNode*>(children->data[0]);
            if (textNode->type == GUMBO_NODE_TEXT) {
                cssParser.parseStylesheet(textNode->v.text.text);
            }
        }
    }

    const GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; ++i) {
        extractStyles(static_cast<const GumboNode*>(children->data[i]));
    }
}

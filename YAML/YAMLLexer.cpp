//
// Created by Oskar on 22.05.2020.
//

#include "YAMLLexer.h"
#include <stdexcept>
#include <iostream>

namespace YAML {

    std::string tolow(std::string a)
    {
        for(char & i : a)
        {
            i=tolower(i);
        }
        return a;
    }

    YAMLLexer::YAMLLexer(const std::string &filename) {
        is = std::ifstream(filename, std::ifstream::in);
    }

    unsigned char YAMLLexer::nextChar() {
        unsigned char c;
        if (lookahead.empty()) {
            c = is.get();
            if (is.eof()) {
                eof = true;
            }
        } else {
            c = lookahead.front();
            lookahead.pop_front();
            eof = false;
        }
        return c;
    }

    YAMLToken YAMLLexer::nextToken() {
        if(state == END) {
            return YAMLToken();
        }
        state = START;
        std::string value;

        while(true) {
            switch(state) {
                case START: {
                    unsigned char c = nextChar();
                    if(eof) {
                        state = END;
                    }
                    else if(c == '|') {
                        state = LITERAL_BLOCK_SCALAR_START;
                    }
                    else if(c == '>') {
                        state = FOLDED_BLOCK_SCALAR_START;
                    }
                    else if(c == '"') {
                        state = DOUBLE_QUOTED_SCALAR_START;
                    }
                    else if(c == '\'') {
                        state = SINGLE_QUOTED_SCALAR_START;
                    }
                    else if(c == ':') {
                        state = MAPPER;
                    }
                    else if(c == '-') {
                        state = HYPHEN;
                    }
                    else if(c == '#') {
                        state = HASH;
                    }
                    else if(c == '\n') {
                        state = EOL;
                    }
                    else if(c == ' ') {
                        state = SPACE;
                    }
                    else if(isalpha(c) || isdigit(c) || c == '.') {
                        value += c;
                        state = PLAIN_SCALAR_START;
                    }
                    break;
                }
                case END: {
                    return YAMLToken();
                }
                case HYPHEN: {
                    return YAMLToken(HYPHEN_TOKEN, "");
                }
                case MAPPER: {
                    return YAMLToken(MAPPER_TOKEN, "");
                }
                case HASH: {
                    return YAMLToken(COMMENT_TOKEN, "");
                }
                case EOL: {
                    return YAMLToken(EOL_TOKEN, "");
                }
                case PLAIN_SCALAR_START: {
                    unsigned char c = nextChar();
                    if (c == '\n' || eof) {
                        state = PLAIN;
                        lookahead.push_back(c);
                    }
                    else if(c == ':' || c == '#') {
                        state = PLAIN;
                        lookahead.push_back(c);
                    }
                    else if (c != '\r') {
                        value += c;
                    }
                    break;
                }
                case DOUBLE_QUOTED_SCALAR_START: {
                    unsigned char c = nextChar();
                    if (eof) {
                        state = INVALID;
                        value = "\"";
                    }
                    else if (c == '"') {
                        state = QUOTED;
                    }
                    else if (c != '\r') {
                        if(!value.empty() && value.back() == '\n') {
                            while(c == ' ') {
                                c = nextChar();
                            }
                        }
                        value += c;
                    }
                    break;
                }
                case SINGLE_QUOTED_SCALAR_START: {
                    unsigned char c = nextChar();
                    if (eof) {
                        state = INVALID;
                        value = "'";
                    }
                    else if (c == '\'') {
                        state = QUOTED;
                    }
                    else if (c != '\r') {
                        if(!value.empty() && value.back() == '\n') {
                            while(c == ' ') {
                                c = nextChar();
                            }
                        }
                        value += c;
                    }
                    break;
                }
                case SPACE: {
                    return YAMLToken(INDENT_TOKEN, "");
                }
                case FOLDED_BLOCK_SCALAR_START: {
                    return YAMLToken(FOLDED_BLOCK_TOKEN, "");
                }
                case LITERAL_BLOCK_SCALAR_START: {
                    return YAMLToken(LITERAL_BLOCK_TOKEN, "");
                }
                case PLAIN: {
                    if(tolow(value) == "true") {
                        return YAMLToken(TRUE_TOKEN, "true");
                    }
                    if(tolow(value) == "false") {
                        return YAMLToken(FALSE_TOKEN, "false");
                    }
                    if(tolow(value) == "null") {
                        return YAMLToken(YNULL_TOKEN, "null");
                    }
                    return YAMLToken(PLAIN_SCALAR_TOKEN, value);
                }
                case QUOTED: {
                    return YAMLToken(QUOTED_SCALAR_TOKEN, value);
                }
                default: {
                    throw std::runtime_error("Error: Expected " + value);
                }
            }
        }
    }
}
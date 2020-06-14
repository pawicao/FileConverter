//
// Created by joanna on 24.05.2020.
//

#include "JSONLexer.h"
#include <stdexcept>
#include <iostream>

namespace JSON
{
    // file to be parsed
    JSONLexer::JSONLexer(std::string path){
        file = std::ifstream(path, std::ifstream::in);;
    }

    unsigned char JSONLexer::readChar(){
        if(rest.empty()){
            unsigned char c = file.get();
            if(file.eof()){
                end = true;
            }
            return c;
        }else{
            unsigned char c = rest.front();
            rest.pop_front();
            end = false;
            return c;
        }
    }

    JSONToken JSONLexer::readToken(){
        std::string value;
        if (jsonState == END)
        {
            return JSONToken();
        }
        jsonState = START;
        for (;;){
            switch (jsonState)
                case START:{
                {
                    unsigned char next = readChar();
                    if(end){
                        jsonState = END;
                    }else if(next == '{') {
                        jsonState = LBRACE;
                    }else if(next == '}') {
                        jsonState = RBRACE;
                    }else if(next == '[') {
                        jsonState  = LBRACKET;
                    }else if(next == ']') {
                        jsonState  = RBRACKET;
                    }else if(next == ':') {
                        jsonState  = KEY_VALUE;
                    }else if(next == ',') {
                        jsonState  = NEXT;
                    }else if(next == '"') {
                        jsonState = START_STRING;
                    }else if(isalpha(next)) {
                        jsonState = START_SPECIAL;
                        value+=next;
                    }else if(isdigit(next) || next == '.') {
                        jsonState = START_NUMBER;
                        value+=next;
                    };
                    break;
                }
                case END:
                    return JSONToken();
                case KEY_VALUE:
                    return JSONToken(COLON, "");
                case NEXT:
                    return JSONToken(COMMA, "");
                case START_STRING:
                {
                    unsigned char next = readChar();
                    if (end){
                        jsonState = INVALID;
                        value = "\"";
                    }else if (next == '"'){
                        jsonState = STRING_STATE;
                    }
                    else{
                        value += next;
                    }
                    break;
                }
                case START_NUMBER:
                {
                    unsigned char next = readChar();
                    if (end){
                        jsonState = INVALID;
                        value = "\"";
                    }else if (isdigit(next) || next == '.'){
                        value += next;
                    }else if(next == ',' || next == '}' || next == ']'){
                        rest.push_back(next);
                        jsonState = NUMBER_STATE;
                    }
                    break;
                }
                case START_SPECIAL:
                {
                    unsigned char next = readChar();
                    if (end){
                        jsonState = INVALID;
                        value = "\"";
                    }else if (isalpha(next)){
                        value += next;
                    }else if(next == ',' || next == '}' || next == ']'){
                        rest.push_back(next);
                        if(value == "true"){
                            jsonState = TRUE_STATE;
                        }else if(value == "false"){
                            jsonState = FALSE_STATE;
                        }else if(value == "null"){
                            jsonState = NULL_STATE;
                        }
                    }
                    break;
                }
                case NULL_STATE:
                    return JSONToken(JNULL, "null");
                case TRUE_STATE:
                    return JSONToken(TRUE,"true");
                case FALSE_STATE:
                    return JSONToken(FALSE, "false");
                case LBRACE:
                    return JSONToken(LEFT_BRACE, "");
                case RBRACE:
                    return JSONToken(RIGHT_BRACE, "");
                case LBRACKET:
                    return JSONToken(LEFT_BRACKET, "");
                case RBRACKET:
                    return JSONToken(RIGHT_BRACKET, "");
                case NUMBER_STATE:
                    return JSONToken(NUMBER, value);
                case STRING_STATE:
                    return JSONToken(STRING, value);
                default:
                    throw std::runtime_error("Error: Expected" + value);
            }
        }
    }



}

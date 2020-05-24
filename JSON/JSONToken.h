//
// Created by joanna on 23.05.2020.
//
#ifndef FILECONVERTER_JSONTOKEN_H
#define FILECONVERTER_JSONTOKEN_H
namespace JSON
{
    enum JSONTokenType {
        LEFT_BRACE, // {
        RIGHT_BRACE, // }
        LEFT_BRACKET, // [
        RIGHT_BRACKET, // ]
        COLON, // :
        COMMA, // ,
        STRING,
        NUMBER,
        TRUE, // true
        FALSE, // false
        JNULL, // null
        INVALID_TYPE
    };

    enum JSONState {
        START,
        END,
        LBRACE,
        RBRACE,
        LBRACKET,
        RBRACKET,
        START_STRING,
        START_NUMBER,
        START_SPECIAL,
        STRING_STATE,
        NUMBER_STATE,
        TRUE_STATE,
        FALSE_STATE,
        NULL_STATE,
        KEY_VALUE,
        NEXT,
        INVALID
    };

    class  JSONToken{

    public:
        JSONTokenType token;
        std::string value;

        JSONToken(){
            this->token = INVALID_TYPE;
            this->value = "";
        }
        JSONToken(JSONTokenType tokenType, std::string value){
            this->token = tokenType;
            this->value = value;
        }


    };
}

#endif //FILECONVERTER_JSONTOKEN_H

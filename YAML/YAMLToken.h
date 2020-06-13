//
// Created by Oskar on 22.05.2020.
//

#ifndef FILECONVERTER_YAMLTOKEN_H
#define FILECONVERTER_YAMLTOKEN_H

#include <string>
#include <memory>

namespace YAML {

    enum YAMLTokenValue {
        MAPPER_TOKEN,
        QUOTED_SCALAR_TOKEN,
        PLAIN_SCALAR_TOKEN,
        LITERAL_BLOCK_TOKEN,
        FOLDED_BLOCK_TOKEN,
        HYPHEN_TOKEN,
        INDENT_TOKEN,
        TRUE_TOKEN,
        FALSE_TOKEN,
        YNULL_TOKEN,
        COMMENT_TOKEN,
        EOL_TOKEN,
        INVALID_TYPE_TOKEN
    };

    enum YAMLState {
        START,
        END,
        EOL,
        DOUBLE_QUOTED_SCALAR_START,
        SINGLE_QUOTED_SCALAR_START,
        PLAIN_SCALAR_START,
        PLAIN,
        QUOTED,
        SPACE,
        HASH,
        HYPHEN,
        MAPPER,
        LITERAL_BLOCK_SCALAR_START,
        FOLDED_BLOCK_SCALAR_START,
        INVALID
    };

    class YAMLToken {

    public:
        YAMLTokenValue token;
        std::string value;

        YAMLToken() {
            this->token = INVALID_TYPE_TOKEN;
            this->value = "";
        }

        YAMLToken(YAMLTokenValue token, const std::string &value) : token(token), value(value) {}


    };
}

#endif //FILECONVERTER_YAMLTOKEN_H

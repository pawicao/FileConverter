//
// Created by Oskar on 22.05.2020.
//

#ifndef FILECONVERTER_YAML_TOKENS_H
#define FILECONVERTER_YAML_TOKENS_H

#include <string>
#include <memory>

namespace YAML {
    enum YAMLTokenValue {
        // TODO: Add Values
    };

    enum YAMLState {
        // TODO: Add States
    };

    class YAMLToken {

    public:
        YAMLTokenValue token;
        std::string value;

        YAMLToken() {
            // TODO: Add this->token (add empty)
            this->value = "";
        }

        YAMLToken(YAMLTokenValue token, const std::string &value) : token(token), value(value) {}


    };
}

#endif //FILECONVERTER_YAML_TOKENS_H

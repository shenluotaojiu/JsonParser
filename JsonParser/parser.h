#pragma once
#include<iostream>
#include<string>
#include"Json.h"

namespace sltj {
	namespace json {

        class Parser {
        public:
            Parser();
            ~Parser() = default;

            void load(const std::string& str);
            Json parse();
            void skip_white_space();
            char get_next_token();

        private:
            Json parse_null();
            Json parse_bool();
            Json parse_number();
            Json parse_array();
            Json parse_object();
            std::string parse_string();
        private:
            std::string m_str;
            int m_idx;
        };
	}
}
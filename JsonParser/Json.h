#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<stdexcept>
#include<sstream>

namespace sltj
{
    namespace json
    {
        class Json
        {
        public:
            enum Type
            {
                json_null = 0,
                json_bool,
                json_int,
                json_double,
                json_string,
                json_array,
                json_object
            };

            Json();
            ~Json();
            Json(bool val);
            Json(int val);
            Json(double val);
            Json(const char* val);
            Json(std::string& val);
            Json(Type type);
            Json(const Json& other);
            Json(Json& other);

            operator bool();
            operator int();  
            operator std::string();
            operator double();
            
            Json& operator[] (int index);
            Json& operator[] (const char* index);
            void operator= (const Json& other);
            bool operator== (const Json& other);
            bool operator!= (const Json& other);    

            void append(const Json& other);
            void deepcopy(const Json& other);
            void clear();
            std::string str() const;
            bool isNull() const { return m_type == json_null; }
            bool isBool() const { return m_type == json_bool; }
            bool isInt()const { return m_type == json_int; }
            bool isDouble()const { return m_type == json_double; }
            bool isString()const { return m_type == json_string; }
            bool isArray()const { return m_type == json_array; }
            bool isObject()const { return m_type == json_object; }
            void remove(int index);
            void remove(const char* str);
            void parser(const std::string& str);
            
            typedef std::vector<Json>::iterator iterator;
            iterator begin() {
                return m_value.m_vector->begin();
            }
            iterator end() {
                return m_value.m_vector->end();
            }

        private:
            union Value
            {
                bool m_bool;
                int m_int;
                double m_double;
                std::string* m_string;
                std::vector<Json>* m_vector;
                std::unordered_map<std::string, Json>* m_object;
            };
            Type m_type;
            Value m_value;
        };
    }
}
#include "json.h"
#include "parser.h"

using namespace sltj::json;

Json::Json() : m_type(json_null)
{
}
sltj::json::Json::~Json()
{
    clear();
}
Json::Json(bool val) : m_type(json_bool)
{
    m_value.m_bool = val;
}
Json::Json(int val) : m_type(json_int)
{
    m_value.m_int = val;
}
Json::Json(double val) : m_type(json_double)
{
    m_value.m_double = val;
}
Json::Json(const char* val) : m_type(json_string)
{
    m_value.m_string = new std::string(val);
}
Json::Json(std::string& val) : m_type(json_string)
{
    m_value.m_string = new std::string(val);
}
Json::Json(Type type) : m_type(type)
{
    switch (m_type)
    {
    case json_null:
        break;
    case json_array:
        m_value.m_vector = new std::vector<Json>();
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_object:
        m_value.m_object = new std::unordered_map<std::string, Json>();
        break;
    case json_string:
        m_value.m_string = new std::string("");
        break;
    default:
        break;
    }
}

Json::Json(Json& other)
{
    deepcopy(other);
}

Json::Json(const Json& other)
{
    deepcopy(other);
}

sltj::json::Json::operator bool()
{
    if (m_type != json_bool)
    {
        throw new std::logic_error("type error,not bool value");
    }
    return m_value.m_bool;
}

sltj::json::Json::operator int()
{
    if (m_type != json_int)
    {
        throw new std::logic_error("type error,not bool value");
    }
    return m_value.m_int;
}

sltj::json::Json::operator std::string()
{
    if (m_type != json_string)
    {
        throw new std::logic_error("type error,not bool value");
    }
    return *(m_value.m_string);
}

sltj::json::Json::operator double()
{
    if (m_type != json_double)
    {
        throw new std::logic_error("type error,not bool value");
    }
    return m_value.m_double;
}

Json& sltj::json::Json::operator[](int index)
{
    if (m_type != json_array) {
        m_type = json_array;
        m_value.m_vector = new std::vector<Json>();
    }
    int size = m_value.m_vector->size();
    if (index < 0) {
        throw new std::logic_error("非法越界");
    }
    if (index >= size)
    {
        for (int i = index+1; i != size; i--) {
            m_value.m_vector->push_back(*(new Json()));
        }
    }
    return m_value.m_vector->at(index);
}

Json& sltj::json::Json::operator[](const char* index)
{
    if (m_type != json_object) {
        clear();
        m_type = json_object;
        m_value.m_object = new std::unordered_map<std::string, Json>();
    }
    return (*m_value.m_object)[index];
}

void sltj::json::Json::operator=(const Json& other)
{
    clear();
    deepcopy(other);
}

bool sltj::json::Json::operator==(const Json& other)
{
    if (this->m_type != other.m_type)
        return false;
    switch (m_type)
    {
    case json_null:
        return true;
        break;
    case json_bool:
        return this->m_value.m_bool == other.m_value.m_bool;
        break;
    case json_double:
        return this->m_value.m_double == other.m_value.m_double;
        break;
    case json_int:
        return this->m_value.m_int == other.m_value.m_int;
        break;
    case json_array:
        /*if (m_value.m_vector->size() != other.m_value.m_vector->size())
            return false;
        for (int i = 0; i < m_value.m_vector->size(); i++) {
            if (this->m_value.m_vector[i] != other.m_value.m_vector[i])
                return false;
        }*/
        return m_value.m_vector == other.m_value.m_vector;
        break;
    case json_object:
        return m_value.m_object == other.m_value.m_object;
        break;
    case json_string:
        return *m_value.m_string == *other.m_value.m_string;
        break;
    default:
        break;
    }
    return false;
}

bool sltj::json::Json::operator!=(const Json& other)
{
    return !(*this == other);
}

//添加对象
void sltj::json::Json::append(const Json& other)
{
    if (m_type != json_array) {
        m_type = json_array;
        m_value.m_vector = new std::vector<Json>();
    }
    m_value.m_vector->push_back(other);
}

void sltj::json::Json::deepcopy(const Json& other)
{
    m_type = other.m_type;
    switch (m_type)
    {
    case json_null:
        break;
    case json_array:
        m_value.m_vector = new std::vector<Json>(*other.m_value.m_vector);
        break;
    case json_bool:
        m_value.m_bool = other.m_value.m_bool;
        break;
    case json_double:
        m_value.m_double = other.m_value.m_double;
        break;
    case json_int:
        m_value.m_int = other.m_value.m_int;
        break;
    case json_object:
        m_value.m_object = new std::unordered_map<std::string, Json>(*other.m_value.m_object);
        break;
    case json_string:
        m_value.m_string = new std::string(*other.m_value.m_string);
        break;
    default:
        break;
    }
}

void sltj::json::Json::clear()
{
    switch (m_type)
    {
    case json_null:
        break;
    case json_array:
        for (auto ite : *m_value.m_vector)
            ite.clear();
        delete m_value.m_vector;
        m_value.m_vector = nullptr;
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_object:
        for (auto ite : *m_value.m_object)
            ite.second.clear();
        delete m_value.m_object;
        m_value.m_object = nullptr;
        break;
    case json_string:
        delete m_value.m_string;
        m_value.m_string = nullptr;
        break;
    default:
        break;
    }
    m_type = json_null;
}

//生成Json语句
std::string sltj::json::Json::str() const
{
    std::stringstream ss;
    switch (this->m_type)
    {
    case json_null:
        ss << "null";
        break;
    case json_bool:
        ss << (m_value.m_bool ? "true" : "false");
        break;
    case json_int:
        ss << m_value.m_int;
        break;
    case json_double:
        ss << m_value.m_double;
        break;
    case json_string:
        ss << "\"" << *(m_value.m_string) << "\"";
        break;
    case json_array:
        ss << "[";
        for (auto i = m_value.m_vector->begin(); i != m_value.m_vector->end();i++) {
            if (i != (m_value.m_vector)->begin())
                ss << ",";
            ss << i->str();
        }
        ss << "]";
        break;
    case json_object:
        ss << "{";
        for (auto i = m_value.m_object->begin(); i != m_value.m_object->end(); i++) {
            if (i != (m_value.m_object)->begin())
                ss << ",";
            ss << "\"" << i->first << "\"" << ":" << i->second.str();
        }
        ss << "}";
        break;
    default:
        break;
    }
    return ss.str();
}

void sltj::json::Json::parser(const std::string& str)
{
    Parser p;
    p.load(str);
    *this = p.parse();
}



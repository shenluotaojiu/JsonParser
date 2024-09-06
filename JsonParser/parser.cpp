#include "parser.h"
using namespace sltj::json;

sltj::json::Parser::Parser()
{
	m_idx = 0;
	m_str = "";
}

void sltj::json::Parser::load(const std::string& str)
{
	m_idx = 0;
	m_str = str;
}

Json sltj::json::Parser::parse()
{
	char ch = get_next_token();
	switch (ch)
	{
	case 'n':
		m_idx--;
		return parse_null();
		break;
	case 't':
	case 'f':
		m_idx--;
		return parse_bool();
		break;
	case '-':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		m_idx--;
		return parse_number();
		break;
	case '"':
		return Json(parse_string().c_str());
		break; 
	case '[':
		return parse_array();
		break;
	case '{':
		return parse_object();
		break;
	default:
		break;
	}
	throw new std::logic_error("");
}

void sltj::json::Parser::skip_white_space()
{
	while (m_str[m_idx] == ' '
		|| m_str[m_idx] == '\n'
		|| m_str[m_idx] == '\t'
		|| m_str[m_idx] == '\r')
		m_idx++;
}

char sltj::json::Parser::get_next_token()
{
	skip_white_space();
	return m_str[m_idx++];
}

Json sltj::json::Parser::parse_null()
{
	if (m_str.compare(m_idx, 4, "null") == 0) {
		m_idx += 4;
		return Json();
	}
	throw new std::logic_error("Json文本格式错误");
	return Json();
}

Json sltj::json::Parser::parse_bool()
{
	if (m_str.compare(m_idx, 4, "true") == 0) {
		m_idx += 4;
		return Json(true);
	}
	else if(m_str.compare(m_idx,5,"false")== 0){
		m_idx += 5;
		return Json(false);
	}
	throw new std::logic_error("Json文本格式错误");
}

Json sltj::json::Parser::parse_number()
{
	int pos = m_idx;
	if (m_str[m_idx] == '-') {
		m_idx++;
	}
	if (m_str[m_idx] < '0' || m_str[m_idx] > '9') {
		throw "error";
	}
	while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9') {
		m_idx++;
	}
	if (m_str[m_idx] != '.') {
		int i = std::atoi(m_str.c_str() + pos);
		return Json(i);
	}
	m_idx++;
	if (m_str[m_idx] < '0' || m_str[m_idx] > '9') {
		throw "error";
	}
	while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9') {
		m_idx++;
	}
	double f = std::atof(m_str.c_str() + pos);
	return Json(f);
}

Json sltj::json::Parser::parse_array()
{
	Json arr(Json::json_array);
	char ch = get_next_token();
	m_idx--;
	if (ch == ']')
		return arr;
	while (true) {
		arr.append(parse());
		ch = get_next_token();
		//m_idx--;
		if (ch == ']')
			break;
		if (ch != ',')
			throw "error";
	}
	return arr;
}

Json sltj::json::Parser::parse_object()
{
	Json obj(Json::json_object);
	char ch = get_next_token();
	m_idx--;
	if (ch == '}') 
		return obj;
	while (true) {
		ch = get_next_token();
		if (ch != '"')
			throw "error";
		std::string key = parse_string();
		ch = get_next_token();
		if (ch != ':')
			throw __LINE__;
		obj[key.c_str()] = parse();
		ch = get_next_token();
		if (ch == '}')
			break;
		if (ch != ',')
			throw __LINE__;
	}
	
}

std::string sltj::json::Parser::parse_string()
{
	std::string out;
	while (true) {
		char ch = get_next_token();
		if (ch == '"') {
			break;
		}
		if (ch == '\\') {
			ch = m_str[++m_idx];
			switch (ch)
			{
			case '\n':
				out += '\n';
				break;
			case '\r':
				out += '\r';
				break;
			case '\t':
				out += '\t';
				break;
			case '\b':
				out += '\b';
				break;
			case '\f':
				out += '\f';
				break;
			case '\\':
				out += "\\\\";
				break;
			case '"':
				out += "\\\"";
				break;
			case '\\u':
				out += "\\u";
				for (int i = 0; i < 4; i++) {
					out += m_str[++m_idx];
				}
			default:
				break;
			}
		}
		else {
			out += ch;
		}
	}
	return out;
}

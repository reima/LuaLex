#include "../include/LuaLex.hpp"

#include <boost/assign/list_of.hpp>
#include <fstream>
#include <map>

std::map<LuaLex::TokenType, std::string> TypeToString =
  boost::assign::map_list_of
    (LuaLex::AND,           "AND")
    (LuaLex::BREAK,         "BREAK")
    (LuaLex::COMMENT,       "COMMENT")
    (LuaLex::DO,            "DO")
    (LuaLex::ELSE,          "ELSE")
    (LuaLex::ELSEIF,        "ELSEIF")
    (LuaLex::END,           "END")
    (LuaLex::FALSE,         "FALSE")
    (LuaLex::FOR,           "FOR")
    (LuaLex::FUNCTION,      "FUNCTION")
    (LuaLex::IF,            "IF")
    (LuaLex::IN,            "IN")
    (LuaLex::LOCAL,         "LOCAL")
    (LuaLex::NIL,           "NIL")
    (LuaLex::NOT,           "NOT")
    (LuaLex::OR,            "OR")
    (LuaLex::REPEAT,        "REPEAT")
    (LuaLex::RETURN,        "RETURN")
    (LuaLex::THEN,          "THEN")
    (LuaLex::TRUE,          "TRUE")
    (LuaLex::UNTIL,         "UNTIL")
    (LuaLex::WHILE,         "WHILE")
    (LuaLex::NUMBER,        "NUMBER")
    (LuaLex::STRING,        "STRING")
    (LuaLex::EQ_OP,         "EQ_OP")
    (LuaLex::NE_OP,         "NE_OP")
    (LuaLex::LE_OP,         "LE_OP")
    (LuaLex::GE_OP,         "GE_OP")
    (LuaLex::THREE_DOTS,    "THREE_DOTS")
    (LuaLex::TWO_DOTS,      "TWO_DOTS")
    (LuaLex::PLUS,          "PLUS")
    (LuaLex::DASH,          "DASH")
    (LuaLex::STAR,          "STAR")
    (LuaLex::SLASH,         "SLASH")
    (LuaLex::PERCENT,       "PERCENT")
    (LuaLex::CARET,         "CARET")
    (LuaLex::HASH,          "HASH")
    (LuaLex::LEFT_ANGLE,    "LEFT_ANGLE")
    (LuaLex::RIGHT_ANGLE,   "RIGHT_ANGLE")
    (LuaLex::EQUAL,         "EQUAL")
    (LuaLex::LEFT_PAREN,    "LEFT_PAREN")
    (LuaLex::RIGHT_PAREN,   "RIGHT_PAREN")
    (LuaLex::LEFT_BRACE,    "LEFT_BRACE")
    (LuaLex::RIGHT_BRACE,   "RIGHT_BRACE")
    (LuaLex::LEFT_BRACKET,  "LEFT_BRACKET")
    (LuaLex::RIGHT_BRACKET, "RIGHT_BRACKET")
    (LuaLex::SEMICOLON,     "SEMICOLON")
    (LuaLex::COLON,         "COLON")
    (LuaLex::COMMA,         "COMMA")
    (LuaLex::DOT,           "DOT")
    (LuaLex::IDENTIFIER,    "IDENTIFIER")
    ;

std::map<LuaLex::TokenCategory, std::string> CategoryToString =
  boost::assign::map_list_of
    (LuaLex::KeywordCategory,    "Keyword")
    (LuaLex::NumberCategory,     "Number")
    (LuaLex::StringCategory,     "String")
    (LuaLex::IdentifierCategory, "Identifier")
    (LuaLex::DelimiterCategory,  "Delimiter")
    ;

std::string ReadFile(const char* path) {
  std::ifstream ifs(path);
  if (!ifs.is_open()) {
    std::cerr << "File not readable: " << path << std::endl;
    exit(0);
  }
  ifs.unsetf(std::ios::skipws);
  return std::string(std::istreambuf_iterator<char>(ifs.rdbuf()),
                     std::istreambuf_iterator<char>());
}

int main(int argc, char* argv[]) {
  std::string str = ReadFile(argc > 1 ? argv[1] : "hello.lua");

  LuaLex::Lexer lexer;
  std::string::const_iterator beg = str.cbegin();
  std::string::const_iterator end = str.cend();
  for (auto it = lexer.begin(beg, end); it != lexer.end(); ++it) {
    std::cout << CategoryToString[it->category]
              << " (" << TypeToString[it->type] << ")"
              << ": `" << it->range << "'"
              << std::endl;
  }

  if (beg != end) {
    std::cerr << "Lexing stopped prematurely at `"
              << std::string(beg, end) << "'" << std::endl;
  }

  return 0;
}

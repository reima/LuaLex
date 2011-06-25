#include <boost/iterator/iterator_facade.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/scoped_ptr.hpp>

#include <string>

namespace LuaLex {

enum TokenType {
  AND,
  BREAK,
  COMMENT,
  DO,
  ELSE,
  ELSEIF,
  END,
  FALSE,
  FOR,
  FUNCTION,
  IF,
  IN,
  LOCAL,
  NIL,
  NOT,
  OR,
  REPEAT,
  RETURN,
  THEN,
  TRUE,
  UNTIL,
  WHILE,

  NUMBER,
  STRING,

  EQ_OP,
  NE_OP,
  LE_OP,
  GE_OP,

  THREE_DOTS,
  TWO_DOTS,

  PLUS,
  DASH,
  STAR,
  SLASH,
  PERCENT,
  CARET,  
  HASH,
  LEFT_ANGLE,
  RIGHT_ANGLE,
  EQUAL,
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  LEFT_BRACKET,
  RIGHT_BRACKET,
  SEMICOLON,
  COLON,
  COMMA,
  DOT,

  IDENTIFIER
};

enum TokenCategory {
  KeywordCategory,
  NumberCategory,
  StringCategory,
  IdentifierCategory,
  DelimiterCategory
};

struct Token {
  typedef boost::iterator_range<std::string::const_iterator> range_type;

  TokenType type;
  TokenCategory category;
  range_type range;
};

class LexerIterator
    : public boost::iterator_facade<LexerIterator,
                                    const Token,
                                    std::forward_iterator_tag> {
 public:
  LexerIterator();
  LexerIterator(const LexerIterator& other);
  ~LexerIterator();
  LexerIterator& operator=(const LexerIterator& other);  

 private:
  friend class boost::iterator_core_access;
  friend class Lexer;

  const Token& dereference() const;
  bool equal(const LexerIterator& other) const;
  void increment();

  void UpdateToken();

  struct Pimpl;
  boost::scoped_ptr<Pimpl> impl_;
};

class Lexer : public boost::noncopyable {
 public:
  typedef LexerIterator iterator_type;

  Lexer();
  ~Lexer();

  iterator_type begin(std::string::const_iterator& first,
                      std::string::const_iterator last);
  iterator_type end();

 private:
  struct Pimpl;
  boost::scoped_ptr<Pimpl> impl_;
};

} // namespace LuaLex

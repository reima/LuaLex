#include "LuaLex.hpp"

#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/bind.hpp>

//#include <boost/spirit/include/phoenix.hpp>

namespace LuaLex {
namespace detail {

namespace lex = boost::spirit::lex;

template<typename Lexer>
struct LuaLexer : lex::lexer<Lexer> {
  LuaLexer() {
    this->self += lex::token_def<>("[ \\t\\n\\r]+")
      [lex::_pass = lex::ignore()];

    this->self.add
      // TODO: skip whitespace
      ("--[^\\n]*"  ,       COMMENT)
      // TODO: multi line comments
                       
      ("and"        ,           AND)
      ("break"      ,         BREAK)
      ("do"         ,            DO)
      ("else"       ,          ELSE)
      ("elseif"     ,        ELSEIF)
      ("end"        ,           END)
      ("false"      ,         FALSE)
      ("for"        ,           FOR)
      ("function"   ,      FUNCTION)
      ("if"         ,            IF)
      ("in"         ,            IN)
      ("local"      ,         LOCAL)
      ("nil"        ,           NIL)
      ("not"        ,           NOT)
      ("or"         ,            OR)
      ("repeat"     ,        REPEAT)    
      ("return"     ,        RETURN)
      ("then"       ,          THEN)
      ("true"       ,          TRUE)
      ("until"      ,         UNTIL)      
      ("while"      ,         WHILE)

      ("[0-9]+\\.[0-9]*([eE][+-]?[0-9]+)?"
                    ,        NUMBER)
      ("[1-9][0-9]*",        NUMBER)      
      ("0[xX][0-9a-fA-F]+",  NUMBER)
      ('0'          ,        NUMBER)

      ("\\\"(\\\\.|[^\"])*\\\""
                    ,        STRING)
      ("'(\\\\.|[^'])*'"
                    ,        STRING)

      ("=="         ,         EQ_OP)
      ("~="         ,         NE_OP)
      ("<="         ,         LE_OP)
      (">="         ,         GE_OP)
      ("\\.\\.\\."  ,    THREE_DOTS)
      ("\\.\\."     ,      TWO_DOTS)
      ('+'          ,          PLUS)
      ('-'          ,          DASH)
      ('*'          ,          STAR)
      ('/'          ,         SLASH)
      ('%'          ,       PERCENT)
      ('^'          ,         CARET)
      ('#'          ,          HASH)
      ('<'          ,    LEFT_ANGLE)
      ('>'          ,   RIGHT_ANGLE)
      ('='          ,         EQUAL)
      ('('          ,    LEFT_PAREN)
      (')'          ,   RIGHT_PAREN)
      ('{'          ,    LEFT_BRACE)
      ('}'          ,   RIGHT_BRACE)
      ('['          ,  LEFT_BRACKET)
      (']'          , RIGHT_BRACKET)
      (';'          ,     SEMICOLON)
      (':'          ,         COLON)
      (','          ,         COMMA)
      ('.'          ,           DOT)

      ("[_a-zA-Z][_a-zA-Z0-9]*",
                         IDENTIFIER)
      ;
  }
};

typedef lex::lexertl::token<
  std::string::const_iterator,
  boost::mpl::vector0<>,
  boost::mpl::true_,
  TokenType
> token_type;
typedef lex::lexertl::actor_lexer<token_type> lexer_type;
typedef LuaLexer<lexer_type>::iterator_type iterator_type;

} // namespace LuaLex::detail

struct LexerIterator::Pimpl {
  Token token;
  detail::iterator_type iter;
};

LexerIterator::LexerIterator() : impl_(new Pimpl) {
}

LexerIterator::LexerIterator(const LexerIterator& other) : impl_(new Pimpl) {
  *impl_ = *other.impl_;
}

LexerIterator::~LexerIterator() {
}

LexerIterator& LexerIterator::operator=(const LexerIterator& other) {
  if (this != &other) {
    *impl_ = *other.impl_;
  }
  return *this;
}

const Token& LexerIterator::dereference() const {
  return impl_->token;
}

bool LexerIterator::equal(const LexerIterator& other) const {
  return impl_->iter == other.impl_->iter;
}

void LexerIterator::increment() {
  ++impl_->iter;
  UpdateToken();
}

void LexerIterator::UpdateToken() {
  if (token_is_valid(*impl_->iter)) {
    impl_->token.type = impl_->iter->id();
    if (impl_->token.type <= WHILE) {
      impl_->token.category = KeywordCategory;
    } else if (impl_->token.type == NUMBER) {
      impl_->token.category = NumberCategory;
    } else if (impl_->token.type == STRING) {
      impl_->token.category = StringCategory;
    } else if (impl_->token.type <= DOT) {
      impl_->token.category = DelimiterCategory;
    } else {
      impl_->token.category = IdentifierCategory;
    }
    impl_->token.range = impl_->iter->value();
  }
}

struct Lexer::Pimpl {
  detail::LuaLexer<detail::lexer_type> lexer_;
};

Lexer::Lexer() : impl_(new Pimpl) {
}

Lexer::~Lexer() {
}

Lexer::iterator_type Lexer::begin(std::string::const_iterator& first,
                                  std::string::const_iterator last) {
  Lexer::iterator_type it;
  it.impl_->iter = impl_->lexer_.begin(first, last);
  it.UpdateToken();
  return it;
}

Lexer::iterator_type Lexer::end() {
  Lexer::iterator_type it;
  it.impl_->iter = impl_->lexer_.end();
  return it;
}

} // namespace LuaLex

#pragma once

#include <string>
#include <vector>


enum class TokenType {
    _exit,
    INT_LIT,
    semi, open_paren, close_paren, ident, let, eq
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
}; 

class Tokenizer {
public:
    inline explicit Tokenizer(std::string src) : m_src(std::move(src)) {}

    std::vector<Token> tokenize()
    {
    std::vector<Token> tokens;
    std::string buf;

    while(peek().has_value()) {

        if (std::isalpha(peek().value())){
            buf.push_back(consume());
            while (peek().has_value() && std::isalnum(peek().value())){
                buf.push_back(consume());
            }
            if (buf == "exit"){
                tokens.push_back(Token{TokenType::_exit, buf});
                buf.clear();
                continue;
            } 
            else if (buf == "let") {
                tokens.push_back(Token{TokenType::let, buf});
                buf.clear();
                continue;
            }
            else {
                tokens.push_back(Token{TokenType::ident, buf});
                buf.clear();
                continue;
            }
       } 
       else if ( std::isdigit(peek().value()) ){

           buf.push_back(consume()) ;
           while (std::isdigit(peek().value())  && peek().has_value()){
               buf.push_back(consume());
           }
           tokens.push_back(Token{TokenType::INT_LIT, buf});
           buf.clear();
           continue;
       }
       else if (peek().value() == '(' ) {
        consume();
        tokens.push_back(Token{TokenType::open_paren});
        continue;
       } 
       else if (peek().value() == ')' ) {
        consume();
        tokens.push_back(Token{TokenType::close_paren});
        continue;
       }
       
        else if (peek().value() == '=') {
                consume();
                tokens.push_back({ TokenType::eq});
                continue;
            }

       else if (peek().value() == ' '){
           consume();
           continue;
       }
        
       else if (peek().value() == ';') {
                consume();
                tokens.push_back({ TokenType::semi });
                continue;
            }

        else {
           std::cerr<<"Unexpected character ? "<<std::endl;
           exit(EXIT_FAILURE);
       }
    }
    return tokens;
}

    private:

    [[nodiscard]] std::optional<char> peek(int offset = 0) const {
        if (m_index + offset >= m_src.length()) {
            return {};
        }
        return m_src.at(m_index + offset);
    }

    char consume() {
        return m_src.at(m_index++);
    }
    const std::string m_src;
    size_t m_index = 0;
};
#ifndef FRONTEND_INCLUDE_LEXER_HPP
#define FRONTEND_INCLUDE_LEXER_HPP

#include <vector>
#include <memory>
#include <string>
#include "token.hpp"
#include "common.hpp"
#include "lexem_table.hpp"
#include <fstream>
#include <iostream>

namespace language {

class Lexer {
private:
    std::vector<std::unique_ptr<Token>> tokens_{};
    std::string source_{};
    std::size_t i_{0};
    std::size_t source_size_{0};
public:
    explicit Lexer(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + filename);
        }
        source_ = std::string(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
        source_size_ = source_.size();
    }

    explicit Lexer(std::string&& source) : source_{std::move(source)}, source_size_{source_.size()} {}

    Lexer(const Lexer&) = delete;
    Lexer& operator=(const Lexer&) = delete;

    Lexer(Lexer&&) noexcept = default;
    Lexer& operator=(Lexer&&) noexcept = default;

    ~Lexer() = default;

    [[nodiscard]] const std::vector<std::unique_ptr<Token>>& 
    get_tokens() const noexcept { return tokens_; }

    void print_source() const {              //NOTE for debug
        std::cout << source_ << std::endl;
    }

    void print_tokens() const  {             //NOTE for debug
        for (std::size_t i = 0; i < tokens_.size(); ++i) {
            std::cout << static_cast<std::size_t>(tokens_[i].get()->type_) << '\n';
        }
    }

    void tokenize();
 
private:

    void skip_spaces();
    
    void find_tokens();
};

/*—————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
/*                                                                                                                 */
/*——————————————————————————————————————————| implementation of functions |————————————————————————————————————————*/
/*                                                                                                                 */
/*—————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

template<class Tok, class Enum>
void emit_(std::vector<std::unique_ptr<Token>>& tokens, Enum e, size_t& i) {
    tokens.push_back(std::make_unique<Tok>(e));
    ++i;
}


/*—————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
/*                                                                                                                 */
/*——————————————————————————————————————————| implementation of methods |——————————————————————————————————————————*/
/*                                                                                                                 */
/*—————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

inline void Lexer::skip_spaces() {
    while (i_ < source_.size()) {
        char c = source_[i_];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            ++i_;
        } else {
            break;
        }
    }
}

inline void Lexer::find_tokens() {
    skip_spaces();
    char c = source_[i_];
    
    Lexem lexem = lexem_table[c];
    switch (lexem.lexem_type) {
    case KBinop: {
        emit_<Token_binary_operator>(tokens_,
            static_cast<Binary_operators>(lexem.lexem_value), i_);
        break;
    }
    case KIdent: {
        emit_<Token_identifier>(tokens_,
            static_cast<Identifiers>(lexem.lexem_value), i_);
        break;
    }
    case KNum: {
        number_type num = static_cast<number_type>(c) - static_cast<number_type>(48);
        while (i_ < source_size_ && lexem_table[c = source_[++i_]].lexem_type == KNum) {
            num *= 10;
            num += static_cast<number_type>(c) - static_cast<number_type>(48);
        }
        tokens_.push_back(std::make_unique<Token_number>(num));
    }
    default:
        break; 
    }
}


inline void Lexer::tokenize() {
    while(i_ < source_size_) {
        find_tokens();
        //TODO check not primitive_tokens;

    }
}

} // namespace language

#endif // FRONTEND_INCLUDE_LEXER_HPP
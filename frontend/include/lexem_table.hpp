#ifndef FRONTEND_INCLUDE_LEXEM_TABLE_HPP
#define FRONTEND_INCLUDE_LEXEM_TABLE_HPP

#include <array>
#include <cstdint>
#include "common.hpp"

namespace language {

constexpr std::size_t n_lexems = 256;

enum : uint8_t { KNone = 0, KBinop = 1, KIdent = 2, KNum = 3 };

struct Lexem {
    uint8_t lexem_type{KNone};
    uint8_t lexem_value{0};
};

inline constexpr std::array<Lexem, n_lexems> lexem_table = []{
    std::array<Lexem,n_lexems> a{}; 
    a[static_cast<unsigned char>('+')] = {KBinop, static_cast<uint8_t>(Binary_operators::operator_add)};
    a[static_cast<unsigned char>('-')] = {KBinop, static_cast<uint8_t>(Binary_operators::operator_sub)};
    a[static_cast<unsigned char>('*')] = {KBinop, static_cast<uint8_t>(Binary_operators::operator_mul)};
    a[static_cast<unsigned char>('?')] = {KBinop, static_cast<uint8_t>(Binary_operators::operator_input)};
    a[static_cast<unsigned char>('/')] = {KBinop, static_cast<uint8_t>(Binary_operators::operator_div)};
    a[static_cast<unsigned char>('^')] = {KBinop, static_cast<uint8_t>(Binary_operators::operator_pow)};
    a[static_cast<unsigned char>('=')] = {KBinop, static_cast<uint8_t>(Binary_operators::operator_assign)};

    a[static_cast<unsigned char>(';')] = {KIdent, static_cast<uint8_t>(Identifiers::id_semicolon)};
    a[static_cast<unsigned char>('(')] = {KIdent, static_cast<uint8_t>(Identifiers::id_left_parenthesis)};
    a[static_cast<unsigned char>(')')] = {KIdent, static_cast<uint8_t>(Identifiers::id_right_parenthesis)};
    a[static_cast<unsigned char>('{')] = {KIdent, static_cast<uint8_t>(Identifiers::id_left_curly_bracket)};
    a[static_cast<unsigned char>('}')] = {KIdent, static_cast<uint8_t>(Identifiers::id_right_curly_bracket)};

    a[static_cast<unsigned char>('0')] = {KNum, static_cast<uint8_t>(0)};
    a[static_cast<unsigned char>('1')] = {KNum, static_cast<uint8_t>(0)};
    a[static_cast<unsigned char>('2')] = {KNum, static_cast<uint8_t>(0)};
    a[static_cast<unsigned char>('3')] = {KNum, static_cast<uint8_t>(0)};
    a[static_cast<unsigned char>('4')] = {KNum, static_cast<uint8_t>(0)};
    a[static_cast<unsigned char>('5')] = {KNum, static_cast<uint8_t>(0)};
    a[static_cast<unsigned char>('6')] = {KNum, static_cast<uint8_t>(0)};
    a[static_cast<unsigned char>('7')] = {KNum, static_cast<uint8_t>(0)};
    a[static_cast<unsigned char>('8')] = {KNum, static_cast<uint8_t>(0)};
    a[static_cast<unsigned char>('9')] = {KNum, static_cast<uint8_t>(0)};


    return a;
}();

} // namespace language

#endif // FRONTEND_INCLUDE_LEXEM_TABLE_HPP
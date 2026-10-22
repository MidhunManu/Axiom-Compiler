#pragma once

#include <cstddef>
#include <string>

enum class TokenKind
{
    Identifier,
    IntegerLiteral,
    KeywordLet,
    KeywordFn,
    KeywordReturn,
    Plus,
    Minus,
    Star,
    Slash,
    Equal,
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    Colon,
    Semicolon,
    Arrow,
    EndOfFile
};

struct SourceLocation
{
    std::size_t line;
    std::size_t column;
};

struct Token
{
    TokenKind kind;
    std::string lexeme;
    SourceLocation location;
};

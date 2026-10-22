#pragma once
#include <string>

class Lexer
{
public:
    explicit Lexer(std::string source);

    [[nodiscard]]
    std::optional<char> peek() const;

    std::optional<char> advance();

    [[nodiscard]]
    std::size_t line() const noexcept;

    [[nodiscard]]
    std::size_t column() const noexcept;

private:
    std::string m_source;
    std::size_t m_current = 0;
    std::size_t m_line = 1;
    std::size_t m_column = 1;
};

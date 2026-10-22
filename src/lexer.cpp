#include "lexer.hpp"

Lexer::Lexer(std::string source)
    :m_source(std::move(source))
{}

std::optional<char> Lexer::peek() const
{
    if (m_current >= m_source.size())
    {
        return std::nullopt;
    }

    return m_source[m_current];
}

std::optional<char> Lexer::advance()
{
	if (m_current >= m_source.size())
	{
		return std::nullopt;
	}

	char next = m_source[m_current++];
	if (next == '\n')
	{
		++line;
		m_current = 1;
	}
	else
	{
		++column;
	}

	return next;
}

std::size_t line() const noexcept
{
	return m_line;
}

std::size_t column() const noexcept
{
	return m_column;
}


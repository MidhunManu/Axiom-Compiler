#include "lexer.hpp"

Lexer::Lexer(std::string source)
	: m_source(std::move(source))
{
}

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
		++m_line;
		m_column = 1;
	}
	else
	{
		++m_column;
	}

	return next;
}

std::size_t Lexer::line() const noexcept
{
	return m_line;
}

std::size_t Lexer::column() const noexcept
{
	return m_column;
}

void Lexer::skipWhiteSpaces()
{
	while (const auto next = peek())
	{
		if (!std::isspace(static_cast<unsigned char>(*next)))
		{
			break;
		}

		advance();
	}
}

Token Lexer::nextToken()
{
	skipWhiteSpaces();
	const SourceLocation start{m_line, m_column};
	const auto next = peek();

	if (!next)
	{
		return {TokenKind::EndOfFile, "", start};
	}

	if (std::isalpha(static_cast<unsigned char>(*next)) || *next == '_')
	{
		std::string lexeme;
		while (const auto current = peek())
		{
			const auto ch = static_cast<unsigned char>(*current);
			if (!std::isalnum(*current) && *current != '_')
			{
				break;
			}

			lexeme += *advance();
		}

		const TokenKind kind = lexeme == "let"
								   ? TokenKind::KeywordLet
								   : TokenKind::Identifier;

		return {kind, std::move(lexeme), start};
	}

	if (std::isdigit(static_cast<unsigned char>(*next)))
	{
		std::string lexeme;
		while (const auto current = peek())
		{
			if (!std::isdigit(static_cast<unsigned char>(*current)))
			{
				break;
			}

			lexeme += *advance();
		}

		return {TokenKind::IntegerLiteral, std::move(lexeme), start};
	}

	advance();

	switch (*next)
	{
	case '=':
		if (peek() == '=')
		{
			advance();
			return {TokenKind::EqualEqual, "==", start};
		}
		return {TokenKind::Equal, "=", start};
	case ';':
		return {TokenKind::Semicolon, ";", start};
	case '!':
		if (peek() == '=')
		{
			advance();
			return {TokenKind::BangEqual, "!=", start};
		}
		return {TokenKind::Bang, "!", start};
	case '>':
		if (peek() == '=')
		{
			advance();
			return {TokenKind::GreaterEqual, ">=", start};
		}
		return {TokenKind::Greater, ">", start};
	case '<':
		if (peek() == '=')
		{
			advance();
			return {TokenKind::LessEqual, "<=", start};
		}
		return {TokenKind::Less, "<", start};
	case '-':
		if (peek() == '>')
		{
			advance();
			return {TokenKind::Arrow, "->", start};
		}
		return {TokenKind::Minus, "-", start};
	case '+':
		return {TokenKind::Plus, "+", start};
	case '*':
		return {TokenKind::Star, "*", start};
	case '/':
		return {TokenKind::Slash, "/", start};
	case ':':
		return {TokenKind::Colon, ":", start};
	case '(':
		return {TokenKind::LeftParen, "(", start};
	case ')':
		return {TokenKind::RightParen, ")", start};
	case '{':
		return {TokenKind::LeftBrace, "{", start};
	case '}':
		return {TokenKind::RightBrace, "}", start};
	default:
		throw std::runtime_error(
			"Unexpected character at line " +
			std::to_string(start.line) +
			", column " +
			std::to_string(start.column));
	}
}

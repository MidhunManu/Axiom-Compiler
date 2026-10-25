#include "../src/lexer.hpp"

#include <gtest/gtest.h>

TEST(LexerTest, PeekDoesNotConsume)
{
    Lexer lexer("abc\nx");

    EXPECT_EQ(lexer.peek(), 'a');
    EXPECT_EQ(lexer.peek(), 'a');
}

TEST(LexerTest, AdvanceReturnsCurrentAndMovesForward)
{
    Lexer lexer("abc\nx");

    EXPECT_EQ(lexer.advance(), 'a');
    EXPECT_EQ(lexer.peek(), 'b');
    EXPECT_EQ(lexer.column(), 2);

    EXPECT_EQ(lexer.advance(), 'b');
    EXPECT_EQ(lexer.advance(), 'c');
}

TEST(LexerTest, NewlineIncrementsLineAndResetsColumn)
{
    Lexer lexer("abc\nx");

    lexer.advance(); // a
    lexer.advance(); // b
    lexer.advance(); // c

    EXPECT_EQ(lexer.peek(), '\n');
    EXPECT_EQ(lexer.advance(), '\n');

    EXPECT_EQ(lexer.line(), 2);
    EXPECT_EQ(lexer.column(), 1);
    EXPECT_EQ(lexer.peek(), 'x');
}

TEST(LexerTest, PeekReturnsNothingAtEndOfInput)
{
    Lexer lexer("x");

    EXPECT_EQ(lexer.advance(), 'x');
    EXPECT_FALSE(lexer.peek().has_value());
}

TEST(LexerTest, RecognizesKeywordAndIdentifier)
{
    Lexer lexer("let count");

    const auto keyword = lexer.nextToken();
    EXPECT_EQ(keyword.kind, TokenKind::KeywordLet);
    EXPECT_EQ(keyword.lexeme, "let");

    const auto identifier = lexer.nextToken();
    EXPECT_EQ(identifier.kind, TokenKind::Identifier);
    EXPECT_EQ(identifier.lexeme, "count");
}

TEST(LexerTest, DoesNotSplitKeywordPrefixes)
{
    Lexer lexer("letter let2 _let");

    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Identifier);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Identifier);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Identifier);
}

TEST(LexerTest, RecognizesIntegerAndPunctuation)
{
    Lexer lexer("count = 42;");

    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Identifier);

    const auto equal = lexer.nextToken();
    EXPECT_EQ(equal.kind, TokenKind::Equal);
    EXPECT_EQ(equal.lexeme, "=");

    const auto number = lexer.nextToken();
    EXPECT_EQ(number.kind, TokenKind::IntegerLiteral);
    EXPECT_EQ(number.lexeme, "42");

    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Semicolon);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::EndOfFile);
}

TEST(LexerTest, SkipsWhitespaceAndTracksLocations)
{
    Lexer lexer("let\n  count");

    const auto keyword = lexer.nextToken();
    EXPECT_EQ(keyword.location.line, 1);
    EXPECT_EQ(keyword.location.column, 1);

    const auto identifier = lexer.nextToken();
    EXPECT_EQ(identifier.location.line, 2);
    EXPECT_EQ(identifier.location.column, 3);
}

TEST(LexerTest, WhitespaceOnlyInputReturnsEOF)
{
    Lexer lexer(" \n\t ");

    EXPECT_EQ(lexer.nextToken().kind, TokenKind::EndOfFile);
}

TEST(LexerTest, RecognizesMultiCharacterOperators)
{
    Lexer lexer("== != <= >= ->");

    EXPECT_EQ(lexer.nextToken().kind, TokenKind::EqualEqual);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::BangEqual);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::LessEqual);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::GreaterEqual);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Arrow);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::EndOfFile);
}

TEST(LexerTest, DistinguishesSingleAndDoubleCharacterOperators)
{
    Lexer lexer("= == < <= - ->");

    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Equal);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::EqualEqual);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Less);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::LessEqual);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Minus);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Arrow);
}

TEST(LexerTest, RecognizesArithmeticOperators)
{
    Lexer lexer("+ - * /");

    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Plus);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Minus);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Star);
    EXPECT_EQ(lexer.nextToken().kind, TokenKind::Slash);
}

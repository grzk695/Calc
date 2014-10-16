#pragma once

#include <regex>

namespace PR
{

	enum TOKEN_CLASS
	{
		NUMBER,
		ID,
		OPEN_PARENTHESIS,
		CLOSE_PARENTHESIS,
		OPERATOR,
		SEMICOLON,
		MATRIX_START,
		MATRIX_END,
		WHITE_SPACE,
		KEY_WORD,
		FUNCTION,
		QUOTATION_MARK,
		APOSTROPHE,
		STRING,
		UNKNOWN,
		NEW_LINE,
		SPACE,
		TABULATOR,
		COLON,
		COMMA,
		NONE,
		FUNCTON_ARGS_END,
		VERSE_END,
		MATRIX_ALL,
		OUTPUT_OFF,
		IF_KEYWORD,
		WHILE_KEYWORD,
		FOR_KEYWORD,
		CONTINUE_KEYWORD,
		BREAK_KEYWORD,
		END_IF,
		END_WHILE,
		END_FOR,
		END_KEYWORD,
		ELSE_KEYWORD,
		ASSIGNMENT
	};
}
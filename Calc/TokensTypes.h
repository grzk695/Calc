#pragma once

#include <regex>

namespace KLab
{
	enum TOKEN_CLASS
	{
		NUMBER,
		ID,
		LAST_INDEX_OF,
		OPEN_PARENTHESIS,
		CLOSE_PARENTHESIS,
		OPERATOR,
		SEMICOLON,
		MATRIX_START,
		MATRIX_END,
		KEY_WORD,
		FUNCTION,
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
		ELSEIF_KEYWORD,
		WHILE_KEYWORD,
		FOR_KEYWORD,
		CONTINUE_KEYWORD,
		RETURN_KEYWORD,
		BREAK_KEYWORD,
		END_IF,
		END_WHILE,
		END_FOR,
		END_KEYWORD,
		ELSE_KEYWORD,
		ASSIGNMENT,
		ASSIGNMENT_TARGET,
		FUNCTION_KEYWORD,
		END_FUNCTION,
		SHORT_CIRCUIT_END,
		SHORT_CIRCUIT_OR,
		FOR_DELETE,
		VARIABLES_MANAGEMENT
	};
}
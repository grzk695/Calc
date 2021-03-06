#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

#include "CalcException.h"
#include "Token.h"
#include "functions.h"

namespace KLab
{
	class LexicalBalanceHelper
	{
	public:
		LexicalBalanceHelper();
		~LexicalBalanceHelper();

		void reset();
		void setMode(Token &token);
		PARSE_MODE getMode()const;

		void throwOnUnbalancedEnd();
	private:

		vector<int> balance;
		vector<PARSE_MODE> mode;
		int pos;
		int line;
		vector<TOKEN_CLASS> key_word_mode;
		int key_word_balance;

		void changeMode(PARSE_MODE mode, bool start);
		void onParenthesis(bool start);
		void onInstructionKeyWord(Token &token);
		void onEndKeyword(Token &token);
		void onElseIf(Token &token);
		void onContBreakKeyWords();
		void onFunction();
		void onFunctionEnd();
	};
}

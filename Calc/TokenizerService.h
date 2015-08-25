#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

using std::unordered_map;
using std::unique_ptr;
using std::make_unique;
using std::string;
using std::vector;

#include "CalcException.h"
#include "export.h"
#include "functions.h"
#include "Token.h"
#include "SNumber.h"
#include "String.h"
#include "Operator.h"
#include "OperatorsFactory.h"
#include "NumberReader.h"
#include "TokenizerContext.h"

namespace KLab
{
	class TokenizerService {
	public:
		const static vector<TOKEN_CLASS> FOR_SPACE_DELETE;

	private:
		static const unordered_map<string, TOKEN_CLASS> KEYWORDS;
		static const unordered_map<char, TOKEN_CLASS> OTHERS;
		static const vector<string> END_SYNONIMS;
		vector<unique_ptr<Token>> tokens;
		int _line;
		int _position;

		TokenizerContext tokenizerContext;

	public:
		TokenizerService(const string &input);
		TokenizerService(string &&input);
		~TokenizerService();
		TokenizerService & operator = (TokenizerService &&b);
		void tokenize();
		vector<unique_ptr<Token>> getTokens();

	protected:
		char at(int index);
		bool EoI() const;

	private:

		void readNumber();
		void readWord();
		void readWhiteSpace();
		void readOthers();
		bool readOperator();
		void skipBlockComment();
		void skipLineComment();
		void readString();
		void whiteSpacesBegin();
		void whiteSpacesEnd();
		void deleteUneccessary();
		bool onDot();
		TOKEN_CLASS prev();
		char prevChar();
		void init();
		void onNewLine();
		void throwMessage(const string &message);
		void inc(int val = 1);
		void setLine();
	};
}

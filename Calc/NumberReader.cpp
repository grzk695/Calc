#include "stdafx.h"
#include "NumberReader.h"

namespace KLab
{
	string NumberReader::read(const string &input, int start_p)
	{
		NumberReader numberReader(input, start_p);
		numberReader.readDigits();
		numberReader.fractionalPart();
		numberReader.ePart();
		numberReader.readI();
		return numberReader.out;
	}

	NumberReader::NumberReader(const string &in, int position)
		:in(in), out(out), position(position)
	{
		out = "";
		N = in.size();
		exp = false;
	}

	NumberReader::~NumberReader()
	{
	}

	void NumberReader::readDigits()
	{
		while (position < N && TokenizerHelper::isDigit(in[position]))
			out += in[position++];
	}

	void NumberReader::fractionalPart()
	{
		if (position < N - 1 && in[position] == '.'&&TokenizerHelper::isDigit(in[position + 1]))
		{
			position++;
			out.append(".");
			readDigits();
			if (out.back() == '.')
				throw KLab::CalcException("Number read error!", position - 1);
		}
	}

	void NumberReader::ePart()
	{
		bool error = false;
		if (position < N && (in[position] == 'e' || in[position] == 'E'))
		{
			out += in[position++];
			if (position == N)
				error = true;
			if (in[position] == '-' || in[position] == '+')
				out += in[position++];
			readDigits();

			if (!TokenizerHelper::isDigit(out.back()) || error)
				throw KLab::CalcException("Number read error!", position - 1);

			exp = true;
		}
	}

	void NumberReader::readI()
	{
		if (position < N&&in[position] == 'i')
		{
			if (position < N - 1 && TokenizerHelper::isDigit(in[position + 1]))
				throw CalcException("Unexpected expression.", position + 1);
			out.append("i");
		}
	}
}
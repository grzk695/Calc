#include "stdafx.h"
#include "Operator.h"

namespace KLab
{
	Operator::Operator(const string &name, int priority, int arguments, EVAULATED ev)
		:
		priority(priority),
		argumentsNum(arguments),
		ev(ev)
	{
		setLexeme(name);
		set_class(TOKEN_CLASS::OPERATOR);
	}

	Operator::~Operator()
	{
	}

	bool Operator::operator<(const Operator &b) const
	{
		return priority > b.priority || ev == EVAULATED::RIGHT && priority == b.priority;
	}

	void Operator::setArguments(vector<shared_ptr<Data>> &stack)
	{
		if (stack.size() < argumentsNum)
			throw EvalException("Too few arguments for operator: " + getLexemeR());

		arguments.resize(argumentsNum);

		auto iter = stack.rbegin();
		int k = argumentsNum - 1;
		for (int i = 0; i < argumentsNum; i++)
		{
			arguments[i] = std::move(iter[k--]);
		}

		promoteArguments();

		stack.erase(stack.begin() + stack.size() - argumentsNum, stack.end());
	}

	void Operator::promoteArguments()
	{
		if (argumentsNum == 1 && arguments[0]->isOutput())
		{
			TypePromotor::promote(arguments[0], arguments[0]->max_type());
		}
		else if (argumentsNum == 2)
			TypePromotor::promote(arguments[0], arguments[1]);
		else if (argumentsNum > 2)
			TypePromotor::promote(arguments);
	}

	void Operator::promoteToMatrix()
	{
		if (argumentsNum == 1 && arguments[0]->isOutput())
		{
			TypePromotor::promote(arguments[0], arguments[0]->max_type());
		}
		else if (argumentsNum == 2)
			TypePromotor::promoteToMatrix(arguments[0], arguments[1]);
		else if (argumentsNum > 2)
			TypePromotor::promoteToMatrix(arguments);
	}

	void Operator::throwIfNot(const string &message, bool(*fun)(const shared_ptr<Data>&)) const
	{
		for_each(arguments.begin(), arguments.end(), [&](const shared_ptr<Data> &data){
			if (!fun(data))
				throw CalcException("Error in \"" + getLexemeR() + "\" operator: " + message);
		});
	}

	void Operator::clearArguments()
	{
		arguments.clear();
	}
}
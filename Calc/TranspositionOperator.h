#pragma once
#include "Operator.h"
namespace KLab
{
	class TranspositionOperator : public Operator
	{
	public:
		TranspositionOperator();
		~TranspositionOperator();
		virtual shared_ptr<Data> evaluate() override;
	};

	class ConjugateTranspositionOperator : public Operator
	{
	public:
		ConjugateTranspositionOperator();
		~ConjugateTranspositionOperator();
		virtual shared_ptr<Data> evaluate() override;
	};
}

#include "stdafx.h"
#include "Colon2Operator.h"

namespace KLab
{
	Colon2Operator::Colon2Operator()
		:Operator(":", 60, 2, EVAULATED::RIGHT)
	{
	}

	Colon2Operator::~Colon2Operator()
	{
	}
}
#include "stdafx.h"
#include "Function.h"

namespace KLab
{
	Function::Function()
	{
	}

	Function::~Function()
	{
	}

	shared_ptr<Data> Function::run()
	{
		throw CalcException("Unimplemented function behaviour!");
	}

	void Function::set(const vector<shared_ptr<Data>> &args, int output_nr)
	{
		checkArgsCount(args.size());
		arguments = args;
		TypePromotor::promote(arguments);
		output = output_nr;
	}

	void Function::checkArgsCount(int i)
	{
		if (min_args_count == max_args_count == -1)
			return;

		if (i<min_args_count || i>max_args_count)
			throw CalcException("Wrong number of parameters in function call '" + name + "'.");
	}
}
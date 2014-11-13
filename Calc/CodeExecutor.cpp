#include "stdafx.h"
#include "CodeExecutor.h"


namespace PR
{
	Variables CodeExecutor::globals = Variables();
	bool CodeExecutor::stop_computing = false;

	int CodeExecutor::recursions = 0;
	int CodeExecutor::recursion_limit = 300;

	CodeExecutor::CodeExecutor()
		:vars_ref(internal_vars)
	{
	}

	CodeExecutor::CodeExecutor(Variables &ref)
		:
		vars_ref(ref)
	{
	}

	CodeExecutor::CodeExecutor(const ExternalFunction &fun,const vector<shared_ptr<Data>> &args)
		:
		vars_ref(internal_vars),
		code(fun.getCode())
	{
		auto input = fun.getInput();
		int i = 0;
		int N = input.size();
		for (const shared_ptr<Data> &data : args)
		{
			if (i >= N)
				throw CalcException("Too many input parameters in function call (" + fun.getName() + ")");
			
			shared_ptr<Data> temp;
			if (data->isOutput())
			{
				temp.reset(data.get());
				TypePromotor::promote(temp, temp->max_type());
			}
			else
				temp = data;
			
			vars_ref.set(input[i++],data);
		}
	}

	CodeExecutor::~CodeExecutor()
	{
	}

	void CodeExecutor::start()
	{
		output_off_flag = false;
		while (!code.eof())
		{
			if (CodeExecutor::stop_computing)
				throw CalcException("Canceled!");

			ip = code.get();
			if (checkIF())
				continue;
			if (checkWhile())
				continue;
			if (checkFor())
				continue;
			run();
			code.inc();
		}
	}

	shared_ptr<Data> CodeExecutor::run()
	{
		stack.clear();
		assignment.clear();
		for (i = ip->begin(); i != ip->end(); i++)
		{
			switch ((*i)->getClass())
			{
			case TOKEN_CLASS::NUMBER:
				stack.push_back(shared_ptr<Data>((*i)->evaluate()));
				break;
			case TOKEN_CLASS::OPERATOR:
				onOperator();
				break;
			case TOKEN_CLASS::MATRIX_START:
				stack.push_back(make_shared<Token>(MATRIX_START));
				break;
			case TOKEN_CLASS::VERSE_END:
				stack.push_back(make_shared<Token>(VERSE_END));
				break;
			case TOKEN_CLASS::MATRIX_END:
				onMatrixEnd();
				break;
			case TOKEN_CLASS::FUNCTON_ARGS_END:
				onFunctionArgs();
				break;
			case TOKEN_CLASS::FUNCTION:
				onFunction();
				break;
			case TOKEN_CLASS::ASSIGNMENT_TARGET:
				stack.push_back(*i);
				break;
			case TOKEN_CLASS::ASSIGNMENT:
				onAssignment();
				break;
			case TOKEN_CLASS::ID:
				onID();
				break;
			case TOKEN_CLASS::OUTPUT_OFF:
				output_off_flag = true;
				break;
			case TOKEN_CLASS::MATRIX_ALL:
				onMatrixAll();
				break;
			default:
				throw CalcException("!");
			}
		}

		if (assignment_flag == false)
			defaultAssignment();
		if (!output_off_flag)
		{
			for (const auto & i : assignment)
			{
				SignalEmitter::get()->call(i.first->first, i.first->second);
			}
		}

		output_off_flag = false;
		assignment_flag = false;
		if (stack.size())
			return stack.back();
		else
			return make_shared<Data>();
	}

	vector<shared_ptr<Data>> CodeExecutor::run_single(const vector<shared_ptr<Token>> &onp,Variables &vars)
	{
		CodeExecutor exec(vars);
		exec.assignment_flag = true;
		exec.output_off_flag = true;
		vector<vector<shared_ptr<Token>>> code = { onp };
		exec.ip = code.begin();
		exec.run();
		return exec.stack;
	}

	void CodeExecutor::onOperator()
	{
		auto p = dynamic_cast<Operator *>(i->get());
		p->setArguments(stack);
 		stack.push_back(shared_ptr<Data>(p->evaluate()));
	}

	bool CodeExecutor::isKeyword(TOKEN_CLASS _class)
	{
		return ip->size() == 1 && ip->at(0)->getClass() == _class;
	}

	vector<shared_ptr<Data>>::iterator CodeExecutor::find(TOKEN_CLASS _class, bool ex)
	{
		for (auto ii = stack.end() - 1; ii >= stack.begin(); ii--)
		{
			if ((*ii)->_type == TYPE::TOKEN && dynamic_cast<Token *>(ii->get())->getClass() == _class)
				return ii;
		}

		if (ex)
			throw CalcException("Expected token not found!");
		else
			return stack.end();
	}

	void CodeExecutor::setIPTo(const vector<TOKEN_CLASS> &set, int balance)
	{
		if (set.size() == 0)
			return;
		while (1)
		{
			if (ip->size() == 1 && balance == ip->at(0)->getKeywordBalance() &&
				std::find(set.begin(), set.end(), ip->at(0)->getClass()) != set.end())
				return;
			next();
		}
	}
}

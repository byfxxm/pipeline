#include "pch.h"
#include "semantic.h"
#include "syntax.h"
#include "../pipeline/part.h"

semantic::semantic(std::ifstream& fin) : __fin(fin) {}

std::shared_ptr<part> semantic::next_code()
{
	syntax syntax_(__fin);

	for (auto sen = syntax_.next_sentence(); sen.has_value(); sen = syntax_.next_sentence())
	{
		auto sent = sen.value();
		//sent.pred
	}

	return std::make_shared<part>();
}
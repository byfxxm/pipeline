#include "pch.h"
#include "semantic.h"
#include "syntax.h"
#include "../pipeline/part.h"

semantic_c::semantic_c(std::ifstream& fin) : __fin(fin) {}

std::shared_ptr<part> semantic_c::next_code()
{
	syntax_c syntax_c_(__fin);

	for (auto sen = syntax_c_.next_sentence_s(); sen.has_value(); sen = syntax_c_.next_sentence_s())
	{
		auto sent = sen.value();
		//sent.pred
	}

	return std::make_shared<part>();
}
#include "pch.h"
#include "procedure1.h"
#include "../pipeline/pipeline.h"

struct code_s : public part_s
{
	int index{ 0 };
};

void procedure1(utilities_s* utils)
{
	for (int i = 0; i < 10000; i++)
	{
		auto p = std::make_shared<code_s>();
		p->index = i;
		utils->write(p);
	}
}
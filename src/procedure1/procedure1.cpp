#include "pch.h"
#include "procedure1.h"
#include "../pipeline/pipeline.h"

struct code : public part
{
	int index{ 0 };
};

void procedure1(utility* util)
{
	for (int i = 0; i < 10000; i++)
	{
		auto p = new code();
		p->index = i;
		util->write(p);
	}
}
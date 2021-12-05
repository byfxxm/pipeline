#include "pch.h"
#include "semantic.h"
#include "../pipeline/part.h"

semantic::semantic(std::ifstream& fin) : __fin(fin) {}

part* semantic::next_code()
{
	return new part();
}
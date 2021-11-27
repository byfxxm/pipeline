#pragma once
#include <future>

enum class tag_t
{
	TAG_NORMAL,
	TAG_SYN,
};

struct part
{
	part() = default;
	part(tag_t tag_) : tag(tag_) {}
	tag_t tag{ tag_t::TAG_NORMAL };
};

struct part_syn : public part
{
	part_syn(std::promise<void>* pr) : part(tag_t::TAG_SYN), prom(pr) {}
	std::promise<void>* prom{ nullptr };
};
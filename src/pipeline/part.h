#pragma once
#include <future>

enum class tag_t
{
	TAG_NORMAL,
	TAG_SYN,
};

struct part
{
	constexpr part() = default;
	constexpr part(tag_t tag_) : tag(tag_) {}
	tag_t tag{ tag_t::TAG_NORMAL };
};

struct part_syn : public part
{
	part_syn() : part(tag_t::TAG_SYN) {}
	std::promise<void> prom;
};
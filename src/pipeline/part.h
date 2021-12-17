#pragma once
#include <future>

enum class tag_t
{
	NORMAL,
	SYN,
};

struct part_s
{
	constexpr part_s() = default;
	constexpr part_s(tag_t tag_) : tag(tag_) {}
	tag_t tag{ tag_t::NORMAL };
};

struct part_syn_s : public part_s
{
	part_syn_s() : part_s(tag_t::SYN) {}
	std::promise<void> prom;
};
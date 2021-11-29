#pragma once
#include <future>

namespace pipeline
{
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
		part_syn() : part(tag_t::TAG_SYN) {}
		std::promise<void> prom;
	};
}
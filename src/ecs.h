#pragma once

#include <cstdint>

namespace Byte {

	struct EntityID {
		uint64_t index;
		uint64_t id{};

		operator uint32_t() const {
			return index;
		}
	};

	struct ComponentID {
		uint32_t index;

		operator uint32_t() const {
			return index;
		}
	};

}
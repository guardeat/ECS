#pragma once

#include <bitset>
#include <array>
#include <cstdint>

namespace Byte {

	template<typename ComponentID ,size_t _MAX_COMPONENT_COUNT>
	class Signature {
	public:
		inline static constexpr size_t MAX_COMPONENT_COUNT{ _MAX_COMPONENT_COUNT };
		inline static constexpr size_t BITSET_SIZE{ 64 };
		inline static constexpr size_t BITSET_COUNT{ MAX_COMPONENT_COUNT / BITSET_SIZE + 1 };

		using Bitset = std::bitset<BITSET_SIZE>;
		using BitsetArray = std::array<Bitset, BITSET_COUNT>;

	private:
		BitsetArray bitsets;

	public:
		void set(ComponentID id, bool value = true) {
			bitsets[id / BITSET_SIZE].set(id % BITSET_SIZE, value);
		}

		bool test(ComponentID id) const {
			return bitsets[id / BITSET_SIZE].test(id % BITSET_SIZE);
		}


		bool includes(const Signature& signature) const {
			for (size_t index{}; index < BITSET_COUNT; ++index) {
				if ((bitsets[index] | signature.bitsets[index]) != bitsets[index]) {
					return false;
				}
			}
			return true;
		}

		bool matches(const Signature& signature) const {
			for (size_t index{}; index < BITSET_COUNT; ++index) {
				if ((bitsets[index] & signature.bitsets[index]) > 0) {
					return true;
				}
			}
			return false;
		}

		bool any() const {
			for (Bitset bitset: bitsets) {
				if (bitset.any()) {
					return true;
				}
			}
			return false;
		}

		bool operator==(const Signature& other) const {
			for (size_t index{}; index < BITSET_COUNT; ++index) {
				if (bitsets[index] != other.bitsets[index]) {
					return false;
				}
			}
			return true;
		}

		bool operator!=(const Signature& other) const {
			return !(*this == other);
		}

		Signature operator+(const Signature& other) const {
			Signature out;
			for (size_t i = 0; i < BITSET_COUNT; ++i) {
				out.bitsets[i] = bitsets[i] | other.bitsets[i];
			}
			return out;
		}

		Signature& operator+=(const Signature& other) {
			for (size_t i = 0; i < BITSET_COUNT; ++i) {
				bitsets[i] |= other.bitsets[i];
			}
			return *this;
		}

		const BitsetArray& data() const {
			return bitsets;
		}

		template<typename... Components>
		static Signature build() {
			Signature out;
			(out.set(ComponentRegistry<Components,ComponentID>::id()), ...);
			return out;
		}

	};

}

namespace std {

	template<typename ComponentID, size_t MAX_COMPONENT_COUNT>
	struct hash<Byte::Signature<ComponentID, MAX_COMPONENT_COUNT>> {

		using Signature = Byte::Signature<ComponentID, MAX_COMPONENT_COUNT>;

		size_t operator()(const Signature& signature) const {
			size_t result{};

			for (size_t i{}; i < signature.BITSET_COUNT; ++i) {
				result += signature.data()[i].to_ullong();
			}

			return result;
		}
	};

}


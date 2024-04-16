#pragma once

#include <cstdint>

namespace Byte {

	template<typename ComponentID>
	class ComponentIDGenerator {
	private:
		inline static ComponentID nextID{ 0 };

	public:
		template<typename Component>
		static ComponentID generate() {
			return nextID++;
		}

	};

	template<typename Component, typename ComponentID>
	class ComponentRegistry {
	private:
		inline static ComponentID _id{ 
			ComponentIDGenerator<ComponentID>::template generate<Component>() };

	public:
		static ComponentID id() {
			return _id;
		}

		static void set(ComponentID newID) {
			_id = newID;
		}

	};

}
#pragma once

#include <memory>
#include <utility>
#include <type_traits>
#include <algorithm>

namespace Byte {

	template<template<typename> class Container>
	class IAccessor;

	template<template<typename> class Container>
	using UAccessor = std::unique_ptr<IAccessor<Container>>;

	template<typename Component, template<typename> class Container>
	class Accessor;

	template<template<typename> class Container>
	class IAccessor {
	public:
		virtual ~IAccessor() = default;

		virtual UAccessor<Container> copy() const = 0;

		virtual UAccessor<Container> carry() = 0;

		virtual void copyComponent(size_t index, const UAccessor<Container>& from) = 0;

		virtual void carryComponent(size_t index, UAccessor<Container>& from) = 0;

		virtual void swapComponents(size_t first, size_t second) = 0;

		virtual void popBack() = 0;

		virtual size_t size() const = 0;

		virtual size_t capacity() const = 0;

		virtual void clear() = 0;

		virtual UAccessor<Container> instance() const = 0;

		template<typename Component>
		Accessor<Component, Container>& receive() {
			return *static_cast<Accessor<Component, Container>*>(this);
		}

	};

	template <typename Component, template<typename> class Container>
	class Accessor: public IAccessor<Container> {
	public:
		using ComponentContainer = Container<Component>;

	private:
		ComponentContainer container;

	public:
		UAccessor<Container> copy() const override {
			return std::make_unique<Accessor>(*this);
		}

		UAccessor<Container> carry() override {
			return std::make_unique<Accessor>(std::move(*this));
		}

		void copyComponent(size_t index, const UAccessor<Container>& from) override {
			Accessor* castedFrom{ static_cast<Accessor*>(from.get()) };
			container.push_back(castedFrom->get(index));
		}

		void carryComponent(size_t index, UAccessor<Container>& from) override {
			Accessor* castedFrom{ static_cast<Accessor*>(from.get()) };
			container.push_back(std::move(castedFrom->get(index)));
		}

		void swapComponents(size_t first, size_t second) override {
			std::swap(container.at(first), container.at(second));
		}

		void popBack()  override {
			container.pop_back();
		}

		size_t size() const override {
			return container.size();
		}

		size_t capacity() const override {
			return container.capacity();
		}

		void clear() override {
			container.clear();
		}

		UAccessor<Container> instance() const override {
			return std::make_unique<Accessor>();
		}

		Component& get(size_t index) {
			return container.at(index);
		}

		const Component& get(size_t index) const {
			return container.at(index);
		}

		void pushBack(const Component& component) {
			container.push_back(component);
		}

		void pushBack(Component&& component) {
			container.push_back(std::move(component));
		}
		
		template<typename... Args>
		void emplaceBack(Args&&... args) {
			container.emplace_back(std::forward<Args>(args)...);
		}
 
	};

}
#ifndef MDGE_OBJECT_H
#define MDGE_OBJECT_H

#include <cassert>
#include <initializer_list>
#include <typeindex>
#include <unordered_map>

namespace mdge {
class Object;

class Component {
public:
	Component() {}
	virtual ~Component() {}

	void SetOwner(Object *owner) { m_owner = owner; }
	Object *GetOwner() { return m_owner; }

protected:
	Object *m_owner = nullptr;
};

class Object {
public:
	Object() {}
	virtual ~Object() {
		for (auto &component : m_components) { delete component.second; }
		m_components.clear();
	}

	template<typename t_component>
	void AddComponent(t_component *component) {
		std::type_index id = typeid(t_component);
		assert(m_components.find(id) == m_components.end());
		component->SetOwner(this);
		m_components[id] = component;
	}

	template<typename t_component>
	t_component *GetComponent() {
		std::type_index id = typeid(t_component);
		auto componentItterator = m_components.find(id);
		if (componentItterator != m_components.end()) { return static_cast<t_component *>(componentItterator->second); }
		return nullptr;
	}

	template<typename t_component>
	void DeleteComponent() {
		delete m_components.find(typeid(t_component))->second;
		m_components.erase(typeid(t_component));
	}

	virtual void Update() = 0;

protected:
	std::unordered_map<std::type_index, Component *> m_components;
};
}// namespace mdge

#endif// !MDGE_GAME_OBJECT_H

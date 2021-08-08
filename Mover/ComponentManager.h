#pragma once
#include "Component.h"
#include "ComponentArray.h"
#include <memory>

class ComponentManager {
private:
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrayMap;

	template <typename ComponentType>
	std::shared_ptr<ComponentArray<ComponentType>> getComponentArray() {
		const char* componentTypeName = typeid(ComponentType).name();

		// Initialize a new componnt array if first time component is added
		if (componentArrayMap.find(componentTypeName) == componentArrayMap.end()) {
			componentArrayMap[componentTypeName] = std::make_shared<ComponentArray<ComponentType>>();
		}

		return std::static_pointer_cast<ComponentArray<ComponentType>>(componentArrayMap[componentTypeName]);
	}

public:
	template <typename ComponentType>
	void addComponent(Entity entity, ComponentType component) {
		getComponentArray<ComponentType>()->addEntityComponent(entity, component);
	}

	template <typename ComponentType>
	void removeComponent(Entity entity) {
		getComponentArray<ComponentType>()->removeEntityComponent(entity);
	}

	template<typename ComponentType>
	ComponentType& getComponent(Entity entity) {
		return getComponentArray<ComponentType>()->getComponent(entity);
	}

	void removeEntity(Entity entity) {
		for (auto const& pair : componentArrayMap) {
			auto const& componentArray = pair.second;
			componentArray->removeEntityComponent(entity);
		}
	}
};

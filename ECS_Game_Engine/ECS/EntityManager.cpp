#include"EntityManager.h"
#include<string>

/*
	The Modern Method: std::erase_if
	This function is part of the <vector> header. 
	It performs a "linear scan" of your vector 
	and removes every element that meets a specific condition (a "predicate").

	How it works: It moves all the "live" entities to the front of
	the vector and then shrinks the vector
	to chop off the "dead" ones at the end in one operation.
*/



using namespace std::string_literals;

EntityManager::EntityManager(){}

std::shared_ptr<Entity> EntityManager::addEntity(){
	//here we allocate the resource in memory and then 
	//return shared pointer to that resource
	auto entity = std::make_shared<Entity>(m_total_entities++, "default"s);

	//add this into the waiting room so that this does not cause 
	//iterator invalidation
	m_entities_to_be_added.push_back(entity);	//add the shared pointer to the list
	return entity;
}
void EntityManager::update(){
	//this will make all the requried changes to be made in the next frame 
	//since in the current frame one cannot simply change every thing
	//the changes are recorded and then updated before the next frame

	//first add all the entities
	for (auto e : m_entities_to_be_added) {
		m_entities.push_back(e);
		m_tag_to_entities[e->getTag()].push_back(e);
	}

	//now delete all of the non active elements from the list
	std::erase_if(m_entities, [](const std::shared_ptr<Entity> &e) {
		return !e->isActive();
	});

	//erase dead entities from the mapped vectors
	for (auto& pair : m_tag_to_entities) {
		std::erase_if(pair.second, [](const std::shared_ptr<Entity>& e) {return !e->isActive();});
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag){
	auto entity = std::make_shared<Entity>(m_total_entities++, tag);
	m_entities_to_be_added.push_back(entity);
	return entity;
}

const Entities& EntityManager::getEntities(){
	//returns all the entities
	return m_entities;
}

const Entities& EntityManager::getEntities(const std::string& tag){
	return m_tag_to_entities[tag];
}
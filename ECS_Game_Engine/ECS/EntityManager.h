#pragma once
#include<vector>
#include<unordered_map>
#include"Entity.h"
#include<memory>

using Entities		= std::vector<std::shared_ptr<Entity>>;
using EntitiesMap	= std::unordered_map<std::string, std::vector<std::shared_ptr<Entity>>>;


/*
	What is a factory and how does it work in handeling a certain type of class
	how is this entity manager a factory

	this entity manager is a factory because it is used to only create , store
	and delete the generic Entity type now it does not have the ability 
	to make changes to or call the entities this is done elsewhere
*/

class EntityManager {
private:
	Entities	m_entities;
	Entities	m_entities_to_be_added;
	Entities	m_entites_to_be_deleted;	//may not be needed

	EntitiesMap m_tag_to_entities;		//gets all the entities that are of 
										//one certain type identified by the tag

	size_t m_total_entities{0};

public:
	EntityManager();

	std::shared_ptr<Entity> addEntity();
	void update();		//this changes the current entities vector and
						//the current add and remove entities vector

	//this will be used by the game class fucntions 
	//the entity manager is just going to be a container and a factory for 
	//all the entities and it cannot create the specialised type of entity
	//once the entity is created the pointer to that entity is shared to the 
	//game class function spawnEnemy or spawnPlayer to create the requried special form
	//of entity , the entity is shared and components are added to that entity
	std::shared_ptr<Entity> addEntity(const std::string & tag);	
	const Entities& getEntities();		//get all the entities
	const Entities& getEntities(const std::string& tag); //get all the entities of this tag
};
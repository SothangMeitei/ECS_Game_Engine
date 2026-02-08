
#include"Entity.h"

void Entity::destroy() {
	this->m_is_Active = false;
}
bool Entity::isActive() {
	return m_is_Active;
}

const std::string& Entity::getTag() const {
	return m_Tag;
}
const std::size_t& Entity::getID() const {
	return m_ID;
}

//note that the manager will create the entity in the heap dynamically and
//then the address of the entity is used to referece to it
//when the entity is no longer alive the entity in the heap is deallocated form memory
Entity::Entity(size_t id_form_the_manager, const std::string& tag_form_the_manager) 
	: m_ID(id_form_the_manager) , m_Tag(tag_form_the_manager) , m_is_Active(true)
{
}
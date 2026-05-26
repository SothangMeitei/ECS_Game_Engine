#pragma once
#include<vector>
#include<memory>
#include"ECS/Entity.h"
#include"AABB_bound.h"

class QuadTree {
private:
	const int MAX_ENTITIES = 4;
	const int MAX_DEPTH = 5;

	int			m_level;
	AABB_Bounds m_bound;

	std::vector<std::shared_ptr<Entity>>	m_entities;
	std::unique_ptr<QuadTree>				m_nodes[4];

	bool m_isDivided = false;

	//make no other outer thing be able to divide this quad
	void subdivide() {
		float subWidth = m_bounds.w / 2.0f;
		float subHeight = m_bounds.h / 2.0f;
		float x = m_bounds.x;
		float y = m_bounds.y;

		m_nodes[0] = std::make_unique<QuadTree>(m_level + 1, AABB_Bounds{ x , y , subWidth , subHeight });
		m_nodes[1] = std::make_unique<QuadTree>(m_level + 1, AABB_Bounds{ x + subWidth , y , subWidth , subHeight });
		m_nodes[2] = std::make_unique<QuadTree>(m_level + 1, AABB_Bounds{ x , y + subHeight , subWidth , subHeight });
		m_nodes[3] = std::make_unique<QuadTree>(m_level + 1, AABB_Bounds{ x + subWidht , y + subHeight , subWidth , subHeigth });
	}
public:

	//FOR NOW USE THE POINTER METHOD WHICH IS LESS CACHE FRIENDLY AS COMPARED TO THAT OF THE CONTINOUS ARRAY VECTOR
	void clear() {}
	QuadTree() {}


};


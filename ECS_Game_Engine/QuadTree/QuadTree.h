#pragma once
#include<vector>
#include<memory>
#include"../ECS/Entity.h"
#include"AABB_bound.h"

//this is not the whole tree but a part of the tree a square of the tree
class QuadTree {
private:
	const int MAX_ENTITIES = 4;     //max entities that can be stored in one node
	const int MAX_DEPTH = 5;		//max depth is to make sure that the quad ends at some point when there are a lot of entites in some region

	int			m_level;
	AABB_Bounds m_bounds;

	std::vector<std::shared_ptr<Entity>>	m_entities;     //entites that this node is storing
	std::unique_ptr<QuadTree>				m_nodes[4];     //the 4 child nodes of this tree

	bool m_isDivided = false;

	//make no other outer thing be able to divide this quad
	void subdivide() {
		float subWidth      = m_bounds.w / 2.0f;
		float subHeight     = m_bounds.h / 2.0f;
		float x             = m_bounds.x;
		float y             = m_bounds.y;

		/*
						|
					0	|	1
			----------------------------
					2	|	3
						|
		*/
		m_nodes[0] = std::make_unique<QuadTree>(m_level + 1, AABB_Bounds{ x , y , subWidth , subHeight });
		m_nodes[1] = std::make_unique<QuadTree>(m_level + 1, AABB_Bounds{ x + subWidth , y , subWidth , subHeight });
		m_nodes[2] = std::make_unique<QuadTree>(m_level + 1, AABB_Bounds{ x , y + subHeight , subWidth , subHeight });
		m_nodes[3] = std::make_unique<QuadTree>(m_level + 1, AABB_Bounds{ x + subWidth , y + subHeight , subWidth , subHeight});

		m_isDivided = true;
	}
public:

	//FOR NOW USE THE POINTER METHOD WHICH IS LESS CACHE FRIENDLY AS COMPARED TO THAT OF THE CONTINOUS ARRAY VECTOR
	QuadTree(int level , AABB_Bounds bounds) : m_level(level) , m_bounds(bounds) {}

	void clear() {
		m_entities.clear();
		for (int i = 0; i < 4;++i) {
			if (m_nodes[i]) {
				m_nodes[i]->clear();
			}
		}
	}

    void insert(std::shared_ptr<Entity> entity) {
        if (!entity->m_Transform || !entity->m_Collider) return;

        AABB_Bounds entityBounds = {
            entity->m_Transform->position.get_x() + entity->m_Collider->offsetX,
            entity->m_Transform->position.get_y() + entity->m_Collider->offsetY,
            (float)entity->m_Collider->w,
            (float)entity->m_Collider->h
        };
		//if entity not within the current quad do not add it to this quad
        if (!m_bounds.intersect(entityBounds)) {
            return;
        }

        if (m_isDivided) {
            for (int i = 0; i < 4; i++) {
                m_nodes[i]->insert(entity);
            }
            return; //since this is not to be added to this quad this is already added to one of it's children
        }

        m_entities.push_back(entity);

        if (m_entities.size() > MAX_ENTITIES && m_level < MAX_DEPTH) {
            subdivide();

            for (auto& e : m_entities) {
                for (int i = 0; i < 4; i++) {
                    m_nodes[i]->insert(e);
                }
            }
            m_entities.clear();
        }
    }

    //this takes in a bound and then returns all the quads that intersects with this bound
    void retrieve(std::vector<std::shared_ptr<Entity>>& returnEntities, const AABB_Bounds& searchBounds) {
        if (!m_bounds.intersect(searchBounds)) return;

        // If divided is the current quad then pass the search area down to the children
        // recursively search all the children
        if (m_isDivided) {
            for (int i = 0; i < 4; i++) {
                m_nodes[i]->retrieve(returnEntities, searchBounds);
            }
            return;
        }

        // If we hit a leaf node (not divided) then return its entities
        for (auto& e : m_entities) {
            returnEntities.push_back(e);
        }
    }
};
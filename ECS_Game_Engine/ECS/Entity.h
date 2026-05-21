#pragma once

#include<iostream>
#include<unordered_map>
#include<vector>
#include<memory>
#include<string>
#include"../Components/cTransform.h"
#include"../Components/cInput.h"
#include"../Components/cScore.h"
#include"../Components/cShape.h"
#include"../Components/cCollision.h"
#include"../Components/cVelocity.h"
#include"../Components/cLifeSpan.h"
#include"../Components/cTextOutput.h"
#include"../Components/cLinkedScene.h"

class Entity {
private:
    size_t      m_ID;
    std::string m_Tag;
    bool        m_is_Active;
public:
    std::shared_ptr<cTransform> m_Transform;
    std::shared_ptr<cInput>     m_Input;
    std::shared_ptr<cScore>     m_Score;
    std::shared_ptr<cShape>     m_Shape;
    std::shared_ptr<cCollision> m_Collider;
    std::shared_ptr<cVelocity>  m_Velocity;
    std::shared_ptr<cLifeSpan>  m_LifeSpan;
    std::shared_ptr<cTextOutput> m_TextOutput;
    std::shared_ptr<cLinkedScene> m_LinkScene;

    //for manipulating and getting the active state of the entity
    void destroy();
    bool isActive();

    const std::string& getTag() const;
    const std::size_t& getID() const;

    Entity(size_t, const std::string&);
};
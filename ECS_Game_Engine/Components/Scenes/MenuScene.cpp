#include <fstream>
#include <iostream>
#include <string>
#include "MenuScene.h"
#include"../../GameEngine.h"

MenuScene::MenuScene(const std::string_view pathConfigurationFile) {

    std::ifstream configFile{ std::string(pathConfigurationFile) };

    if (!configFile.is_open()) {
        std::cerr << "Error: Could not open configuration file: " << pathConfigurationFile << "\n";
        numberOfLevels = 0;
        return;
    }

    std::string line;

    // 1. Skip the first 4 lines of metadata
    for (int i = 0; i < 4; ++i) {
        std::getline(configFile, line);
    }

    // Default configuration variables
    int rectWidth = 0;
    int rectHeight = 0;

    // 2. Read the 5th line: #numberOfLevels#width#height
    if (std::getline(configFile, line)) {
        if (!line.empty() && line.front() == '#') {
            try {
                size_t firstHash = 0;
                size_t secondHash = line.find('#', firstHash + 1);
                size_t thirdHash = line.find('#', secondHash + 1);

                if (secondHash != std::string::npos && thirdHash != std::string::npos) {
                    numberOfLevels = std::stoi(line.substr(firstHash + 1, secondHash - firstHash - 1));
                    rectWidth = std::stoi(line.substr(secondHash + 1, thirdHash - secondHash - 1));
                    rectHeight = std::stoi(line.substr(thirdHash + 1));
                }
                else {
                    std::cerr << "Warning: Malformed configuration on line 5.\n";
                    numberOfLevels = 0;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error parsing level/selector data on line 5: " << e.what() << "\n";
                numberOfLevels = 0;
            }
        }
    }

    // Layout variables for stacking the menu boxes
    float startX = 200.0f;
    float startY = 100.0f;
    float padding = 20.0f;

    // 3. Parse the level lines and create "LevelBox" Entities
    for (int i = 0; i < numberOfLevels; ++i) {
        if (std::getline(configFile, line)) {
            size_t firstHash = line.find('#');
            size_t secondHash = line.find('#', firstHash + 1);

            if (firstHash != std::string::npos && secondHash != std::string::npos) {
                std::string textDisplayed = line.substr(firstHash + 1, secondHash - firstHash - 1);
                std::string sceneName = line.substr(secondHash + 1);

                auto boxEntity = m_manager.addEntity("LevelBox");
                float currentY = startY + i * (rectHeight + padding);

                // 100% single-line, fully initialized components
                boxEntity->m_Transform = std::make_shared<cTransform>(vec2(startX, currentY), 1.0, 0.0);
                boxEntity->m_Shape = std::make_shared<cShape>(rectWidth, rectHeight, 255, 255, 255, 4);
                boxEntity->m_TextOutput = std::make_shared<cTextOutput>(textDisplayed);
                boxEntity->m_LinkScene = std::make_shared<cLinkedScene>(sceneName);
            }
            else {
                std::cerr << "Warning: Malformed line in config at level " << i + 1 << "\n";
            }
        }
    }

    auto selectorEntity = m_manager.addEntity("Selector");

    selectorEntity->m_Transform = std::make_shared<cTransform>(vec2(startX, startY), 1.0, 0.0);
    selectorEntity->m_Shape = std::make_shared<cShape>(rectWidth, rectHeight, 255, 0, 0, 4);
    selectorEntity->m_Input = std::make_shared<cInput>();
    m_manager.update();

    m_selectorEntity = selectorEntity;
    m_rectHeight = rectHeight;

    registerAction(SDL_SCANCODE_UP, "up");
    registerAction(SDL_SCANCODE_DOWN, "down");
    registerAction(SDL_SCANCODE_RETURN, "enterScene");
}

void MenuScene::play(float deltaTime) {
    updateInternals(deltaTime);
    render();
}

void MenuScene::sDoAction(const Action& action) {
    if (action.get_type() == "START") {
        if (action.get_name() == "down") {
            // Move index down, wrap around to 0 if at the bottom
            m_selectedIndex++;
            if (m_selectedIndex >= numberOfLevels) {
                m_selectedIndex = 0;
            }
        }
        else if (action.get_name() == "up") {
            // Move index up, wrap around to bottom if at the top
            m_selectedIndex--;
            if (m_selectedIndex < 0) {
                m_selectedIndex = numberOfLevels - 1;
            }
        }
        else if (action.get_name() == "enterScene") {
           
            const auto& boxes = m_manager.getEntities("LevelBox");

            if (m_selectedIndex < boxes.size()) {
                //here due to the entities being pushed into the entites vector in order the entitie's ordering and their index are the same
                std::string targetScene = boxes[m_selectedIndex]->m_LinkScene->name_of_scene_link;       
                m_gameEngineOwnerBackPointer->changeScene(targetScene);
            }
        }
    }
}

void MenuScene::updateInternals(float deltaTime) {
    // Synchronize the Visual State to the Logical State
    // Calculate the exact Y position the selector should be at
    float targetY = m_startY + (m_selectedIndex * (m_rectHeight + m_padding));

    // Directly update the cached selector's transform
    m_selectorEntity->m_Transform->position.y = targetY;
}

void MenuScene::render() {
    SDL_Renderer* renderer = m_gameEngineOwnerBackPointer->getRenderer();
    for (const auto& entity : m_manager.getEntities()) {

        if (entity->m_Transform && entity->m_Shape) {

            //Set the color defined in the entity's cShape
            SDL_SetRenderDrawColor(
                renderer,
                entity->m_Shape->r,
                entity->m_Shape->g,
                entity->m_Shape->b,
                255 //alpha
            );

            //Map the ECS components directly to an SDL_FRect (using top-left coordinates)
            SDL_FRect rect;
            rect.x = entity->m_Transform->position.x;
            rect.y = entity->m_Transform->position.y;
            rect.w = entity->m_Shape->w;
            rect.h = entity->m_Shape->h;

            //while drawing Use SDL_RenderRect for an outline, or SDL_RenderFillRect for a solid box
            SDL_RenderRect(renderer, &rect);
        }

        //implementing the text rendering if the entity has the text component
        if (entity->m_Transform && entity->m_TextOutput) {
            // Draw text at entity->m_Transform->position using entity->m_TextOutput->text
        }
    }
}
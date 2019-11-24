//
//  GameManager.cpp
//  FlappyCocos
//
//  Created by Arthur Motelevicz on 21/11/19.
//

#include "GameManager.hpp"

GameManager::GameManager(){}
GameManager::~GameManager(){}

GameManager* GameManager::instance = 0;

GameManager* GameManager::getInstance()
{
    if (instance == 0)
    {
        instance = new GameManager();
    }
    return instance;
}
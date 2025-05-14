#pragma once
#include "Scene.h"
#include "Player.h"
#include "Wizard.h"
#include "SwordMan.h"
#include "Archer.h"
#include "Arrow.h"
#include <vector>



class Stage1 : public Scene {
public:
    Stage1();
    ~Stage1();
    void Initialize();
    void Update();
    void LateUpdate();
    void Render(HDC hdc);

    void AddArrow(Arrow* arrow) { arrows.push_back(arrow); }

private:
    Player player;
    Wizard wizard;
    SwordMan swordman;
    Archer archer;
    std::vector<Arrow*> arrows;
    float swordmanAttackCooldown;
    float playerSlashAttackCooldown;
};
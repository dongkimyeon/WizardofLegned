#pragma once
#include "Scene.h"
#include "Player.h"
#include "Wizard.h"
#include "SwordMan.h"
#include "Archer.h"
#include "Arrow.h"
#include "FireBall.h"
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
    void AddFireBall(FireBall* fireball) { fireballs.push_back(fireball); }

private:
    Player player;
    std::vector<Wizard*> wizards;    // ¸¶¹ý»ç °´Ã¼ º¤ÅÍ
    std::vector<SwordMan*> swordmans;// ¼Òµå¸Ç °´Ã¼ º¤ÅÍ
    std::vector<Archer*> archers;    // ±Ã¼ö °´Ã¼ º¤ÅÍ
    std::vector<Arrow*> arrows;      // È­»ì °´Ã¼ º¤ÅÍ
    std::vector<FireBall*> fireballs;// ÆÄÀÌ¾îº¼ °´Ã¼ º¤ÅÍ
  
};
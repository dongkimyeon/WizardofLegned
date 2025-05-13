#pragma once
#include "CommonInclude.h"
#include "Player.h"

class Stage1;

class Arrow
{
public:
    Arrow(float x, float y, float dirX, float dirY);
    ~Arrow();

    void Update(Player& player);
    void Render(HDC hdc);

    static void FireArrow(Player& player, float mX, float mY, Stage1* stage);

    bool IsActive() const { return mIsActive; }

private:
    CImage mArrowImage;
    POINT hitboxPoints[4]; // 회전된 충돌 박스
    float mX;
    float mY;
    float mDirectionX;
    float mDirectionY;
    float speed;
    bool mIsActive;
    int damage;

    void UpdateHitbox();
    bool CheckCollisionWithRect(const RECT& rect);
};
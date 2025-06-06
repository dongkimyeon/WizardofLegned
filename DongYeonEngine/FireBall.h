#pragma once
#include "CommonInclude.h"
#include "Player.h"

class Scene;

class FireBall
{
public:
    FireBall(float x, float y, float dirX, float dirY);
    ~FireBall();

    void Update(Player& player);
    void Render(HDC hdc);

    static void ThrowFireBall(Player& player, float mX, float mY, Scene* stage);

    bool IsActive() const { return mIsActive; }

	// 상태 변경
	void SetActive(bool active) { mIsActive = active; }

    POINT* GetHitboxPoints() { return hitboxPoints; } // 히트박스 포인터 반환

    float GetPositionX() const { return mX; }
    float GetPositionY() const { return mY; }



private:
    CImage mFireBallAnimation[6];
    POINT hitboxPoints[4]; // 회전된 충돌 박스
    float mX;
    float mY;
    float mDirectionX;
    float mDirectionY;
    float speed;
    bool mIsActive;
    int damage;

    int mCurrentFrame; // 현재 애니메이션 프레임
    float mAnimationTimer; // 애니메이션 타이머
    const float mFrameDuration = 0.1f; // 각 프레임 지속 시간 (초)

    void UpdateHitbox();
    bool CheckCollisionWithRect(const RECT& rect);
};
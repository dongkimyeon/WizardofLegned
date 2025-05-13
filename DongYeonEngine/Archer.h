#pragma once
#include "GameObject.h"
#include "Player.h"

class Stage1;

class Archer : public GameObject
{
public:
    Archer();
    ~Archer();
    enum EnemyState { LEFT, RIGHT };
    void Update(Player& p, Stage1* stage);
    void LateUpdate() override;
    void Render(HDC hdc, Player& p);
    void SetPosition(float x, float y) override;

    void TakeDamage(int playerDamage) { hp -= playerDamage; }

    EnemyState GetState() { return state; }
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    RECT GetRect() override { return rect; }

private:
    float mX;
    float mY;
    Vector2 position;
    int hp;

    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[4];
    CImage mRightDieAnimaion[6];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[5];

    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[4];
    CImage mLeftDieAnimaion[6];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[5];

    CImage mBowAttackAnimation[4];

    EnemyState state = EnemyState::RIGHT;

    bool mIsMoving;
    int mCurrentWalkFrame;
    bool mIsAttack;
    int mCurrenAttackFrame;
    bool mIsdead;
    int mCurrentDeadFrame;
    bool mIsHit;
    int mCurrentHitFrame;

    bool mIsShot;

    float PlayerDetectRange = 300.0f;
    float AttackRange = 250.0f;

    float speed;
    RECT rect;

    float mAttackFrameTime;
    float mAttackCooldown;

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};
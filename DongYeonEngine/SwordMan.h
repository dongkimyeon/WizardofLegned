#pragma once
#include "GameObject.h"
#include "Player.h"


class SwordMan : public GameObject
{
public:
    SwordMan();
    ~SwordMan(); // GDI+ 종료를 위한 소멸자
    enum EnemyState { LEFT, RIGHT };
    void Update(Player& p);
    void LateUpdate() override;
    void Render(HDC hdc, Player& p);
    void SetPosition(float x, float y) override;

    void TakeDamage(int playerDamage) { hp -= playerDamage; }

    int GetDamage() { return damage; }
    int GetHp() { return hp; }
    EnemyState GetState() { return state; }
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }

    bool GetEffectHitbox(POINT outPoints[4]);
    bool CheckCollisionWithRect(RECT& otherRect) ; // 새 메서드

    RECT GetRect() override { return rect; }

private:
    float mX;
    float mY;
    int hp;
    int damage;
    Vector2 position;

    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[3];
    CImage mRightDieAnimaion[6];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[6];
    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[3];
    CImage mLeftDieAnimaion[6];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[6];
    CImage mAttackEffectAnimation[4];

    EnemyState state = EnemyState::RIGHT;

    bool mIsMoving = false;
    int mCurrentWalkFrame = 0;
    bool mIsAttack = false;
    int mCurrenAttackFrame = 0;
    bool mIsdead = false;
    int mCurrentDeadFrame = 0;
    bool mIsHit = false;
    int mCurrentHitFrame = 0;
    
    //공격시작 방향
    float mAttackDirectionX;
    float mAttackDirectionY;
    
    float mAttackCooldown = 0;
    float mAttackFrameTime;
    float PlayerDetectRange = 300.0f;
    float AttackRange = 30.0f;
    float speed = 100.0f;
    RECT rect;

    POINT mEffectHitboxPoints[4]; 
    bool mHasEffectHitbox;

    // GDI+ 관련
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};
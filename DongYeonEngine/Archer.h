#pragma once
#include "GameObject.h"
#include "Player.h"

class Archer : public GameObject
{
public:
    Archer();
    ~Archer(); // 소멸자 추가
    enum EnemyState { LEFT, RIGHT };
    void Update(Player& p);
    void LateUpdate() override;
    void Render(HDC hdc, Player& p);

    void SetPosition(float x, float y) override;
    void TakeDamage(int playerDamage) { hp -= playerDamage; }

    EnemyState GetState() { return state; }
    float GetPositionX() override { return mX; }
    float GetPositionY() override { return mY; }
    float GetSpeed() override { return speed; }
    RECT GetRect() override { return rect; }
    bool GetIsShot() { return mIsShot; }

private:
    float mX;
    float mY;
    Vector2 position;
    int hp;

    // Right 애니메이션
    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[4];
    CImage mRightDieAnimaion[6];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[5];

    // Left 애니메이션
    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[4];
    CImage mLeftDieAnimaion[6];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[5];

    // 아처가 들고있는 활
    CImage mBowAttackAnimation[4];

    EnemyState state = EnemyState::RIGHT;

    // 움직임 변수
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

    // 공격 타이밍
    float mAttackFrameTime; // 공격 애니메이션 타이머
    float mAttackCooldown; // 공격 쿨타임


    // GDI+ 관련 멤버 변수
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};


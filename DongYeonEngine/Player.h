#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
    Player();
    void Update() override;
    void LateUpdate() override;
    void Render(HDC hdc) override;

    void SetPosition(float x, float y) override;
    void TakeDamage(int d);
    int GetHp() { return hp; }
    int GetDamage() { return damage; }
    PlayerState GetState() { return state; }
    float GetPositionX() override;
    float GetPositionY() override;
    float GetSpeed() override;
    RECT GetRect() override; // GetHitbox 대신 GetRect 사용 (기존 코드 기반)
    bool GetIsDead() { return mIsDead; }

    bool GetEffectHitbox(POINT outPoints[4]); // SwordMan과 동일한 인터페이스 추가
    bool CheckCollisionWithRect(RECT& otherRect); // 충돌 검사 함수 추가

private:
    float mX;
    float mY;
    Vector2 position;
    int hp;
    int damage;
    // Front 애니메이션
    CImage mFrontIdleAnimation;
    CImage mFrontAttackAnimation[16];
    CImage mFrontDashAnimation[8];
    CImage mFrontDieAnimation[7];
    CImage mFrontHitAnimation[2];
    CImage mFrontWalkAnimation[10];

    // Right 애니메이션
    CImage mRightIdleAnimation;
    CImage mRightAttackAnimation[16];
    CImage mRightDashAnimation[8];
    CImage mRightHitAnimation[2];
    CImage mRightWalkAnimation[10];
 
    // Left 애니메이션
    CImage mLeftIdleAnimation;
    CImage mLeftAttackAnimation[16];
    CImage mLeftDashAnimation[8];
    CImage mLeftHitAnimation[2];
    CImage mLeftWalkAnimation[10];
  
    // Back 애니메이션
    CImage mBackIdleAnimation;
    CImage mBackAttackAnimation[16];
    CImage mBackDashAnimation[8];
    CImage mBackHitAnimation[2];
    CImage mBackWalkAnimation[10];

    // 이펙트 애니메이션 왼쪽 오른쪽 어택애니메이션 8프레임까지는 라이트이펙트애니메이션 이후에는 래프트 이펙트 애니메이션
    CImage mRightAttackEffectAnimation[6];
    CImage mLeftAttackEffectAnimation[6];

    PlayerState state = PlayerState::FRONT;

    // 상태 변수
    bool mIsMoving;
    bool mIsDashing;
    bool mIsAttack;
    bool mIsHit;
    bool mIsDead;

    bool mMouseClickFlag;

    // 애니메이션 프레임
    int mCurrentWalkFrame;
    int mCurrentDashFrame;
    int mCurrentAttackFrame;
    int mCurrentHitFrame;
    int mCurrentDieFrame;

    // 타이머 및 속도
    float mDashTimer;
    float mAttackTimer;
    float mHitTimer;
    float speed;
    RECT rect;

    POINT mEffectHitboxPoints[4]; // 공격 이펙트 히트박스 꼭짓점
    bool mHasEffectHitbox; // 히트박스 활성화 여부
    float attackAngle; // 마우스 방향 각도



    bool CheckPointInPolygon(POINT& point, POINT polygon[4]);
    

};
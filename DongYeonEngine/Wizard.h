#pragma once
#include "GameObject.h"
#include "Player.h"
#include "FireBall.h"
class Wizard: public GameObject
{
public:

	Wizard();
	~Wizard();
	enum EnemyState { LEFT, RIGHT };

	void Update(Player& p, Stage1* stage);
	void LateUpdate() override;
	void Render(HDC hdc, Player& p);

	void SetHp(int playerDamage) { hp -= playerDamage; }
	void TakeDamage(int d);

	void SetPosition(float x, float y) override;
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
	int damage;
	// Right 애니메이션
	CImage mRightIdleAnimation;
	CImage mRightAttackAnimation[4];
	CImage mRightDieAnimaion[5];
	CImage mRightHitAnimation[2];
	CImage mRightWalkAnimation[4];

	// Left 애니메이션
	CImage mLeftIdleAnimation;
	CImage mLeftAttackAnimation[4];
	CImage mLeftDieAnimaion[5];
	CImage mLeftHitAnimation[2];
	CImage mLeftWalkAnimation[4];


	EnemyState state = EnemyState::RIGHT;

	//움직임 변수
	bool mIsMoving;
	int mCurrentWalkFrame;
	bool mIsAttack;
	int mCurrenAttackFrame;
	bool mIsDead;
	int mCurrentDeadFrame;
	bool mIsHit;
	int mCurrentHitFrame;
	
	float PlayerDetectRange = 300.0f;
	float AttackRange = 250.0f;


	float mHitTimer;

	float speed;
	RECT rect;


	float mAttackFrameTime;
	float mAttackCooldown;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	

};


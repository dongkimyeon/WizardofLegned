#include "Wizard.h"
#include "Time.h"

Wizard::Wizard()
{
    mX = 1920 / 2;
    mY = 1080 / 2;
   
    speed = 100.0f;
    state = EnemyState::RIGHT;
    mIsMoving = false;
    mCurrentWalkFrame = 0;
    mIsAttack = false;
    mCurrenAttackFrame = 0;
    mIsdead = false;
    mCurrentDeadFrame = 0;
    mIsHit = false;
    mCurrentHitFrame = 0;

    
    // Right 애니메이션 로드
    mRightIdleAnimation.Load(L"resources/Monster/WIZARD/WizardRight/WizardIdle/WIZARD_RIGHT_00.png");
    if (mRightIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/WIZARD/WizardRight/WizardIdle/WIZARD_RIGHT_00.png\n");
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardRight/WizardAttack/WIZARD_RIGHT_%02d.png", i);
        mRightAttackAnimation[i].Load(path);
        if (mRightAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardRight/WizardDie/WIZARD_RIGHT_%02d.png", i);
        mRightDieAnimaion[i].Load(path);
        if (mRightDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardRight/WizardHit/WIZARD_RIGHT_%02d.png", i);
        mRightHitAnimation[i].Load(path);
        if (mRightHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardRight/WizardWalk/WIZARD_RIGHT_%02d.png", i);
        mRightWalkAnimation[i].Load(path);
        if (mRightWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Left 애니메이션 로드
    mLeftIdleAnimation.Load(L"resources/Monster/WIZARD/WizardLeft/WizardIdle/WIZARD_LEFT_00.png");
    if (mLeftIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/WIZARD/WizardLeft/WizardIdle/WIZARD_LEFT_00.png\n");
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardLeft/WizardAttack/WIZARD_LEFT_%02d.png", i);
        mLeftAttackAnimation[i].Load(path);
        if (mLeftAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardLeft/WizardDie/WIZARD_LEFT_%02d.png", i);
        mLeftDieAnimaion[i].Load(path);
        if (mLeftDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardLeft/WizardHit/WIZARD_LEFT_%02d.png", i);
        mLeftHitAnimation[i].Load(path);
        if (mLeftHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardLeft/WizardWalk/WIZARD_LEFT_%02d.png", i);
        mLeftWalkAnimation[i].Load(path);
        if (mLeftWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
}


void Wizard::Update(Player& p)
{
    // rect 업데이트
    rect.left = mX - 22;
    rect.top = mY - 38;
    rect.right = mX + 20;
    rect.bottom = mY + 38;

    // 애니메이션 프레임 업데이트
    static float frameTime = 0.0f;
    frameTime += Time::DeltaTime();

    if (mIsdead)
    {
        if (frameTime >= 0.1f)
        {
            mCurrentDeadFrame = (mCurrentDeadFrame + 1) % 5; // Die: 5프레임
            frameTime = 0.0f;
        }
    }
    else if (mIsHit)
    {
        if (frameTime >= 0.1f)
        {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2; // Hit: 2프레임
            frameTime = 0.0f;
        }
    }
    else if (mIsAttack)
    {
        if (frameTime >= 0.1f)
        {
            mCurrenAttackFrame = (mCurrenAttackFrame + 1) % 4; // Attack: 4프레임
            frameTime = 0.0f;
        }
    }
    else if (mIsMoving)
    {
        if (frameTime >= 0.1f)
        {
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 4; // Walk: 4프레임
            frameTime = 0.0f;
        }
    }


}

void Wizard::LateUpdate()
{
}

void Wizard::Render(HDC hdc)
{
    // 충돌영역 사각형
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    CImage* currentImage = nullptr;

    // 상태에 따라 렌더링할 이미지 선택 (기존 코드 유지)
    if (mIsdead)
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightDieAnimaion[mCurrentDeadFrame];
        else
            currentImage = &mLeftDieAnimaion[mCurrentDeadFrame];
    }
    else if (mIsHit)
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightHitAnimation[mCurrentHitFrame];
        else
            currentImage = &mLeftHitAnimation[mCurrentHitFrame];
    }
    else if (mIsAttack)
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightAttackAnimation[mCurrenAttackFrame];
        else
            currentImage = &mLeftAttackAnimation[mCurrenAttackFrame];
    }
    else if (mIsMoving)
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightWalkAnimation[mCurrentWalkFrame];
        else
            currentImage = &mLeftWalkAnimation[mCurrentWalkFrame];
    }
    else
    {
        if (state == EnemyState::RIGHT)
            currentImage = &mRightIdleAnimation;
        else
            currentImage = &mLeftIdleAnimation;
    }

    // 이미지 크기 (기존 코드에서 사용된 크기)
    int imageWidth = 61;  // 106 - 45
    int imageHeight = 101; // 146 - 45

    // 중심을 mX, mY에 맞추기 위해 왼쪽 상단 좌표 계산
    int drawX = static_cast<int>(mX - imageWidth / 2.0f);
    int drawY = static_cast<int>(mY - imageHeight / 2.0f);
    
    // 이미지 그리기
    currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);
}

void Wizard::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
}

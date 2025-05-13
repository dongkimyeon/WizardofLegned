#include "Archer.h"
#include "Time.h"
#include <cmath>

Archer::Archer()
{
    // GDI+ 초기화
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 20), (int)(mY + 200), (int)(mX + 20), (int)(mY + 20) };
    speed = 100.0f;
    state = EnemyState::RIGHT;
    hp = 100;
    mIsMoving = false;
    mCurrentWalkFrame = 0;
    mIsAttack = false;
    mCurrenAttackFrame = 0;
    mIsdead = false;
    mCurrentDeadFrame = 0;
    mIsHit = false;
    mCurrentHitFrame = 0;
    mIsShot = false;

    mAttackFrameTime = 0.0f;
    mAttackCooldown = 0.0f;

    // Right 애니메이션 로드
    mRightIdleAnimation.Load(L"resources/Monster/ARCHER/ArcherRight/ArcherIdle/ARCHER_RIGHT_00.png");
    if (mRightIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/ARCHER/ArcherRight/ArcherIdle/ARCHER_RIGHT_00.png\n");
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherRight/ArcherAttack/ARCHER_RIGHT_%02d.png", i);
        mRightAttackAnimation[i].Load(path);
        if (mRightAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherRight/ArcherDie/ARCHER_RIGHT_%02d.png", i);
        mRightDieAnimaion[i].Load(path);
        if (mRightDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherRight/ArcherHit/ARCHER_RIGHT_%02d.png", i);
        mRightHitAnimation[i].Load(path);
        if (mRightHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherRight/ArcherWalk/ARCHER_RIGHT_%02d.png", i);
        mRightWalkAnimation[i].Load(path);
        if (mRightWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Left 애니메이션 로드
    mLeftIdleAnimation.Load(L"resources/Monster/ARCHER/ArcherLeft/ArcherIdle/ARCHER_LEFT_00.png");
    if (mLeftIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/ARCHER/ArcherLeft/ArcherIdle/ARCHER_LEFT_00.png\n");
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherLeft/ArcherAttack/ARCHER_LEFT_%02d.png", i);
        mLeftAttackAnimation[i].Load(path);
        if (mLeftAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherLeft/ArcherDie/ARCHER_LEFT_%02d.png", i);
        mLeftDieAnimaion[i].Load(path);
        if (mLeftDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherLeft/ArcherHit/ARCHER_LEFT_%02d.png", i);
        mLeftHitAnimation[i].Load(path);
        if (mLeftHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ArcherLeft/ArcherWalk/ARCHER_LEFT_%02d.png", i);
        mLeftWalkAnimation[i].Load(path);
        if (mLeftWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/ARCHER/ARCHER_BOW/Idle/ARCHER_BOW_%02d.png", i);
        mBowAttackAnimation[i].Load(path);
        if (mBowAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
}

Archer::~Archer()
{
    Gdiplus::GdiplusShutdown(gdiplusToken); // GDI+ 해제
}

void Archer::Update(Player& p)
{
    // rect 업데이트 (이미지 중심에 맞게)
    int imageWidth = mRightIdleAnimation.GetWidth();
    int imageHeight = mRightIdleAnimation.GetHeight();
    rect.left = static_cast<int>(mX - imageWidth / 2.0f) + 45;
    rect.top = static_cast<int>(mY - imageHeight / 2.0f) + 40;
    rect.right = static_cast<int>(mX + imageWidth / 2.0f) - 54;
    rect.bottom = static_cast<int>(mY + imageHeight / 2.0f) - 33;

    // 플레이어와의 거리 계산
    float dx = p.GetPositionX() - mX;
    float dy = p.GetPositionY() - mY;
    float distance = sqrt(dx * dx + dy * dy);

    // 상태 설정 (플레이어 방향에 따라)
    state = (dx > 0) ? EnemyState::RIGHT : EnemyState::LEFT;

    // 공격 쿨타임 업데이트
    if (mAttackCooldown > 0.0f)
    {
        mAttackCooldown -= Time::DeltaTime();
    }

    // 애니메이션 프레임 업데이트
    float frameTime = 0.0f; // 비공격 애니메이션용
    frameTime += Time::DeltaTime();

    if (mIsdead)
    {
        if (frameTime >= 0.1f)
        {
            mCurrentDeadFrame = (mCurrentDeadFrame + 1) % 6;
            frameTime = 0.0f;
        }
        return;
    }
    else if (mIsHit)
    {
        if (frameTime >= 0.1f)
        {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2;
            frameTime = 0.0f;
            if (mCurrentHitFrame == 0) mIsHit = false;
        }
        return;
    }
    else
    {
        if (distance <= AttackRange && mAttackCooldown <= 0.0f && !mIsAttack)
        {
            mIsAttack = true;
            mCurrenAttackFrame = 0;
            mAttackFrameTime = 0.0f; // 공격 시작 시 타이머 초기화
            mIsMoving = false;
        }
        else if (distance <= PlayerDetectRange && distance > AttackRange)
        {
            mIsMoving = true;
            mIsAttack = false;
            float angle = atan2(dy, dx);
            mX += cos(angle) * speed * Time::DeltaTime();
            mY += sin(angle) * speed * Time::DeltaTime();
        }
        else
        {
            mIsMoving = false;
            mIsAttack = false;
        }
    }

    // 공격 애니메이션 프레임 업데이트
    if (mIsAttack)
    {
        mAttackFrameTime += Time::DeltaTime();
       
        float frameDuration = 0.5f;
        if (mAttackFrameTime >= frameDuration)
        {
            mCurrenAttackFrame++;
            if (mCurrenAttackFrame >= 4)
            {
                mIsAttack = false;
                mCurrenAttackFrame = 0;
                mAttackCooldown = 3.0f; // 3초 쿨타임
                mIsShot = true; // 공격 후 샷 플래그
            }
            mAttackFrameTime = 0.0f;
        }
        std::cout << mAttackFrameTime << std::endl;
    }
    else if (mIsMoving)
    {
        if (frameTime >= 0.1f)
        {
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 5;
            frameTime = 0.0f;
        }
    }

    // 체력 확인
    if (hp <= 0)
    {
        mIsdead = true;
        mCurrentDeadFrame = 0;
    }

    std::cout << mIsAttack << std::endl;
}

void Archer::LateUpdate()
{
}

void Archer::Render(HDC hdc, Player& p)
{
    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    CImage* currentImage = nullptr;
    if (mIsdead)
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightDieAnimaion[mCurrentDeadFrame] : &mLeftDieAnimaion[mCurrentDeadFrame];
    }
    else if (mIsHit)
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightHitAnimation[mCurrentHitFrame] : &mLeftHitAnimation[mCurrentHitFrame];
    }
    else if (mIsAttack)
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightAttackAnimation[mCurrenAttackFrame] : &mLeftAttackAnimation[mCurrenAttackFrame];
    }
    else if (mIsMoving)
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightWalkAnimation[mCurrentWalkFrame] : &mLeftWalkAnimation[mCurrentWalkFrame];
    }
    else
    {
        currentImage = (state == EnemyState::RIGHT) ? &mRightIdleAnimation : &mLeftIdleAnimation;
    }

    int imageWidth = 141 - 60;
    int imageHeight = 145 - 60;
    int drawX = static_cast<int>(mX - imageWidth / 2.0f);
    int drawY = static_cast<int>(mY - imageHeight / 2.0f);

    // 아처 본체 이미지 렌더링
    currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);

    // 활 애니메이션 렌더링 (공격 중일 때)
    if (mIsAttack)
    {
        CImage* bowImage = &mBowAttackAnimation[mCurrenAttackFrame];
        int bowWidth = bowImage->GetWidth() * 0.4;
        int bowHeight = bowImage->GetHeight() * 0.4;

        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        Gdiplus::ImageAttributes imageAttr;
        imageAttr.SetColorKey(Gdiplus::Color(0, 0, 0), Gdiplus::Color(0, 0, 0));

        // 플레이어 방향으로 회전
        float dx = p.GetPositionX() - mX;
        float dy = p.GetPositionY() - mY;
        float angle = atan2(dy, dx) * 180.0f / 3.1415926535f;

        // 활 위치를 아처 중심에서 약간 오프셋
        float bowOffset = 20.0f; // 스워드맨 effectOffset(30.0f)보다 작게 조정
        float directionX = cos(atan2(dy, dx));
        float directionY = sin(atan2(dy, dx));
        int bowDrawX = static_cast<int>(mX + directionX * bowOffset - bowWidth / 2.0f);
        int bowDrawY = static_cast<int>(mY + directionY * bowOffset - bowHeight / 2.0f);

        Gdiplus::Matrix matrix;
        matrix.RotateAt(angle, Gdiplus::PointF(mX + directionX * bowOffset, mY + directionY * bowOffset));
        graphics.SetTransform(&matrix);

        Gdiplus::Bitmap bowBitmap((HBITMAP)*bowImage, nullptr);
        graphics.DrawImage(&bowBitmap,
            Gdiplus::Rect(bowDrawX, bowDrawY, bowWidth, bowHeight),
            0, 0, bowBitmap.GetWidth(), bowBitmap.GetHeight(),
            Gdiplus::UnitPixel, &imageAttr);

        graphics.ResetTransform();
    }
}

void Archer::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
}
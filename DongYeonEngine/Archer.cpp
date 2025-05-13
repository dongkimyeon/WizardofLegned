#include "Archer.h"
#include "Time.h"
#include "Arrow.h"
#include "Stage1.h"


Archer::Archer()
{
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
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

void Archer::Update(Player& p, Stage1* stage)
{
    int imageWidth = mRightIdleAnimation.GetWidth();
    int imageHeight = mRightIdleAnimation.GetHeight();
    rect.left = static_cast<int>(mX - imageWidth / 2.0f) + 45;
    rect.top = static_cast<int>(mY - imageHeight / 2.0f) + 40;
    rect.right = static_cast<int>(mX + imageWidth / 2.0f) - 54;
    rect.bottom = static_cast<int>(mY + imageHeight / 2.0f) - 33;

    float playerX = p.GetPositionX();
    float playerY = p.GetPositionY();
    float distance = static_cast<float>(sqrt(pow(mX - playerX, 2) + pow(mY - playerY, 2)));

    if (mAttackCooldown > 0.0f)
    {
        mAttackCooldown -= Time::DeltaTime();
    }

    if (mIsdead || mIsHit)
    {
        mIsMoving = false;
        mIsAttack = false;
        mIsShot = false;
        mCurrentWalkFrame = 0;
    }
    else
    {
        if (playerX > mX)
        {
            state = EnemyState::RIGHT;
        }
        else if (playerX < mX)
        {
            state = EnemyState::LEFT;
        }

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
                    mAttackCooldown = 3.0f;
                    Arrow::FireArrow(p, mX, mY, stage);
                    mIsShot = true;
                }
                mAttackFrameTime = 0.0f;
            }
        }
        else
        {
            if (distance <= AttackRange && mAttackCooldown <= 0.0f)
            {
                mIsAttack = true;
                mIsMoving = false;
                mCurrenAttackFrame = 0;
                mAttackFrameTime = 0.0f;
                mCurrentWalkFrame = 0;
            }
            else if (distance <= PlayerDetectRange)
            {
                if (!mIsMoving)
                {
                    mCurrentWalkFrame = 0;
                }
                mIsAttack = false;
                mIsMoving = true;
                float directionX = (playerX - mX) / distance;
                float directionY = (playerY - mY) / distance;
                mX += directionX * speed * Time::DeltaTime();
                mY += directionY * speed * Time::DeltaTime();
            }
            else
            {
                mIsAttack = false;
                mIsMoving = false;
                mCurrenAttackFrame = 0;
                mCurrentWalkFrame = 0;
            }
        }
    }

    static float walkFrameTime = 0.0f;
    if (!mIsAttack && mIsMoving)
    {
        walkFrameTime += Time::DeltaTime();
        if (walkFrameTime >= 0.1f)
        {
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 5;
            walkFrameTime = 0.0f;
        }
    }
    else
    {
        walkFrameTime = 0.0f;
    }

    static float deadFrameTime = 0.0f;
    static float hitFrameTime = 0.0f;
    if (mIsdead)
    {
        deadFrameTime += Time::DeltaTime();
        if (deadFrameTime >= 0.1f)
        {
            mCurrentDeadFrame = (mCurrentDeadFrame + 1) % 6;
            deadFrameTime = 0.0f;
        }
    }
    else if (mIsHit)
    {
        hitFrameTime += Time::DeltaTime();
        if (hitFrameTime >= 0.1f)
        {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2;
            hitFrameTime = 0.0f;
        }
    }
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

    currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);

    if (mIsAttack)
    {
        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        Gdiplus::ImageAttributes imageAttr;
        imageAttr.SetColorKey(Gdiplus::Color(0, 0, 0), Gdiplus::Color(0, 0, 0));

        int alpha = (mCurrenAttackFrame == 0) ? 64 :
            (mCurrenAttackFrame == 1) ? 128 :
            (mCurrenAttackFrame == 2) ? 192 : 255;
        Gdiplus::Pen pen(Gdiplus::Color(alpha, 255, 0, 0), 2.0f);

        float dx = p.GetPositionX() - mX;
        float dy = p.GetPositionY() - mY;
        float distance = static_cast<float>(sqrt(dx * dx + dy * dy));
        float directionX = (distance > 0.0f) ? dx / distance : (state == EnemyState::RIGHT ? 1.0f : -1.0f);
        float directionY = (distance > 0.0f) ? dy / distance : 0.0f;
        float angle = static_cast<float>(atan2(dy, dx) * 180.0 / 3.1415926535);

        float bowOffset = 10.0f;
        Gdiplus::PointF startPoint(mX + directionX * bowOffset, mY + directionY * bowOffset);
        Gdiplus::PointF endPoint(mX + directionX * AttackRange * 2, mY + directionY * AttackRange * 2);
        graphics.DrawLine(&pen, startPoint, endPoint);

        int bowFrame = (std::min)(mCurrenAttackFrame, 3);
        CImage* bowImage = &mBowAttackAnimation[bowFrame];
        int bowWidth = static_cast<int>(bowImage->GetWidth() * 0.4f);
        int bowHeight = static_cast<int>(bowImage->GetHeight() * 0.4f);
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
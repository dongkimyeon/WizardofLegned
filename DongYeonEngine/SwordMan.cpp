#include "SwordMan.h"
#include "Time.h"
#include <cmath>

SwordMan::SwordMan()
{
    // GDI+ 초기화
    Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    mX = 1920 / 2;
    mY = 1080 / 2;
    speed = 100.0f;
    state = EnemyState::RIGHT;
    mIsMoving = false;
    mCurrentWalkFrame = 0;
    mIsAttack = false;
    mCurrenAttackFrame = 0;
    mIsDead = false;
    mHitTimer = 0.0f;
    mCurrentDeadFrame = 0;
    mIsHit = false;
    mCurrentHitFrame = 0;
    PlayerDetectRange = 300.0f;
    AttackRange = 80.0f;
    mAttackCooldown = 0.0f;
    mAttackFrameTime = 0.0f;
    mAttackDirectionX = 0.0f;
    mAttackDirectionY = 0.0f;
    mHasEffectHitbox = false;
    mHasAttackedPlayer = false; // 공격 플래그 초기화
    hp = 100;
    damage = 20;
    for (int i = 0; i < 4; ++i) mEffectHitboxPoints[i] = { 0, 0 };

    rect = { (int)(mX - 20), (int)(mY - 20), (int)(mX + 20), (int)(mY + 20) };

    // 이미지 로드 (기존 코드 유지)
    mRightIdleAnimation.Load(L"resources/Monster/SwordMan/SwordManRight/Idle/SWORDMAN_RIGHT_0.png");
    if (mRightIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/SwordMan/SwordManRight/Idle/SWORDMAN_RIGHT_0.png\n");
    for (int i = 0; i < 3; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManRight/Attack/SWORDMAN_RIGHT_%d.png", i);
        mRightAttackAnimation[i].Load(path);
        if (mRightAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManRight/Die/SWORDMAN_RIGHT_%d.png", i);
        mRightDieAnimaion[i].Load(path);
        if (mRightDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManRight/Hit/SWORDMAN_RIGHT_%d.png", i);
        mRightHitAnimation[i].Load(path);
        if (mRightHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManRight/Walk/SWORDMAN_RIGHT_%d.png", i);
        mRightWalkAnimation[i].Load(path);
        if (mRightWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    mLeftIdleAnimation.Load(L"resources/Monster/SwordMan/SwordManLeft/Idle/SWORDMAN_LEFT_0.png");
    if (mLeftIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Monster/SwordMan/SwordManLeft/Idle/SWORDMAN_LEFT_0.png\n");
    for (int i = 0; i < 3; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManLeft/Attack/SWORDMAN_LEFT_%d.png", i);
        mLeftAttackAnimation[i].Load(path);
        if (mLeftAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManLeft/Die/SWORDMAN_LEFT_%d.png", i);
        mLeftDieAnimaion[i].Load(path);
        if (mLeftDieAnimaion[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManLeft/Hit/SWORDMAN_LEFT_%d.png", i);
        mLeftHitAnimation[i].Load(path);
        if (mRightHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordManLeft/Walk/SWORDMAN_LEFT_%d.png", i);
        mLeftWalkAnimation[i].Load(path);
        if (mLeftWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    for (int i = 0; i < 4; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/SwordMan/SwordMan_Effect/SWORDMAN_ATTACK_%d.png", i);
        mAttackEffectAnimation[i].Load(path);
        if (mAttackEffectAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
}

SwordMan::~SwordMan()
{
    Gdiplus::GdiplusShutdown(gdiplusToken);
}

void SwordMan::Update(Player& p)
{
    // rect 업데이트
    int imageWidth = mRightIdleAnimation.GetWidth();
    int imageHeight = mRightIdleAnimation.GetHeight();
    rect.left = static_cast<int>(mX - imageWidth / 2.0f) + 45;
    rect.top = static_cast<int>(mY - imageHeight / 2.0f) + 45;
    rect.right = static_cast<int>(mX + imageWidth / 2.0f) - 45;
    rect.bottom = static_cast<int>(mY + imageHeight / 2.0f) - 45;

    // 죽음 상태 처리
    if (mIsDead) {
        static float dieTimer = 0.0f;
        if (mCurrentDeadFrame < 5) { // 0~5까지 6프레임
            dieTimer += Time::DeltaTime();
            if (dieTimer >= 0.1f) {
                mCurrentDeadFrame++;
                dieTimer = 0.0f;
            }
        }
        return;
    }

    // 피격 상태 처리
    float deltaTime = Time::DeltaTime();
    static float animationTimer = 0.0f;
    const float hitFrameDuration = 0.1f;
    if (mIsHit) {
        mHitTimer -= deltaTime;
        if (mHitTimer <= 0.0f) {
            mIsHit = false;
        }
        animationTimer += deltaTime;
        if (animationTimer >= hitFrameDuration) {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2;
            animationTimer = 0.0f;
        }
        return;
    }

    // 플레이어와의 거리 계산
    float playerX = p.GetPositionX();
    float playerY = p.GetPositionY();
    float distance = sqrt(pow(mX - playerX, 2) + pow(mY - playerY, 2));

    // 공격 쿨타임 업데이트
    if (mAttackCooldown > 0.0f)
    {
        mAttackCooldown -= deltaTime;
    }

    // 방향 설정
    if (playerX > mX)
    {
        state = EnemyState::RIGHT;
    }
    else if (playerX < mX)
    {
        state = EnemyState::LEFT;
    }

    // 공격 상태 관리
    if (mIsAttack)
    {
        mAttackFrameTime += deltaTime;
        float frameDuration = (mCurrenAttackFrame == 0) ? 0.7f : 0.2f;
        if (mAttackFrameTime >= frameDuration)
        {
            mCurrenAttackFrame++;
            if (mCurrenAttackFrame >= 3)
            {
                mIsAttack = false;
                mCurrenAttackFrame = 0;
                mAttackCooldown = 3.0f;
                mHasEffectHitbox = false;
                mHasAttackedPlayer = false; // 공격 종료 시 플래그 리셋
            }
            mAttackFrameTime = 0.0f;
        }

        // 공격 이펙트가 활성화된 동안 충돌 감지 및 데미지 처리
        if (mCurrenAttackFrame >= 1 && !mHasAttackedPlayer)
        {
            RECT playerRect = p.GetRect();
            if (CheckCollisionWithRect(playerRect))
            {
                p.TakeDamage(damage);
                mHasAttackedPlayer = true; // 한 번만 데미지 입힘
            }
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
            if (distance > 0.0f)
            {
                mAttackDirectionX = (playerX - mX) / distance;
                mAttackDirectionY = (playerY - mY) / distance;
            }
            else
            {
                mAttackDirectionX = (state == EnemyState::RIGHT) ? 1.0f : -1.0f;
                mAttackDirectionY = 0.0f;
            }
            mHasAttackedPlayer = false; // 공격 시작 시 플래그 리셋
        }
        else if (distance <= PlayerDetectRange)
        {
            mIsAttack = false;
            mIsMoving = true;
            float directionX = (playerX - mX) / distance;
            float directionY = (playerY - mY) / distance;
            mX += directionX * speed * deltaTime;
            mY += directionY * speed * deltaTime;
        }
        else
        {
            mIsAttack = false;
            mIsMoving = false;
            mHasEffectHitbox = false;
        }
    }

    // 히트박스 업데이트 (기존 코드 유지)
    if (mIsAttack && mCurrenAttackFrame >= 1)
    {
        int hitboxWidth = 64;
        int hitboxHeight = 100;
        float effectOffset = 30.0f;
        float centerX = mX + mAttackDirectionX * effectOffset;
        float centerY = mY + mAttackDirectionY * effectOffset;
        float angle = atan2(mAttackDirectionY, mAttackDirectionX);

        POINT basePoints[4] = {
            { -hitboxWidth / 2, -hitboxHeight / 2 },
            {  hitboxWidth / 2, -hitboxHeight / 2 },
            {  hitboxWidth / 2,  hitboxHeight / 2 },
            { -hitboxWidth / 2,  hitboxHeight / 2 }
        };

        for (int i = 0; i < 4; ++i)
        {
            float x = (float)basePoints[i].x;
            float y = (float)basePoints[i].y;
            mEffectHitboxPoints[i].x = static_cast<LONG>(centerX + (x * cos(angle) - y * sin(angle)));
            mEffectHitboxPoints[i].y = static_cast<LONG>(centerY + (x * sin(angle) + y * cos(angle)));
        }

        mHasEffectHitbox = true;
    }
    else
    {
        mHasEffectHitbox = false;
    }

    // 이동 애니메이션 프레임 업데이트 (기존 코드 유지)
    if (!mIsDead && !mIsHit && !mIsAttack && mIsMoving)
    {
        static float frameTime = 0.0f;
        frameTime += deltaTime;
        if (frameTime >= 0.1f)
        {
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 6;
            frameTime = 0.0f;
        }
    }
}

void SwordMan::LateUpdate()
{
}

void SwordMan::Render(HDC hdc, Player& p)
{
    // 디버그 범위 그리기 (기존 코드 유지)
    HPEN attackPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN detectPen = CreatePen(PS_DOT, 1, RGB(0, 255, 0));
    HBRUSH nullBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
    HPEN oldPen = (HPEN)SelectObject(hdc, attackPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, nullBrush);

    Ellipse(hdc,
        static_cast<int>(mX - AttackRange),
        static_cast<int>(mY - AttackRange),
        static_cast<int>(mX + AttackRange),
        static_cast<int>(mY + AttackRange));

    SelectObject(hdc, detectPen);
    Ellipse(hdc,
        static_cast<int>(mX - PlayerDetectRange),
        static_cast<int>(mY - PlayerDetectRange),
        static_cast<int>(mX + PlayerDetectRange),
        static_cast<int>(mY + PlayerDetectRange));

    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(attackPen);
    DeleteObject(detectPen);

    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);

    // 히트박스 디버그 그리기 (기존 코드 유지)
    if (mHasEffectHitbox)
    {
        HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
        HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
        Polygon(hdc, mEffectHitboxPoints, 4);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hitboxPen);
    }

    CImage* currentImage = nullptr;
    if (mIsDead)
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

    int imageWidth = 139 - 45;
    int imageHeight = 151 - 45;
    int drawX = static_cast<int>(mX - imageWidth / 2.0f);
    int drawY = static_cast<int>(mY - imageHeight / 2.0f);

    currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);

    if (mIsAttack && mCurrenAttackFrame >= 1)
    {
        int effectFrame;
        if (mCurrenAttackFrame == 1)
        {
            effectFrame = (mAttackFrameTime < 0.1f) ? 0 : 1;
        }
        else if (mCurrenAttackFrame == 2)
        {
            effectFrame = (mAttackFrameTime < 0.1f) ? 2 : 3;
        }
        else
        {
            effectFrame = 0;
        }

        CImage* effectImage = &mAttackEffectAnimation[effectFrame];
        int effectWidth = effectImage->GetWidth();
        int effectHeight = effectImage->GetHeight();

        Gdiplus::Graphics graphics(hdc);
        graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
        Gdiplus::ImageAttributes imageAttr;
        imageAttr.SetColorKey(Gdiplus::Color(0, 0, 0), Gdiplus::Color(0, 0, 0));

        float directionX = mAttackDirectionX;
        float directionY = mAttackDirectionY;
        float angle = atan2(directionY, directionX) * 180.0f / 3.1415926535f;

        float effectOffset = 30.0f;
        int effectDrawX = static_cast<int>(mX + directionX * effectOffset - effectWidth / 2.0f);
        int effectDrawY = static_cast<int>(mY + directionY * effectOffset - effectHeight / 2.0f);

        Gdiplus::Matrix matrix;
        matrix.RotateAt(angle, Gdiplus::PointF(mX + directionX * effectOffset, mY + directionY * effectOffset));
        graphics.SetTransform(&matrix);

        Gdiplus::Bitmap effectBitmap((HBITMAP)*effectImage, nullptr);
        graphics.DrawImage(&effectBitmap,
            Gdiplus::Rect(effectDrawX, effectDrawY, effectWidth, effectHeight),
            0, 0, effectBitmap.GetWidth(), effectBitmap.GetHeight(),
            Gdiplus::UnitPixel, &imageAttr);

        graphics.ResetTransform();
    }
}

void SwordMan::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
}

void SwordMan::TakeDamage(int d)
{
    if (mIsDead) return;
    hp -= d;
    if (hp <= 0) {
        hp = 0;
        mIsDead = true;
        mIsHit = false;
        mIsAttack = false;
        mIsMoving = false;
        mCurrentDeadFrame = 0;
    }
    else {
        mIsHit = true;
        mHitTimer = 0.2f;
        mCurrentHitFrame = 0;
        mIsAttack = false;
        mIsMoving = false;
    }
}

bool SwordMan::GetEffectHitbox(POINT outPoints[4])
{
    for (int i = 0; i < 4; ++i) outPoints[i] = mEffectHitboxPoints[i];
    return mHasEffectHitbox;
}

bool SwordMan::CheckPointInPolygon(POINT& point, POINT polygon[4])
{
    int crossings = 0;
    for (int i = 0; i < 4; ++i) {
        auto p1 = polygon[i];
        auto p2 = polygon[(i + 1) % 4];
        if (((p1.y > point.y) != (p2.y > point.y)) &&
            (point.x < (p2.x - p1.x) * (point.y - p1.y) / (p2.y - p1.y) + p1.x)) {
            crossings++;
        }
    }
    return (crossings % 2) == 1;
}

bool SwordMan::CheckCollisionWithRect(RECT& otherRect)
{
    if (!mHasEffectHitbox) return false;

    POINT rectPoints[4] = {
        { otherRect.left, otherRect.top },
        { otherRect.right, otherRect.top },
        { otherRect.right, otherRect.bottom },
        { otherRect.left, otherRect.bottom }
    };

    for (auto& p : rectPoints) {
        if (CheckPointInPolygon(p, mEffectHitboxPoints)) {
            return true;
        }
    }

    for (auto& p : mEffectHitboxPoints) {
        if (p.x >= otherRect.left && p.x <= otherRect.right &&
            p.y >= otherRect.top && p.y <= otherRect.bottom) {
            return true;
        }
    }

    return false;
}
#include "Player.h"
#include "Input.h"
#include "Time.h"

Player::Player()
{
    mX = 1920 / 2;
    mY = 1080 / 2;
    rect = { (int)(mX - 20), (int)(mY + 200), (int)(mX + 20), (int)(mY + 20) };
    speed = 200.0f;
    state = PlayerState::FRONT;
    hp = 100;
    damage = 20;
    mIsMoving = false;
    mIsDashing = false;
    mIsAttack = false;
    mIsHit = false;
    mIsDead = false;
    mCurrentWalkFrame = 0;
    mCurrentDashFrame = 0;
    mCurrentAttackFrame = 0;
    mCurrentHitFrame = 0;
    mCurrentDieFrame = 0;
    mDashTimer = 0.0f;
    mAttackTimer = 0.0f;
    mHitTimer = 0.0f;
    mMouseClickFlag = false;

    mHasEffectHitbox = false;
    for (int i = 0; i < 4; ++i) mEffectHitboxPoints[i] = { 0, 0 }; // 히트박스 초기화

    // Front 애니메이션 로드
    mFrontIdleAnimation.Load(L"resources/Player/Front/Idle/FRONT_COMPLETE_00.png");
    if (mFrontIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Player/Front/Idle/FRONT_COMPLETE_00.png\n");
    for (int i = 0; i < 16; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Attack/FRONT_COMPLETE_%02d.png", i);
        mFrontAttackAnimation[i].Load(path);
        if (mFrontAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 8; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Dash/FRONT_COMPLETE_%02d.png", i);
        mFrontDashAnimation[i].Load(path);
        if (mFrontDashAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 7; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Die/FRONT_COMPLETE_%02d.png", i);
        mFrontDieAnimation[i].Load(path);
        if (mFrontDieAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Hit/FRONT_COMPLETE_%02d.png", i);
        mFrontHitAnimation[i].Load(path);
        if (mFrontHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 10; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Front/Walk/FRONT_COMPLETE_%02d.png", i);
        mFrontWalkAnimation[i].Load(path);
        if (mFrontWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Back 애니메이션 로드
    mBackIdleAnimation.Load(L"resources/Player/Back/Idle/BACK_COMPLETE_00.png");
    if (mBackIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Player/Back/Idle/BACK_COMPLETE_00.png\n");
    for (int i = 0; i < 16; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Back/Attack/BACK_COMPLETE_%02d.png", i);
        mBackAttackAnimation[i].Load(path);
        if (mBackAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 8; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Back/Dash/BACK_COMPLETE_%02d.png", i);
        mBackDashAnimation[i].Load(path);
        if (mBackDashAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Back/Hit/BACK_COMPLETE_%02d.png", i);
        mBackHitAnimation[i].Load(path);
        if (mBackHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 10; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Back/Walk/BACK_COMPLETE_%02d.png", i);
        mBackWalkAnimation[i].Load(path);
        if (mBackWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Right 애니메이션 로드
    mRightIdleAnimation.Load(L"resources/Player/Right/Idle/RIGHT_COMPLETE_00.png");
    if (mRightIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Player/Right/Idle/RIGHT_COMPLETE_00.png\n");
    for (int i = 0; i < 16; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Right/Attack/RIGHT_COMPLETE_%02d.png", i);
        mRightAttackAnimation[i].Load(path);
        if (mRightAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 8; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Right/Dash/RIGHT_COMPLETE_%02d.png", i);
        mRightDashAnimation[i].Load(path);
        if (mRightDashAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Right/Hit/RIGHT_COMPLETE_%02d.png", i);
        mRightHitAnimation[i].Load(path);
        if (mRightHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 10; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Right/Walk/RIGHT_COMPLETE_%02d.png", i);
        mRightWalkAnimation[i].Load(path);
        if (mRightWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }

    // Left 애니메이션 로드
    mLeftIdleAnimation.Load(L"resources/Player/Left/Idle/LEFT_COMPLETE_00.png");
    if (mLeftIdleAnimation.IsNull()) wprintf(L"Failed to load: resources/Player/Left/Idle/LEFT_COMPLETE_00.png\n");
    for (int i = 0; i < 16; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Left/Attack/LEFT_COMPLETE_%02d.png", i);
        mLeftAttackAnimation[i].Load(path);
        if (mLeftAttackAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 8; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Left/Dash/LEFT_COMPLETE_%02d.png", i);
        mLeftDashAnimation[i].Load(path);
        if (mLeftDashAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 2; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Left/Hit/LEFT_COMPLETE_%02d.png", i);
        mLeftHitAnimation[i].Load(path);
        if (mLeftHitAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 10; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Left/Walk/LEFT_COMPLETE_%02d.png", i);
        mLeftWalkAnimation[i].Load(path);
        if (mLeftWalkAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    //공격 이펙트 애니메이션 로드
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Attack/LSlashEffect/LSlash%02d.png", i);
        mLeftAttackEffectAnimation[i].Load(path);
        if (mLeftAttackEffectAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Attack/RSlashEffect/RSlash%02d.png", i);
        mRightAttackEffectAnimation[i].Load(path);
        if (mRightAttackEffectAnimation[i].IsNull()) wprintf(L"Failed to load: %s\n", path);
    }
}

void Player::Update()
{
    if (mIsDead) {
        static float dieTimer = 0.0f;
        if (mCurrentDieFrame < 6) {
            dieTimer += Time::DeltaTime();
            if (dieTimer >= 0.1f) {
                mCurrentDieFrame++;
                dieTimer = 0.0f;
            }
        }
        return;
    }

    bool isMoving = false;
    float deltaTime = Time::DeltaTime();
    static float animationTimer = 0.0f;
    const float walkFrameDuration = 0.1f;
    const float dashDuration = 0.4f;
    const float dashFrameDuration = dashDuration / 8.0f;
    const float attackDuration = 0.8f;
    const float attackFrameDuration = attackDuration / 16.0f;
    const float hitDuration = 0.2f;
    const float hitFrameDuration = hitDuration / 2.0f;
    float currentSpeed = speed;


    // rect 업데이트
    int imageWidth = mFrontIdleAnimation.GetWidth();
    int imageHeight = mFrontIdleAnimation.GetHeight();
    rect.left = static_cast<int>(mX - imageWidth / 2.0f) + 30;
    rect.top = static_cast<int>(mY - imageHeight / 2.0f) + 10;
    rect.right = static_cast<int>(mX + imageWidth / 2.0f) - 18;
    rect.bottom = static_cast<int>(mY + imageHeight / 2.0f);

    // 피격 상태 처리
    if (mIsHit) {
        mHitTimer -= deltaTime;
        if (mHitTimer <= 0.0f) mIsHit = false;
        animationTimer += deltaTime;
        if (animationTimer >= hitFrameDuration) {
            mCurrentHitFrame = (mCurrentHitFrame + 1) % 2;
            animationTimer = 0.0f;
        }
        return;
    }

    // 공격 처리
    if (!mIsAttack && Input::GetKeyDown(eKeyCode::LButton)) {
        mIsAttack = true;
        mAttackTimer = attackDuration;
        mCurrentAttackFrame = 0;
        animationTimer = 0.0f;
        mMouseClickFlag = !mMouseClickFlag;

        Vector2 mousePos = Input::GetMousePosition();
        float dx = mousePos.x - mX;
        float dy = mousePos.y - mY;
        attackAngle = atan2f(dy, dx);

        // 방향 설정
        float angle = attackAngle * 180.0f / 3.14159f;
        if (angle >= -45.0f && angle < 45.0f) state = PlayerState::RIGHT;
        else if (angle >= 45.0f && angle < 135.0f) state = PlayerState::FRONT;
        else if (angle >= 135.0f || angle < -135.0f) state = PlayerState::LEFT;
        else state = PlayerState::BACK;
    }
    if (mIsAttack) {
        mAttackTimer -= deltaTime;
        if (mAttackTimer <= 0.0f) {
            mIsAttack = false;
            mCurrentAttackFrame = 0; // 공격 종료 시 프레임 리셋
            mHasEffectHitbox = false; // 충돌 범위 비활성화
            for (int i = 0; i < 4; ++i) mEffectHitboxPoints[i] = { 0, 0 }; // 히트박스 초기화
            animationTimer = 0.0f;
        }
        else {
            animationTimer += deltaTime;
            if (animationTimer >= attackFrameDuration) {
                mCurrentAttackFrame++;
                if (mCurrentAttackFrame >= 16) {
                    mCurrentAttackFrame = 0; // 16프레임 도달 시 리셋
                    mIsAttack = false; // 공격 종료
                    mHasEffectHitbox = false; // 충돌 범위 비활성화
                    for (int i = 0; i < 4; ++i) mEffectHitboxPoints[i] = { 0, 0 }; // 히트박스 초기화
                }
                animationTimer = 0.0f;
            }

            // 히트박스 업데이트
            if (mCurrentAttackFrame >= 4 && mCurrentAttackFrame < 16) { // 4~15프레임 동안 히트박스 활성화
                int hitboxWidth = 65;
                int hitboxHeight = 70;
                float effectOffset = 27.0f;
                float centerX = mX + cos(attackAngle) * effectOffset;
                float centerY = mY + sin(attackAngle) * effectOffset;

                POINT basePoints[4] = {
                    { -hitboxWidth / 2, -hitboxHeight / 2 },
                    { hitboxWidth / 2, -hitboxHeight / 2 },
                    { hitboxWidth / 2, hitboxHeight / 2 },
                    { -hitboxWidth / 2, hitboxHeight / 2 }
                };

                for (int i = 0; i < 4; ++i) {
                    float x = (float)basePoints[i].x;
                    float y = (float)basePoints[i].y;
                    mEffectHitboxPoints[i].x = static_cast<LONG>(centerX + (x * cos(attackAngle) - y * sin(attackAngle)));
                    mEffectHitboxPoints[i].y = static_cast<LONG>(centerY + (x * sin(attackAngle) + y * cos(attackAngle)));
                }
                mHasEffectHitbox = true;
            }
            else {
                mHasEffectHitbox = false;
                for (int i = 0; i < 4; ++i) mEffectHitboxPoints[i] = { 0, 0 }; // 히트박스 초기화
            }
        }
        return;
    }


    // 대쉬 처리
    if (!mIsDashing && Input::GetKeyDown(eKeyCode::SPACE)) {
        mIsDashing = true;
        mDashTimer = dashDuration;
        mCurrentDashFrame = 0;
        animationTimer = 0.0f;
    }
    if (mIsDashing) {
        currentSpeed = 400.0f;
        mDashTimer -= deltaTime;
        if (mDashTimer <= 0.0f) mIsDashing = false;
        animationTimer += deltaTime;
        if (animationTimer >= dashFrameDuration) {
            mCurrentDashFrame = (mCurrentDashFrame + 1) % 8;
            animationTimer = 0.0f;
        }
    }

    // 이동 로직
    if (mIsDashing) {
        switch (state) {
        case PlayerState::BACK: mY -= currentSpeed * deltaTime; break;
        case PlayerState::LEFT: mX -= currentSpeed * deltaTime; break;
        case PlayerState::FRONT: mY += currentSpeed * deltaTime; break;
        case PlayerState::RIGHT: mX += currentSpeed * deltaTime; break;
        }
        isMoving = true;
    }
    else {
        if (Input::GetKey(eKeyCode::W)) {
            state = PlayerState::BACK;
            mY -= currentSpeed * deltaTime;
            isMoving = true;
        }
        if (Input::GetKey(eKeyCode::A)) {
            state = PlayerState::LEFT;
            mX -= currentSpeed * deltaTime;
            isMoving = true;
        }
        if (Input::GetKey(eKeyCode::S)) {
            state = PlayerState::FRONT;
            mY += currentSpeed * deltaTime;
            isMoving = true;
        }
        if (Input::GetKey(eKeyCode::D)) {
            state = PlayerState::RIGHT;
            mX += currentSpeed * deltaTime;
            isMoving = true;
        }
    }

    // 이동 애니메이션
    if (isMoving && !mIsDashing && !mIsAttack && !mIsHit) {
        animationTimer += deltaTime;
        if (animationTimer >= walkFrameDuration) {
            mCurrentWalkFrame = (mCurrentWalkFrame + 1) % 10;
            animationTimer = 0.0f;
        }
    }
    else if (!mIsDashing && !mIsAttack && !mIsHit) {
        mCurrentWalkFrame = 0;
        animationTimer = 0.0f;
    }

    mIsMoving = isMoving;
}
void Player::LateUpdate()
{
}

void Player::Render(HDC hdc)
{

    Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
    // 디버그 히트박스 렌더링
    if (mHasEffectHitbox) {
        HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
        HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
        Polygon(hdc, mEffectHitboxPoints, 4);
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(hitboxPen);
    }

    CImage* currentImage = nullptr;
    int imageWidth = 0;
    int imageHeight = 0;

    // 상태에 따라 이미지 선택 (기존 로직 유지)
    if (mIsDead) {
        currentImage = &mFrontDieAnimation[mCurrentDieFrame];
    }
    else if (mIsHit) {
        switch (state) {
        case PlayerState::FRONT: currentImage = &mFrontHitAnimation[mCurrentHitFrame]; break;
        case PlayerState::BACK: currentImage = &mBackHitAnimation[mCurrentHitFrame]; break;
        case PlayerState::LEFT: currentImage = &mLeftHitAnimation[mCurrentHitFrame]; break;
        case PlayerState::RIGHT: currentImage = &mRightHitAnimation[mCurrentHitFrame]; break;
        }
    }
    else if (mIsAttack) {
        switch (state) {
        case PlayerState::FRONT: currentImage = &mFrontAttackAnimation[mCurrentAttackFrame]; break;
        case PlayerState::BACK: currentImage = &mBackAttackAnimation[mCurrentAttackFrame]; break;
        case PlayerState::LEFT: currentImage = &mLeftAttackAnimation[mCurrentAttackFrame]; break;
        case PlayerState::RIGHT: currentImage = &mRightAttackAnimation[mCurrentAttackFrame]; break;
        }
    }
    else if (mIsMoving) {
        if (mIsDashing) {
            switch (state) {
            case PlayerState::FRONT: currentImage = &mFrontDashAnimation[mCurrentDashFrame]; break;
            case PlayerState::BACK: currentImage = &mBackDashAnimation[mCurrentDashFrame]; break;
            case PlayerState::LEFT: currentImage = &mLeftDashAnimation[mCurrentDashFrame]; break;
            case PlayerState::RIGHT: currentImage = &mRightDashAnimation[mCurrentDashFrame]; break;
            }
        }
        else {
            switch (state) {
            case PlayerState::FRONT: currentImage = &mFrontWalkAnimation[mCurrentWalkFrame]; break;
            case PlayerState::BACK: currentImage = &mBackWalkAnimation[mCurrentWalkFrame]; break;
            case PlayerState::LEFT: currentImage = &mLeftWalkAnimation[mCurrentWalkFrame]; break;
            case PlayerState::RIGHT: currentImage = &mRightWalkAnimation[mCurrentWalkFrame]; break;
            }
        }
    }
    else {
        switch (state) {
        case PlayerState::FRONT: currentImage = &mFrontIdleAnimation; break;
        case PlayerState::BACK: currentImage = &mBackIdleAnimation; break;
        case PlayerState::LEFT: currentImage = &mLeftIdleAnimation; break;
        case PlayerState::RIGHT: currentImage = &mRightIdleAnimation; break;
        }
    }

    // 플레이어 이미지 렌더링
    if (currentImage != nullptr) {
        imageWidth = currentImage->GetWidth();
        imageHeight = currentImage->GetHeight();
        int drawX = static_cast<int>(mX - imageWidth / 2.0f);
        int drawY = static_cast<int>(mY - imageHeight / 2.0f);
        currentImage->Draw(hdc, drawX, drawY, imageWidth, imageHeight);
    }

    // 공격 이펙트 렌더링
    if (mIsAttack && mCurrentAttackFrame >= 4) { // 4프레임부터 이펙트 출력
        CImage* effectImage = nullptr;
        int effectFrame = 0;
        if (mCurrentAttackFrame < 8) 
        {
            // 레프트 이펙트: 4~7프레임 -> 0~3
            effectFrame = (mCurrentAttackFrame - 4) * 6 / 4; // 4프레임을 6프레임으로 확장
            effectImage = &mLeftAttackEffectAnimation[effectFrame % 6];
        }
        else {
            // 라이트 이펙트: 8~15프레임 -> 0~5
            effectFrame = (mCurrentAttackFrame - 8) * 6 / 8; // 8프레임을 6프레임으로 매핑
            effectImage = &mRightAttackEffectAnimation[effectFrame % 6];
        }

        if (effectImage != nullptr) {
            int effectWidth = effectImage->GetWidth() + 50;
            int effectHeight = effectImage->GetHeight() + 30;
            float effectOffset = 20.0f;
            int effectDrawX = static_cast<int>(mX + cos(attackAngle) * effectOffset - effectWidth / 2.0f);
            int effectDrawY = static_cast<int>(mY + sin(attackAngle) * effectOffset - effectHeight / 2.0f);

            Gdiplus::Graphics graphics(hdc);
            graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
            Gdiplus::ImageAttributes imageAttr;
            imageAttr.SetColorKey(Gdiplus::Color(0, 0, 0), Gdiplus::Color(0, 0, 0));

            float angle = attackAngle * 180.0f / 3.1415926535f;
            Gdiplus::Matrix matrix;
            matrix.RotateAt(angle, Gdiplus::PointF(mX + cos(attackAngle) * effectOffset, mY + sin(attackAngle) * effectOffset));
            graphics.SetTransform(&matrix);

            Gdiplus::Bitmap effectBitmap((HBITMAP)*effectImage, nullptr);
            graphics.DrawImage(&effectBitmap,
                Gdiplus::Rect(effectDrawX, effectDrawY, effectWidth, effectHeight),
                0, 0, effectBitmap.GetWidth(), effectBitmap.GetHeight(),
                Gdiplus::UnitPixel, &imageAttr);

            graphics.ResetTransform();
        }
    }
}

void Player::SetPosition(float x, float y)
{
    mX = x;
    mY = y;
    position.x = x;
    position.y = y;
}

void Player::TakeDamage(int d)
{
    if (mIsDead) return;
    hp -= d;
    if (hp <= 0) {
        hp = 0;
        mIsDead = true;
        mIsHit = false;
        mIsAttack = false;
        mIsDashing = false;
        mIsMoving = false;
        mCurrentDieFrame = 0;
    }
    else {
        mIsHit = true;
        mHitTimer = 0.2f; // 피격 애니메이션 0.2초
        mCurrentHitFrame = 0;
        mIsAttack = false;
        mIsDashing = false;
    }
}

float Player::GetPositionX()
{
    return mX;
}

float Player::GetPositionY()
{
    return mY;
}

float Player::GetSpeed()
{
    return speed;
}

RECT Player::GetRect()
{
    return rect;
}


bool Player::GetEffectHitbox(POINT outPoints[4])
{
    for (int i = 0; i < 4; ++i) outPoints[i] = mEffectHitboxPoints[i];
    return mHasEffectHitbox;
}


bool Player::CheckPointInPolygon(POINT& point, POINT polygon[4])
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


bool Player::CheckCollisionWithRect(RECT& otherRect)
{
    if (!mHasEffectHitbox) return false;

    // 플레이어 사각형 꼭짓점
    POINT rectPoints[4] = {
        { otherRect.left, otherRect.top },
        { otherRect.right, otherRect.top },
        { otherRect.right, otherRect.bottom },
        { otherRect.left, otherRect.bottom }
    };

    // 1. 플레이어 꼭짓점이 이펙트 사각형 내부에 있는지
    for (auto& p : rectPoints) {
        if (CheckPointInPolygon(p, mEffectHitboxPoints)) {
            return true;
        }
    }

    // 2. 이펙트 사각형 꼭짓점이 플레이어 사각형 내부에 있는지
    for (auto& p : mEffectHitboxPoints) {
        if (p.x >= otherRect.left && p.x <= otherRect.right &&
            p.y >= otherRect.top && p.y <= otherRect.bottom) {
            return true;
        }
    }

    return false;
}



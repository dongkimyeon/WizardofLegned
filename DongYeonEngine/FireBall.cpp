#include "FireBall.h"
#include "Stage1.h"
#include "Time.h"
#include <cmath>

FireBall::FireBall(float x, float y, float dirX, float dirY)
    : mX(x), mY(y), mDirectionX(dirX), mDirectionY(dirY), speed(400.0f), mIsActive(true), damage(10),
    mCurrentFrame(0), mAnimationTimer(0.0f)
{
    for (int i = 0; i < 6; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Monster/WIZARD/WizardFire/Attack/WIZARD_FIRE_%02d.png", i);
        HRESULT hr = mFireBallAnimation[i].Load(path);
        if (FAILED(hr)) wprintf(L"Failed to load: %s\n", path);
    }
    UpdateHitbox();
}

FireBall::~FireBall()
{
    for (int i = 0; i < 6; ++i)
    {
        mFireBallAnimation[i].Destroy();
    }
}

void FireBall::Update(Player& player)
{
    if (!mIsActive) return;

    // 위치 업데이트
    mX += mDirectionX * speed * Time::DeltaTime();
    mY += mDirectionY * speed * Time::DeltaTime();
    UpdateHitbox();

    // 애니메이션 업데이트
    mAnimationTimer += Time::DeltaTime();
    if (mAnimationTimer >= mFrameDuration)
    {
        mCurrentFrame = (mCurrentFrame + 1) % 6; // 0~5 순환
        mAnimationTimer -= mFrameDuration;
    }

    // 플레이어 충돌 체크
    if (CheckCollisionWithRect(player.GetRect()))
    {
        player.TakeDamage(damage);
        mIsActive = false;
        return;
    }

    // 화면 밖으로 나가면 비활성화
    if (mX < 0 || mX > 1920 || mY < 0 || mY > 1080)
    {
        mIsActive = false;
        return;
    }
}

void FireBall::Render(HDC hdc)
{
    if (!mIsActive) return;

    // 히트박스 그리기
    HPEN hitboxPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
    HPEN oldPen = (HPEN)SelectObject(hdc, hitboxPen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, (HBRUSH)GetStockObject(NULL_BRUSH));
    Polygon(hdc, hitboxPoints, 4);
    SelectObject(hdc, oldPen);
    SelectObject(hdc, oldBrush);
    DeleteObject(hitboxPen);

    // 파이어볼 이미지 렌더링
    int imageWidth = mFireBallAnimation[mCurrentFrame].GetWidth();
    int imageHeight = mFireBallAnimation[mCurrentFrame].GetHeight();
    float scale = 1.5f;
    int renderWidth = static_cast<int>(imageWidth * scale);
    int renderHeight = static_cast<int>(imageHeight * scale);
    int drawX = static_cast<int>(mX - renderWidth / 2.0f);
    int drawY = static_cast<int>(mY - renderHeight / 2.0f);

    Gdiplus::Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
    Gdiplus::ImageAttributes imageAttr;
    imageAttr.SetColorKey(Gdiplus::Color(0, 0, 0), Gdiplus::Color(0, 0, 0));

    float angle = static_cast<float>(atan2(mDirectionY, mDirectionX) * 180.0 / 3.1415926535);
    Gdiplus::Matrix matrix;
    matrix.RotateAt(angle, Gdiplus::PointF(mX, mY));
    graphics.SetTransform(&matrix);

    Gdiplus::Bitmap arrowBitmap((HBITMAP)mFireBallAnimation[mCurrentFrame], nullptr);
    graphics.DrawImage(&arrowBitmap,
        Gdiplus::Rect(drawX, drawY, renderWidth, renderHeight),
        0, 0, imageWidth, imageHeight,
        Gdiplus::UnitPixel, &imageAttr);

    graphics.ResetTransform();
}

void FireBall::UpdateHitbox()
{
    float scale = 0.6f;
    int imageWidth = static_cast<int>(mFireBallAnimation[0].GetWidth() * scale);
    int imageHeight = static_cast<int>(mFireBallAnimation[0].GetHeight() * scale);

    POINT basePoints[4] = {
        { -imageWidth / 2, -imageHeight / 2 }, // 좌상
        {  imageWidth / 2, -imageHeight / 2 }, // 우상
        {  imageWidth / 2,  imageHeight / 2 }, // 우하
        { -imageWidth / 2,  imageHeight / 2 }  // 좌하
    };

    float angle = static_cast<float>(atan2(mDirectionY, mDirectionX));
    float cosA = cos(angle);
    float sinA = sin(angle);

    for (int i = 0; i < 4; ++i)
    {
        float x = basePoints[i].x;
        float y = basePoints[i].y;
        hitboxPoints[i].x = static_cast<LONG>(mX + (x * cosA - y * sinA));
        hitboxPoints[i].y = static_cast<LONG>(mY + (x * sinA + y * cosA));
    }
}

bool FireBall::CheckCollisionWithRect(const RECT& rect)
{
    POINT rectPoints[4] = {
        { rect.left, rect.top },
        { rect.right, rect.top },
        { rect.right, rect.bottom },
        { rect.left, rect.bottom }
    };

    auto PointInPolygon = [](const POINT& point, const POINT polygon[4]) {
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
        };

    for (const auto& p : rectPoints)
    {
        if (PointInPolygon(p, hitboxPoints))
        {
            return true;
        }
    }

    for (const auto& p : hitboxPoints)
    {
        if (p.x >= rect.left && p.x <= rect.right && p.y >= rect.top && p.y <= rect.bottom)
        {
            return true;
        }
    }

    return false;
}

void FireBall::ThrowFireBall(Player& player, float mX, float mY, Stage1* stage)
{
    float playerX = player.GetPositionX();
    float playerY = player.GetPositionY();
    float distance = sqrt(pow(mX - playerX, 2) + pow(mY - playerY, 2));
    float dirX = (distance > 0.0f) ? (playerX - mX) / distance : 1.0f;
    float dirY = (distance > 0.0f) ? (playerY - mY) / distance : 0.0f;

    stage->AddFireBall(new FireBall(mX, mY, dirX, dirY));
}
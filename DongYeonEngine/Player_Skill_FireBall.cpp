﻿#include "Player_Skill_FireBall.h"
#include "Stage1.h"
#include "Time.h"
#include "SoundManager.h"
#include <cmath>
#include <random>
#include <iostream>

Player_Skill_FireBall::Player_Skill_FireBall(float x, float y, float dirX, float dirY)
    : mX(x), mY(y), mDirectionX(dirX), mDirectionY(dirY), speed(800.0f), mIsActive(true), damage(80),
    mCurrentFrame(0), mAnimationTimer(0.0f), mParticleTimer(0.0f), mParticleSpawnInterval(0.15f),
    mFrameDuration(0.1f)
{
    // 마우스 방향(진행 방향)으로 오프셋 적용
    float offsetDistance = 50.0f;
    mX += dirX * offsetDistance;
    mY += dirY * offsetDistance;

    // 파이어볼 애니메이션 이미지 로드
    for (int i = 0; i < 5; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/Skill_FireBall/Fireball_%02d.png", i);
        mFireBallAnimation[i].Load(path);
    }

    // 파티클 이미지 로드
    for (int i = 0; i < 20; ++i)
    {
        wchar_t path[256];
        swprintf_s(path, L"resources/Player/FireEffect/FIRE_PARTICLE_%02d.png", i);
        if (mFireParticleImage[i].Load(path) != S_OK) {
            std::cout << "Failed to load particle image: " << i << std::endl;
        }
    }

    
    UpdateHitbox();
}

Player_Skill_FireBall::~Player_Skill_FireBall()
{
    for (int i = 0; i < 5; ++i)
    {
        mFireBallAnimation[i].Destroy();
    }
    for (int i = 0; i < 20; ++i)
    {
        mFireParticleImage[i].Destroy();
    }
    SoundManager::GetInstance()->mPlaySound("FireBlastEnd", false);
}

void Player_Skill_FireBall::Move()
{
    if (!mIsActive) return;

    // 위치 업데이트
    mX += mDirectionX * speed * Time::DeltaTime();
    mY += mDirectionY * speed * Time::DeltaTime();
    UpdateHitbox();

    // 파티클 업데이트
    UpdateParticles();

    // 애니메이션 업데이트
    mAnimationTimer += Time::DeltaTime();
    if (mAnimationTimer >= mFrameDuration)
    {
        mCurrentFrame = (mCurrentFrame + 1) % 5;
        mAnimationTimer -= mFrameDuration;
    }

    // 맵 경계 체크
    const float mapWidth = 5000.0f;
    const float mapHeight = 5000.0f;
    if (mX < 0 || mX > mapWidth || mY < 0 || mY > mapHeight)
    {
        mIsActive = false;
    }
}

bool Player_Skill_FireBall::CheckCollision(GameObject& obj)
{
    if (!mIsActive || obj.GetIsDead()) return false;

    if (CheckCollisionWithRect(obj.GetRect()))
    {
        obj.TakeDamage(damage);
        mIsActive = false;
        return true;
    }
    return false;
}

void Player_Skill_FireBall::Render(HDC hdc)
{
    if (!mIsActive) return;

    CImage& currentImage = mFireBallAnimation[mCurrentFrame];
    const int imageWidth = 64;  // 원본 가로
    const int imageHeight = 48; // 원본 세로
    float scale = 2.5f;
    int renderWidth = static_cast<int>(imageWidth * scale);
    int renderHeight = static_cast<int>(imageHeight * scale);

    float angle = static_cast<float>(atan2(mDirectionY, mDirectionX));

    XFORM xForm = { 0 };
    xForm.eM11 = cos(angle);
    xForm.eM12 = sin(angle);
    xForm.eM21 = -sin(angle);
    xForm.eM22 = cos(angle);
    xForm.eDx = mX;
    xForm.eDy = mY;

    SetGraphicsMode(hdc, GM_ADVANCED);
    SetWorldTransform(hdc, &xForm);

    HDC srcDC = currentImage.GetDC();
    TransparentBlt(
        hdc,
        -renderWidth / 2, -renderHeight / 2, renderWidth, renderHeight,
        srcDC,
        0, 0, imageWidth, imageHeight,
        RGB(0, 0, 0)
    );
    currentImage.ReleaseDC();

    XFORM identity = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
    SetWorldTransform(hdc, &identity);
    SetGraphicsMode(hdc, GM_COMPATIBLE);

    // 파티클 렌더링
    for (const auto& particle : mParticles)
    {
        CImage& particleImage = mFireParticleImage[particle.frame];
        int pWidth = particleImage.GetWidth();
        int pHeight = particleImage.GetHeight();
        float pScale = 1.2f;
        int pRenderWidth = static_cast<int>(pWidth * pScale);
        int pRenderHeight = static_cast<int>(pHeight * pScale);
        int pDrawX = static_cast<int>(particle.x - pRenderWidth / 2.0f);
        int pDrawY = static_cast<int>(particle.y - pRenderHeight / 2.0f);

        HDC particleDC = particleImage.GetDC();
        TransparentBlt(
            hdc,
            pDrawX, pDrawY, pRenderWidth, pRenderHeight,
            particleDC,
            0, 0, pWidth, pHeight,
            RGB(0, 0, 0)
        );
        particleImage.ReleaseDC();
    }

    
}

void Player_Skill_FireBall::SpawnParticle()
{
    float magnitude = std::sqrt(mDirectionX * mDirectionX + mDirectionY * mDirectionY);
    float normDirX = (magnitude > 0) ? mDirectionX / magnitude : 0.0f;
    float normDirY = (magnitude > 0) ? mDirectionY / magnitude : 0.0f;

    float tailOffset = -30.0f;
    float spawnX = mX + tailOffset * normDirX;
    float spawnY = mY + tailOffset * normDirY;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> velDist(-5.0f, 5.0f);
    std::uniform_real_distribution<float> lifeDist(0.5f, 1.0f);

    Particle particle;
    particle.x = spawnX;
    particle.y = spawnY;
    particle.velX = mDirectionX * speed * 0.5f + velDist(gen);
    particle.velY = mDirectionY * speed * 0.5f + velDist(gen);
    particle.lifetime = lifeDist(gen);
    particle.initialLifetime = particle.lifetime;
    particle.frame = 0;

    mParticles.emplace_back(particle);
}

void Player_Skill_FireBall::UpdateParticles()
{
    mParticleTimer += Time::DeltaTime();
    if (mParticleTimer >= mParticleSpawnInterval) {
        SpawnParticle();
        mParticleTimer = 0.0f;
    }

    for (auto it = mParticles.begin(); it != mParticles.end();) {
        it->lifetime -= Time::DeltaTime();
        if (it->lifetime <= 0) {
            it = mParticles.erase(it);
            continue;
        }

        it->x += it->velX * Time::DeltaTime();
        it->y += it->velY * Time::DeltaTime();
        it->frame = static_cast<int>(20 * (1.0f - it->lifetime / it->initialLifetime)) % 20;
        ++it;
    }
}

void Player_Skill_FireBall::UpdateHitbox()
{
    float scale = 1.5f;
    int imageWidth = static_cast<int>(mFireBallAnimation[0].GetWidth() * scale) - 40;
    int imageHeight = static_cast<int>(mFireBallAnimation[0].GetHeight() * scale) - 10;

    POINT basePoints[4] = {
        { -imageWidth / 2, -imageHeight / 2 },
        { imageWidth / 2, -imageHeight / 2 },
        { imageWidth / 2, imageHeight / 2 },
        { -imageWidth / 2, imageHeight / 2 }
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

bool Player_Skill_FireBall::CheckCollisionWithRect(const RECT& rect)
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

void Player_Skill_FireBall::Active(float mX, float mY, float angle, Scene* stage)
{
    float dirX = cos(angle);
    float dirY = sin(angle);
    stage->AddPlayerSkillFireBall(new Player_Skill_FireBall(mX, mY, dirX, dirY));
}
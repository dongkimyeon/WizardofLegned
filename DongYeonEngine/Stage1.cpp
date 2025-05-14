#include "Stage1.h"
#include "Input.h"
#include "Time.h"
#include "Archer.h"
#include "Arrow.h"

Stage1::Stage1()
{
    player.SetPosition(1000, 500);
    archer.SetPosition(500, 100);
    swordman.SetPosition(700, 100);
    wizard.SetPosition(900, 100);
    swordmanAttackCooldown = 0.0f;
    playerSlashAttackCooldown = 0.0f;
}

Stage1::~Stage1()
{
    for (Arrow* arrow : arrows)
    {
        delete arrow;
    }
    arrows.clear();
}

void Stage1::Initialize()
{
}

void Stage1::Update()
{
    player.Update();
    archer.Update(player, this);
    wizard.Update(player);
    swordman.Update(player);

    for (auto it = arrows.begin(); it != arrows.end();)
    {
        if ((*it)->IsActive())
        {
            (*it)->Update(player);
            ++it;
        }
        else
        {
            delete* it;
            it = arrows.erase(it);
        }
    }

    if (swordmanAttackCooldown > 0.0f)
    {
        swordmanAttackCooldown -= Time::DeltaTime();
    }
    //플레이어 공격쿨타임
    if (playerSlashAttackCooldown > 0.0f)
    {
        playerSlashAttackCooldown -= Time::DeltaTime();
    }

    RECT playerRect = player.GetRect();
    if (swordmanAttackCooldown <= 0.0f && swordman.CheckCollisionWithRect(playerRect))
    {
        std::cout << "Swordman hit player" << std::endl;
        player.TakeDamage(swordman.GetDamage());
        swordmanAttackCooldown = 0.5f;
    }

    RECT EnemyRect = swordman.GetRect();
    if (playerSlashAttackCooldown <= 0.0f && player.CheckCollisionWithRect(EnemyRect))
    {
        std::cout << "player hit swordman" << std::endl;
        swordman.TakeDamage(player.GetDamage());
        playerSlashAttackCooldown = 0.4;
      
    }
}

void Stage1::LateUpdate()
{
    Scene::LateUpdate();
}

void Stage1::Render(HDC hdc)
{
    // 배경을 검정색으로 채우기
    RECT screenRect = { 0, 0, 1920, 1080 }; // 화면 크기에 맞게 조정 (예: 1920x1080)
    HBRUSH blackBrush = CreateSolidBrush(RGB(200, 200, 200)); // 검정색 브러시 생성
    FillRect(hdc, &screenRect, blackBrush); // 화면을 검정색으로 채움
    DeleteObject(blackBrush); // 브러시 삭제

    wizard.Render(hdc);
    archer.Render(hdc, player);
    swordman.Render(hdc, player);
    player.Render(hdc);

    for (Arrow* arrow : arrows)
    {
        arrow->Render(hdc);
    }

    WCHAR swordManHpText[100];
    wsprintf(swordManHpText, L"enemy Hp : %d", swordman.GetHp());
    TextOut(hdc, 0, 20, swordManHpText, lstrlen(swordManHpText));
    WCHAR PlayerHpText[100];
    wsprintf(PlayerHpText, L"player Hp : %d", player.GetHp());
    TextOut(hdc, 0, 40, PlayerHpText, lstrlen(PlayerHpText));

    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", static_cast<int>(Input::GetMousePosition().x), static_cast<int>(Input::GetMousePosition().y));
    TextOut(hdc, static_cast<int>(Input::GetMousePosition().x) + 10, static_cast<int>(Input::GetMousePosition().y), Text, lstrlen(Text));
}
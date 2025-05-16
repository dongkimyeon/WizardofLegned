#include "Stage1.h"
#include "Input.h"
#include "Time.h"
#include "Archer.h"
#include "Arrow.h"

Stage1::Stage1()
{
    player.SetPosition(1000, 500);

    // 초기 적 객체 추가
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(700, 100);
    swordmans.push_back(new SwordMan());
    swordmans.back()->SetPosition(800, 200);

    wizards.push_back(new Wizard());
    wizards.back()->SetPosition(900, 100);

    archers.push_back(new Archer());
    archers.back()->SetPosition(500, 100);
}

Stage1::~Stage1()
{
    // 동적 할당된 객체 해제
    for (auto* swordman : swordmans) delete swordman;
    for (auto* wizard : wizards) delete wizard;
    for (auto* archer : archers) delete archer;
    for (auto* arrow : arrows) delete arrow;
    for (auto* fireball : fireballs) delete fireball;
}

void Stage1::Initialize()
{
}

void Stage1::Update()
{
    player.Update();

    // 모든 적 객체 업데이트
    for (auto* archer : archers) archer->Update(player, this);
    for (auto* wizard : wizards) wizard->Update(player, this);
    for (auto* swordman : swordmans) swordman->Update(player);

    // 화살 업데이트
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

    // 파이어볼 업데이트
    for (auto it = fireballs.begin(); it != fireballs.end();)
    {
        if ((*it)->IsActive())
        {
            (*it)->Update(player);
            ++it;
        }
        else
        {
            delete* it;
            it = fireballs.erase(it);
        }
    }

    // 플레이어 공격 처리
    POINT effectHitboxPoints[4];
    bool hasEffectHitbox = player.GetEffectHitbox(effectHitboxPoints);

    //피격 처리
    if (hasEffectHitbox)
    {
        for (auto* swordman : swordmans)
        {
            RECT enemyRect = swordman->GetRect();
            if (player.CheckCollisionWithRect(enemyRect) && !swordman->HasBeenHit())
            {
                swordman->TakeDamage(player.GetDamage());
                swordman->SetHitFlag(true); // 피격 플래그 설정
            }
        }
        for (auto* wizard : wizards)
        {
            RECT enemyRect = wizard->GetRect();
            if (player.CheckCollisionWithRect(enemyRect) && !wizard->HasBeenHit())
            {
                wizard->TakeDamage(player.GetDamage());
                wizard->SetHitFlag(true); // 피격 플래그 설정
            }
        }
        for (auto* archer : archers)
        {
            RECT enemyRect = archer->GetRect();
            if (player.CheckCollisionWithRect(enemyRect) && !archer->HasBeenHit())
            {
                archer->TakeDamage(player.GetDamage());
                archer->SetHitFlag(true); // 피격 플래그 설정
            }
        }
    }
    else
    {
        // 공격이 끝났을 때 모든 적의 피격 플래그 리셋
        for (auto* swordman : swordmans) swordman->SetHitFlag(false);
        for (auto* wizard : wizards) wizard->SetHitFlag(false);
        for (auto* archer : archers) archer->SetHitFlag(false);
    }

	// 각 객체들끼리 겹치지않게 처리
    HandleCollision();

}

void Stage1::LateUpdate()
{
    Scene::LateUpdate();
}

void Stage1::Render(HDC hdc)
{
    // 격자 배경 그리기
    int gridSize = 50; // 격자 한 칸의 크기 (픽셀)
    int screenWidth = 1920; // 화면 너비
    int screenHeight = 1080; // 화면 높이

    // 배경을 단색으로 채우기 (선택 사항)
    RECT screenRect = { 0, 0, screenWidth, screenHeight };
    HBRUSH bgBrush = CreateSolidBrush(RGB(200, 200, 200)); // 회색 배경
    FillRect(hdc, &screenRect, bgBrush);
    DeleteObject(bgBrush);

    // 격자 선 그리기
    HPEN gridPen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150)); // 격자 선 색상 (밝은 회색)
    HPEN oldPen = (HPEN)SelectObject(hdc, gridPen);

    // 세로 선 그리기
    for (int x = 0; x <= screenWidth; x += gridSize)
    {
        MoveToEx(hdc, x, 0, NULL);
        LineTo(hdc, x, screenHeight);
    }

    // 가로 선 그리기
    for (int y = 0; y <= screenHeight; y += gridSize)
    {
        MoveToEx(hdc, 0, y, NULL);
        LineTo(hdc, screenWidth, y);
    }

    // 펜 객체 복원 및 삭제
    SelectObject(hdc, oldPen);
    DeleteObject(gridPen);

    // 모든 적과 플레이어 렌더링
    for (auto* wizard : wizards) wizard->Render(hdc, player);
    for (auto* archer : archers) archer->Render(hdc, player);
    for (auto* swordman : swordmans) swordman->Render(hdc, player);
    player.Render(hdc);

    for (auto* arrow : arrows) arrow->Render(hdc);
    for (auto* fireball : fireballs) fireball->Render(hdc);

    // UI 텍스트 출력
    if (!swordmans.empty())
    {
        WCHAR swordManHpText[100];
        wsprintf(swordManHpText, L"enemy Hp : %d", swordmans[0]->GetHp());
        TextOut(hdc, 0, 20, swordManHpText, lstrlen(swordManHpText));
    }
    WCHAR PlayerHpText[100];
    wsprintf(PlayerHpText, L"player Hp : %d", player.GetHp());
    TextOut(hdc, 0, 40, PlayerHpText, lstrlen(PlayerHpText));

    WCHAR Text[100];
    wsprintf(Text, L"X : %d Y : %d", static_cast<int>(Input::GetMousePosition().x), static_cast<int>(Input::GetMousePosition().y));
    TextOut(hdc, static_cast<int>(Input::GetMousePosition().x) + 10, static_cast<int>(Input::GetMousePosition().y), Text, lstrlen(Text));
}

void Stage1::HandleCollision()
{
    // 모든 객체를 하나의 벡터에 모음
    std::vector<GameObject*> allObjects;
    allObjects.push_back(&player); // 플레이어 추가
    for (auto* swordman : swordmans) allObjects.push_back(swordman);
    for (auto* wizard : wizards) allObjects.push_back(wizard);
    for (auto* archer : archers) allObjects.push_back(archer);

    // 모든 객체 쌍에 대해 충돌 확인
    for (size_t i = 0; i < allObjects.size(); ++i)
    {
        for (size_t j = i + 1; j < allObjects.size(); ++j)
        {
            if (allObjects[i]->IsCollidingWith(allObjects[j]))
            {
                ResolveCollision(*allObjects[i], *allObjects[j]);
            }
        }
    }
	
}

void Stage1::ResolveCollision(GameObject& obj1, GameObject& obj2)
{

    RECT rect1 = obj1.GetRect();
    RECT rect2 = obj2.GetRect();
    RECT intersect;

    // 충돌 확인
    if (IntersectRect(&intersect, &rect1, &rect2))
    {
        int overlapX = min(rect1.right, rect2.right) - max(rect1.left, rect2.left);
        int overlapY = min(rect1.bottom, rect2.bottom) - max(rect1.top, rect2.top);

        if (overlapX < overlapY)
        {
            // x축 밀어내기
            if (rect1.left < rect2.left)
            {
                obj1.SetPosition(obj1.GetPositionX() - overlapX, obj1.GetPositionY());
                obj2.SetPosition(obj2.GetPositionX() + overlapX, obj2.GetPositionY());
            }
            else
            {
                obj1.SetPosition(obj1.GetPositionX() + overlapX, obj1.GetPositionY());
                obj2.SetPosition(obj2.GetPositionX() - overlapX, obj2.GetPositionY());
            }
        }
        else
        {
            // y축 밀어내기
            if (rect1.top < rect2.top)
            {
                obj1.SetPosition(obj1.GetPositionX(), obj1.GetPositionY() - overlapY);
                obj2.SetPosition(obj2.GetPositionX(), obj2.GetPositionY() + overlapY);
            }
            else
            {
                obj1.SetPosition(obj1.GetPositionX(), obj1.GetPositionY() + overlapY);
                obj2.SetPosition(obj2.GetPositionX(), obj2.GetPositionY() - overlapY);
            }
        }
    }
}

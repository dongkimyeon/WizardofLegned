#include "UI.h"
#include "SceneManager.h"	


ATL::CImage UI::UI_HPBAR;
ATL::CImage UI::UI_MPBAR;
ATL::CImage UI::UI_PLAYERBAR;
ATL::CImage UI::UI_SKILLBAR;


UI::UI()
{
    UI_HPBAR.Load(L"resources/Ui/UI_HPBAR.png");
    UI_MPBAR.Load(L"resources/Ui/UI_MPBAR.png");
    UI_PLAYERBAR.Load(L"resources/Ui/UI_PLAYERBAR.png");
    UI_SKILLBAR.Load(L"resources/Ui/UI_SKILLBAR.png");
}

UI::~UI()
{
	// 자원 해제
	UI_HPBAR.Destroy();
	UI_MPBAR.Destroy();
	UI_PLAYERBAR.Destroy();
	UI_SKILLBAR.Destroy();

}

void UI::Initialize()
{
	UI_HPBAR.Load(L"resources/Ui/UI_HPBAR.png");
	UI_MPBAR.Load(L"resources/Ui/UI_MPBAR.png");
	UI_PLAYERBAR.Load(L"resources/Ui/UI_PLAYERBAR.png");
	UI_SKILLBAR.Load(L"resources/Ui/UI_SKILLBAR.png");

  
}

void UI::Update()
{

}

void UI::Render(HDC hdc)
{
    Player* player = SceneManager::GetSharedPlayer();

    UI_PLAYERBAR.Draw(hdc, 0, 0);
    // HP 바 렌더링 (오른쪽에서 왼쪽으로 줄어듦)
    float hpRatio = static_cast<float>(player->GetHp()) / 100;
    int hpBarWidth = static_cast<int>(UI_HPBAR.GetWidth() * hpRatio);
    if (hpBarWidth > 0) {
        int srcX = UI_HPBAR.GetWidth() - hpBarWidth; // 오른쪽에서 시작
        UI_HPBAR.Draw(hdc, 75, 13, hpBarWidth, UI_HPBAR.GetHeight(), srcX, 0, hpBarWidth, UI_HPBAR.GetHeight());
    }

    // MP 바 렌더링 (오른쪽에서 왼쪽으로 줄어듦)
    float mpRatio = static_cast<float>(player->GetMp()) / 100;
    int mpBarWidth = static_cast<int>(UI_MPBAR.GetWidth() * mpRatio);
    if (mpBarWidth > 0) {
        int srcX = UI_MPBAR.GetWidth() - mpBarWidth; // 오른쪽에서 시작
        UI_MPBAR.Draw(hdc, 75, 53, mpBarWidth, UI_MPBAR.GetHeight(), srcX, 0, mpBarWidth, UI_MPBAR.GetHeight());
    }
    int originalWidth = UI_SKILLBAR.GetWidth();
    int originalHeight = UI_SKILLBAR.GetHeight();
    int scaledWidth = static_cast<int>(originalWidth * 0.2f);
    int scaledHeight = static_cast<int>(originalHeight * 0.2f);
    int yPos = 720 - scaledHeight;
    UI_SKILLBAR.Draw(hdc, 0, yPos, scaledWidth, scaledHeight);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));
    HFONT hFont = CreateFont(25, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE, L"8BIT WONDER");
    if (!hFont) {
        OutputDebugString(L"Failed to create 8BIT WONDER font\n");
    }
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    float fireBallCooldown = player->GetFireBallCooldown();
    float fireDragonCooldown = player->GetFireDragonCooldown();
    fireBallCooldown = (fireBallCooldown <= 0) ? 0 : fireBallCooldown;
    fireDragonCooldown = (fireDragonCooldown <= 0) ? 0 : fireDragonCooldown;
    wchar_t fireBallText[32];
    wchar_t fireDragonText[32];
    swprintf_s(fireBallText, L"%d", static_cast<int>(fireBallCooldown));
    swprintf_s(fireDragonText, L"%d", static_cast<int>(fireDragonCooldown));

    // 쿨타임이 0이 아닐 때만 텍스트 출력
    if (fireBallCooldown > 0) {
        TextOut(hdc, 163, yPos + 80, fireBallText, wcslen(fireBallText));
    }
    if (fireDragonCooldown > 0) {
        TextOut(hdc, 233, yPos + 80, fireDragonText, wcslen(fireDragonText));
    }

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

#include "SceneManager.h"
#include "Input.h"
#include "Time.h"
std::map<std::wstring, Scene*> SceneManager::mScene = {};
Scene* SceneManager::mActiveScene = nullptr;
CImage SceneManager::mMouseCursorImage;

void SceneManager::Initialize()
{
	mMouseCursorImage.Load(L"resources/MouseCursor.png");
}

void SceneManager::Update()
{
	mActiveScene->Update();
}

void SceneManager::LateUpdate()
{
	mActiveScene->LateUpdate();
}

void SceneManager::Render(HDC hdc)
{
	mActiveScene->Render(hdc);
	int cursorImageWidth = mMouseCursorImage.GetWidth();
	int cursorImageHeight = mMouseCursorImage.GetHeight();
	mMouseCursorImage.Draw(hdc, Input::GetMousePosition().x - 40, Input::GetMousePosition().y - 40, cursorImageWidth + 50, cursorImageHeight + 50);
	Time::Render(hdc);
}
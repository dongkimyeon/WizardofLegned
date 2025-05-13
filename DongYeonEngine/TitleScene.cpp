#include "TitleScene.h"
#include "SceneManager.h"
#include "Time.h"

extern const UINT width;
extern const UINT height;

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
    mBackGroundImage.Load(L"resources/Maintitle.png"); 

    
    mLogoImage.Load(L"resources/TitileLogo.png"); 
   
}

void TitleScene::Update()
{
    Scene::Update();

    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        SceneManager::LoadScene(L"Stage1");
    }
}

void TitleScene::LateUpdate()
{
    Scene::LateUpdate();
}

void TitleScene::Render(HDC hdc)
{

    if (!mBackGroundImage.IsNull())
    {
        mBackGroundImage.Draw(hdc, 0, 0, width, height);
    }

    if (!mLogoImage.IsNull())
    {
        mLogoImage.Draw(hdc, 0, 0, width, height);
    }
        
    

  
    Time::Render(hdc);
}
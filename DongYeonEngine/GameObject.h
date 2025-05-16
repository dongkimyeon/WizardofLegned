#pragma once
#include "CommonInclude.h"

class GameObject
{
public:
	GameObject();
	~GameObject();
	 enum PlayerState { FRONT, BACK, LEFT, RIGHT };
	 virtual void Update();
	 virtual void LateUpdate();
	 virtual void Render(HDC hdc);

	 virtual void SetPosition(float x, float y)
	 {
		mX = x;
		mY = y;
	 }
	 virtual void Setradius(float r)
	 {
		 radius = r;
	 }
	 virtual void SetSpeed(float s)
	 {
		 speed = s;
	 }
	 virtual void SetColor(COLORREF c)
	 {
		 color = c;
	 }
	 virtual float GetPositionX() { return mX; }
	 virtual float GetPositionY() { return mY; }
	 virtual float GetSpeed() { return speed; }
	 virtual COLORREF GetColor() { return color; }
	 virtual float GetRadius() { return radius; }
	 virtual RECT GetRect() 
	 {
		 return rect;
	 }

	 virtual bool IsCollidingWith(GameObject* other)  
	 {
		 RECT rect1 = GetRect();
		 RECT rect2 = other->GetRect();
		 return (rect1.left < rect2.right && rect1.right > rect2.left &&
			 rect1.top < rect2.bottom && rect1.bottom > rect2.top);
	 }

protected:
	float mX;
	float mY;
	float radius;
	float speed;
	RECT rect;
	COLORREF color;
	
};

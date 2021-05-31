//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
//It rained on us at 4am.
#include "Object.h"
#include "Picture.h"
#include "windowsize.h"
#include "debugger.h"
Object::Object(int _x, int _y, HBITMAP _hBM):
	x(_x), y(_y), hBM(_hBM){
	BITMAP bm;
	GetObject(hBM, sizeof(bm), &bm);
	h = bm.bmHeight, w = bm.bmWidth;
	if (y < h) y = h;
	if (y > wd::height - h) y = wd::height - h;
}
int Object::getx(){ return x; }
int Object::gety(){ return y; }
bool Object::isrosa() const { return false; }
bool Object::isdog() const { return false; }
bool Object::MoveAvailable(int dtx, int dty){
	if ((x += dtx) < 0 || x > wd::width) return false;
	if ((y += dty) > wd::height) y = wd::height;
	if (y < 0) y = 0;
	return true;
}
void Object::PrintOnDC(HDC hDC){
	HBITMAP hmask = CreateBitmap(w, h, 1, 1, NULL);
	HDC memdc = CreateCompatibleDC(hDC);
	SelectObject(memdc, hmask);
	HDC maskdc = CreateCompatibleDC(hDC);
	SelectObject(maskdc, hBM);
	SetBkColor(maskdc, RGB(255, 255, 255));
	BitBlt(memdc, 0, 0, w, h, maskdc, 0, 0, SRCCOPY);
	BitBlt(hDC, x, y, w, h, memdc, 0, 0, SRCAND);
	HDC sDC = CreateCompatibleDC(hDC);
	HBITMAP hCombmp = CreateBitmap(w, h, 4, 8, NULL);
	SelectObject(sDC, hCombmp);
	BitBlt(sDC, 0, 0, w, h, memdc, 0, 0, NOTSRCCOPY);
	BitBlt(sDC, 0, 0, w, h, maskdc, 0, 0, SRCAND);
	BitBlt(hDC, x, y, w, h, sDC, 0, 0, SRCPAINT);
	DeleteObject(hCombmp), DeleteObject(hmask);
	DeleteDC(sDC), DeleteDC(maskdc), DeleteDC(memdc);
}
int Object::isCollided(const Object &t){
	if (x + w < t.x || t.x + t.w < x) return COLLI_FALSE;
	if (y + h < t.y || t.y + t.h < y) return COLLI_FALSE;
	return COLLI_TRUE;
}
double Object::getColliP(const Object &t){
	int area = max(t.x - x - w, x - t.x - t.w)
		* max(t.y - y - h, y - t.y - t.h);
	return 1.0 * area / w / h;
}
Dog::Dog(int x, int y): Object(x, y, hBDog){ }
bool Dog::onSky(){ return y < wd::dgY; }
int Dog::dltHeight(){ return wd::dgY - y; }
bool Dog::isdog() const{ return true; }
Rosa::Rosa(int x, int y): Object(x, y, hBRosa){ }
bool Rosa::isrosa() const { return true; }
int Rosa::isCollided(const Object &t){
	if (!t.isdog()){
		if (Object::isCollided(t) == COLLI_TRUE) return COLLI_ROSA;
		return COLLI_FALSE;
	} else {
		double res = getColliP(t);
		//debug("precentage: %.3lf", res);
		if (res > 0.5) return COLLI_ROSA; else return COLLI_FALSE;
	}
}
Heart::Heart(int x, int y): Object(x, y, hBHeart){ }
int Heart::isCollided(const Object &t){
	if (Object::isCollided(t) == COLLI_TRUE) return COLLI_HEART;
	return COLLI_FALSE;
}
Ring::Ring(int x, int y) : Object(x, y, hBRing) { }
int Ring::isCollided(const Object& t) {
	if (Object::isCollided(t) == COLLI_TRUE) return COLLI_RING;
	return COLLI_FALSE;
}

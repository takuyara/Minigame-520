//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
//It rained on us at 4am.
#define _CRT_SECURE_NO_WARNINGS
#include "Status.h"
#include "Picture.h"
#include "windowsize.h"
#include "debugger.h"
#include "RainbowC.h"
#include <vector>
#include <deque>
#include <cstdlib>

const char szMusicName[] = "winbgm.wav";

HFONT GetFont(int nCharH, const char *fname = "Arial"){
	return CreateFont(nCharH, 0, 0, 0, 400, 0, 0, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fname);
}
int Gmin(int x, int y){ return x < y ? x : y; }
bool Roll(double p){ return rand() % 1000 < 1000 * p; }

Status* Status::currSta = nullptr;
Status* Status::ladelSta = nullptr;
int Status::score = 0;
int Status::upheight = 0;
HWND Status::hWnd = NULL;
scort::scort(int _x, int _y, int _t): x(_x), y(_y), tms(_t){ }
Status::Status(HBITMAP _hBM): hBM(_hBM){
	createTime = GetTickCount64();
}
int Status::OnKeyDown(WPARAM wParam){ return -1; }
int Status::OnKeyUp(WPARAM wParam){ return -1; }
bool Status::needFlush(){ return false; }
int Status::OnCreate(){
	currSta = new SWelcome();
	return 0;
}
int Status::OnPaint(HWND hWnd){
	HDC hDC, hdcmem; PAINTSTRUCT ps;
	hDC = BeginPaint(hWnd, &ps);
	hdcmem = CreateCompatibleDC(hDC);
	SelectObject(hdcmem, hBM);
	BitBlt(hDC, 0, 0, wd::width, wd::height, hdcmem, 0, 0, SRCCOPY);
	DeleteDC(hdcmem);
	EndPaint(hWnd, &ps);
	return 0;
}
void Status::ChangeStatus(Status *s){
	if (ladelSta) delete ladelSta;
	ladelSta = currSta;
	currSta = s;
	InvalidateRect(hWnd, NULL, TRUE);
}
void Status::OnTimer(){ }
void Status::OnHalt(){ }
bool Status::checkDelay(){
	return GetTickCount64() - createTime > wd::slowtime;
}
SWelcome::SWelcome(): Status(hBWelcome){ }
int SWelcome::OnKeyDown(WPARAM wParam){
	if (checkDelay()){
		if (wParam == 0x48) ChangeStatus(new SHelp());
			else ChangeStatus(new SGame());
	}
	return 0;
}
SWin::SWin(): Status(hBWin){
	//PlaySound(szMusicName, NULL, SND_ASYNC | SND_NODEFAULT);
}
void SWin::OnHalt(){
	//PlaySound(NULL, NULL, 0);
}
int SWin::OnKeyDown(WPARAM wParam){
	if (checkDelay()) ChangeStatus(new SWelcome());
	return 0;
}
SFail::SFail(): Status(hBFail){ }
int SFail::OnKeyDown(WPARAM wParam) {
	if (checkDelay()) ChangeStatus(new SWelcome());
	return 0;
}
SHelp::SHelp() : Status(hBHelp) { }
int SHelp::OnKeyDown(WPARAM wParam) {
	if (checkDelay()) ChangeStatus(new SWelcome());
	return 0;
}
SGame::SGame(): Status(hBGame), dog(wd::dgX, wd::dgY){
	score = startTicks = upheight = 0;
	onPress = onHold = false;
	for (int i = wd::llim; i < wd::rlim; i += wd::pace)
		tryGenerate(i, i > wd::mlim);
}
bool SGame::needFlush(){ return true; }
void SGame::OnTimer(){
	//MessageBox(NULL, "ONTIMER", "T", MB_OK);
	//debug("upht = %d\n", upheight);
	if (!upheight && dog.onSky() && !onHold){
		int curht = Gmin(wd::jpspd, dog.dltHeight());
		dog.MoveAvailable(0, curht);
	}
	if (upheight){
		int curht = Gmin(wd::jpspd, upheight);
		dog.MoveAvailable(0, -curht);
		upheight -= curht;
	}
	if (!onPress || (onPress && onHold)){
		std::vector<std::deque<Object*>::iterator> del;
		del.clear();
		for (auto p = objs.begin(); p != objs.end(); ++p)
			if (!(*p)->MoveAvailable(-wd::pace, 0)) del.push_back(p);
		for (auto p : del){
			Object *pp = *p;
			objs.erase(p);
			delete pp;
		}
		del.clear();
		tryGenerate(wd::rlim, true);
		for (auto p = objs.begin(); p != objs.end(); ++p){
			int res = (*p)->isCollided(dog);
			if (res == COLLI_HEART){
				++score;
				del.push_back(p);
				scorelist.push_back(scort((*p)->getx(), (*p)->gety(), 2));
			}
			if (res == COLLI_RING){
				ChangeStatus(new SWin());
				break;
			}
			if (res == COLLI_ROSA){
				if (Roll(wd::maxRosaColli)) del.push_back(p);
				else {
					ChangeStatus(new SFail());
					break;
				}
			}
		}
		for (auto p : del){
			Object *pp = *p;
			objs.erase(p);
			delete pp;
		}
		del.clear();
	}
}
int SGame::OnKeyDown(WPARAM wParam){
	if (!checkDelay()) return -1;
	if (wParam == VK_SPACE){
		//MessageBox(NULL, "KeyDown", "PrintOBJ", MB_OK);
		if (!onPress){
			if (!dog.onSky())
				startTicks = GetTickCount64();
			else onHold = true;
			onPress = true;
			//debug("keydown time: %lld\n", startTicks);
		}
		return 0;
	}
	return -1;
}
int SGame::OnKeyUp(WPARAM wParam){
	if (!checkDelay()) return -1;
	if (wParam == VK_SPACE){
		//MessageBox(NULL, "KeyUP", "PrintOBJ", MB_OK);
		if (onHold) onHold = false;
		else {
			long long tmp = GetTickCount64();
			upheight = wd::xlspd * min(tmp - startTicks, wd::mxxltm);
			//debug("Keyup time = %lld\n", tmp);
			//debug("cur=%d, mx=%d\n", upheight, wd::mxxltm);
			//upheight = min(upheight, wd::dgY);
			startTicks = 0;
		}
		onPress = false;
		return 0;
	}
	return -1;
}
SGame::~SGame(){
	for (auto p: objs) delete p;
	objs.clear();
}
bool SGame::checkDelay() {
	return GetTickCount64() - createTime > 500;
}
int SGame::OnPaint(HWND hWnd){
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(hWnd, &ps);
	HDC hdcmem = CreateCompatibleDC(hDC);
	SelectObject(hdcmem, hBM);
	BitBlt(hDC, 0, 0, wd::width, wd::height, hdcmem, 0, 0, SRCCOPY);
	DeleteDC(hdcmem);
	SetTextColor(hDC, RGB(20, 50, 120));
	HFONT hf = GetFont(wd::scoreh);
	SelectObject(hDC, hf);
	char strScore[20];
	sprintf(strScore, "Score: %d", score);
	TextOut(hDC, wd::scorex, wd::scorey, strScore, strlen(strScore));
	DeleteObject(hf);
	long long curTick = GetTickCount64();
	if (startTicks){
		int mscnt = min(curTick - startTicks, wd::mxxltm);
		POINT pt;
		for (int i = 0; i < mscnt / wd::xlpert; ++i){
			COLORREF col = GetRainbowColor(i + 1, wd::rbcnt);
			//debug("rainbowcolor: %d\n", col);
			HPEN hp = CreatePen(PS_SOLID, 8, col);
			SelectObject(hDC, hp);
			int y = wd::xlboty - i * wd::xldlty;
			MoveToEx(hDC, wd::xlbotx, y, &pt);
			LineTo(hDC, wd::xlbotx + wd::xllen, y);
			DeleteObject(hp);
		}
	}
	dog.PrintOnDC(hDC);
	for (auto p: objs) p->PrintOnDC(hDC);
	SetTextColor(hDC, RGB(20, 50, 120));
	hf = GetFont(wd::plush);
	SelectObject(hDC, hf);
	strcpy(strScore, "+1");
	for (auto p: scorelist){
		TextOut(hDC, p.x, p.y, strScore, strlen(strScore));
		--p.tms;
		//debug("tms %d\n", p.tms);
	}
	/*
	while (!scorelist.empty() && !scorelist.front().tms)
		debug("remove ps\n"), scorelist.pop_front();
	*/
	scorelist.clear();
	DeleteObject(hf);
	EndPaint(hWnd, &ps);
	return 0;
}
void SGame::CheckConflict(Object *insp){
	if (insp->isCollided(dog) != COLLI_FALSE) return ;
	auto p = objs.rbegin();
	int cnts = 0;
	for (int i = 0; i < 10 && p != objs.rend(); ++i, ++p){
		if (insp->isCollided(**p) != COLLI_FALSE) return ;
		if (insp->isrosa() && (*p)->isrosa())
			if (++cnts, insp->getx() - (*p)->getx() < wd::minrosadis) return ;
	}
	if (cnts <= wd::maxrosa10) objs.push_back(insp);
}
void SGame::tryGenerate(int x, bool yRosa){
	int y = rand() % wd::height;
	if (Roll(wd::pRosa) && yRosa) CheckConflict(new Rosa(x, y));
	if (Roll(wd::pHeart)) CheckConflict(new Heart(x, y));
	if (Roll(wd::pRing(score))) CheckConflict(new Ring(x, y));
}

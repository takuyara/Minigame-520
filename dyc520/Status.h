//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
//It rained on us at 4am.
#pragma once
#include "Object.h"
#include "Callback.h"
#include <deque>
#include <windows.h>

class Status{
protected:
	HBITMAP hBM;
	static Status *currSta, *ladelSta;
	static int score, upheight;
	long long createTime;
	static void ChangeStatus(Status *);
	virtual void OnHalt();
	virtual bool checkDelay();
public:
	static HWND hWnd;
	Status(HBITMAP);
	virtual int OnKeyDown(WPARAM);
	virtual int OnKeyUp(WPARAM);
	virtual int OnPaint(HWND);
	virtual bool needFlush();
	virtual void OnTimer();
	int OnCreate();
	friend LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
} ;

class SWelcome: public Status{
public:
	SWelcome();
	int OnKeyDown(WPARAM);
} ;

class SWin: public Status{
public:
	SWin();
	int OnKeyDown(WPARAM);
	void OnHalt();
} ;

class SFail: public Status{
public:
	SFail();
	int OnKeyDown(WPARAM);
} ;

class SHelp : public Status {
public:
	SHelp();
	int OnKeyDown(WPARAM);
};

struct scort{
	int x, y, tms;
	scort(int, int, int);
} ;

class SGame: public Status{
	Dog dog;
	std::deque<Object*> objs;
	std::deque<scort> scorelist;
	long long startTicks;
	bool onPress, onHold;
	void tryGenerate(int, bool);
	void CheckConflict(Object *);
	bool checkDelay();
public:
	SGame();
	int OnPaint(HWND);
	int OnKeyDown(WPARAM);
	int OnKeyUp(WPARAM);
	void OnTimer();
	bool needFlush();
	virtual ~SGame();
} ;

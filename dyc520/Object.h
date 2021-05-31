//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
//It rained on us at 4am.
#pragma once
#include <windows.h>

const int COLLI_ROSA = 101;
const int COLLI_HEART = 102;
const int COLLI_RING = 103;
const int COLLI_TRUE = 104;
const int COLLI_FALSE = 105;

class Object{
protected:
	int x, y, w, h;
	HBITMAP hBM;
	double getColliP(const Object &);
public:
	Object(int, int, HBITMAP);
	int getx();
	int gety();
	bool MoveAvailable(int, int);
	virtual bool isrosa() const;
	virtual bool isdog() const;
	void PrintOnDC(HDC);
	virtual int isCollided(const Object &);
} ;

class Dog: public Object{
public:
	Dog(int, int);
	bool onSky();
	int dltHeight();
	bool isdog() const;
} ;

class Rosa: public Object{
public:
	Rosa(int, int);
	bool isrosa() const;
	int isCollided(const Object &);
} ;

class Heart: public Object{
public:
	Heart(int, int);
	int isCollided(const Object &);
} ;

class Ring: public Object{
public:
	Ring(int, int);
	int isCollided(const Object &);
} ;

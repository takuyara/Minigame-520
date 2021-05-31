//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
//It rained on us at 4am.
#pragma once
#include <cmath>
namespace wd{
	const int width = 1280, height = 720;
	const int llim = 120, mlim = 700, rlim = 1210;
	const int dgX = 200, dgY = height - 250;
	const int delay = 50, pace = 5;
	const int jpspd = 20, xlpert = 20;
	const double xlspd = 0.5;
	const int mxxltm = 1.0 * dgY / xlspd - 2;
	const int rbcnt = mxxltm / xlpert + 1;
	const int scorex = 40, scorey = 40, scoreh = 40, plush = 50;
	const int xlbotx = 1200, xlboty = 600, xllen = 20, xldlty = 10;
	const int slowtime = 1200;
	const double pRosa = 0.08, pHeart = 0.1;
	inline double pRing(int score){
		return min(0.04, 0.02 * log(score));
	}
	const int maxrosa10 = 3, minrosadis = 200;
	const double maxRosaColli = 0.8;
} ;

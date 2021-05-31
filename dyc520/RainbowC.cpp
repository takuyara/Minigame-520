//Pionniers du TJ, benissiez-moi par votre Esprits Saints!
#include "RainbowC.h"
COLORREF _GetRainbowColor(int x, int n){
	if (x < n / 3) return RGB(255, (int)(765 * x / n), 0);
	if (x < n / 2) return RGB((int)(750 - 1500 * x / n), 255, 0);
	if (x < n * 2 / 3) return RGB(0, 255, (int)(1500 * x / n - 750));
	if (x < n * 5 / 6) return RGB(0, (int)(1250 - 1500 * x / n), 255);
	return RGB((int)(900 * x / n - 750), 0, 255);
}
COLORREF GetRainbowColor(int x, int n){
	if (x == 15 || x == 23 || x == 31 || x == 39) --x;
	return _GetRainbowColor(x, n);
}

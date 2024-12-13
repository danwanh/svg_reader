#ifndef _PATH_H_
#define _PATH_H_

#include "Lib.h"
#include "Shape.h"
#include "Point.h"
using namespace std;

class path : public Shape {
private:
	vector<pair<string, vector<point>>> pathVct;
public:
	path();
	vector<pair<string, vector<point>>> getPath();
	void setPath(vector<pair<string, vector<point>>>);
};

#endif // !PATH_H

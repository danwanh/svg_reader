#ifndef _FILEPROCESS_H
#define _FILEPROCESS_H

#include "Stroke.h"
#include "TransformCommand.h"
#include "Shape.h"
#include "Rectangle.h"
#include "Text.h"

#include "Polyline.h"
#include "Polygon.h"
#include "Ellipse.h"
#include "Circle.h"
#include "Line.h"
#include "Group.h"
#include "Figure.h"
#include "path.h"
#include <unordered_map>

class FileProcess {
	string fileName;
	unordered_map <string, MyColor> colorMap;
public:
	string GetFileName();
	FileProcess();
	FileProcess(string);
	void LoadColorMap();
	MyColor ReadColor(string);
	vector< TransformCommand> ReadTranCom(string);
	vector <point> ReadPoint(string);
	path ReadPath(string);
	void ReadStrokeAndFill(map<string, string>, Shape*);
	map<string, string> ParseAttributes(string attributes);
	Shape* ReadShape(map<string, string>, string);
	void ReadGroupChild(map<string, string>&, group*, fstream&);
	vector <Shape*> ReadFile();
	void ShowShape(Shape*);
};

#endif

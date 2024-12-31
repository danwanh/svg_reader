//#ifndef _FILEPROCESS_H
//#define _FILEPROCESS_H
//
//#include "Stroke.h"
//#include "TransformCommand.h"
//#include "Shape.h"
//#include "Rectangle.h"
//#include "Text.h"
//
//#include "Polyline.h"
//#include "Polygon.h"
//#include "Ellipse.h"
//#include "Circle.h"
//#include "Line.h"
//#include "Group.h"
//#include "Figure.h"
//#include "path.h"
//#include "ViewBox.h"
//#include <unordered_map>
//
//class FileProcess {
//	string fileName;
//	unordered_map <string, MyColor> colorMap;
//	map <string, gradient*> gradientMap;
//	ViewBox* viewbox;
//	map<string, map<string, string>> styleMap;
//
//public:
//	string GetFileName();
//	FileProcess();
//	FileProcess(string);
//	void LoadColorMap();
//	MyColor ReadColor(string);
//	void ReadStrokeAndFill(map<string, string>, Shape*);
//	map<string, gradient*> ReadDefs(fstream& fi);
//	vector <point> ReadPoint(string);
//	path ReadPath(string);
//	vector< TransformCommand> ReadTranCom(string);
//
//	ViewBox* GetViewBox();
//	void SetViewBox(ViewBox* vb);
//
//	//map<string, string> ParseAttributes(string attributes);
//	Shape* ReadShape(map<string, string>, string);
//	void ReadGroupChild(map<string, string>&, group*, fstream&);
//	vector <Shape*> ReadFile();
//	void ShowShape(Shape*);
//};
//
//#endif
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
#include "ViewBox.h"
#include <unordered_map>

class FileProcess {
	string fileName;
	unordered_map <string, MyColor> colorMap;
	map <string, gradient*> gradientMap;
	ViewBox* viewbox;
	map<string, map<string, string>> styleMap;

public:
	string GetFileName();
	FileProcess();
	FileProcess(string);
	void LoadColorMap();
	MyColor ReadColor(string);
	void ReadStrokeAndFill(map<string, string>, Shape*);
	map <string, gradient*> ReadDefs(fstream& fi);
	vector <point> ReadPoint(string);
	path ReadPath(string);
	vector< TransformCommand> ReadTranCom(string);

	ViewBox* GetViewBox();
	void SetViewBox(ViewBox* vb);

	//map<string, string> ParseAttributes(string attributes);
	Shape* ReadShape(map<string, string>, string);
	void ReadGroupChild(map<string, string>&, group*, fstream&);
	vector <Shape*> ReadFile();
	void ShowShape(Shape*);
	map <string, gradient*> GetGradient() {
		return this->gradientMap;
	}
};

void ShowGradient(map <string, gradient*> gradientMap);
#endif

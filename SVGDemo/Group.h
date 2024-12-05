#ifndef _GROUP_H_
#define _GROUP_H_
#include <vector>
#include "Shape.h"
using namespace std;
class group : public Shape {
private:
	vector<Shape*> children;
public:
	group();
	group(MyColor stroke, float strokeW, MyColor fill, string shapeName, vector<TransformCommand> t);

	vector<Shape*>& getChildren();
	void setChildren(vector<Shape*> list);
	void addChild(Shape* shape);
	
	void calculateBoundingBox() override;
	~group();
};
#endif // !_GROUP_H_


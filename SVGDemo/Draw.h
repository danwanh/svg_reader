#ifndef _DRAW_H_
#define _DRAW_H_

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
#include "Path.h"
#include "Group.h"
#include "Figure.h"
class Draw {

	public:
		void drawRectangle(Graphics& graphics, rectangle* rect);
		void drawCircle(Graphics& graphics, circle* cir);
		void drawText(Graphics& graphics, text* text);
		void drawPolyline(Graphics& graphics, polyline* polyline);
		void drawPolygon(Graphics& graphics, polygon* polygon);
		void drawEllipse(Graphics& graphics, ellipse* elipse);
		void drawLine(Graphics& graphics, line* line);
		void drawPath(Graphics& graphics, path* p);

		void drawGroup(Graphics& graphics, group* group);
		void drawFigure(Graphics& graphics, Figure& shape, float angle, float scale, float transX, float transY);
	};

#endif // !_DRAW_H_


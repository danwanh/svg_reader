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
#include "ViewBox.h"

class Draw {

	public:
		void renderFillGradient(Graphics& graphics,  GraphicsPath* path, gradient* fillGrad, Shape* shape, ViewBox *vb);
		void renderStrokeGradient(Graphics& graphics, GraphicsPath* path, gradient* strokeGrad, Shape* shape, ViewBox *vb);

		void drawRectangle(Graphics& graphics, rectangle* rect, ViewBox *vb);
		void drawCircle(Graphics& graphics, circle* cir, ViewBox *vb);
		void drawText(Graphics& graphics, text* text, ViewBox *vb);
		void drawPolyline(Graphics& graphics, polyline* polyline, ViewBox *vb);
		void drawPolygon(Graphics& graphics, polygon* polygon, ViewBox *vb);
		void drawEllipse(Graphics& graphics, ellipse* elipse, ViewBox *vb);
		void drawLine(Graphics& graphics, line* line, ViewBox *vb);
		void drawPath(Graphics& graphics, path* p, ViewBox *vb);
		void drawGroup(Graphics& graphics, group* group, ViewBox *vb);
		void drawFigure(Graphics& graphics, Figure& shape, float angle, float scale, float transX, float transY, ViewBox *vb);
	};

#endif // !_DRAW_H_


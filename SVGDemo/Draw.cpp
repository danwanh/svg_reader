﻿#include "stdafx.h"
#include "Draw.h"
void Draw::drawRectangle(Graphics& graphics, rectangle* rect) {
	GraphicsState save = graphics.Save();
	rect->applyTransform(graphics);
	for (auto& command : rect->getTransform()) {
		if (command.getName() == "translate") {
			graphics.TranslateTransform(command.getTransX(), command.getTransY());
		}
		else if (command.getName() == "rotate") {
			graphics.RotateTransform(command.getAngle());

		}
		else if (command.getName() == "scale") {
			graphics.ScaleTransform(command.getScaleX(), command.getScaleY());
		}
	}
	SolidBrush fillBrush(Color((rect->getFillColor().getOpacity() * 255), rect->getFillColor().getRed(), rect->getFillColor().getGreen(), rect->getFillColor().getBlue()));

	Pen pen(Color(rect->getStrokeColor().getOpacity() * 255, rect->getStrokeColor().getRed(), rect->getStrokeColor().getGreen(), rect->getStrokeColor().getBlue()), rect->getStrokeWidth());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	graphics.FillRectangle(&fillBrush, rect->getRecX(), rect->getRecY(), rect->getWidth(), rect->getHeight());

	graphics.DrawRectangle(&pen, rect->getRecX(), rect->getRecY(), rect->getWidth(), rect->getHeight());
	graphics.Restore(save);

}
void Draw::drawCircle(Graphics& graphics, circle* cir) {
	GraphicsState save = graphics.Save();
	cir->applyTransform(graphics);

	SolidBrush fillBrush(Color(cir->getFillColor().getOpacity() * 255, cir->getFillColor().getRed(), cir->getFillColor().getGreen(), cir->getFillColor().getBlue()));
	Pen pen(Color(cir->getStrokeColor().getOpacity() * 255, cir->getStrokeColor().getRed(), cir->getStrokeColor().getGreen(), cir->getStrokeColor().getBlue()), cir->getStrokeWidth());
	
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.FillEllipse(&fillBrush, cir->getCx() - cir->getRadius(), cir->getCy() - cir->getRadius(), cir->getRadius() * 2, cir->getRadius() * 2);

	graphics.DrawEllipse(&pen, cir->getCx() - cir->getRadius(), cir->getCy() - cir->getRadius(), cir->getRadius() * 2, cir->getRadius() * 2);
	graphics.Restore(save);
}

void Draw::drawText(Graphics& graphics, text* text) {
	GraphicsState save = graphics.Save();
	text->applyTransform(graphics);

	wstring_convert<codecvt_utf8<wchar_t>> converter;
	wstring wContent = converter.from_bytes(text->getContent());
	wstring wFontFamily = converter.from_bytes(text->getFontFamily());
	FontFamily fontFamily(wFontFamily.c_str());

	int gdiFontStyle = FontStyleRegular;
	if (text->getFontStyle() == "bold") {
		gdiFontStyle = FontStyleBold;
	}
	else if (text->getFontStyle() == "italic") {
		gdiFontStyle = FontStyleItalic;
	}
	else{
		gdiFontStyle = FontStyleRegular;
	}
	Font font(&fontFamily, text->getFontSize(), gdiFontStyle, UnitPixel);
	PointF textPosition;

	StringFormat stringFormat;
	
	RectF layoutRect;
	graphics.MeasureString(wContent.c_str(), -1, &font, PointF(0, 0), &layoutRect);
	if (text->getTextAnchor() == "middle") {
		textPosition = PointF(text->getTextPos().getX() - text->getFontSize() / 25, text->getTextPos().getY() - text->getFontSize() / 4 - layoutRect.Height * 0.7);
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentCenter);
	}
	else if (text->getTextAnchor() == "end") {
		textPosition = PointF(text->getTextPos().getX() + text->getFontSize() / 6.5, text->getTextPos().getY() + text->getFontSize() / 2.8 - layoutRect.Height * 0.7);
		stringFormat.SetAlignment(StringAlignmentFar);
		stringFormat.SetLineAlignment(StringAlignmentFar);
	}
	else {
		textPosition = PointF(text->getTextPos().getX() - text->getFontSize() / 7,
			text->getTextPos().getY() - layoutRect.Height * 0.7);
		stringFormat.SetAlignment(StringAlignmentNear);
		stringFormat.SetLineAlignment(StringAlignmentNear);
	}

	SolidBrush fillBrush(Color(text->getFillColor().getOpacity() * 255,
		text->getFillColor().getRed(),
		text->getFillColor().getGreen(),
		text->getFillColor().getBlue()));
	GraphicsPath path;
	path.AddString(wContent.c_str(), -1, &fontFamily, gdiFontStyle, text->getFontSize(), textPosition, &stringFormat);

	Pen outlinePen(Color(text->getStrokeColor().getOpacity() * 255,
		text->getStrokeColor().getRed(),
		text->getStrokeColor().getGreen(),
		text->getStrokeColor().getBlue()),
		text->getStrokeWidth());

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	outlinePen.SetLineJoin(LineJoinRound); 
	graphics.DrawPath(&outlinePen, &path);
	graphics.DrawString(wContent.c_str(), -1, &font, textPosition, &stringFormat, &fillBrush);
	graphics.Restore(save);
}

void Draw::drawPolyline(Graphics& graphics, polyline* polyline) {
	GraphicsState save = graphics.Save();
	polyline->applyTransform(graphics);

	if (!polyline || polyline->getPoints().size() < 2) return; 
	SolidBrush fillBrush(Color(polyline->getFillColor().getOpacity() * 255,
		polyline->getFillColor().getRed(),
		polyline->getFillColor().getGreen(),
		polyline->getFillColor().getBlue()));
	vector<point> points = polyline->getPoints();
	size_t numPoints = points.size();
	PointF* gdiPoints = new PointF[numPoints];
	for (size_t i = 0; i < numPoints; ++i) {
		gdiPoints[i].X = points[i].getX();
		gdiPoints[i].Y = points[i].getY();
	}
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.FillPolygon(&fillBrush, gdiPoints, numPoints);


	Pen pen(Color(polyline->getStrokeColor().getOpacity() * 255,
		polyline->getStrokeColor().getRed(),
		polyline->getStrokeColor().getGreen(),
		polyline->getStrokeColor().getBlue()),
		polyline->getStrokeWidth()); 

	graphics.DrawLines(&pen, gdiPoints, numPoints);
	graphics.Restore(save);

	delete[] gdiPoints;
}
void Draw::drawPolygon(Graphics& graphics, polygon* polygon) {
	GraphicsState save = graphics.Save();
	polygon->applyTransform(graphics);

	SolidBrush fillBrush(Color(polygon->getFillColor().getOpacity() * 255, polygon->getFillColor().getRed(), polygon->getFillColor().getGreen(), polygon->getFillColor().getBlue()));
	PointF* gdiPoints = new PointF[polygon->getPoints().size()];
	vector<point> points = polygon->getPoints();
	for (size_t i = 0; i < polygon->getPoints().size(); ++i) {
		gdiPoints[i].X = points[i].getX();
		gdiPoints[i].Y = points[i].getY();
	}
	Pen pen(Color(polygon->getStrokeColor().getOpacity() * 255, polygon->getStrokeColor().getRed(), polygon->getStrokeColor().getGreen(), polygon->getStrokeColor().getBlue()), polygon->getStrokeWidth());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	graphics.FillPolygon(&fillBrush, gdiPoints, points.size());
	graphics.DrawPolygon(&pen, gdiPoints, points.size());
	graphics.Restore(save);

	delete[] gdiPoints;
}
void Draw::drawEllipse(Graphics& graphics, ellipse* ellipse) {
	GraphicsState save = graphics.Save();
	ellipse->applyTransform(graphics);
	float x = ellipse->getCx() - ellipse->getRx();
	float y = ellipse->getCy() - ellipse->getRy();

	SolidBrush fillBrush(Color(ellipse->getFillColor().getOpacity() * 255, ellipse->getFillColor().getRed(), ellipse->getFillColor().getGreen(), ellipse->getFillColor().getBlue()));
	Pen pen(Color(ellipse->getStrokeColor().getOpacity() * 255, ellipse->getStrokeColor().getRed(), ellipse->getStrokeColor().getGreen(), ellipse->getStrokeColor().getBlue()), ellipse->getStrokeWidth());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	graphics.FillEllipse(&fillBrush, x, y , ellipse->getRx() * 2, ellipse->getRy() * 2);
	graphics.DrawEllipse(&pen, x, y , ellipse->getRx() * 2, ellipse->getRy() * 2);
	graphics.Restore(save);
}
void Draw::drawLine(Graphics& graphics, line* line) {
	GraphicsState save = graphics.Save();
	line->applyTransform(graphics);

	Pen pen(Color(line->getStrokeColor().getOpacity() * 255, line->getStrokeColor().getRed(), line->getStrokeColor().getGreen(), line->getStrokeColor().getBlue()), line->getStrokeWidth());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	graphics.DrawLine(&pen, line->getX1(), line->getY1(), line->getX2(), line->getY2());
	graphics.Restore(save);
}

void Draw::drawPath(Graphics& graphics, path* path) {
	GraphicsState save = graphics.Save();
	path->applyTransform(graphics);

	SolidBrush fillBrush(Color(
		path->getFillColor().getOpacity() * 255,
		path->getFillColor().getRed(),
		path->getFillColor().getGreen(),
		path->getFillColor().getBlue()));
	Pen pen(Color(
		path->getStrokeColor().getOpacity() * 255,
		path->getStrokeColor().getRed(),
		path->getStrokeColor().getGreen(),
		path->getStrokeColor().getBlue()),
		path->getStrokeWidth());

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	auto commands = path->getPath(); 
	GraphicsPath graphicsPath;
	PointF currentPoint;

	
	for (auto cmd : commands) {
		string command = cmd.first;         
		vector<point> points = cmd.second;  

		if (command == "M") { // Move to
			if (points.empty()) continue;
			currentPoint = PointF(points[0].getX(), points[0].getY());
			graphicsPath.StartFigure();
		}
		else if (command == "L") { // Line to
			for (auto pt : points) {
				PointF nextPoint(pt.getX(), pt.getY());
				graphicsPath.AddLine(currentPoint, nextPoint);
				currentPoint = nextPoint;
			}
		}
		else if (command == "C") { // Cubic Bézier Curve
			if (points.size() < 3) continue; 
			for (size_t i = 0; i + 2 < points.size(); i += 3) {
				PointF control1(points[i].getX(), points[i].getY());
				PointF control2(points[i + 1].getX(), points[i + 1].getY());
				PointF endPoint(points[i + 2].getX(), points[i + 2].getY());
				graphicsPath.AddBezier(currentPoint, control1, control2, endPoint);
				currentPoint = endPoint;
			}
		}
		else if (command == "H") { // Horizontal line
			for (auto& pt : points) {
				PointF nextPoint(pt.getX(), currentPoint.Y);
				graphicsPath.AddLine(currentPoint, nextPoint);
				currentPoint = nextPoint;
			}
		}
		else if (command == "V") { // Vertical line
			for (auto& pt : points) {
				PointF nextPoint(currentPoint.X, pt.getY());
				graphicsPath.AddLine(currentPoint, nextPoint);
				currentPoint = nextPoint;
			}
		}
		else if (command == "z" || command == "Z") { // Close path
			graphicsPath.CloseFigure();
		}
	}
	graphics.FillPath(&fillBrush, &graphicsPath);
	if(path->getStrokeWidth() != 0) graphics.DrawPath(&pen, &graphicsPath);

	graphics.Restore(save);
}

void Draw::drawGroup(Graphics& graphics, group* g) {
	GraphicsState save = graphics.Save();
	/*for (auto& command : g->getTransform()) {
		if (command.getName() == "translate") {
			graphics.TranslateTransform(command.getTransX(), command.getTransY());
		}
		else if (command.getName() == "rotate") {
			graphics.RotateTransform(command.getAngle());

		}
		else if (command.getName() == "scale") {
			graphics.ScaleTransform(command.getScaleX(), command.getScaleY());
		}
	}*/
	g->applyTransform(graphics);

	//if (g->getChildren().size() == 0) {
	//	Pen pen(Color(255, 0, 0, 0));
	//	graphics.DrawRectangle(&pen, 0, 0, 200, 100);
	//}
	/*for (Shape* child : g->getChildren()) {
		child->setFillColor(g->getFillColor());
		child->setStrokeColor(g->getStrokeColor());
		child->setStrokeWidth(g->getStrokeWidth());
	}*/
	for (Shape* child : g->getChildren()) {
		int id = child->nameTonum();
		switch (id) {
		case 1:
		{
			rectangle* rect = dynamic_cast<rectangle*>(child);
			drawRectangle(graphics, rect);
			break;
		}

		case 2:
		{
			circle* cir = dynamic_cast<circle*> (child);
			drawCircle(graphics, cir);
			break;
		}
		case 3:
		{
			polygon* pg = dynamic_cast<polygon*> (child);
			drawPolygon(graphics, pg);
			break;
		}
		case 4:
		{
			polyline* pl = dynamic_cast<polyline*> (child);
			drawPolyline(graphics, pl);
			break;
		}
		case 5:
		{
			line* l = dynamic_cast<line*> (child);
			drawLine(graphics, l);
			break;
		}
		case 6:
		{
			text* t = dynamic_cast<text*> (child);
			drawText(graphics, t);
			break;
		}
		case 7:
		{
			ellipse* e = dynamic_cast<ellipse*> (child);
			drawEllipse(graphics, e);
			break;
		}
		case 8: {
			group* g = dynamic_cast<group*> (child);
			drawGroup(graphics, g);
			break;
		}
		case 9: {
			path* p = dynamic_cast<path*> (child);
			drawPath(graphics, p);
			break;
		}
		default:
		{
			break;
		}
		}
	}
	graphics.Restore(save);
}
void Draw::drawFigure(Graphics& graphics, Figure &figure, float angle, float scale, float transX, float transY) {
	GraphicsState save = graphics.Save();

	float x = 0, y = 0;
	figure.calCenter(x, y); // center of image
	//
	//Matrix sharedMatrix;

	graphics.TranslateTransform(transX, transY, MatrixOrderPrepend);
	graphics.TranslateTransform(x, y, MatrixOrderPrepend);
	graphics.ScaleTransform(scale, scale, MatrixOrderPrepend);
	graphics.RotateTransform(angle, MatrixOrderPrepend);
	graphics.TranslateTransform(-x, -y, MatrixOrderPrepend);
	


	//if (figure.getList().size() == 0) {
	//	//	Graphics graphics(hdc);
	//	Pen pen(Color(255, 0, 0, 0)); 
	//	graphics.DrawRectangle(&pen, 0, 0, 200, 100);
	//}
	
	//if (y == 0 && x == 0) {
	//	//Graphics graphics(hdc);
	//	Pen pen(Color(255, 0, 0, 0));
	//	graphics.DrawRectangle(&pen, 0, 0, 200, 100);
	//}

	//graphics.MultiplyTransform(&sharedMatrix, MatrixOrderAppend);


	for (Shape* shape : figure.getList()) {
		int id = shape->nameTonum();
		switch (id) {
		case 1:
		{
			rectangle* rect = dynamic_cast<rectangle*>(shape);
			drawRectangle(graphics, rect);
			break;
		}

		case 2:
		{
			circle* cir = dynamic_cast<circle*> (shape);
			drawCircle(graphics, cir);
			break;
		}
		case 3:
		{
			polygon* pg = dynamic_cast<polygon*> (shape);
			drawPolygon(graphics, pg);
			break;
		}
		case 4:
		{
			polyline* pl = dynamic_cast<polyline*> (shape);
			drawPolyline(graphics, pl);
			break;
		}
		case 5:
		{
			line* l = dynamic_cast<line*> (shape);
			drawLine(graphics, l);
			break;
		}
		case 6:
		{
			text* t = dynamic_cast<text*> (shape);
			drawText(graphics, t);
			break;
		}
		case 7:
		{
			ellipse* e = dynamic_cast<ellipse*> (shape);
			drawEllipse(graphics, e);
			break;
		}
		case 8: {
			group* g = dynamic_cast<group*> (shape);
			drawGroup(graphics, g);
			break;
		}
		case 9: {
			path* p = dynamic_cast<path*> (shape);
			drawPath(graphics, p);
			break;
		}
		default:
		{
			break;
		}
		}
	}

	graphics.Restore(save);
}
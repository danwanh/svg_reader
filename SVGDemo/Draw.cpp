﻿#include "stdafx.h"
#include "Draw.h"
void Draw::drawRectangle(Graphics& graphics, rectangle* rect) {
	GraphicsState save = graphics.Save();
	rect->applyTransform(graphics);
	stroke str = rect->getStroke();

	Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	//Fill 
	gradient* grad = rect->getGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color((rect->getFillColor().getOpacity() * 255), rect->getFillColor().getRed(), rect->getFillColor().getGreen(), rect->getFillColor().getBlue()));
		graphics.FillRectangle(&fillBrush, rect->getRecX(), rect->getRecY(), rect->getWidth(), rect->getHeight());
	}
	else {
		if (grad->getType() == GradientType::LINEAR) {
			linearGradient* linearGrad = dynamic_cast<linearGradient*>(grad);
			if (linearGrad) {
				vector<stop> colorStops = grad->getColorStop();
				if (!colorStops.empty()) {
					int numStops = colorStops.size();
					vector<Color> colors(numStops);
					vector<REAL> positions(numStops);

					// tách colorStops thành 2 mảng colors, positions
					for (int i = 0; i < numStops; ++i) {
						colors[i] = Color(
							colorStops[i].stopColor.getOpacity() * 255,
							colorStops[i].stopColor.getRed(),
							colorStops[i].stopColor.getGreen(),
							colorStops[i].stopColor.getBlue()
						);
						positions[i] = static_cast<REAL>(colorStops[i].offset);
					}

					LinearGradientBrush linearBrush(
						PointF(rect->getRecX() + rect->getWidth() * linearGrad->getX1(), rect->getRecY() + rect->getHeight() * linearGrad->getY1()),
						PointF(rect->getRecX() + rect->getWidth() * linearGrad->getX2(), rect->getRecY() + rect->getHeight() * linearGrad->getY2()),
						colors[0],  // Màu bắt đầu
						colors[numStops - 1]  // Màu kết thúc
					);
					
					//transform gradient
					vector<TransformCommand> trans = grad->getTransform();
					for (TransformCommand t : trans) {
						if (t.getName() == "translate") {
							linearBrush.TranslateTransform(t.getTransX() * rect->getWidth(), t.getTransY() * rect->getHeight());
						}
						else if (t.getName() == "rotate") {
							linearBrush.RotateTransform(t.getAngle());
						}
						else if(t.getName() == "scale"){
							linearBrush.ScaleTransform(t.getScaleX(), t.getScaleY());
						}
					}
					linearBrush.SetWrapMode(WrapModeTileFlipXY);
					linearBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);

					graphics.FillRectangle(&linearBrush, rect->getRecX(), rect->getRecY(), rect->getWidth(), rect->getHeight());
				}
			}
		}
		else if (grad->getType() == GradientType::RADIAL) {
			radialGradient* radialGrad = dynamic_cast<radialGradient*>(grad);
			if (radialGrad) {
				vector<stop> colorStops = grad->getColorStop();
				if (!colorStops.empty()) {
					int numStops = colorStops.size();
					vector<Color> colors(numStops);
					vector<REAL> positions(numStops);
					//ghi ngược với mảng <stop
					for (int i = 0; i < numStops; ++i) {
						colors[numStops - 1 - i] = Color(
							static_cast<BYTE>(colorStops[i].stopColor.getOpacity() * 255),
							colorStops[i].stopColor.getRed(),
							colorStops[i].stopColor.getGreen(),
							colorStops[i].stopColor.getBlue()
						);
						positions[i] = static_cast<REAL>(colorStops[i].offset);
					}

					// Xác định tọa độ và bán kính ellipse
					REAL rectX = rect->getRecX();
					REAL rectY = rect->getRecY();
					REAL rectWidth = rect->getWidth();
					REAL rectHeight = rect->getHeight();

					// Tâm gradient (cx, cy)
					REAL centerX = rectX + rectWidth * radialGrad->getCx();
					REAL centerY = rectY + rectHeight * radialGrad->getCy();

					// Tâm tiêu cự (fx, fy)
					REAL focusX = rectX + rectWidth * radialGrad->getFx();
					REAL focusY = rectY + rectHeight * radialGrad->getFy();

					// Bán kính x và y
					REAL radiusX = rectWidth * radialGrad->getR();
					REAL radiusY = rectHeight * radialGrad->getR();

					// Tạo ellipse path cho gradient
					GraphicsPath ellipsePath;
					ellipsePath.AddEllipse(centerX - radiusX, centerY - radiusY, radiusX * 2, radiusY * 2);
					PathGradientBrush radialBrush(&ellipsePath);

					vector<TransformCommand> trans = grad->getTransform();
					for (TransformCommand t : trans) {
						if (t.getName() == "translate") {
							radialBrush.TranslateTransform(t.getTransX() * rectWidth, t.getTransY() * rectHeight);
						}
						else if (t.getName() == "rotate") {
							radialBrush.RotateTransform(t.getAngle());
						}
						else if (t.getName() == "scale") {
							radialBrush.ScaleTransform(t.getScaleX(), t.getScaleY());
						}
					}
					// Thiết lập màu sắc gradient
					radialBrush.SetCenterColor(colors[numStops - 1]);
					radialBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);

					// Đặt tâm tiêu cự (focus point)
					radialBrush.SetCenterPoint(PointF(focusX, focusY));

					// Phần còn lại của hình chữ nhật ko được fill bới ellipse
					SolidBrush fallbackBrush(colors[0]); //màu cuối cùng trong color stop (100%)
					graphics.FillRectangle(&fallbackBrush, RectF(rectX, rectY, rectWidth, rectHeight));

					// Fill ellipse
					graphics.FillRectangle(&radialBrush, RectF(rectX, rectY, rectWidth, rectHeight));
				}
			}
		}


	}

	if (str.getStrokeWidth() != 0) graphics.DrawRectangle(&pen, rect->getRecX(), rect->getRecY(), rect->getWidth(), rect->getHeight());
	graphics.Restore(save);

}
void Draw::drawCircle(Graphics& graphics, circle* cir) {
	GraphicsState save = graphics.Save();
	cir->applyTransform(graphics);
	stroke str = cir->getStroke();

	SolidBrush fillBrush(Color(cir->getFillColor().getOpacity() * 255, cir->getFillColor().getRed(), cir->getFillColor().getGreen(), cir->getFillColor().getBlue()));
	Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	graphics.FillEllipse(&fillBrush, cir->getCx() - cir->getRadius(), cir->getCy() - cir->getRadius(), cir->getRadius() * 2, cir->getRadius() * 2);

	if (str.getStrokeWidth() != 0) graphics.DrawEllipse(&pen, cir->getCx() - cir->getRadius(), cir->getCy() - cir->getRadius(), cir->getRadius() * 2, cir->getRadius() * 2);
	graphics.Restore(save);
}

void Draw::drawText(Graphics& graphics, text* text) {
	GraphicsState save = graphics.Save();
	text->applyTransform(graphics);
	stroke str = text->getStroke();

	wstring_convert<codecvt_utf8<wchar_t>> converter;
	wstring wContent = converter.from_bytes(text->getContent());
	wstring wFontFamily = converter.from_bytes(text->getFontFamily());
	Gdiplus::FontFamily* fontFamily = new Gdiplus::FontFamily(wFontFamily.c_str());
	if (!fontFamily->IsAvailable()) {
		delete fontFamily;  // Giải phóng bộ nhớ cũ
		fontFamily = new Gdiplus::FontFamily(L"Times New Roman");  // Thay thế bằng font mới
	}
	int gdiFontStyle = FontStyleRegular;

	Font font(fontFamily, text->getFontSize(), gdiFontStyle, UnitPixel);
	PointF textPosition;

	StringFormat stringFormat;

	RectF layoutRect;
	graphics.MeasureString(wContent.c_str(), -1, &font, PointF(0, 0), &layoutRect);
	if (text->getTextAnchor() == "middle") {
		textPosition = PointF(text->getTextPos().getX() + text->getDx() - text->getFontSize() / 25, text->getTextPos().getY() + text->getDy() - text->getFontSize() / 4 - layoutRect.Height * 0.7);
		stringFormat.SetAlignment(StringAlignmentCenter);
		stringFormat.SetLineAlignment(StringAlignmentCenter);
	}
	else if (text->getTextAnchor() == "end") {
		textPosition = PointF(text->getTextPos().getX() + text->getDx() + text->getFontSize() / 6.5, text->getTextPos().getY() + text->getDy() + text->getFontSize() / 2.8 - layoutRect.Height * 0.7);
		stringFormat.SetAlignment(StringAlignmentFar);
		stringFormat.SetLineAlignment(StringAlignmentFar);
	}
	else {
		textPosition = PointF(text->getTextPos().getX() + text->getDx() - text->getFontSize() / 7, text->getTextPos().getY() + text->getDy() - layoutRect.Height * 0.7);
		stringFormat.SetAlignment(StringAlignmentNear);
		stringFormat.SetLineAlignment(StringAlignmentNear);
	}

	GraphicsPath path;
	if (text->getFontStyle() == "italic")
		path.AddString(wContent.c_str(), -1, fontFamily, FontStyleItalic, text->getFontSize() / 1.05, textPosition, &stringFormat);
	else if (text->getFontStyle() == "bold")
		path.AddString(wContent.c_str(), -1, fontFamily, FontStyleBold, text->getFontSize() / 1.05, textPosition, &stringFormat);
	else path.AddString(wContent.c_str(), -1, fontFamily, FontStyleRegular, text->getFontSize() / 1.05, textPosition, &stringFormat);

	SolidBrush fillBrush(Color(text->getFillColor().getOpacity() * 255,
		text->getFillColor().getRed(),
		text->getFillColor().getGreen(),
		text->getFillColor().getBlue()));
	Pen outlinePen(Color(str.getStrokeColor().getOpacity() * 255,
		str.getStrokeColor().getRed(),
		str.getStrokeColor().getGreen(),
		str.getStrokeColor().getBlue()),
		str.getStrokeWidth());

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	graphics.FillPath(&fillBrush, &path);
	if (str.getStrokeWidth() != 0)
		graphics.DrawPath(&outlinePen, &path);
	graphics.Restore(save);
	delete fontFamily;
}

void Draw::drawPolyline(Graphics& graphics, polyline* polyline) {
	GraphicsState save = graphics.Save();
	polyline->applyTransform(graphics);
	stroke str = polyline->getStroke();

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


	Pen pen(Color(str.getStrokeColor().getOpacity() * 255,
		str.getStrokeColor().getRed(),
		str.getStrokeColor().getGreen(),
		str.getStrokeColor().getBlue()),
		str.getStrokeWidth());

	if (str.getStrokeWidth() != 0)
		graphics.DrawLines(&pen, gdiPoints, numPoints);
	graphics.Restore(save);

	delete[] gdiPoints;
}
void Draw::drawPolygon(Graphics& graphics, polygon* polygon) {
	GraphicsState save = graphics.Save();
	polygon->applyTransform(graphics);
	stroke str = polygon->getStroke();

	SolidBrush fillBrush(Color(polygon->getFillColor().getOpacity() * 255, polygon->getFillColor().getRed(), polygon->getFillColor().getGreen(), polygon->getFillColor().getBlue()));
	PointF* gdiPoints = new PointF[polygon->getPoints().size()];
	vector<point> points = polygon->getPoints();
	for (size_t i = 0; i < polygon->getPoints().size(); ++i) {
		gdiPoints[i].X = points[i].getX();
		gdiPoints[i].Y = points[i].getY();
	}

	Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
	pen.SetAlignment(PenAlignmentCenter);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	graphics.FillPolygon(&fillBrush, gdiPoints, points.size());

	if (str.getStrokeWidth() != 0)
		graphics.DrawPolygon(&pen, gdiPoints, points.size());
	graphics.Restore(save);

	delete[] gdiPoints;
}
void Draw::drawEllipse(Graphics& graphics, ellipse* ellipse) {
	GraphicsState save = graphics.Save();
	ellipse->applyTransform(graphics);
	stroke str = ellipse->getStroke();

	float x = ellipse->getCx() - ellipse->getRx();
	float y = ellipse->getCy() - ellipse->getRy();

	SolidBrush fillBrush(Color(ellipse->getFillColor().getOpacity() * 255, ellipse->getFillColor().getRed(), ellipse->getFillColor().getGreen(), ellipse->getFillColor().getBlue()));
	Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	graphics.FillEllipse(&fillBrush, x, y, ellipse->getRx() * 2, ellipse->getRy() * 2);
	if (str.getStrokeWidth() != 0)graphics.DrawEllipse(&pen, x, y, ellipse->getRx() * 2, ellipse->getRy() * 2);
	graphics.Restore(save);
}
void Draw::drawLine(Graphics& graphics, line* line) {
	GraphicsState save = graphics.Save();
	line->applyTransform(graphics);
	stroke str = line->getStroke();

	Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	if (str.getStrokeWidth() != 0) graphics.DrawLine(&pen, line->getX1(), line->getY1(), line->getX2(), line->getY2());
	graphics.Restore(save);
}
void Draw::drawPath(Graphics& graphics, path* path) {
	GraphicsState save = graphics.Save();
	path->applyTransform(graphics);
	stroke str = path->getStroke();

	SolidBrush fillBrush(Color(
		path->getFillColor().getOpacity() * 255,
		path->getFillColor().getRed(),
		path->getFillColor().getGreen(),
		path->getFillColor().getBlue()));
	Pen pen(Color(
		str.getStrokeColor().getOpacity() * 255,
		str.getStrokeColor().getRed(),
		str.getStrokeColor().getGreen(),
		str.getStrokeColor().getBlue()),
		str.getStrokeWidth());

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	auto commands = path->getPath();
	GraphicsPath graphicsPath;

	// Đặt chế độ fill mode thành FillModeAlternate (even-odd rule)
	graphicsPath.SetFillMode(FillModeAlternate);

	PointF currentPoint;
	PointF lastControlPoint(0, 0); // Khởi tạo điểm điều khiển mặc định (0, 0)
	bool hasPreviousControlPoint = false; // Cờ xác định có điểm điều khiển trước đó

	for (auto cmd : commands) {
		string command = cmd.first;
		vector<point> points = cmd.second;

		if (command == "M") { // Move to
			if (points.empty()) continue;
			currentPoint = PointF(points[0].getX(), points[0].getY());
			graphicsPath.StartFigure();
		}
		else if (command == "m") { // Move to (relative)
			if (points.empty()) continue;
			currentPoint = PointF(points[0].getX() + currentPoint.X, points[0].getY() + currentPoint.Y);
			graphicsPath.StartFigure();
		}
		else if (command == "L") { // Line to
			for (auto pt : points) {
				PointF nextPoint(pt.getX(), pt.getY());
				graphicsPath.AddLine(currentPoint, nextPoint);
				currentPoint = nextPoint;
			}
		}
		else if (command == "l") { // Line to (relative)
			for (auto pt : points) {
				PointF nextPoint(pt.getX() + currentPoint.X, pt.getY() + currentPoint.Y);
				graphicsPath.AddLine(currentPoint, nextPoint);
				currentPoint = nextPoint;
			}
		}
		else if (command == "C") { // Cubic Bézier Curve (absolute)
			if (points.size() < 3) continue;
			for (size_t i = 0; i + 2 < points.size(); i += 3) {
				PointF control1(points[i].getX(), points[i].getY());
				PointF control2(points[i + 1].getX(), points[i + 1].getY());
				PointF endPoint(points[i + 2].getX(), points[i + 2].getY());
				graphicsPath.AddBezier(currentPoint, control1, control2, endPoint);
				currentPoint = endPoint;
				lastControlPoint = control2;
				hasPreviousControlPoint = true;
			}
		}
		else if (command == "c") { // Cubic Bézier Curve (relative)
			if (points.size() < 3) continue;
			for (size_t i = 0; i + 2 < points.size(); i += 3) {
				PointF control1(points[i].getX() + currentPoint.X, points[i].getY() + currentPoint.Y);
				PointF control2(points[i + 1].getX() + currentPoint.X, points[i + 1].getY() + currentPoint.Y);
				PointF endPoint(points[i + 2].getX() + currentPoint.X, points[i + 2].getY() + currentPoint.Y);
				graphicsPath.AddBezier(currentPoint, control1, control2, endPoint);
				currentPoint = endPoint;
				lastControlPoint = control2;
				hasPreviousControlPoint = true;
			}
		}
		else if (command == "S") { // Smooth Cubic Bézier Curve (absolute)
			if (points.size() < 2) continue;
			for (size_t i = 0; i + 1 < points.size(); i += 2) {
				PointF control1;
				if (hasPreviousControlPoint) {
					control1 = PointF(lastControlPoint.X, lastControlPoint.Y);
				}
				else {
					control1 = currentPoint; // Nếu không có, sử dụng currentPoint
				}

				PointF control2(points[i].getX(), points[i].getY());
				PointF endPoint(points[i + 1].getX(), points[i + 1].getY());
				graphicsPath.AddBezier(currentPoint, control1, control2, endPoint);
				currentPoint = endPoint;
				lastControlPoint = control2;
				hasPreviousControlPoint = true;
			}
		}
		else if (command == "s") { // Smooth Cubic Bézier Curve (relative)
			if (points.size() < 2) continue;
			for (size_t i = 0; i + 1 < points.size(); i += 2) {
				PointF control1;
				if (hasPreviousControlPoint) {
					control1 = PointF(currentPoint.X + lastControlPoint.X, currentPoint.Y + lastControlPoint.Y);
				}
				else {
					control1 = currentPoint;
				}

				PointF control2(points[i].getX() + currentPoint.X, points[i].getY() + currentPoint.Y);
				PointF endPoint(points[i + 1].getX() + currentPoint.X, points[i + 1].getY() + currentPoint.Y);
				graphicsPath.AddBezier(currentPoint, control1, control2, endPoint);
				currentPoint = endPoint;
				lastControlPoint = control2;
				hasPreviousControlPoint = true;
			}
		}
		else if (command == "H") { // Horizontal line (absolute)
			for (auto& pt : points) {
				PointF nextPoint(pt.getX(), currentPoint.Y);
				graphicsPath.AddLine(currentPoint, nextPoint);
				currentPoint = nextPoint;
			}
		}
		else if (command == "h") { // Horizontal line (relative)
			for (auto& pt : points) {
				PointF nextPoint(currentPoint.X + pt.getX(), currentPoint.Y);
				graphicsPath.AddLine(currentPoint, nextPoint);
				currentPoint = nextPoint;
			}
		}
		else if (command == "V") { // Vertical line (absolute)
			for (auto& pt : points) {
				PointF nextPoint(currentPoint.X, pt.getY());
				graphicsPath.AddLine(currentPoint, nextPoint);
				currentPoint = nextPoint;
			}
		}
		else if (command == "v") { // Vertical line (relative)
			for (auto& pt : points) {
				PointF nextPoint(currentPoint.X, currentPoint.Y + pt.getY());
				graphicsPath.AddLine(currentPoint, nextPoint);
				currentPoint = nextPoint;
			}
		}
		else if (command == "Z" || command == "z") { // Close path
			graphicsPath.CloseFigure();
		}
	}

	// Vẽ các đường path
	graphics.FillPath(&fillBrush, &graphicsPath);
	if (str.getStrokeWidth() != 0)
		graphics.DrawPath(&pen, &graphicsPath);

	graphics.Restore(save);
}


void Draw::drawGroup(Graphics& graphics, group* g) {
	GraphicsState save = graphics.Save();
	g->applyTransform(graphics);

	//if (g->getChildren().size() == 0) {
	//	Pen pen(Color(255, 0, 0, 0));
	//	graphics.DrawRectangle(&pen, 0, 0, 200, 100);
	//}

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
void Draw::drawFigure(Graphics& graphics, Figure& figure, float angle, float scale, float transX, float transY) {
	GraphicsState save = graphics.Save();

	//float x = 0, y = 0;
	//figure.calCenter(x, y); // center of image

	graphics.TranslateTransform(transX, transY, MatrixOrderPrepend);
	//graphics.TranslateTransform(x, y, MatrixOrderPrepend);
	graphics.ScaleTransform(scale, scale, MatrixOrderPrepend);
	graphics.RotateTransform(angle, MatrixOrderPrepend);
//	graphics.TranslateTransform(-x, -y, MatrixOrderPrepend);



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

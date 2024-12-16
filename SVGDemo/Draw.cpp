#include "stdafx.h"
#include "Draw.h"
void Draw::renderFillGradient(Graphics& graphics, GraphicsPath* path, gradient* grad, Shape* shape) {
	if (shape->getName() != "path") {
		path = shape->createGraphicsPath();
	}
	if (!path) return;

	// Lấy bounding box của shape
	float x = 0, y = 0, width = 0, height = 0;
	if (grad->getGradientUnits() == "userSpaceOnUse")
	{
		shape->getBoundingBox(x, y, width, height);
	}


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
					PointF(x + width * linearGrad->getX1(), y + height * linearGrad->getY1()),
					PointF(x + width * linearGrad->getX2(), y + height * linearGrad->getY2()),
					colors[numStops - 1],  // Màu bắt đầu
					colors[0]  // Màu kết thúc
				);


				vector<TransformCommand> trans = grad->getTransform();
				for (TransformCommand t : trans) {
					if (t.getName() == "translate") {
						linearBrush.TranslateTransform(t.getTransX() * width, t.getTransY() * height);
					}
					else if (t.getName() == "rotate") {
						linearBrush.RotateTransform(t.getAngle());
					}
					else if (t.getName() == "scale") {
						linearBrush.ScaleTransform(t.getScaleX(), t.getScaleY());
					}
				}
				if (grad->getSpreadMethod() == "reflect") linearBrush.SetWrapMode(WrapModeTileFlipXY); //reflect
				else if (grad->getSpreadMethod() == "repeat") {
					linearBrush.SetWrapMode(WrapModeClamp); //repeat
				}

				linearBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);
				graphics.SetClip(path, CombineModeReplace);

				// Vẽ gradient trên bounding box
				graphics.FillRectangle(&linearBrush, RectF(x, y, width, height));

				// Xóa clip sau khi vẽ
				graphics.ResetClip();
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

				// Ghi ngược mảng <stop>
				for (int i = 0; i < numStops; ++i) {
					colors[numStops - 1 - i] = Color(
						static_cast<BYTE>(colorStops[i].stopColor.getOpacity() * 255),
						colorStops[i].stopColor.getRed(),
						colorStops[i].stopColor.getGreen(),
						colorStops[i].stopColor.getBlue()
					);
					positions[i] = static_cast<REAL>(colorStops[i].offset);
				}


				// Tâm gradient (cx, cy)
				REAL centerX = x + width * radialGrad->getCx();
				REAL centerY = y + height * radialGrad->getCy();

				// Tâm tiêu cự (fx, fy)
				REAL focusX = x + width * radialGrad->getFx();
				REAL focusY = y + height * radialGrad->getFy();

				// Bán kính x và y
				REAL radiusX = width * radialGrad->getR();
				REAL radiusY = height * radialGrad->getR();

				// Tạo ellipse path cho gradient
				GraphicsPath ellipsePath;
				ellipsePath.AddEllipse(centerX - radiusX, centerY - radiusY, radiusX * 2, radiusY * 2);

				// Path gradient brush
				PathGradientBrush radialBrush(&ellipsePath);

				// Áp dụng transform cho gradient (nếu có)
				graphics.SetClip(path, CombineModeReplace);

				SolidBrush fallbackBrush(colors[0]); //màu cuối cùng trong color stop (100%)
				graphics.FillRectangle(&fallbackBrush, RectF(x, y, width, height));



				if (grad->getSpreadMethod() == "repeat") {
					REAL radiusX_ = radiusX * 4;
					REAL radiusY_ = radiusY * 4;
					int repeatCount = 5; // Số lần lặp (giảm dần bán kính)
					REAL shrinkFactor = 0.5; // Tỉ lệ giảm bán kính mỗi lần lặp

					for (int i = 0; i < repeatCount; ++i) {
						// Bán kính giảm dần từ lớn xuống nhỏ
						REAL newRadiusX = radiusX_ * (1.0 - i * shrinkFactor);
						REAL newRadiusY = radiusY_ * (1.0 - i * shrinkFactor);

						if (newRadiusX <= 0 || newRadiusY <= 0) break; // Dừng nếu bán kính <= 0

						// Tạo ellipse gradient với bán kính thu nhỏ
						GraphicsPath repeatedPath;
						repeatedPath.AddEllipse(
							centerX - newRadiusX,
							centerY - newRadiusY,
							newRadiusX * 2,
							newRadiusY * 2
						);
						vector<TransformCommand> trans = grad->getTransform();
						Matrix transformMatrix;
						for (TransformCommand t : trans) {
							if (t.getName() == "translate") {
								transformMatrix.Translate(t.getTransX() * width, t.getTransY() * height);
							}
							else if (t.getName() == "rotate") {
								transformMatrix.Rotate(t.getAngle());
							}
							else if (t.getName() == "scale") {
								transformMatrix.Scale(t.getScaleX(), t.getScaleY());
							}
						}
						// Áp dụng ma trận biến đổi lên repeatedPath
						repeatedPath.Transform(&transformMatrix);
						// Tạo PathGradientBrush cho mỗi vòng lặp
						PathGradientBrush repeatedBrush(&repeatedPath);


						// Đặt màu trung tâm và dải màu
						repeatedBrush.SetCenterColor(colors[numStops - 1]);
						repeatedBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);

						// Đặt tâm gradient cố định
						repeatedBrush.SetCenterPoint(PointF(centerX, centerY));

						// Vẽ gradient thu nhỏ
						graphics.FillPath(&repeatedBrush, &repeatedPath);
					}
				}


				else if (grad->getSpreadMethod() == "reflect") {
					int reflectCount = 5; // Số lần lặp (giảm dần bán kính)
					REAL shrinkFactor = 0.5; // Tỉ lệ giảm bán kính mỗi lần lặp
					REAL radiusX_ = radiusX * 6;
					REAL radiusY_ = radiusY * 6;
					for (int i = 0; i < reflectCount; ++i) {
						// Bán kính giảm dần từ lớn xuống nhỏ
						REAL newRadiusX = radiusX_ * (1.0 - i * shrinkFactor);
						REAL newRadiusY = radiusY_ * (1.0 - i * shrinkFactor);

						if (newRadiusX <= 0 || newRadiusY <= 0) break; // Dừng nếu bán kính <= 0

						// Tạo ellipse gradient với bán kính thu nhỏ
						GraphicsPath reflectedPath;
						reflectedPath.AddEllipse(
							centerX - newRadiusX,
							centerY - newRadiusY,
							newRadiusX * 2,
							newRadiusY * 2
						);
						vector<TransformCommand> trans = grad->getTransform();
						Matrix transformMatrix;
						for (TransformCommand t : trans) {
							if (t.getName() == "translate") {
								transformMatrix.Translate(t.getTransX() * width, t.getTransY() * height);
							}
							else if (t.getName() == "rotate") {
								transformMatrix.Rotate(t.getAngle());
							}
							else if (t.getName() == "scale") {
								transformMatrix.Scale(t.getScaleX(), t.getScaleY());
							}
						}
						// Áp dụng ma trận biến đổi lên reflectedPath
						reflectedPath.Transform(&transformMatrix);
						// Tạo PathGradientBrush cho mỗi vòng lặp
						PathGradientBrush reflectedBrush(&reflectedPath);


						// Đặt màu trung tâm và dải màu
						reflectedBrush.SetCenterColor(colors[numStops - 1]);
						reflectedBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);

						// Đặt tâm gradient cố định
						reflectedBrush.SetCenterPoint(PointF(centerX, centerY));
						reflectedBrush.SetWrapMode(WrapModeTile);
						// Vẽ gradient thu nhỏ
						graphics.FillPath(&reflectedBrush, &reflectedPath);
					}
				}

				vector<TransformCommand> trans = grad->getTransform();

				for (TransformCommand t : trans) {
					if (t.getName() == "translate") {
						radialBrush.TranslateTransform(t.getTransX() * width, t.getTransY() * height);
					}
					else if (t.getName() == "rotate") {
						radialBrush.RotateTransform(t.getAngle());
					}
					else if (t.getName() == "scale") {
						radialBrush.ScaleTransform(t.getScaleX(), t.getScaleY());
					}
				}

				// Thiết lập màu gradient
				radialBrush.SetCenterColor(colors[numStops - 1]);
				radialBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);

				// Đặt tâm tiêu cự (focus point)
				radialBrush.SetCenterPoint(PointF(focusX, focusY));

				// Vẽ gradient trong vùng path
				graphics.FillRectangle(&radialBrush, RectF(x, y, width, height));

				// Reset clip để không ảnh hưởng các thao tác sau
				graphics.ResetClip();
			}
		}
	}
}
void Draw::renderStrokeGradient(Graphics& graphics, GraphicsPath* path, gradient* grad, Shape* shape) {
	if (shape->getName() != "path") {
		path = shape->createGraphicsPath();
	}
	if (!path) return;

	// Lấy bounding box của shape
	float x = 0, y = 0, width = 0, height = 0;
	if (grad->getGradientUnits() == "userSpaceOnUse")
	{
		shape->getBoundingBox(x, y, width, height);
	}


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
					PointF(x + width * linearGrad->getX1(), y + height * linearGrad->getY1()),
					PointF(x + width * linearGrad->getX2(), y + height * linearGrad->getY2()),
					colors[numStops - 1],  // Màu bắt đầu
					colors[0]  // Màu kết thúc
				);


				vector<TransformCommand> trans = grad->getTransform();
				for (TransformCommand t : trans) {
					if (t.getName() == "translate") {
						linearBrush.TranslateTransform(t.getTransX() * width, t.getTransY() * height);
					}
					else if (t.getName() == "rotate") {
						linearBrush.RotateTransform(t.getAngle());
					}
					else if (t.getName() == "scale") {
						linearBrush.ScaleTransform(t.getScaleX(), t.getScaleY());
					}
				}
				if (grad->getSpreadMethod() == "reflect") linearBrush.SetWrapMode(WrapModeTileFlipXY); //reflect
				else if (grad->getSpreadMethod() == "repeat") {
					linearBrush.SetWrapMode(WrapModeClamp); //repeat
				}

				linearBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);
				Pen gradientPen(&linearBrush, shape->getStroke().getStrokeWidth()); // Dùng gradient pen với độ dày viền
				graphics.SetClip(path, CombineModeReplace);
				graphics.DrawPath(&gradientPen, path); // Vẽ đường viền với gradient

				// Xóa clip sau khi vẽ
				graphics.ResetClip();
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

				// Ghi ngược mảng <stop>
				for (int i = 0; i < numStops; ++i) {
					colors[numStops - 1 - i] = Color(
						static_cast<BYTE>(colorStops[i].stopColor.getOpacity() * 255),
						colorStops[i].stopColor.getRed(),
						colorStops[i].stopColor.getGreen(),
						colorStops[i].stopColor.getBlue()
					);
					positions[i] = static_cast<REAL>(colorStops[i].offset);
				}


				// Tâm gradient (cx, cy)
				REAL centerX = x + width * radialGrad->getCx();
				REAL centerY = y + height * radialGrad->getCy();

				// Tâm tiêu cự (fx, fy)
				REAL focusX = x + width * radialGrad->getFx();
				REAL focusY = y + height * radialGrad->getFy();

				// Bán kính x và y
				REAL radiusX = width * radialGrad->getR();
				REAL radiusY = height * radialGrad->getR();

				// Tạo ellipse path cho gradient
				GraphicsPath ellipsePath;
				ellipsePath.AddEllipse(centerX - radiusX, centerY - radiusY, radiusX * 2, radiusY * 2);

				PathGradientBrush radialBrush(&ellipsePath);

				Pen fallbackPen(colors[0], shape->getStroke().getStrokeWidth()); //màu cuối cùng trong color stop (100%)
				graphics.DrawPath(&fallbackPen, path); // Vẽ đường viền với gradient

				vector<TransformCommand> trans = grad->getTransform();

				for (TransformCommand t : trans) {
					if (t.getName() == "translate") {
						radialBrush.TranslateTransform(t.getTransX() * width, t.getTransY() * height);
					}
					else if (t.getName() == "rotate") {
						radialBrush.RotateTransform(t.getAngle());
					}
					else if (t.getName() == "scale") {
						radialBrush.ScaleTransform(t.getScaleX(), t.getScaleY());
					}
				}

				// Thiết lập màu gradient
				radialBrush.SetCenterColor(colors[numStops - 1]);
				radialBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);

				// Đặt tâm tiêu cự (focus point)
				radialBrush.SetCenterPoint(PointF(focusX, focusY));

				Pen gradientPen(&radialBrush, shape->getStroke().getStrokeWidth()); // Dùng gradient pen với độ dày viền
				graphics.DrawPath(&gradientPen, path); // Vẽ đường viền với gradient

			}
		}
	}
	delete path;

}
void Draw::drawRectangle(Graphics& graphics, rectangle* rect) {
	GraphicsState save = graphics.Save();
	GraphicsPath* p = NULL;
	rect->applyTransform(graphics);
	stroke str = rect->getStroke();

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	gradient* grad = rect->getFillGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color((rect->getFillColor().getOpacity() * 255), rect->getFillColor().getRed(), rect->getFillColor().getGreen(), rect->getFillColor().getBlue()));
		graphics.FillRectangle(&fillBrush, rect->getRecX(), rect->getRecY(), rect->getWidth(), rect->getHeight());
	}
	else {
		renderFillGradient(graphics, p, grad, rect);
	}
	grad = rect->getStrokeGradient();
	if(grad == NULL) {
		Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
		if (str.getStrokeWidth() != 0) graphics.DrawRectangle(&pen, rect->getRecX(), rect->getRecY(), rect->getWidth(), rect->getHeight());
	}
	else {
		renderStrokeGradient(graphics, p, grad, rect);
	}

	graphics.Restore(save);
	delete p;
}
void Draw::drawCircle(Graphics& graphics, circle* cir) {
	GraphicsState save = graphics.Save();
	GraphicsPath* p = NULL;
	cir->applyTransform(graphics);
	stroke str = cir->getStroke();
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	gradient* grad = cir->getFillGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color(cir->getFillColor().getOpacity() * 255, cir->getFillColor().getRed(), cir->getFillColor().getGreen(), cir->getFillColor().getBlue()));
		graphics.FillEllipse(&fillBrush, cir->getCx() - cir->getRadius(), cir->getCy() - cir->getRadius(), cir->getRadius() * 2, cir->getRadius() * 2);
	}
	else {
		renderFillGradient(graphics,p, grad, cir);
	}
	Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
	if (str.getStrokeWidth() != 0) graphics.DrawEllipse(&pen, cir->getCx() - cir->getRadius(), cir->getCy() - cir->getRadius(), cir->getRadius() * 2, cir->getRadius() * 2);
	graphics.Restore(save);
	delete p;
}

void Draw::drawText(Graphics& graphics, text* text) {
	GraphicsState save = graphics.Save();
	text->applyTransform(graphics);
	stroke str = text->getStroke();
	GraphicsPath* p = NULL;
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
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	gradient* grad = text->getFillGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color(text->getFillColor().getOpacity() * 255,
			text->getFillColor().getRed(),
			text->getFillColor().getGreen(),
			text->getFillColor().getBlue()));
		graphics.FillPath(&fillBrush, &path);
	}
	else {
		renderFillGradient(graphics, p, grad, text);
	}

	Pen outlinePen(Color(str.getStrokeColor().getOpacity() * 255,
		str.getStrokeColor().getRed(),
		str.getStrokeColor().getGreen(),
		str.getStrokeColor().getBlue()),
		str.getStrokeWidth());


	if (str.getStrokeWidth() != 0)
		graphics.DrawPath(&outlinePen, &path);
	graphics.Restore(save);
	delete fontFamily;
	delete p;
}

void Draw::drawPolyline(Graphics& graphics, polyline* polyline) {
	GraphicsState save = graphics.Save();
	GraphicsPath* p = NULL;
	polyline->applyTransform(graphics);
	stroke str = polyline->getStroke();

	if (!polyline || polyline->getPoints().size() < 2) return;

	vector<point> points = polyline->getPoints();
	size_t numPoints = points.size();
	PointF* gdiPoints = new PointF[numPoints];
	for (size_t i = 0; i < numPoints; ++i) {
		gdiPoints[i].X = points[i].getX();
		gdiPoints[i].Y = points[i].getY();
	}
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	gradient* grad = polyline->getFillGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color(polyline->getFillColor().getOpacity() * 255,
			polyline->getFillColor().getRed(),
			polyline->getFillColor().getGreen(),
			polyline->getFillColor().getBlue()));		
		graphics.FillPolygon(&fillBrush, gdiPoints, numPoints);
	}
	else {
		renderFillGradient(graphics, p, grad, polyline);
	}

	Pen pen(Color(str.getStrokeColor().getOpacity() * 255,
		str.getStrokeColor().getRed(),
		str.getStrokeColor().getGreen(),
		str.getStrokeColor().getBlue()),
		str.getStrokeWidth());

	if (str.getStrokeWidth() != 0)
		graphics.DrawLines(&pen, gdiPoints, numPoints);
	graphics.Restore(save);
	delete p;
	delete[] gdiPoints;
}
void Draw::drawPolygon(Graphics& graphics, polygon* polygon) {
	GraphicsState save = graphics.Save();
	polygon->applyTransform(graphics);
	stroke str = polygon->getStroke();
	GraphicsPath* p = NULL;
	PointF* gdiPoints = new PointF[polygon->getPoints().size()];
	vector<point> points = polygon->getPoints();
	for (size_t i = 0; i < polygon->getPoints().size(); ++i) {
		gdiPoints[i].X = points[i].getX();
		gdiPoints[i].Y = points[i].getY();
	}

	Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
	pen.SetAlignment(PenAlignmentCenter);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	gradient* grad = polygon->getFillGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color(polygon->getFillColor().getOpacity() * 255, polygon->getFillColor().getRed(), polygon->getFillColor().getGreen(), polygon->getFillColor().getBlue()));
		graphics.FillPolygon(&fillBrush, gdiPoints, points.size());
	}
	else {
		renderFillGradient(graphics, p, grad, polygon);
	}

	if (str.getStrokeWidth() != 0)
		graphics.DrawPolygon(&pen, gdiPoints, points.size());
	graphics.Restore(save);
	delete p;
	delete[] gdiPoints;
}
void Draw::drawEllipse(Graphics& graphics, ellipse* ellipse) {
	GraphicsState save = graphics.Save();
	GraphicsPath* p = NULL;
	ellipse->applyTransform(graphics);
	stroke str = ellipse->getStroke();

	float x = ellipse->getCx() - ellipse->getRx();
	float y = ellipse->getCy() - ellipse->getRy();

	Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	gradient* grad = ellipse->getFillGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color(ellipse->getFillColor().getOpacity() * 255, ellipse->getFillColor().getRed(), ellipse->getFillColor().getGreen(), ellipse->getFillColor().getBlue()));
		graphics.FillEllipse(&fillBrush, x, y, ellipse->getRx() * 2, ellipse->getRy() * 2);
	}
	else {
		renderFillGradient(graphics, p,  grad, ellipse);
	}
	if (str.getStrokeWidth() != 0)graphics.DrawEllipse(&pen, x, y, ellipse->getRx() * 2, ellipse->getRy() * 2);
	graphics.Restore(save);
	delete p;
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

	
	Pen pen(Color(
		str.getStrokeColor().getOpacity() * 255,
		str.getStrokeColor().getRed(),
		str.getStrokeColor().getGreen(),
		str.getStrokeColor().getBlue()),
		str.getStrokeWidth());
	pen.SetMiterLimit(path->getStrokeMiterLimit());

	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	auto commands = path->getPath();
	GraphicsPath graphicsPath;

	//fill mode dựa trên fillRude
	if (path->getFillRule() == "evenodd") {
		graphicsPath.SetFillMode(FillModeWinding);
	}
	else graphicsPath.SetFillMode(FillModeAlternate);

	// Đặt chế độ fill mode thành FillModeAlternate (even-odd rule)
	//graphicsPath.SetFillMode(FillModeAlternate);

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
	gradient* grad = path->getFillGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color(
			path->getFillColor().getOpacity() * 255,
			path->getFillColor().getRed(),
			path->getFillColor().getGreen(),
			path->getFillColor().getBlue()));
		graphics.FillPath(&fillBrush, &graphicsPath);
	}
	else {
		renderFillGradient(graphics, &graphicsPath, grad, path);
	}
	// Vẽ các đường path
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

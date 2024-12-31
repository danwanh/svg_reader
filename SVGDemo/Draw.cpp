#include"stdafx.h"
#include "Draw.h"
void Draw::renderFillGradient(Graphics& graphics, GraphicsPath* path, gradient* grad, Shape* shape, ViewBox* vb) {
	if (shape->getName() != "path") {
		path = shape->createGraphicsPath();
	}
	if (!path) return;

	float x = 0, y = 0, width = 0, height = 0;

	if (grad->getGradientUnits() == "userSpaceOnUse") {
		x = vb->getMinX();
		y = vb->getMinY();
		width = vb->getPortHeight();
		height = vb->getPortWidth();
	}
	else {
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
				auto x1 = linearGrad->getX1();
				auto x2 = linearGrad->getX2();
				auto y1 = linearGrad->getY1();
				auto y2 = linearGrad->getY2();

				double startX, startY, endX, endY;
				if (x1 <= 1.0 && x1 > 0.0) {
					startX = x + width * x1;
				}
				else startX = x1;

				if (y1 <= 1.0 && y1 > 0.0) {
					startY = y + height * y1;
				}
				else startY = y1;

				if (x2 <= 1.0 && x2 > 0.0) {
					endX = x + width * x2;
				}
				else endX = x2;

				if (y2 <= 1.0 && y2 > 0.0) {
					endY = y + height * y2;
				}
				else endY = y2;
				LinearGradientBrush linearBrush(
					PointF(startX, startY),
					PointF(endX, endY),
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
				else linearBrush.SetWrapMode(WrapModeTileFlipXY);

				linearBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);
				GraphicsState save = graphics.Save();
				graphics.SetClip(path, CombineModeIntersect);

				// Vẽ gradient trên bounding box
				graphics.FillRectangle(&linearBrush, RectF(x, y, width, height));

				// Xóa clip sau khi vẽ
				graphics.Restore(save);
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
				REAL centerX, centerY, focusX, focusY, radiusX, radiusY;

				// Tâm gradient (cx, cy)
				if (radialGrad->getCx() < 1.0) {
					centerX = x + width * radialGrad->getCx();
				}
				else {
					centerX = radialGrad->getCx();
				}

				if (radialGrad->getCy() < 1.0) {
					centerY = y + height * radialGrad->getCy();
				}
				else {
					centerY = radialGrad->getCy();
				}

				// Tâm tiêu cự (fx, fy)
				if (radialGrad->getFx() < 1.0) {
					focusX = x + width * radialGrad->getFx();
				}
				else {
					focusX = radialGrad->getFx();
				}

				if (radialGrad->getFy() < 1.0) {
					focusY = y + height * radialGrad->getFy();
				}
				else {
					focusY = radialGrad->getFy();
				}

				// Bán kính x và y (r)
				if (radialGrad->getR() < 1.0) {
					radiusX = width * radialGrad->getR();
					radiusY = height * radialGrad->getR();
				}
				else {
					radiusX = radialGrad->getR();
					radiusY = radialGrad->getR();
				}

				// Tạo ellipse path cho gradient
				GraphicsPath ellipsePath;
				ellipsePath.AddEllipse(centerX - radiusX, centerY - radiusY, radiusX * 2, radiusY * 2);

				// Path gradient brush
				PathGradientBrush radialBrush(&ellipsePath);
				GraphicsState save = graphics.Save();
				graphics.SetClip(path, CombineModeIntersect);

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

				//xoa clip
				graphics.Restore(save);
			}
		}
	}
}
void Draw::renderStrokeGradient(Graphics& graphics, GraphicsPath* path, gradient* grad, Shape* shape, ViewBox* vb) {
	if (shape->getName() != "path") {
		path = shape->createGraphicsPath();
	}

	if (!path) return;

	// Lấy bounding box của shape
	float x = 0, y = 0, width = 0, height = 0;
	if (grad->getGradientUnits() == "userSpaceOnUse") {
		x = vb->getMinX();
		y = vb->getMinY();
		width = vb->getPortHeight();
		height = vb->getPortWidth();
	}
	else {
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

				auto x1 = linearGrad->getX1();
				auto x2 = linearGrad->getX2();
				auto y1 = linearGrad->getY1();
				auto y2 = linearGrad->getY2();

				double startX, startY, endX, endY;
				if (x1 < 1.0 && x1 > 0.0) {
					startX = x + width * x1;
				}
				else startX = x1;

				if (y1 < 1.0 && y1 > 0.0) {
					startY = y + height * y1;
				}
				else startY = y1;

				if (x2 < 1.0 && x2 > 0.0) {
					endX = x + width * x2;
				}
				else endX = x2;

				if (y2 < 1.0 && y2 > 0.0) {
					endY = y + height * y2;
				}
				else endY = y2;
				LinearGradientBrush linearBrush(
					PointF(startX, startY),
					PointF(endX, endY),
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
				else {
					linearBrush.SetWrapMode(WrapModeTileFlipXY); //reflect
				}

				linearBrush.SetInterpolationColors(colors.data(), positions.data(), numStops);
				Pen gradientPen(&linearBrush, shape->getStroke().getStrokeWidth()); // Dùng gradient pen với độ dày viền
				GraphicsState save = graphics.Save();
				graphics.SetClip(path, CombineModeIntersect);
				graphics.DrawPath(&gradientPen, path); // Vẽ đường viền với gradient
				graphics.Restore(save);
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
				graphics.DrawPath(&fallbackPen, path);

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
	if (shape->getName() != "path") delete path;
}
void Draw::drawRectangle(Graphics& graphics, rectangle* rect, ViewBox* vb) {
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
		renderFillGradient(graphics, p, grad, rect, vb);
	}
	grad = rect->getStrokeGradient();
	if (grad == NULL) {
		Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
		if (str.getStrokeWidth() != 0) graphics.DrawRectangle(&pen, rect->getRecX(), rect->getRecY(), rect->getWidth(), rect->getHeight());
	}
	else {
		renderStrokeGradient(graphics, p, grad, rect, vb);
	}

	graphics.Restore(save);
	delete p;
}
void Draw::drawCircle(Graphics& graphics, circle* cir, ViewBox* vb) {
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
		renderFillGradient(graphics, p, grad, cir, vb);
	}
	grad = cir->getStrokeGradient();
	if (grad == NULL)
	{
		Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
		if (str.getStrokeWidth() != 0)
			graphics.DrawEllipse(&pen, cir->getCx() - cir->getRadius(), cir->getCy() - cir->getRadius(), cir->getRadius() * 2, cir->getRadius() * 2);
	}
	else {
		renderStrokeGradient(graphics, p, grad, cir, vb);
	}
	graphics.Restore(save);
	delete p;
}

void Draw::drawText(Graphics& graphics, text* text, ViewBox* vb) {
	GraphicsState save = graphics.Save();
	text->applyTransform(graphics);
	stroke str = text->getStroke();
	GraphicsPath* p = NULL;
	wstring_convert<codecvt_utf8<wchar_t>> converter;
	wstring wContent = converter.from_bytes(text->getContent());
	wstring wFontFamily = converter.from_bytes(text->getFontFamily());
	Gdiplus::FontFamily* fontFamily = new Gdiplus::FontFamily(wFontFamily.c_str());
	if (!fontFamily->IsAvailable()) {
		delete fontFamily;
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
		renderFillGradient(graphics, p, grad, text, vb);
	}
	grad = text->getStrokeGradient();
	if (grad == NULL)
	{
		Pen outlinePen(Color(str.getStrokeColor().getOpacity() * 255,
			str.getStrokeColor().getRed(),
			str.getStrokeColor().getGreen(),
			str.getStrokeColor().getBlue()),
			str.getStrokeWidth());

		if (str.getStrokeWidth() != 0)
			graphics.DrawPath(&outlinePen, &path);
	}
	else {
		renderStrokeGradient(graphics, p, grad, text, vb);
	}
	graphics.Restore(save);
	delete fontFamily;
	delete p;
}

void Draw::drawPolyline(Graphics& graphics, polyline* polyline, ViewBox* vb) {
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
		renderFillGradient(graphics, p, grad, polyline, vb);
	}
	grad = polyline->getStrokeGradient();
	if (grad == NULL)
	{
		Pen pen(Color(str.getStrokeColor().getOpacity() * 255,
			str.getStrokeColor().getRed(),
			str.getStrokeColor().getGreen(),
			str.getStrokeColor().getBlue()),
			str.getStrokeWidth());

		if (str.getStrokeWidth() != 0)
			graphics.DrawLines(&pen, gdiPoints, numPoints);
	}
	else {
		renderStrokeGradient(graphics, p, grad, polyline, vb);
	}
	graphics.Restore(save);
	delete p;
	delete[] gdiPoints;
}
void Draw::drawPolygon(Graphics& graphics, polygon* polygon, ViewBox* vb) {
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


	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	gradient* grad = polygon->getFillGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color(polygon->getFillColor().getOpacity() * 255, polygon->getFillColor().getRed(), polygon->getFillColor().getGreen(), polygon->getFillColor().getBlue()));
		graphics.FillPolygon(&fillBrush, gdiPoints, points.size());
	}
	else {
		renderFillGradient(graphics, p, grad, polygon, vb);
	}
	grad = polygon->getStrokeGradient();
	if (grad == NULL)
	{
		Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
		pen.SetAlignment(PenAlignmentCenter);
		if (str.getStrokeWidth() != 0)
			graphics.DrawPolygon(&pen, gdiPoints, points.size());
	}
	else {
		renderStrokeGradient(graphics, p, grad, polygon, vb);
	}
	graphics.Restore(save);
	delete p;
	delete[] gdiPoints;
}
void Draw::drawEllipse(Graphics& graphics, ellipse* ellipse, ViewBox* vb) {
	GraphicsState save = graphics.Save();
	GraphicsPath* p = NULL;
	ellipse->applyTransform(graphics);
	stroke str = ellipse->getStroke();

	float x = ellipse->getCx() - ellipse->getRx();
	float y = ellipse->getCy() - ellipse->getRy();

	gradient* grad = ellipse->getFillGradient();
	if (grad == NULL) {
		SolidBrush fillBrush(Color(ellipse->getFillColor().getOpacity() * 255, ellipse->getFillColor().getRed(), ellipse->getFillColor().getGreen(), ellipse->getFillColor().getBlue()));
		graphics.FillEllipse(&fillBrush, x, y, ellipse->getRx() * 2, ellipse->getRy() * 2);
	}
	else {
		renderFillGradient(graphics, p, grad, ellipse, vb);
	}
	grad = ellipse->getStrokeGradient();
	if (grad == NULL)
	{
		Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		if (str.getStrokeWidth() != 0)graphics.DrawEllipse(&pen, x, y, ellipse->getRx() * 2, ellipse->getRy() * 2);
	}
	else {
		renderStrokeGradient(graphics, p, grad, ellipse, vb);
	}
	graphics.Restore(save);
	delete p;
}
void Draw::drawLine(Graphics& graphics, line* line, ViewBox* vb) {
	GraphicsState save = graphics.Save();
	line->applyTransform(graphics);
	stroke str = line->getStroke();
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	gradient* grad = line->getStrokeGradient();
	if (grad == NULL) {
		Pen pen(Color(str.getStrokeColor().getOpacity() * 255, str.getStrokeColor().getRed(), str.getStrokeColor().getGreen(), str.getStrokeColor().getBlue()), str.getStrokeWidth());
		if (str.getStrokeWidth() != 0) graphics.DrawLine(&pen, line->getX1(), line->getY1(), line->getX2(), line->getY2());
	}
	else {
		GraphicsPath* p = NULL;
		renderStrokeGradient(graphics, p, grad, line, vb);
		delete p;
	}
	graphics.Restore(save);
}
std::vector<PointF> approximateArcToBezier(
	const PointF& startPoint,
	double rx, double ry,
	double xAxisRotation,
	bool largeArcFlag,
	bool sweepFlag,
	const PointF& endPoint)
{
	std::vector<PointF> bezierSegments;

	if (startPoint.X == endPoint.X && startPoint.Y == endPoint.Y) {
		return bezierSegments; // No arc needed for identical points
	}

	if (rx == 0 || ry == 0) {
		bezierSegments.push_back(startPoint);
		bezierSegments.push_back(endPoint);
		return bezierSegments; // Degenerate case
	}

	rx = std::abs(rx);
	ry = std::abs(ry);
	double angleRad = xAxisRotation * (M_PI / 180.0);
	double cosAngle = std::cos(angleRad);
	double sinAngle = std::sin(angleRad);

	double dx2 = (startPoint.X - endPoint.X) / 2.0;
	double dy2 = (startPoint.Y - endPoint.Y) / 2.0;
	double x1 = cosAngle * dx2 + sinAngle * dy2;
	double y1 = -sinAngle * dx2 + cosAngle * dy2;

	double rxSq = rx * rx;
	double rySq = ry * ry;
	double x1Sq = x1 * x1;
	double y1Sq = y1 * y1;

	double radiiCheck = x1Sq / rxSq + y1Sq / rySq;
	if (radiiCheck > 1.0) {
		double scale = std::sqrt(radiiCheck);
		rx *= scale;
		ry *= scale;
		rxSq = rx * rx;
		rySq = ry * ry;
	}

	double sign = (largeArcFlag != sweepFlag) ? 1 : -1;
	double sq = ((rxSq * rySq) - (rxSq * y1Sq) - (rySq * x1Sq)) / ((rxSq * y1Sq) + (rySq * x1Sq));
	sq = (sq < 0) ? 0 : sq;
	double coef = sign * std::sqrt(sq);
	double cx1 = coef * ((rx * y1) / ry);
	double cy1 = coef * -((ry * x1) / rx);

	double sx2 = (startPoint.X + endPoint.X) / 2.0;
	double sy2 = (startPoint.Y + endPoint.Y) / 2.0;
	double cx = cosAngle * cx1 - sinAngle * cy1 + sx2;
	double cy = sinAngle * cx1 + cosAngle * cy1 + sy2;

	double ux = (x1 - cx1) / rx;
	double uy = (y1 - cy1) / ry;
	double vx = (-x1 - cx1) / rx;
	double vy = (-y1 - cy1) / ry;

	double n = std::sqrt((ux * ux) + (uy * uy));
	double p = ux;
	sign = (uy < 0) ? -1.0 : 1.0;
	double angleStart = sign * std::acos(p / n);

	n = std::sqrt((ux * ux + uy * uy) * (vx * vx + vy * vy));
	p = ux * vx + uy * vy;
	sign = (ux * vy - uy * vx < 0) ? -1.0 : 1.0;
	double angleExtent = sign * std::acos(p / n);

	if (!sweepFlag && angleExtent > 0) {
		angleExtent -= 2 * M_PI;
	}
	else if (sweepFlag && angleExtent < 0) {
		angleExtent += 2 * M_PI;
	}

	int numSegments = std::ceil(std::abs(angleExtent) / (M_PI / 2));
	double segmentAngle = angleExtent / numSegments;

	for (int i = 0; i < numSegments; ++i) {
		double theta1 = angleStart + i * segmentAngle;
		double theta2 = theta1 + segmentAngle;

		double cosTheta1 = std::cos(theta1);
		double sinTheta1 = std::sin(theta1);
		double cosTheta2 = std::cos(theta2);
		double sinTheta2 = std::sin(theta2);

		PointF p1 = {
			static_cast<Gdiplus::REAL>(cosAngle * rx * cosTheta1 - sinAngle * ry * sinTheta1 + cx),
			static_cast<Gdiplus::REAL>(sinAngle * rx * cosTheta1 + cosAngle * ry * sinTheta1 + cy)
		};
		PointF p2 = {
			static_cast<Gdiplus::REAL>(cosAngle * rx * cosTheta2 - sinAngle * ry * sinTheta2 + cx),
			static_cast<Gdiplus::REAL>(sinAngle * rx * cosTheta2 + cosAngle * ry * sinTheta2 + cy)
		};

		double alpha = std::sin(segmentAngle) * (std::sqrt(4 + 3 * std::pow(std::tan(segmentAngle / 2), 2)) - 1) / 3;

		PointF cp1 = {
			static_cast<Gdiplus::REAL>(p1.X + alpha * (-cosAngle * rx * sinTheta1 - sinAngle * ry * cosTheta1)),
			static_cast<Gdiplus::REAL>(p1.Y + alpha * (-sinAngle * rx * sinTheta1 + cosAngle * ry * cosTheta1))
		};
		PointF cp2 = {
			static_cast<Gdiplus::REAL>(p2.X - alpha * (-cosAngle * rx * sinTheta2 - sinAngle * ry * cosTheta2)),
			static_cast<Gdiplus::REAL>(p2.Y - alpha * (-sinAngle * rx * sinTheta2 + cosAngle * ry * cosTheta2))
		};

		bezierSegments.push_back(cp1);
		bezierSegments.push_back(cp2);
		bezierSegments.push_back(p2);
	}

	return bezierSegments;
}


void Draw::drawPath(Graphics& graphics, path* path, ViewBox* vb) {
	GraphicsState save = graphics.Save();
	path->applyTransform(graphics);
	stroke str = path->getStroke();


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

	string command;
	PointF currentPoint;
	PointF lastControlPoint(0, 0); // Khởi tạo điểm điều khiển mặc định (0, 0)
	bool hasPreviousControlPoint = false; // Cờ xác định có điểm điều khiển trước đó
	PointF startPoint;
	for (auto cmd : commands) {
		command = cmd.first;
		vector<point> points = cmd.second;

		if (command == "M") { // Move to
			if (points.empty()) continue;
			currentPoint = PointF(points[0].getX(), points[0].getY());
			startPoint = currentPoint; // Lưu điểm bắt đầu
			graphicsPath.StartFigure();
		}

		else if (command == "m") { // Move to (relative)
			if (points.empty()) continue;
			currentPoint = PointF(points[0].getX() + currentPoint.X, points[0].getY() + currentPoint.Y);
			startPoint = currentPoint; // Lưu điểm bắt đầu
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
					control1 = PointF(
						2 * currentPoint.X - lastControlPoint.X,
						2 * currentPoint.Y - lastControlPoint.Y
					);
				}
				else {
					control1 = currentPoint; // No reflection, use current point
				}

				PointF control2(points[i].getX(), points[i].getY());
				PointF endPoint(points[i + 1].getX(), points[i + 1].getY());
				graphicsPath.AddBezier(currentPoint, control1, control2, endPoint);

				currentPoint = endPoint;
				lastControlPoint = control2; // Update the last control point
				hasPreviousControlPoint = true;
			}
		}
		else if (command == "s") { // Smooth Cubic Bézier Curve (relative)
			if (points.size() < 2) continue;
			for (size_t i = 0; i + 1 < points.size(); i += 2) {
				PointF control1;
				if (hasPreviousControlPoint) {
					control1 = PointF(
						2 * currentPoint.X - lastControlPoint.X,
						2 * currentPoint.Y - lastControlPoint.Y
					);
				}
				else {
					control1 = currentPoint; // No reflection, use current point
				}

				PointF control2(points[i].getX() + currentPoint.X, points[i].getY() + currentPoint.Y);
				PointF endPoint(points[i + 1].getX() + currentPoint.X, points[i + 1].getY() + currentPoint.Y);
				graphicsPath.AddBezier(currentPoint, control1, control2, endPoint);

				currentPoint = endPoint;
				lastControlPoint = control2; // Update the last control point
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
			graphicsPath.CloseFigure(); // Khép kín hình
			currentPoint = startPoint;  // Cập nhật currentPoint về điểm bắt đầu
		}
		else if (command == "Q") { // Quadratic Bézier Curve (absolute)
			for (size_t i = 0; i + 1 < points.size(); i += 2) {
				PointF controlPoint(points[i].getX(), points[i].getY());
				PointF endPoint(points[i + 1].getX(), points[i + 1].getY());

				// Tính điểm điều khiển trung gian để chuyển sang cubic Bézier
				PointF cubicControl1 = PointF(
					currentPoint.X + (2.0f / 3.0f) * (controlPoint.X - currentPoint.X),
					currentPoint.Y + (2.0f / 3.0f) * (controlPoint.Y - currentPoint.Y));
				PointF cubicControl2 = PointF(
					endPoint.X + (2.0f / 3.0f) * (controlPoint.X - endPoint.X),
					endPoint.Y + (2.0f / 3.0f) * (controlPoint.Y - endPoint.Y));

				graphicsPath.AddBezier(currentPoint, cubicControl1, cubicControl2, endPoint);
				currentPoint = endPoint;
			}
		}
		else if (command == "q") { // Quadratic Bézier Curve (relative)
			for (size_t i = 0; i + 1 < points.size(); i += 2) {
				PointF controlPoint(points[i].getX() + currentPoint.X,
					points[i].getY() + currentPoint.Y);
				PointF endPoint(points[i + 1].getX() + currentPoint.X,
					points[i + 1].getY() + currentPoint.Y);

				PointF cubicControl1 = PointF(
					currentPoint.X + (2.0f / 3.0f) * (controlPoint.X - currentPoint.X),
					currentPoint.Y + (2.0f / 3.0f) * (controlPoint.Y - currentPoint.Y));
				PointF cubicControl2 = PointF(
					endPoint.X + (2.0f / 3.0f) * (controlPoint.X - endPoint.X),
					endPoint.Y + (2.0f / 3.0f) * (controlPoint.Y - endPoint.Y));

				graphicsPath.AddBezier(currentPoint, cubicControl1, cubicControl2, endPoint);
				currentPoint = endPoint;
			}
		}

		else if (command == "A") { // Elliptical arc (absolute)

			for (size_t i = 0; i + 3 < points.size(); i += 4) {
				float rx = points[i].getX();
				float ry = points[i].getY();
				float xAxisRotation = points[i + 1].getX(); // Rotation angle
				bool largeArcFlag = points[i + 1].getY() != 0;
				bool sweepFlag = points[i + 2].getX() != 0;
				PointF endPoint(points[i + 2].getY(), points[i + 3].getX());

				// Chuyển đổi từ arc thành Bézier curves
				vector<PointF> bezierSegments = approximateArcToBezier(
					currentPoint,
					rx, ry,
					xAxisRotation,
					largeArcFlag, sweepFlag,
					endPoint
				);

				// Add each Bezier segment to the GraphicsPath
				for (size_t j = 0; j + 2 < bezierSegments.size(); j += 3) {
					graphicsPath.AddBezier(
						currentPoint,
						bezierSegments[j],
						bezierSegments[j + 1],
						bezierSegments[j + 2]
					);
					currentPoint = bezierSegments[j + 2];
				}

			}
		}
		else if (command == "a") { // Elliptical arc (relative)
			for (size_t i = 0; i + 3 < points.size(); i += 4)
			{
				float rx = points[i].getX();
				float ry = points[i].getY();
				float xAxisRotation = points[i + 1].getX();
				bool largeArcFlag = points[i + 1].getY() != 0;
				bool sweepFlag = points[i + 2].getX() != 0;
				PointF endPoint(points[i + 2].getY() + currentPoint.X,
					points[i + 3].getX() + currentPoint.Y);

				// Chuyển đổi từ arc thành Bézier curves
				vector<PointF> bezierSegments = approximateArcToBezier(
					currentPoint,
					rx, ry,
					xAxisRotation,
					largeArcFlag, sweepFlag,
					endPoint
				);

				// Add each Bezier segment to the GraphicsPath
				for (size_t j = 0; j + 2 < bezierSegments.size(); j += 3) {
					graphicsPath.AddBezier(
						currentPoint,
						bezierSegments[j],
						bezierSegments[j + 1],
						bezierSegments[j + 2]
					);
					currentPoint = bezierSegments[j + 2];
				}

			}
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
		renderFillGradient(graphics, &graphicsPath, grad, path, vb);
	}
	// Vẽ các đường path
	grad = path->getStrokeGradient();
	if (grad == NULL) {
		Pen pen(Color(
			str.getStrokeColor().getOpacity() * 255,
			str.getStrokeColor().getRed(),
			str.getStrokeColor().getGreen(),
			str.getStrokeColor().getBlue()),
			str.getStrokeWidth());
		pen.SetMiterLimit(path->getStrokeMiterLimit());
		if (str.getStrokeWidth() != 0)
			graphics.DrawPath(&pen, &graphicsPath);
	}
	else {
		renderStrokeGradient(graphics, &graphicsPath, grad, path, vb);
	}

	graphics.Restore(save);
}


void Draw::drawGroup(Graphics& graphics, group* g, ViewBox* vb) {
	GraphicsState save = graphics.Save();
	g->applyTransform(graphics);

	for (Shape* child : g->getChildren()) {
		int id = child->nameTonum();
		switch (id) {
		case 1:
		{
			rectangle* rect = dynamic_cast<rectangle*>(child);
			drawRectangle(graphics, rect, vb);
			break;
		}

		case 2:
		{
			circle* cir = dynamic_cast<circle*> (child);
			drawCircle(graphics, cir, vb);
			break;
		}
		case 3:
		{
			polygon* pg = dynamic_cast<polygon*> (child);
			drawPolygon(graphics, pg, vb);
			break;
		}
		case 4:
		{
			polyline* pl = dynamic_cast<polyline*> (child);
			drawPolyline(graphics, pl, vb);
			break;
		}
		case 5:
		{
			line* l = dynamic_cast<line*> (child);
			drawLine(graphics, l, vb);
			break;
		}
		case 6:
		{
			text* t = dynamic_cast<text*> (child);
			drawText(graphics, t, vb);
			break;
		}
		case 7:
		{
			ellipse* e = dynamic_cast<ellipse*> (child);
			drawEllipse(graphics, e, vb);
			break;
		}
		case 8: {
			group* g = dynamic_cast<group*> (child);
			drawGroup(graphics, g, vb);
			break;
		}
		case 9: {
			path* p = dynamic_cast<path*> (child);
			drawPath(graphics, p, vb);
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
void Draw::drawFigure(Graphics& graphics, Figure& figure, float angle, float scale, float transX, float transY, ViewBox* vb) {
	GraphicsState save = graphics.Save();

	graphics.TranslateTransform(transX, transY);
	graphics.ScaleTransform(scale, scale);
	graphics.RotateTransform(angle);

	for (Shape* shape : figure.getList()) {
		int id = shape->nameTonum();
		switch (id) {
		case 1:
		{
			rectangle* rect = dynamic_cast<rectangle*>(shape);
			drawRectangle(graphics, rect, vb);
			break;
		}

		case 2:
		{
			circle* cir = dynamic_cast<circle*> (shape);
			drawCircle(graphics, cir, vb);
			break;
		}
		case 3:
		{
			polygon* pg = dynamic_cast<polygon*> (shape);
			drawPolygon(graphics, pg, vb);
			break;
		}
		case 4:
		{
			polyline* pl = dynamic_cast<polyline*> (shape);
			drawPolyline(graphics, pl, vb);
			break;
		}
		case 5:
		{
			line* l = dynamic_cast<line*> (shape);
			drawLine(graphics, l, vb);
			break;
		}
		case 6:
		{
			text* t = dynamic_cast<text*> (shape);
			drawText(graphics, t, vb);
			break;
		}
		case 7:
		{
			ellipse* e = dynamic_cast<ellipse*> (shape);
			drawEllipse(graphics, e, vb);
			break;
		}
		case 8: {
			group* g = dynamic_cast<group*> (shape);
			drawGroup(graphics, g, vb);
			break;
		}
		case 9: {
			path* p = dynamic_cast<path*> (shape);
			drawPath(graphics, p, vb);
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
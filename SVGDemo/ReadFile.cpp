

#include "stdafx.h"
#include "ReadFile.h"
FileProcess::FileProcess() {
	this->fileName = "";
	LoadColorMap();
}
FileProcess::FileProcess(string name) {
	this->fileName = name;
}
void FileProcess::LoadColorMap() {
	ifstream color_file("Color.txt", ios::in);

	if (!color_file.is_open()) {
		cout << "Error Loading Color File\n";
		return;
	}
	string color_line = "";
	while (getline(color_file, color_line)) {
		stringstream ss(color_line);
		string name = "", hexa_code = "";
		vector<string> vct;

		getline(ss, name, '#');
		ss >> hexa_code;
		hexa_code = "#" + hexa_code;

		if (!name.empty() && hexa_code.length() == 7) {
			MyColor color;
			string result;
			// xoa khoang trang
			for (char ch : name) {
				if (!std::isspace(static_cast<unsigned char>(ch))) {
					result += ch; // Thêm ký tự không phải khoảng trắng
				}
			}
			name = result;
			//tolower
			transform(name.begin(), name.end(), name.begin(), ::tolower);

			color.setRed(stoi(hexa_code.substr(1, 2), NULL, 16));
			color.setGreen(stoi(hexa_code.substr(3, 2), NULL, 16));
			color.setBlue(stoi(hexa_code.substr(5, 2), NULL, 16));
			colorMap[name] = color;
		}
	}
	colorMap["none"] = { 0, 0, 0, 0 };
	color_file.close();
}

MyColor FileProcess::ReadColor(string color) {
	transform(color.begin(), color.end(), color.begin(), ::tolower);
	string temp;
	for (char ch : color) {
		if (!std::isspace(static_cast<unsigned char>(ch))) {
			temp += ch; // Thêm ký tự không phải khoảng trắng
		}
	}
	color = temp;

	MyColor Color;
	if (color == "")
		return Color;

	if (color.find("rgb") != string::npos) {

		regex colorVal(R"(rgb\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\))");
		smatch match;
		if (regex_search(color, match, colorVal)) {
			int r = stoi(match[1]);
			int g = stoi(match[2]);
			int b = stoi(match[3]);

			if (r < 0 || r > 255) {
				Color.setRed(255);
			}
			else {
				Color.setRed(stoi(match[1]));
			}
			if (g < 0 || g > 255) {
				Color.setRed(255);
			}
			else {
				Color.setGreen(stoi(match[2]));

			}
			if (b < 0 || b > 255) {
				Color.setRed(255);
			}
			else {
				Color.setBlue(stoi(match[3]));

			}
		}
	}
	else if (color[0] == '#' && (color.length() == 7 || color.length() == 4)) {
		if (color.length() == 4) {
			// Expand 3-character shorthand color to 6-character format
			string expandedColor = "#";
			expandedColor += color[1];
			expandedColor += color[1];
			expandedColor += color[2];
			expandedColor += color[2];
			expandedColor += color[3];
			expandedColor += color[3];
			color = expandedColor; // Replace shorthand with expanded
		}
		Color.setRed(stoi(color.substr(1, 2), NULL, 16));
		Color.setGreen(stoi(color.substr(3, 2), NULL, 16));
		Color.setBlue(stoi(color.substr(5, 2), NULL, 16));
	}
	else {
		cout << color << endl;
		auto it = colorMap.find(color);
		if (it != colorMap.end()) {
			Color = it->second;
			//cout << it->second.getRed() << "," << it->second.getBlue() << "," << it->second.getGreen() << endl;
		}
		else {
			cout << "Khong tim duoc mau\n";
		}
	}
	return Color;
}

vector<point> FileProcess::ReadPoint(string Point) {
	vector<point> points;
	regex pointRegex(R"((-?\d*\.?\d+)\s*,?\s*(-?\d*\.?\d+))"); // Xử lý cặp tọa độ
	smatch match;
	auto it = Point.cbegin();

	while (regex_search(it, Point.cend(), match, pointRegex)) {
		float x = stof(match[1].str());
		float y = stof(match[2].str());
		points.push_back(point(x, y));
		it = match[0].second;
	}
	return points;
}

void FileProcess::ReadStrokeAndFile(map<string, string> attributes, Shape* shape) {
	MyColor color;
	if (attributes["fill-opacity"] != "") {
		//color.setOpacity(stof(attributes["fill-opacity"]));
		shape->getFillColor().setOpacity(stof(attributes["fill-opacity"]));
	}
	if (attributes["fill"] != "") {
		color = ReadColor(attributes["fill"]);
		shape->getFillColor().setBlue(color.getBlue());
		shape->getFillColor().setRed(color.getRed());
		shape->getFillColor().setGreen(color.getGreen());
	}
	if (attributes["fill"] == "none") {
		color = MyColor(255, 255, 255, 0);
		shape->setFillColor(color);
	}

	MyColor stroke;
	if (attributes["stroke"] != "") {
		stroke = ReadColor(attributes["stroke"]);
		shape->setStrokeWidth(1);
		shape->setStrokeColor(stroke);
	}
	if (attributes["stroke-opacity"] != "") {
		shape->getStrokeColor().setOpacity(stof(attributes["stroke-opacity"]));
	}
	if (attributes["stroke-width"] != "") {
		shape->setStrokeWidth(stof(attributes["stroke-width"]));
	}
	if (attributes["stroke"] == "none") {
		shape->setStrokeWidth(0);
	}
}

vector<TransformCommand> FileProcess::ReadTranCom(string trans) {
	vector <TransformCommand> transcom;

	regex attriPair(R"((\b[a-zA-Z]+\b)\(([^)]+)\))");
	smatch match;
	std::map<std::string, std::string> attributes;
	auto it = trans.cbegin();
	while (regex_search(it, trans.cend(), match, attriPair)) {
		string name = match[1];
		string value = match[2];
		TransformCommand temp;
		/*if (name == "translate") {
			temp.setName("translate");
			smatch valMatch;
			regex transVal(R"(\s*(-?\d+\.?\d*)\s*,\s*(-?\d+\.?\d*)\s*)");
			if (regex_search(value, valMatch, transVal)) {
				temp.setTranslate(stof(valMatch[1]), stof(valMatch[2]));
			}
		}*/
		if (name == "translate") {
			temp.setName("translate");
			smatch valMatch;
			//regex transVal(R"(\s*(-?\d+(\.\d+)?)\s+(-?\d+(\.\d+)?)\s*)");
			regex transVal(R"(\s*(-?\d+(\.\d+)?)[\s,]*(-?\d+(\.\d+)?)\s*)");
			if (regex_search(value, valMatch, transVal)) {
				cout << "!!!!!!!!!!!!!!!" << valMatch[1] << " " << valMatch[3] << endl;
				temp.setTranslate(stof(valMatch[1]), stof(valMatch[3]));
			}
		}
		else if (name == "rotate") {
			temp.setName("rotate");
			smatch valMatch;
			regex transVal(R"(\s*(-?\d+\.?\d*)\s*)");
			if (regex_search(value, valMatch, transVal)) {
				temp.setRotate(stof(valMatch[1]));
			}
		}
		else if (name == "scale") {
			temp.setName("scale");
			smatch valMatch;
			regex transVal1(R"(\s*(-?\d+\.?\d*)\s*,\s*(-?\d+\.?\d*)\s*)");
			regex transVal2(R"(\s*(-?\d+\.?\d*)\s*)");

			if (regex_search(value, valMatch, transVal1)) {
				temp.setScale(stof(valMatch[1]), stof(valMatch[2]));
			}
			else if (regex_search(value, valMatch, transVal2)) {
				temp.setScale(stof(valMatch[1]));
			}
		}
		transcom.push_back(temp);
		it = match[0].second;
	}
	return transcom;
}


path FileProcess::ReadPath(string d) {
	if (d.empty()) {
		throw std::invalid_argument("Path string cannot be empty.");
	}

	vector<pair<string, vector<point>>> pathVct;
	regex commandRegex(R"(([MLHVZCSmlhvzcs])([^MLHVZCSmlhvzcs]*))");
	smatch match;

	point lastPoint = { 0, 0 };
	auto it = d.cbegin();

	while (regex_search(it, d.cend(), match, commandRegex)) {
		string command = match[1].str(); // Lấy lệnh (M, C, H, V, Z)
		string args = match[2].str();    // Lấy tham số liên quan đến lệnh

		pair<string, vector<point>> pathSegment;

		pathSegment.first = command;
		if (command == "M" || command == "L" || command == "C" || command == "m" || command == "l" || command == "c" || command == "S" || command == "s") {
			vector<point> points = ReadPoint(args); // Chuyển chuỗi tọa độ thành vector<point>
			pathSegment.second = points;
			if (!points.empty()) {
				lastPoint = points.back(); // Cập nhật điểm cuối
			}
		}
		else if (command == "H" || command == "h") {
			stringstream ss(args);
			float coord;
			while (ss >> coord) {
				lastPoint.setX(coord); // Chỉ cập nhật trục x
				pathSegment.second.push_back(lastPoint);
			}
		}
		else if (command == "V" || command == "v") {
			stringstream ss(args);
			float coord;
			while (ss >> coord) {
				lastPoint.setY(coord); // Chỉ cập nhật trục y
				pathSegment.second.push_back(lastPoint);
			}
		}
		else if (command == "Z" || command == "z") {
			pathSegment.second.push_back(lastPoint);
		}

		if (command == "M" && pathSegment.second.size() > 1) {
			vector<point> tmp = pathSegment.second;
			pathVct.push_back({ "M", { tmp[0] } });
			//neu M co nhieu diem theo sau thi L toi cac diem do

			for (size_t i = 1; i < tmp.size(); ++i) {
				pathVct.push_back({ "L", { tmp[i] } });
			}
		}
		else pathVct.push_back(pathSegment);
		it = match[0].second;
	}

	path P;
	P.setPath(pathVct);
	return P;
}

Shape* FileProcess::Readshape(map<string, string> attributes, string name) {

	Shape* shape = NULL;
	if (name == "rect") {
		//cout << " rect ";
		shape = new rectangle;
		rectangle* temp = dynamic_cast<rectangle*>(shape);

		shape->setName(name);  // dat ten
		this->ReadStrokeAndFile(attributes, shape); // Doc fill va stroke color

		//doc thuoc tinh
		if (attributes["x"] != "") {
			temp->setRecX(stof(attributes["x"]));
		}
		if (attributes["y"] != "") {
			temp->setRecY(stof(attributes["y"]));
		}
		if (attributes["width"] != "") {
			temp->setWidth(stof(attributes["width"]));
		}
		if (attributes["height"] != "") {
			temp->setHeight(stof(attributes["height"]));
		}
		if (attributes["transform"] != "") {
			vector<TransformCommand> transcom = ReadTranCom(attributes["transform"]);
			temp->setTransform(transcom);
		}
	}
	else if (name == "circle") {

		shape = new circle;
		circle* temp = dynamic_cast<circle*>(shape);

		shape->setName(name);  // dat ten
		this->ReadStrokeAndFile(attributes, shape); // Doc fill va stroke color

		if (attributes["cx"] != "") {
			temp->setCx(stof(attributes["cx"]));
		}
		if (attributes["cy"] != "") {
			temp->setCy(stof(attributes["cy"]));
		}
		if (attributes["r"] != "") {
			temp->setRadius(stof(attributes["r"]));
		}
		if (attributes["transform"] != "") {
			vector<TransformCommand> transcom = ReadTranCom(attributes["transform"]);
			temp->setTransform(transcom);
		}
	}
	else if (name == "line") {
		shape = new line;
		line* temp = dynamic_cast<line*>(shape);
		shape->setName(name);  // dat ten
		this->ReadStrokeAndFile(attributes, shape); // Doc fill va stroke color

		if (attributes["x1"] != "") {
			temp->setX1(stof(attributes["x1"]));
		}
		if (attributes["y1"] != "") {
			temp->setY1(stof(attributes["y1"]));
		}
		if (attributes["x2"] != "") {
			temp->setX2(stof(attributes["x2"]));
		}
		if (attributes["y2"] != "") {
			temp->setY2(stof(attributes["y2"]));
		}
		if (attributes["transform"] != "") {
			vector<TransformCommand> transcom = ReadTranCom(attributes["transform"]);
			temp->setTransform(transcom);
		}
	}
	else if (name == "polyline") {
		shape = new polyline();
		polyline* temp = dynamic_cast<polyline*>(shape);
		shape->setName(name);  // dat ten
		this->ReadStrokeAndFile(attributes, shape); // Doc fill va stroke color
		if (attributes["points"] != "") {
			temp->setPoints(ReadPoint(attributes["points"]));
		}
		if (attributes["transform"] != "") {
			vector<TransformCommand> transcom = ReadTranCom(attributes["transform"]);
			temp->setTransform(transcom);
		}
	}
	else if (name == "polygon") {
		shape = new polygon();
		polygon* temp = dynamic_cast<polygon*>(shape);
		shape->setName(name);  // dat ten
		this->ReadStrokeAndFile(attributes, shape); // Doc fill va stroke color
		if (attributes["points"] != "") {
			temp->setPoints(ReadPoint(attributes["points"]));
		}
		if (attributes["transform"] != "") {
			vector<TransformCommand> transcom = ReadTranCom(attributes["transform"]);
			temp->setTransform(transcom);
		}
	}
	else if (name == "text") {
		shape = new text;
		text* temp = dynamic_cast<text*>(shape);
		shape->setName(name);  // dat ten
		this->ReadStrokeAndFile(attributes, shape);
		if (attributes["font-size"] != "") {
			temp->setFontSize(stof(attributes["font-size"]));
		}
		if (attributes["x"] != "" && attributes["y"] != "") {
			temp->setTextPos(point(stof(attributes["x"]), stof(attributes["y"])));
		}
		if (attributes["dx"] != "") {
			temp->setDx(stof(attributes["dx"]));
		}
		if (attributes["dy"] != "") {
			temp->setDy(stof(attributes["dy"]));
		}
		if (attributes["font-family"] != "") {
			temp->setFontFamily(attributes["font-family"]);
		}
		if (attributes["font-style"] != "") {
			temp->setFontStyle(attributes["font-style"]);
		}
		if (attributes["text-anchor"] != "") {
			temp->setTextAnchor(attributes["text-anchor"]);
		}
		if (attributes["transform"] != "") {
			vector<TransformCommand> transcom = ReadTranCom(attributes["transform"]);
			temp->setTransform(transcom);
		}
	}
	else if (name == "ellipse") {
		shape = new ellipse;
		ellipse* temp = dynamic_cast<ellipse*>(shape);
		shape->setName(name);  // dat ten
		this->ReadStrokeAndFile(attributes, shape);
		if (attributes["cx"] != "") {
			temp->setCx(stof(attributes["cx"]));
		}
		if (attributes["cy"] != "") {
			temp->setCy(stof(attributes["cy"]));
		}

		if (attributes["rx"] != "") {
			temp->setRx(stof(attributes["rx"]));
		}
		if (attributes["ry"] != "") {
			temp->setRy(stof(attributes["ry"]));
		}
		if (attributes["transform"] != "") {
			vector<TransformCommand> transcom = ReadTranCom(attributes["transform"]);
			temp->setTransform(transcom);
		}
	}
	else if (name == "path") {
		shape = new path();
		path* temp = dynamic_cast<path*>(shape);
		shape->setName(name);  // dat ten
		this->ReadStrokeAndFile(attributes, shape);
		if (attributes["d"] != "") {
			path p = ReadPath(attributes["d"]);
			temp->setName(p.getName());
			temp->setPath(p.getPath());
		}
		if (attributes["transform"] != "") {
			vector<TransformCommand> transcom = ReadTranCom(attributes["transform"]);
			temp->setTransform(transcom);
		}

	}
	else if (name == "g") {
		//Shape* parent = NULL;
		shape = new group();
		group* temp = dynamic_cast<group*>(shape);
		shape->setName(name);  // dat ten
		this->ReadStrokeAndFile(attributes, shape);

		if (attributes["transform"] != "") {
			vector<TransformCommand> transcom = ReadTranCom(attributes["transform"]);
			temp->setTransform(transcom);
		}
	}
	else {
		shape = NULL;
	}
	return shape;
}

void FileProcess::ReadGroupChild(map<string, string> pAttributes, group* parentGroup, fstream& fi) {
	string s;

	while (getline(fi, s, '>')) {
		stringstream ss(s);
		char temp;
		ss >> temp;

		string name;
		ss >> name;

		regex attriPair(R"(\s*([a-zA-Z0-9-]+)=["']([^"']+)["'])");
		smatch match;
		std::map<std::string, std::string> attributes;

		auto it = s.cbegin();
		while (regex_search(it, s.cend(), match, attriPair)) {
			attributes[match[1]] = match[2];
			it = match[0].second;
		}

		if (name == "/g") {
			return;
		}

		//set cac thuoc tinh o dong parrent attibutes cho child
		Shape* shape = this->Readshape(attributes, name);
		if(shape){
			ReadStrokeAndFile(pAttributes, shape); // bị mẹ đè
			ReadStrokeAndFile(attributes, shape);
		}

		if (pAttributes["font-size"] != "" && shape->getName() == "text") {
			dynamic_cast<text*>(shape)->setFontSize(stof(pAttributes["font-size"]));
		}


		if (name == "text") {
			// Đọc content 
			getline(fi, s, '>');
			s += '>';
			smatch matchContent;
			regex textPattern(R"(([\s\S]*?)</text>)");
			if (regex_search(s, matchContent, textPattern)) {
				string content = matchContent[1].str();
				if (shape) {
					text* temp = dynamic_cast<text*>(shape);
					if (temp) {
						temp->setContent(content);
					}
				}
			}
		}

		if (name == "g") {
			map<string, string> grandAndparent = pAttributes; // Start with map1
			grandAndparent.insert(attributes.begin(), attributes.end());
			group* childGroup = dynamic_cast<group*>(shape);
			if (childGroup) {
				this->ReadGroupChild(grandAndparent, childGroup, fi);
			}
		}

		if (shape != nullptr) {
			parentGroup->addChild(shape);
		}
	}
}
void FileProcess::ShowShape(Shape* shape) {
	if (shape == NULL)
		return;
	if (shape->getName() == "rect") {
		rectangle* temp = dynamic_cast<rectangle*>(shape);
		cout << " name " << temp->getName() << endl;
		cout << " x " << temp->getRecX() << endl;
		cout << " y " << temp->getRecY() << endl;
		cout << " width " << temp->getWidth() << endl;
		cout << " height " << temp->getHeight() << endl;
	}
	else if (shape->getName() == "circle") {
		circle* temp = dynamic_cast<circle*>(shape);
		cout << " radius " << temp->getRadius() << endl;
		cout << " cx " << temp->getCx() << endl;
		cout << " cy " << temp->getCy() << endl;
	}
	else if (shape->getName() == "line") {
		line* temp = dynamic_cast<line*>(shape);
		cout << " name " << temp->getName() << endl;
		cout << " x1 " << temp->getX1() << endl;
		cout << " y1 " << temp->getY1() << endl;
		cout << " x2 " << temp->getX2() << endl;
		cout << " y2 " << temp->getY2() << endl;
	}
	else if (shape->getName() == "polyline") {
		polyline* temp = dynamic_cast<polyline*>(shape);
		vector <point> points = temp->getPoints();
		for (auto p : points) {
			cout << "Point: (" << p.getX() << ", " << p.getY() << ")" << endl;
		}
	}
	else if (shape->getName() == "polygon") {
		cout << "polygon" << " ";
		polygon* temp = dynamic_cast<polygon*>(shape);
		vector <point> points = temp->getPoints();
		for (auto p : points) {
			cout << "Point: (" << p.getX() << ", " << p.getY() << ")" << endl;
		}
	}
	else if (shape->getName() == "text") {
		text* temp = dynamic_cast<text*>(shape);
		cout << " fontSize " << temp->getFontSize() << endl;
		cout << " fontFamily " << temp->getFontFamily() << endl;

		cout << " content " << temp->getContent() << endl;
	}

	else if (shape->getName() == "path") {
		path* temp = dynamic_cast<path*>(shape);
		cout << " name " << temp->getName() << endl;
		vector<pair<string, vector<point>>>pairPath = temp->getPath();
		for (auto it : pairPath) {
			vector <point> points = it.second;
			cout << it.first << " " << endl; // in ra command
			for (auto p : points) {
				cout << p.getX() << ", " << p.getY() << " ";
			}
			cout << endl;
		}
	}
	else if (shape->getName() == "g") {
		group* temp = dynamic_cast<group*>(shape);
		cout << " name " << temp->getName() << endl;
		cout << "parent: \n";
		ShowShape(temp->getParent());
		cout << endl;
		vector<Shape*> children = temp->getChildren();
		cout << "Number of children in group: " << children.size() << endl;
		for (auto child : children) {
			cout << "CHILD -----\n";
			ShowShape(child);
		}
	}

	cout << " stroke width " << shape->getStrokeWidth() << endl;
	cout << " fill " << shape->getFillColor().getRed() << " " << shape->getFillColor().getGreen() << " " << shape->getFillColor().getBlue() << " " << shape->getFillColor().getOpacity() << endl;
	cout << " stroke " << shape->getStrokeColor().getRed() << " " << shape->getStrokeColor().getGreen() << " " << shape->getStrokeColor().getBlue() << " " << shape->getStrokeColor().getOpacity() << endl;
	vector<TransformCommand>  Trans = shape->getTransform();
	int size = Trans.size();
	for (int i = 0; i < size; i++) {
		cout << Trans[i].getName() << " transX: " << Trans[i].getTransX() << " transY:  " << Trans[i].getTransY() << " rotate: " << Trans[i].getAngle() << " scaleX: " << Trans[i].getScaleX() << " scaleY: " << Trans[i].getScaleY() << " " << endl;
	}
}

vector <Shape*> FileProcess::ReadFile() {
	vector <Shape*> figure;
	LoadColorMap();
	string key;
	MyColor value;

	figure.resize(0);
	fstream fi;
	string name = this->GetFileName();
	fi.open(name.c_str(), ios::in);
	int count = 0;

	if (fi.fail()) {
		cout << "Cannot open file";
		return figure;
	}
	string s;
	getline(fi, s, '>');
	//getline(fi, s, '>');
	while (getline(fi, s, '>')) {
		stringstream ss(s);
		char temp;
		ss >> temp;

		string name;
		ss >> name;

		regex attriPair(R"(\s*([a-zA-Z0-9-]+)=["']([^"']+)["'])");
		smatch match;

		std::map<std::string, std::string> attributes;

		auto it = s.cbegin();
		while (regex_search(it, s.cend(), match, attriPair)) {
			attributes[match[1]] = match[2]; // match[1]: tên thuộc tính, match[2]: giá trị thuộc tính (còn match[0] là nguyên chuỗi khớp)
			it = match[0].second; // Cập nhật vị trí để tìm kiếm tiếp ( trỏ đến vị trí tiếp theo)
		}

		Shape* shape = this->Readshape(attributes, name);
		if (name == "text") {
			getline(fi, s, '>');
			s += '>';
			smatch matchContent;
			regex textPattern(R"((.*?)</text>)");
			if (regex_search(s, matchContent, textPattern)) {
				// match[1] chứa nội dung giữa thẻ <text>
				string content = matchContent[1].str();
				if (shape) {
					text* temp = dynamic_cast<text*> (shape);
					temp->setContent(content);
				}
			}
		}
		if (name == "g") {
			//Shape* parent = shape;
			this->ReadGroupChild(attributes, dynamic_cast<group*>(shape), fi);
		}
		if (shape != NULL) {
			figure.push_back(shape);
		}
	}
	fi.close();
	return figure;
}

string FileProcess::GetFileName() {
	return this->fileName;
}




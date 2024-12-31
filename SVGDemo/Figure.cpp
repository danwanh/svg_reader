#include "stdafx.h"
#include "Figure.h"
Figure::Figure() {
    vb = new ViewBox();
}
void Figure::setViewBox(ViewBox* vb_) {
	vb = vb_;
}
ViewBox* Figure::getViewBox() {
	return vb;
}
vector<Shape*> Figure::getList() {
	return list;
}
void Figure::setList(vector<Shape*> list) {
	this->list = list;
}

Figure::~Figure() {
    if (vb) {
        delete vb;
        vb = nullptr;
    }

    // Giải phóng từng phần tử trong danh sách `list`
    for (Shape* shape : list) {
        delete shape;
    }
    list.clear(); // Xóa toàn bộ các con trỏ đã bị xóa

    // Giải phóng từng gradient trong `grad`
    for (auto& pair : grad) {
        delete pair.second; // pair.second là con trỏ gradient*
    }
    grad.clear();
}
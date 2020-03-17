#include "MyWiget.h"
#include<qpushbutton.h>
#include<QFileDialog>
#include<QPainter>
#include "../Painter/obj.cpp"
//从dll中调用
void LoadFromFile(ifstream& input, vector<Line>& lines, vector<Ray>& rays,
	vector<Segment>& segments, vector<Circle>& circles) {};

//从dll中调用
void Calculate_Points(set<Point>& points, vector<Line>& lines, vector<Ray>& rays,
	vector<Segment>& segments, vector<Circle>& circles) {};

//从容器中删除几何对象
void MyWiget::delete_obj(QString& str) {
	string s = str.toStdString();
	if (s[0] == 'L') {
		vector<Line>::iterator iter = lines.begin();
		for (; iter != lines.end(); iter++)
			if (s == iter->display()) {
				lines.erase(iter); break;
			}
	}
	else if (s[0] == 'R') {
		vector<Ray>::iterator iter = rays.begin();
		for (; iter != rays.end(); iter++)
			if (s == iter->display()) {
				rays.erase(iter); break;
			}
	}
	else if (s[0] == 'S') {
		vector<Segment>::iterator iter = segments.begin();
		for (; iter != segments.end(); iter++)
			if (s == iter->display()) {
				segments.erase(iter); break;
			}
	}
	else if (s[0] == 'C') {
		vector<Circle>::iterator iter = circles.begin();
		for (; iter != circles.end(); iter++)
			if (s == iter->display()) {
				circles.erase(iter); break;
			}
	}
}

//UI内的各种点击事件
MyWiget::MyWiget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle("Painter");


	//从文件导入几何对象
	connect(ui.btn_loadfile, &QPushButton::clicked, [=]() {

		QString cur_path = QCoreApplication::applicationDirPath();//当前文件夹路径
		QString path = QFileDialog::getOpenFileName(this, "open file", cur_path);
		string file_name = path.toStdString();

		//TODO,调用计算模块读取文件的函数LoadFromFile
		ifstream fin;
		fin.open(file_name);
		LoadFromFile(fin, lines, rays, segments, circles);

		//将各vector的引用传给计算模块,并根据返回值更新point_num
		Calculate_Points(points, lines, rays, segments, circles);

		//更新UI显示的交点数
		QString str;
		str.setNum(points.size());
		ui.lineEdit_ans->setText(str);

		//更新UI中下拉栏已有的几何对象，建立映射
		for (auto& obj : lines) {
			QString str = QString::fromStdString(obj.display());
			ui.comboBox_delete->addItem(str);
			line_map.insert(str, obj);
		}
		for (auto& obj : rays) {
			QString str = QString::fromStdString(obj.display());
			ui.comboBox_delete->addItem(str);
			ray_map.insert(str, obj);
		}
		for (auto& obj : segments) {
			QString str = QString::fromStdString(obj.display());
			ui.comboBox_delete->addItem(str);
			segment_map.insert(str, obj);
		}
		for (auto& obj : circles) {
			QString str = QString::fromStdString(obj.display());
			ui.comboBox_delete->addItem(str);
			circle_map.insert(str, obj);
		}
	});


	//添加几何对象,假设UI输入的几何对象与已有几何对象不重合（不查重）
	connect(ui.btn_add, &QPushButton::clicked, [=]() {
		//提取几何对象种类
		QString str = ui.comboBox_type->currentText();

		//提取四个数TODO
		int fir_num = ui.lineEdit_1->displayText().toInt();
		int sec_num = ui.lineEdit_2->displayText().toInt();
		int	thi_num = ui.lineEdit_3->displayText().toInt();
		int	four_num = ui.lineEdit_4->displayText().toInt();

		//创建对象,并更新UI中下拉栏已有的几何对象,建立映射
		if (str == QString("line")) {
			Line line(fir_num, sec_num, thi_num, four_num);
			lines.push_back(line);
			QString str = QString::fromStdString(line.display());
			ui.comboBox_delete->addItem(str);
			line_map.insert(str, line);
		}
		else if (str == QString("ray")) {
			Ray ray(fir_num, sec_num, thi_num, four_num);
			rays.push_back(ray);
			QString str = QString::fromStdString(ray.display());
			ui.comboBox_delete->addItem(str);
			ray_map.insert(str, ray);
		}
		else if (str == QString("segment")) {
			Segment seg(fir_num, sec_num, thi_num, four_num);
			segments.push_back(seg);
			QString str = QString::fromStdString(seg.display());
			ui.comboBox_delete->addItem(str);
			segment_map.insert(str, seg);
		}
		else if (str == QString("circle")) {
			Circle circle(fir_num, sec_num, thi_num);
			circles.push_back(circle);
			QString str = QString::fromStdString(circle.display());
			ui.comboBox_delete->addItem(str);
			circle_map.insert(str, circle);
		}

		//将各容器引用传给计算模块,并更新points
		Calculate_Points(points, lines, rays, segments, circles);


		//更新UI显示的交点数
		str.setNum(points.size());
		ui.lineEdit_ans->setText(str);
	});


	//删除几何对象
	connect(ui.btn_delete, &QPushButton::clicked, [=]() {
		//更新本地几何对象集合
		QString cur_str = ui.comboBox_delete->currentText();
		this->delete_obj(cur_str);//删除容器中的几何对象
		//删除映射
		int cur_index = ui.comboBox_delete->currentIndex();
		ui.comboBox_delete->removeItem(cur_index);

		//调用计算模块，更新points
		Calculate_Points(points, lines, rays, segments, circles);

		//更新UI的交点数
		QString str;
		str.setNum(points.size());
		ui.lineEdit_ans->setText(str);
	});


	ui.lineEdit_ans->setText("0");
}

////画图函数，TODO
void MyWiget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	painter.drawLine(QPoint(800, 800), QPoint(900, 900));
	painter.drawEllipse(QPoint(800, 800), 50, 50);
}

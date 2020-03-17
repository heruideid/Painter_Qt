#include "MyWiget.h"
#include<qpushbutton.h>
#include<QFileDialog>
#include<QPainter>
#include "../Painter/obj.cpp"
//��dll�е���
void LoadFromFile(ifstream& input, vector<Line>& lines, vector<Ray>& rays,
	vector<Segment>& segments, vector<Circle>& circles) {};

//��dll�е���
void Calculate_Points(set<Point>& points, vector<Line>& lines, vector<Ray>& rays,
	vector<Segment>& segments, vector<Circle>& circles) {};

//��������ɾ�����ζ���
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

//UI�ڵĸ��ֵ���¼�
MyWiget::MyWiget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowTitle("Painter");


	//���ļ����뼸�ζ���
	connect(ui.btn_loadfile, &QPushButton::clicked, [=]() {

		QString cur_path = QCoreApplication::applicationDirPath();//��ǰ�ļ���·��
		QString path = QFileDialog::getOpenFileName(this, "open file", cur_path);
		string file_name = path.toStdString();

		//TODO,���ü���ģ���ȡ�ļ��ĺ���LoadFromFile
		ifstream fin;
		fin.open(file_name);
		LoadFromFile(fin, lines, rays, segments, circles);

		//����vector�����ô�������ģ��,�����ݷ���ֵ����point_num
		Calculate_Points(points, lines, rays, segments, circles);

		//����UI��ʾ�Ľ�����
		QString str;
		str.setNum(points.size());
		ui.lineEdit_ans->setText(str);

		//����UI�����������еļ��ζ��󣬽���ӳ��
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


	//��Ӽ��ζ���,����UI����ļ��ζ��������м��ζ����غϣ������أ�
	connect(ui.btn_add, &QPushButton::clicked, [=]() {
		//��ȡ���ζ�������
		QString str = ui.comboBox_type->currentText();

		//��ȡ�ĸ���TODO
		int fir_num = ui.lineEdit_1->displayText().toInt();
		int sec_num = ui.lineEdit_2->displayText().toInt();
		int	thi_num = ui.lineEdit_3->displayText().toInt();
		int	four_num = ui.lineEdit_4->displayText().toInt();

		//��������,������UI�����������еļ��ζ���,����ӳ��
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

		//�����������ô�������ģ��,������points
		Calculate_Points(points, lines, rays, segments, circles);


		//����UI��ʾ�Ľ�����
		str.setNum(points.size());
		ui.lineEdit_ans->setText(str);
	});


	//ɾ�����ζ���
	connect(ui.btn_delete, &QPushButton::clicked, [=]() {
		//���±��ؼ��ζ��󼯺�
		QString cur_str = ui.comboBox_delete->currentText();
		this->delete_obj(cur_str);//ɾ�������еļ��ζ���
		//ɾ��ӳ��
		int cur_index = ui.comboBox_delete->currentIndex();
		ui.comboBox_delete->removeItem(cur_index);

		//���ü���ģ�飬����points
		Calculate_Points(points, lines, rays, segments, circles);

		//����UI�Ľ�����
		QString str;
		str.setNum(points.size());
		ui.lineEdit_ans->setText(str);
	});


	ui.lineEdit_ans->setText("0");
}

////��ͼ������TODO
void MyWiget::paintEvent(QPaintEvent *event) {
	QPainter painter(this);

	painter.drawLine(QPoint(800, 800), QPoint(900, 900));
	painter.drawEllipse(QPoint(800, 800), 50, 50);
}

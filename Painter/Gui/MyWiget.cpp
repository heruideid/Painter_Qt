#include "MyWiget.h"
#include<qpushbutton.h>
#include<QFileDialog>
#include<QPainter>
#include<QMessageBox>
#include "../Painter/obj.cpp"

////��dll�е���
//void LoadFromFile(ifstream& input, vector<Line>& lines, vector<Ray>& rays,
//	vector<Segment>& segments, vector<Circle>& circles) {};
//
////��dll�е���
//void Calculate_Points(set<Point>& points, vector<Line>& lines, vector<Ray>& rays,
//	vector<Segment>& segments, vector<Circle>& circles) {};

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

	//
	
	//���ļ����뼸�ζ���
	connect(ui.btn_loadfile, &QPushButton::clicked, [=]() {

		QString cur_path = QCoreApplication::applicationDirPath();//��ǰ�ļ���·��
		QString path = QFileDialog::getOpenFileName(this, "open file", cur_path);
		string file_name = path.toStdString();

		//ȡ�����ļ�����
		if (file_name == "") return;

		//���ü���ģ���ȡ�ļ��ĺ���LoadFromFile
		try {
			//��ս��㼯�͸����ζ������¼���
			points.clear();
			lines.clear();
			rays.clear();
			segments.clear();
			circles.clear();
			//
			ifstream fin;
			fin.open(file_name);
			LoadFromFile(fin, lines, rays, segments, circles);
			Calculate_Points(points, lines, rays, segments, circles);
		}
		catch (exception& e) {
			QMessageBox::critical(this,"critical",e.what());
		}

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

		//��ȡ�ĸ���
		int fir_num = ui.lineEdit_1->displayText().toInt();
		int sec_num = ui.lineEdit_2->displayText().toInt();
		int	thi_num = ui.lineEdit_3->displayText().toInt();
		int	four_num = ui.lineEdit_4->displayText().toInt();

		//���Ϸ���
		try {
			exception_if_outBorder(fir_num);
			exception_if_outBorder(sec_num);
			exception_if_outBorder(thi_num);
			exception_if_outBorder(four_num);
			if (str == QString("circle")) { exception_if_illegalRadius(thi_num); }
			else { exception_if_samePoints(fir_num, sec_num, thi_num, four_num); }
		}
		catch (exception& e) {
			QMessageBox::critical(this,"critical",e.what());
			return;
		}

		//��������,������UI�����������еļ��ζ���,����ӳ��
		if (str == QString("line")) {
			Line line(fir_num, sec_num, thi_num, four_num);
			lines.push_back(line);
			QString display_str = QString::fromStdString(line.display());
			ui.comboBox_delete->addItem(display_str);
			line_map.insert(display_str, line);
			try {
				Calculate_Points(points, lines, rays, segments, circles);
			}
			catch (exception& e) {
				QMessageBox::critical(this, "critical", e.what());
				//ɾ���Ѳ���Ķ���������������е���ʾ
				lines.pop_back(); 
				ui.comboBox_delete->removeItem(ui.comboBox_delete->findText(display_str));
			}
		}
		else if (str == QString("ray")) {
			Ray ray(fir_num, sec_num, thi_num, four_num);
			rays.push_back(ray);
			QString display_str = QString::fromStdString(ray.display());
			ui.comboBox_delete->addItem(display_str);
			ray_map.insert(display_str, ray);
			try {
				Calculate_Points(points, lines, rays, segments, circles);
			}
			catch (exception& e) {
				QMessageBox::critical(this, "critical", e.what());
				rays.pop_back();
				ui.comboBox_delete->removeItem(ui.comboBox_delete->findText(display_str));
			}
		}
		else if (str == QString("segment")) {
			Segment seg(fir_num, sec_num, thi_num, four_num);
			segments.push_back(seg);
			QString display_str = QString::fromStdString(seg.display());
			ui.comboBox_delete->addItem(display_str);
			segment_map.insert(display_str, seg);
			try {
				Calculate_Points(points, lines, rays, segments, circles);
			}
			catch (exception& e) {
				QMessageBox::critical(this, "critical", e.what());
				segments.pop_back();
				ui.comboBox_delete->removeItem(ui.comboBox_delete->findText(display_str));
			}
		}
		else if (str == QString("circle")) {
			
			Circle circle(fir_num, sec_num, thi_num);
			circles.push_back(circle);
			QString display_str = QString::fromStdString(circle.display());
			ui.comboBox_delete->addItem(display_str);
			circle_map.insert(display_str, circle);
			try {
				Calculate_Points(points, lines, rays, segments, circles);
			}
			catch (exception& e) {
				QMessageBox::critical(this, "critical", e.what());
				circles.pop_back();
				ui.comboBox_delete->removeItem(ui.comboBox_delete->findText(display_str));
			}
		}

	});


	//ɾ�����ζ���
	connect(ui.btn_delete, &QPushButton::clicked, [=]() {
		//���±��ؼ��ζ��󼯺�
		QString cur_str = ui.comboBox_delete->currentText();
		this->delete_obj(cur_str);//ɾ�������еļ��ζ���
		//ɾ��ӳ��
		int cur_index = ui.comboBox_delete->currentIndex();
		ui.comboBox_delete->removeItem(cur_index);

		//����points
		points.clear();

		//���ü���ģ�飬����points
		try {
			Calculate_Points(points, lines, rays, segments, circles);
		}
		catch (exception& e) {
			QMessageBox::critical(this, "critical", e.what());
		}
	});

	connect(ui.btn_paint, &QPushButton::clicked, [=]() {
		update();
		//����UI��ʾ�Ľ�����
		QString str;
		str.setNum(points.size());
		ui.lineEdit_ans->setText(str);
	});

	//��������ʼ��Ϊ0
	ui.lineEdit_ans->setText("0");
}


////��ͼ����
void MyWiget::paintEvent(QPaintEvent *event) {

	QPainter painter(this);
	//painter.drawLine(QLineF(QPoint(-400, 280), QPoint(100, 280)));
	painter.setRenderHint(QPainter::Antialiasing);

	painter.translate(500, 500);//��������ԭ��
	//���廭������ y>=280,O����(500,500)

	//���������� 
	painter.drawLine(QLineF(QPoint(-500, 0), QPoint(500, 0)));
	painter.drawLine(QLineF(QPoint(0, 500), QPoint(0, -220)));

	//������20:1
	//���Ƽ��ζ���
	QPen pen((QColor)Qt::darkCyan);
	pen.setWidth(3);
	painter.setPen(pen);

	for (auto& line : lines) {
		double A = line.A, B = line.B, C = line.C;
		double x1, y1 = 11, x2, y2 = -15;
		if (A == 0) {
			y1 = y2 = -C / B;
		}
		else {
			x1 = (-C - B * y1) / A;
			x2 = (-C - B * y2) / A;
		}
		painter.drawLine(QPoint(x1 * 20, -y1 * 20), QPoint(x2 * 20, -y2 * 20));
		//y=11;x1
		//y=-15,x2;
	}

	QPen pen1((QColor)Qt::darkBlue);
	pen1.setWidth(3);
	painter.setPen(pen1);
	for (auto& ray : rays) {
		double A = ray.A, B = ray.B, C = ray.C;
		double x1, y1 = 11, x2, y2 = -15;
		double sta_x = ray.start_x, sta_y = ray.start_y;
		if (A == 0) {
			y1 = y2 = -C / B;
		}
		else {
			x1 = (-C - B * y1) / A;
			x2 = (-C - B * y2) / A;
		}
		if (ray.islawful(Point(x1, y1))) painter.drawLine(QPoint(sta_x * 20, -sta_y * 20), QPoint(x1 * 20, -y1 * 20));
		else	painter.drawLine(QPoint(sta_x * 20, -sta_y * 20), QPoint(x2 * 20, -y2 * 20));
	}

	QPen pen2((QColor)Qt::darkYellow);
	pen2.setWidth(3);
	painter.setPen(pen2);
	for (auto& seg : segments) {
		double A = seg.A, B = seg.B, C = seg.C;
		painter.drawLine(QPoint(seg.x1 * 20, -seg.y1 * 20), QPoint(seg.x2 * 20, -seg.y2 * 20));
	}

	QPen pen3((QColor)Qt::black);
	pen3.setWidth(3);
	painter.setPen(pen3);
	for (auto& cir : circles) {
		painter.drawEllipse(QPoint(cir.cx * 20, -cir.cy * 20), int(cir.cr * 20), int(cir.cr * 20));
	}


	//���ƽ���
	/*painter.drawLine(QPoint(800, 800), QPoint(900, 900));
	painter.drawEllipse(QPoint(800, 800), 50, 50);*/

	QPen pen4((QColor)Qt::red);
	pen4.setWidth(5);
	painter.setPen(pen4);
	for (auto& point : points) {
		QPoint p(point.x * 20, -point.y * 20);
		painter.drawPoint(p);
	}
}

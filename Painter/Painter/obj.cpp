#pragma once
#include"obj.h"


inline vector<Point> return_points() {
	return vector<Point>();
}

inline vector<Point> return_points(double x, double y) {
	vector<Point> new_points;
	new_points.push_back({ x,y });
	return new_points;
}

inline vector<Point> return_points(double x, double y, double x2, double y2) {
	vector<Point> new_points;
	new_points.push_back({ x,y });
	new_points.push_back({ x2,y2 });
	return new_points;
}

inline void exception_if_InfPoints(Line& line_1, Line& line_2){
	if (line_1.typecode == 'L' || line_2.typecode == 'L') throw exception("有无穷交点!");
	if (line_1.typecode == 'R') {
		if (line_2.typecode == 'R') {
			Point p1(line_1.x1, line_1.y1);//射线1的端点
			Point p2(line_2.x1, line_2.y1);//射线2的端点
			if (!(p1 == p2) && (line_1.islawful(p2) || line_2.islawful(p1))) throw exception("有无穷交点!"); //两射线端点不重合
			if (p1 == p2) { //两射线端点重合
				Point p3(line_1.x2, line_1.y2);
				if (line_2.islawful(p3)) throw exception("有无穷交点!");
			}
		}
		else if (line_2.typecode == 'S') {
			Point ray_point(line_1.x1, line_1.y1); //射线端点
			Point p1(line_2.x1, line_2.y1); //线段端点1
			Point p2(line_2.x2, line_2.y2); //线段端点2
			if (!(p1 == ray_point) && line_1.islawful(p1)) throw exception("有无穷交点!");
			if (!(p2 == ray_point) && line_1.islawful(p2))	throw exception("有无穷交点!");
		}
	}
	if (line_1.typecode == 'S') {
		if (line_2.typecode == 'R') {
			Point ray_point(line_2.x1, line_2.y1); //射线端点
			Point p1(line_1.x1, line_1.y1); //线段端点1
			Point p2(line_1.x2, line_1.y2); //线段端点2
			if (!(p1 == ray_point) && line_2.islawful(p1)) throw exception("有无穷交点!");
			if (!(p2 == ray_point) && line_2.islawful(p2))	throw exception("有无穷交点!");
		}
		else if (line_2.typecode == 'S') {
			Point p1(line_1.x1, line_1.y1); //线段1 端点
			Point p2(line_1.x2, line_1.y2); //线段1 端点

			Point p3(line_2.x1, line_2.y1); //线段2 端点
			Point p4(line_2.x2, line_2.y2); //线段2 端点

			if (line_2.islawful(p1) && !(p1 == p3) && !(p1 == p4))	 throw exception("有无穷交点!");
			if (line_2.islawful(p2) && !(p2 == p3) && !(p2 == p4))	 throw exception("有无穷交点!");
			if (p1 == p3 && p2 == p4) throw exception("有无穷交点!");
			if (p1 == p4 && p2 == p3) throw exception("有无穷交点!");
		}
	}
}

//构造函数
Point::Point(double x, double y) :x(x), y(y) {}

Line::Line(double x1, double y1, double x2, double y2) : A(y2 - y1), B(x1 - x2), C(x2 * y1 - x1 * y2), x1(x1), y1(y1), x2(x2), y2(y2),typecode('L') {}

Ray::Ray(double x1, double y1, double x2, double y2) : Line(x1, y1, x2, y2) {
	start_x = x1;
	start_y = y1;
	end_x = x2;
	end_y = y2;
	typecode = 'R';
}

Segment::Segment(double x1, double y1, double x2, double y2) :Line(x1, y1, x2, y2) {
	if (x1 < x2 || (x1 == x2 && y1 < y2)) {
		start_x = x1;
		start_y = y1;
		end_x = x2;
		end_y = y2;
	}
	else {
		start_x = x2;
		start_y = y2;
		end_x = x1;
		end_y = y1;
	}
	typecode = 'S';
}

Circle::Circle(double x, double y, double r) :cx(x), cy(y), cr(r),typecode('C') {}

string Line::display() {

	string str = "L " + to_string(x1) + " " + to_string(y1) + " " + to_string(x2) + " " + to_string(y2);
	return str;
}

string Ray::display() {
	string str = "R " + to_string(x1) + " " + to_string(y1) + " " + to_string(x2) + " " + to_string(y2);
	return str;
}

string Segment::display() {
	string str = "S " + to_string(x1) + " " + to_string(y1) + " " + to_string(x2) + " " + to_string(y2);
	return str;
}

string Circle::display() {
	string str = "C " + to_string(int(cx)) + " " + to_string(int(cy)) + " " + to_string(int(cr));
	return str;
}


bool Segment::islawful(Point& point) {
	if (start_x == end_x) return (point.y >= start_y && point.y <= end_y);
	return (point.x >= start_x && point.x <= end_x);
}

bool Ray::islawful(Point& point) {
	if (start_x == end_x) {
		if (start_y < end_y)	return start_y <= point.y;
		return start_y >= point.y;;
	}
	else {
		if (start_x < end_x)	return start_x <= point.x;
		return start_x >= point.x;
	}
}




void Line::upgrade_points(set<Point>& points, Line& line) {
	vector<Point> new_points = this->getIntersectPoint(line);
	for (Point point : new_points) {
		if (line.islawful(point)) {
			points.insert(point);
#ifdef DEBUG
			cout << "L-L: " << point.x << " " << point.y << endl;
#endif
		}
	}
}

void Line::upgrade_points(set<Point>& points, Circle& circle) {
	vector<Point> new_points = this->getIntersectPoint(circle);
	for (Point point : new_points) {
		points.insert(point);
#ifdef DEBUG
		cout << "L-C: " << point.x << " " << point.y << endl;
#endif
	}
}

void Ray::upgrade_points(set<Point>& points, Line& line) {
	vector<Point> new_points = this->getIntersectPoint(line);
	for (Point point : new_points) {
		if (this->islawful(point) && line.islawful(point)) {
			points.insert(point);
#ifdef DEBUG
			cout << "R-L: " << point.x << " " << point.y << endl;
#endif
		}
	}
}

void Ray::upgrade_points(set<Point>& points, Circle& circle) {
	vector<Point> new_points = this->getIntersectPoint(circle);
	for (Point point : new_points) {
		if (this->islawful(point)) {
			points.insert(point);
#ifdef DEBUG
			cout << "R-C: " << point.x << " " << point.y << endl;
#endif
		}
	}
}

void Segment::upgrade_points(set<Point>& points, Line& line) {
	vector<Point> new_points = this->getIntersectPoint(line);
	for (Point point : new_points) {
		if (this->islawful(point) && line.islawful(point)) {
			points.insert(point);
#ifdef DEBUG
			cout << "S-L: " << point.x << " " << point.y << endl;
#endif
		}
	}
}

void Segment::upgrade_points(set<Point>& points, Circle& circle) {
	vector<Point> new_points = this->getIntersectPoint(circle);
	for (Point point : new_points) {
		if (this->islawful(point)) {
			points.insert(point);
#ifdef DEBUG
			cout << "S-C: " << point.x << " " << point.y << endl;
#endif
		}
	}
}

void Circle::upgrade_points(set<Point>& points, Circle& circle) {
	const double cx1 = circle.cx, cy1 = circle.cy, cr1 = circle.cr;
	const double dis = sqrt((cx - cx1)*(cx - cx1) + (cy - cy1)*(cy - cy1));
	if (dis < cr - cr1 || dis<cr1 - cr || dis>cr + cr1) return;
	const double a = 2 * cr1*(cx1 - cx);
	const double b = 2 * cr1*(cy1 - cy);
	const double c = cr * cr - cr1 * cr1 - (cx1 - cx)*(cx1 - cx) - (cy1 - cy)*(cy1 - cy);

	const double p = a * a + b * b;
	const double q = -2 * a*c;
	const double r = c * c - b * b;

	if (EQUAL(q*q - 4 * p*r, 0) || q * q - 4 * p*r > 0) {
		double x = 0, y = 0;
		const double cos1 = (-q + sqrt(q*q - 4 * p*r)) / (2 * p);
		double sin1 = sqrt(1 - cos1 * cos1);
		x = cr1 * cos1 + cx1;
		y = cr1 * sin1 + cy1;
		if (EQUAL((x - cx)*(x - cx) + (y - cy)*(y - cy), cr*cr)) {
			points.insert({ x,y });
			//cout << x << " 9 " << y << endl;//
		}
		y = -sin1 * cr1 + cy1;
		if (EQUAL((x - cx)*(x - cx) + (y - cy)*(y - cy), cr*cr)) {
			points.insert({ x,y });
			//cout << x << " 7 " << y << endl;//
		}

		const double cos2 = (-q - sqrt(q*q - 4 * p*r)) / (2 * p);
		double sin2 = sqrt(1 - cos2 * cos2);
		x = cr1 * cos2 + cx1;
		y = cr1 * sin2 + cy1;
		if (EQUAL((x - cx)*(x - cx) + (y - cy)*(y - cy), cr*cr)) {
			points.insert({ x,y });
			//cout << x << " 10 " << y << endl;//
		}
		y = -sin2 * cr1 + cy1;
		if (EQUAL((x - cx)*(x - cx) + (y - cy)*(y - cy), cr*cr)) {
			points.insert({ x,y });
			//cout << x << " 11 " << y << endl;//
		}
	}
}

vector<Point> Line::getIntersectPoint(Line& line) {
	double x = 0, y = 0;
	const double A2 = line.A, B2 = line.B, C2 = line.C;
	const double m = A * B2 - A2 * B;
	if (m == 0) {
		if (B != 0) {
			if (B*C2 == C * B2) { exception_if_InfPoints(*this, line); }////直线重合报异常
			return return_points();
		}
		else { //A!=0
			if (A*C2 == C * A2) { exception_if_InfPoints(*this, line); }//直线重合就报异常
			return return_points();
		}
	}
	x = (C2*B - C * B2)*1.0 / m;
	y = (C*A2 - C2 * A)*1.0 / m;
	return return_points(x, y);
	//cout << x << " 13 " << y << endl;//
}

vector<Point> Line::getIntersectPoint(Circle& circle) {
	double x = 0, y = 0;
	const double cx = circle.cx, cy = circle.cy, cr = circle.cr;
	const double dis = (A*cx + B * cy + C) / sqrt(A*A + B * B);
	if (B != 0) { //直线：y=kx+d   圆：(x-cx)^2+(y-cy)^2=r^2
		const double k = -1 * A / B, d = -1 * C / B;
		const double a = (1 + k * k);
		const double b = -1 * (2 * cx + 2 * cy*k - 2 * k*d);
		const double c = cx * cx + d * d + cy * cy - 2 * cy*d - cr * cr;
		if (EQUAL(b*b, 4 * a*c)) {
			x = (-b / 2 * a);
			y = k * x + d;
			return return_points(x, y);
			//cout << x << " 1 " << y << endl;//
		}
		else if (b*b - 4 * a*c > 0) {
			const double delt = sqrt(b*b - 4 * a*c);
			x = (-b + delt) / (2 * a);
			y = k * x + d;
			//cout << x << " 2 " << y << endl;//

			double x1 = (-b - delt) / (2 * a);
			double y1 = k * x1 + d;
			//cout << x << "  3 " << y << endl; //
			return return_points(x, y, x1, y1);
		}
		return return_points();
	}
	else {
		const double x0 = -C / A; //直线:x=-C/A  圆：(x-cx)^2+(y-cy)^2=r^2
		if (EQUAL(fabs(x0 - cx), cr)) {
			x = x0;
			y = cy;
			return return_points(x, y);
			//cout << x << " 4 " << y << endl;//
		}
		else if (fabs(x0 - cx) < cr) {
			x = x0;
			y = cy + sqrt(cr*cr - (x0 - cx)*(x0 - cx));
			//cout << x << " 5 " << y << endl;//

			double x1 = x0;
			double y1 = cy - sqrt(cr*cr - (x0 - cx)*(x0 - cx));
			//cout << x << " 6 " << y << endl;//
			return return_points(x, y, x1, y1);
		}
		return return_points();
	}
}

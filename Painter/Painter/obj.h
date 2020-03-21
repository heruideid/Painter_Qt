#pragma once
#include<set>
#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<cmath>


using namespace std;

constexpr auto eps = (1e-4);
#define EQUAL(x,y) fabs((x)-(y))<=eps



class Circle;
class Line;
class Ray;
class Segment;

struct Point {
	double x, y;
	Point(double x, double y);
	bool operator<(const Point& other)const
	{
		if (EQUAL(x,other.x)) {
			if (EQUAL(y,other.y)) return false;
			return y < other.y;
		}
		return x < other.x;
	}

	bool operator==(const Point& other)const
	{
		if (EQUAL(x, other.x) && EQUAL(y, other.y)) return true;
		return false;
	}
};




class Line {
public:
	double A, B, C;//Ax+By+C=0;
	int x1, y1, x2, y2;
	char typecode;
	Line(double x1, double y1, double x2, double y2);
	string display();
	vector<Point> getIntersectPoint(Line& line);
	vector<Point> getIntersectPoint(Circle& circle);
	virtual bool islawful(Point& point) { return true; }
	virtual void upgrade_points(set<Point>& points, Line& line);
	virtual void upgrade_points(set<Point>& points, Circle& circle);
};


class Ray:public Line{
public:
	double start_x, start_y,end_x,end_y;//要求 start_x<end_x || (start_X==end_x && start_y<end_y)
	Ray(double x1, double y1, double x2, double y2);
	bool islawful(Point& point);
	string display();
	void upgrade_points(set<Point>& points, Line& line);
	void upgrade_points(set<Point>& points, Circle& circle);
};

class Segment:public Line {
public:
	double start_x,start_y,end_x,end_y;
	Segment(double x1,double y1,double x2,double y2);
	bool islawful(Point& point);
	string display();
	void upgrade_points(set<Point>& points, Line& line);
	void upgrade_points(set<Point>& points, Circle& circle);
};


class Circle {
public:
	double cx, cy, cr;
	char typecode;
	Circle(double x, double y, double r);
	string display();
	void upgrade_points(set<Point>& points, Circle& circle);
};


#ifdef CORE
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

DLL_API void LoadFromFile(ifstream& input, vector<Line>& lines, vector<Ray>& rays,
	vector<Segment>& segments, vector<Circle>& circles);

DLL_API void Calculate_Points(set<Point>& points, vector<Line>& lines, vector<Ray>& rays,
	vector<Segment>& segments, vector<Circle>& circles);

//异常定义
inline void exception_if_InfPoints(Line& line_1, Line& line_2) {
	if (line_1.typecode == 'L' || line_2.typecode == 'L') throw exception("Has infinite points");
	if (line_1.typecode == 'R') {
		if (line_2.typecode == 'R') {
			Point p1(line_1.x1, line_1.y1);//射线1的端点
			Point p2(line_2.x1, line_2.y1);//射线2的端点
			if (!(p1 == p2) && (line_1.islawful(p2) || line_2.islawful(p1))) throw exception("Has infinite points"); //两射线端点不重合
			if (p1 == p2) { //两射线端点重合
				Point p3(line_1.x2, line_1.y2);
				if (line_2.islawful(p3)) throw exception("Has infinite points");
			}
		}
		else if (line_2.typecode == 'S') {
			Point ray_point(line_1.x1, line_1.y1); //射线端点
			Point p1(line_2.x1, line_2.y1); //线段端点1
			Point p2(line_2.x2, line_2.y2); //线段端点2
			if (!(p1 == ray_point) && line_1.islawful(p1)) throw exception("Has infinite points");
			if (!(p2 == ray_point) && line_1.islawful(p2))	throw exception("Has infinite points");
		}
	}
	if (line_1.typecode == 'S') {
		if (line_2.typecode == 'R') {
			Point ray_point(line_2.x1, line_2.y1); //射线端点
			Point p1(line_1.x1, line_1.y1); //线段端点1
			Point p2(line_1.x2, line_1.y2); //线段端点2
			if (!(p1 == ray_point) && line_2.islawful(p1)) throw exception("Has infinite points");
			if (!(p2 == ray_point) && line_2.islawful(p2))	throw exception("Has infinite points");
		}
		else if (line_2.typecode == 'S') {
			Point p1(line_1.x1, line_1.y1); //线段1 端点
			Point p2(line_1.x2, line_1.y2); //线段1 端点

			Point p3(line_2.x1, line_2.y1); //线段2 端点
			Point p4(line_2.x2, line_2.y2); //线段2 端点

			if (line_2.islawful(p1) && !(p1 == p3) && !(p1 == p4))	 throw exception("Has infinite points");
			if (line_2.islawful(p2) && !(p2 == p3) && !(p2 == p4))	 throw exception("Has infinite points");
			if (p1 == p3 && p2 == p4) throw exception("Has infinite points");
			if (p1 == p4 && p2 == p3) throw exception("Has infinite points");
		}
	}
}

inline void exception_if_InfPoints(Circle& cir_1, Circle& cir_2) {
	if (cir_1.cr == cir_2.cr&&cir_1.cx == cir_2.cx&&cir_1.cy == cir_2.cy) throw exception("Has infinite points");
}

inline void exception_if_samePoints(int x1, int y1, int x2, int y2) {
	if (x1 == x2 && y1 == y2) throw exception("Use same points to define line/ray/segment");
}

inline void exception_if_outBorder(int value) {
	if (value <= -100000 || value >= 100000) throw exception("value of point out of range(-100000,100000)");
}

inline void exception_if_illegalRadius(int r) {
	if (r <= 0) throw exception("radius of circle <=0");
}
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
	double start_x, start_y,end_x,end_y;//ÒªÇó start_x<end_x || (start_X==end_x && start_y<end_y)
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
	Circle(double x, double y, double r);
	string display();
	void upgrade_points(set<Point>& points, Circle& circle);
};




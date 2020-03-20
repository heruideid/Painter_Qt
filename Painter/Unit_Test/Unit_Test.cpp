#include "pch.h"
#include "CppUnitTest.h"
#include"../Painter/obj.h"
#include "../Painter/obj.cpp"
#include "../Painter/core.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		//新增函数测试
		TEST_METHOD(TestMethod1)//getIntersectPoint
		{
			Line *line_1, *line_2;
			Circle *cir_1, *cir_2;
			vector<Point> points;

			//case 1:line & line 相交
			line_1=new Line(0, 1, 2, 3);
			line_2=new Line(0, 0, -1, 1);
			points=line_1->getIntersectPoint(*line_2);
			Assert::AreEqual((int)points.size(), 1);
			Assert::AreEqual(points[0]==Point(-0.5,0.5),true);

			//case 2:Line & line 平行
			line_1=new Line(0, 2, 0, -2);
			line_2=new Line(3, 4, 3,-7);
			points = line_1->getIntersectPoint(*line_2);
			Assert::AreEqual((int)points.size(), 0);

			//case 3:Line & circle 相切
			line_1 = new Line(0, 2, 0, -2);
			cir_2 = new Circle(1, 1, 1);
			points = line_1->getIntersectPoint(*cir_2);
			Assert::AreEqual((int)points.size(), 1);
			Assert::AreEqual(points[0] == Point(0, 1), true);
			
			//case 4:Line & circle 相交
			line_1 = new Line(-4, 1, 0, 1);
			cir_2 = new Circle(0, 1, 1);
			points = line_1->getIntersectPoint(*cir_2);
			Assert::AreEqual((int)points.size(), 2);
			Assert::AreEqual((points[0] == Point(-1, 1) || points[1] == Point(-1, 1)), true);
			Assert::AreEqual((points[0] == Point(1, 1) || points[1] == Point(1, 1)), true);

			//case 5:Line & circle 相离
			line_1 = new Line(0, 2, 0, -2);
			cir_2 = new Circle(2, 1, 1);
			points = line_1->getIntersectPoint(*cir_2);
			Assert::AreEqual((int)points.size(), 0);
		}
		TEST_METHOD(TestMethod2)//isLawful
		{
			Ray ray(0,1,0,-1);
			Segment seg(1, 2, 6, 7);
			Assert::AreEqual(ray.islawful(Point(0,-2)), true);
			Assert::AreEqual(ray.islawful(Point(0,1)), true);
			Assert::AreEqual(ray.islawful(Point(0,2)), false);

			Assert::AreEqual(seg.islawful(Point(3,4)), true);
			Assert::AreEqual(seg.islawful(Point(1,2)), true);
			Assert::AreEqual(seg.islawful(Point(6,7)), true);
			Assert::AreEqual(seg.islawful(Point(-1,0)), false);
			
		}
		//接口测试
		TEST_METHOD(TestMethod3) //LoadFromFile
		{
			ifstream fin;
			fin.open("test_file_in.txt");
			vector<Line> lines;
			vector<Ray> rays;
			vector<Segment> segs;
			vector<Circle> cirs;

			/*4
				C 3 3 3
				S 2 4 3 2
				L - 1 4 5 2
				R 2 5 - 1 2*/
			LoadFromFile(fin, lines, rays, segs, cirs);

			Assert::AreEqual((int)lines.size(), 1);
			Assert::AreEqual((int)rays.size(), 1);
			Assert::AreEqual((int)segs.size(), 1);
			Assert::AreEqual((int)cirs.size(), 1);
			Assert::AreEqual(lines[0].display(), string("L -1 4 5 2"));
			Assert::AreEqual(rays[0].display(), string("R 2 5 -1 2"));
			Assert::AreEqual(segs[0].display(), string("S 2 4 3 2"));
			Assert::AreEqual(cirs[0].display(), string("C 3 3 3"));
		}
		TEST_METHOD(TestMethod4)//Calculate_Points
		{
			//case 1:
			/*	3
				S -1 3 2 - 1
				L -2 2 3 0
				R -3 0 4 2
			*/
			set<Point> points;
			vector<Line> lines;
			vector<Ray> rays;
			vector<Segment> segs;
			vector<Circle> cirs;
			bool flag;

			lines.push_back(Line(-2, 2, 3, 0));
			rays.push_back(Ray(-3, 0, 4, 2));
			segs.push_back(Segment(-1, 3, 2, -1));

			Calculate_Points(points, lines, rays, segs, cirs);
			Assert::AreEqual((int)points.size(),1);
			Assert::AreEqual(true, (*points.begin())==Point(0.5,1));
			lines.clear(); cirs.clear();
			rays.clear(); segs.clear();
			points.clear();

			// case 2:
				/*	4
					C 3 3 3
					S 2 4 3 2
					L -1 4 5 2
					R 2 5 -1 2*/
			lines.push_back(Line(-1, 4, 5, 2));
			rays.push_back(Ray(2, 5,-1, 2));
			segs.push_back(Segment(2, 4, 3, 2));
			cirs.push_back(Circle(3,3,3));
			Calculate_Points(points, lines, rays, segs, cirs);
			Assert::AreEqual((int)points.size(), 5);
			flag = true;
			if (points.find(Point(0,3)) == points.end()) flag = false;
			if (points.find(Point(0.0698057,3.6434)) == points.end()) flag = false;
			if (points.find(Point(0.5,3.5)) == points.end()) flag = false;
			if (points.find(Point(2.6,2.8)) == points.end()) flag = false;
			if (points.find(Point(5.73019,1.7566)) == points.end()) flag = false;
			Assert::AreEqual(flag,true);

			lines.clear(); cirs.clear();
			rays.clear(); segs.clear();
			points.clear();

			//case 3:
			/*8
				C 3 3 3
				S 2 4 3 2
				L - 1 4 5 2
				R 2 5 - 1 2
				C  1 3 3
				S 6 5 5 4
				R 4 1 4 3
				R 0 1 0 - 2*/
			lines.push_back(Line(-1, 4, 5, 2));
			rays.push_back(Ray(2, 5, -1, 2));
			rays.push_back(Ray(4, 1, 4, 3));
			rays.push_back(Ray(0, 1, 0, -2));
			segs.push_back(Segment(2, 4, 3, 2));
			segs.push_back(Segment(6, 5, 5, 4));
			cirs.push_back(Circle(3, 3, 3));
			cirs.push_back(Circle(1,3,3));
			Calculate_Points(points, lines, rays, segs, cirs);
			Assert::AreEqual((int)points.size(), 15);
			flag = true;
			if (points.find(Point(-1.73019,4.2434)) == points.end()) flag = false;
			if (points.find(Point(-1.5615, 1.43845)) == points.end()) flag = false;
			if (points.find(Point(0, 3)) == points.end()) flag = false;
			if (points.find(Point(0, 0.1715)) == points.end()) flag = false;
			if (points.find(Point(0.0698057,3.6434)) == points.end()) flag = false;
			if (points.find(Point(0.5,3.5)) == points.end()) flag = false;
			if (points.find(Point(2,0.171573)) == points.end()) flag = false;
			if (points.find(Point(2,5.82843)) == points.end()) flag = false;
			if (points.find(Point(2.6,2.8)) == points.end()) flag = false;
			if (points.find(Point(3.93019,2.3566)) == points.end()) flag = false;
			if (points.find(Point(4,2.33333)) == points.end()) flag = false;
			if (points.find(Point(4, 3)) == points.end()) flag = false;
			if (points.find(Point(4,5.82843)) == points.end()) flag = false;
			if (points.find(Point(5.56155,4.56155)) == points.end()) flag = false;
			if (points.find(Point(5.73019,1.7566)) == points.end()) flag = false;
			Assert::AreEqual(flag, true);

			lines.clear(); cirs.clear();
			rays.clear(); segs.clear();
		}
		//异常测试
		TEST_METHOD(excep0) {
			//cir & cir
			Circle* c1 = new Circle(0, 0, 1);
			Circle* c2 = new Circle(0, 0, 1);
			bool flag = false; //捕捉到异常
			try {
				exception_if_InfPoints(*c1, *c2);
			}
			catch (exception& e) {
				if ((string)e.what() == "有无穷交点!") flag = true;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep1) //exception
		{
			//line & line 重合
			Line* l1 = new Line(0, 0, 1, 1);
			Line* l2 = new Line(-1,-1,1,1);
			bool flag = false; //捕捉到异常
			try {
				exception_if_InfPoints(*l1,*l2);
			}
			catch (exception& e) {
				if ((string)e.what() == "有无穷交点!") flag = true;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep2) {
			//ray & ray 区间重合
			bool flag = false;
			Ray* r1 = new Ray(0, 0, 1, 1);
			Ray* r2 = new Ray(-1, -1, 5,5);
			try {
				exception_if_InfPoints(*r1, *r2);
			}
			catch (exception& e) {
				if ((string)e.what() == "有无穷交点!") flag = true;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep3) {
			//ray & ray 仅有端点重合，但方向相反
			bool flag = true;
			Ray* r1 = new Ray(0, 0, 1, 1);
			Ray* r2 = new Ray(0, 0, -1, -1);
			try {
				exception_if_InfPoints(*r1, *r2);
			}
			catch (exception& e) {
				 flag = false;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep4) {
			//ray & seg 区间重合
			bool flag = false;
			Ray* r1 = new Ray(0, 0, 1, 1);
			Segment* s2=new Segment(-1,-1,1,1);
			try {
				exception_if_InfPoints(*r1, *s2);
			}
			catch (exception& e) {
				if ((string)e.what() == "有无穷交点!") flag = true;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep5) {
			//ray & seg 仅有一个端点重合
			bool flag = true;
			Ray* r1 = new Ray(0, 0, 1, 1);
			Segment* s2 = new Segment(-1, -1, 0, 0);
			try {
				exception_if_InfPoints(*r1, *s2);
			}
			catch (exception& e) {
				 flag = false;
			}
			Assert::AreEqual(true,flag);
		}
		TEST_METHOD(excep6) {
			//seg & seg 区间重合
			bool flag = false;
			Ray* r1 = new Ray(0, 0, 2, 2);
			Segment* s2 = new Segment(-1, -1, 1, 1);
			try {
				exception_if_InfPoints(*r1, *s2);
			}
			catch (exception& e) {
				if ((string)e.what() == "有无穷交点!") flag = true;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep7) {
			//seg & seg 仅有一个端点重合
			bool flag = true;
			Ray* r1 = new Ray(0, 0, 2, 2);
			Segment* s2 = new Segment(-1, -1, 0, 0);
			try {
				exception_if_InfPoints(*r1, *s2);
			}
			catch (exception& e) {
				flag = false;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep8) {
			//定义线的两个端点相同
			ifstream fin;
			fin.open("excep8.txt");
			vector<Line> lines;
			vector<Ray> rays;
			vector<Segment> segs;
			vector<Circle> cirs;
			bool flag = false;
			/*2
				L -1 4 -1 4
				R 2 5 - 1 2*/
			try {
				LoadFromFile(fin, lines, rays, segs, cirs);
			}
			catch(exception& e){
				if ((string)e.what() == "定义线的两点不能相同!") flag = true;
			}
			Assert::AreEqual(true,flag);
		}
		TEST_METHOD(excep9) {
			//数值超限
			ifstream fin;
			fin.open("excep9.txt");
			vector<Line> lines;
			vector<Ray> rays;
			vector<Segment> segs;
			vector<Circle> cirs;
			bool flag = false;
			/* 1
				L - 100001 4 -1 4
			*/
			try {
				LoadFromFile(fin, lines, rays, segs, cirs);
			}
			catch (exception& e) {
				if ((string)e.what() == "点的数值超限,应为(-100000,100000)内整数!") flag = true;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep10) {
			//illegal n
			ifstream fin;
			fin.open("excep10.txt");
			vector<Line> lines;
			vector<Ray> rays;
			vector<Segment> segs;
			vector<Circle> cirs;
			bool flag = false;
			/*  0
				L - 100 4 -1 4
			*/
			try {
				LoadFromFile(fin, lines, rays, segs, cirs);
			}
			catch (exception& e) {
				if ((string)e.what() == "illegal n!") flag = true;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep11) {
			//圆半径<=0
			ifstream fin;
			fin.open("excep11.txt");
			vector<Line> lines;
			vector<Ray> rays;
			vector<Segment> segs;
			vector<Circle> cirs;
			bool flag = false;
			/*  1
				C - 100 4 0
			*/
			try {
				LoadFromFile(fin, lines, rays, segs, cirs);
			}
			catch (exception& e) {
				if ((string)e.what() == "圆的半径<=0,非法!") flag = true;
			}
			Assert::AreEqual(true, flag);
		}
		TEST_METHOD(excep12) {
			//类别码非法
			ifstream fin;
			fin.open("excep12.txt");
			vector<Line> lines;
			vector<Ray> rays;
			vector<Segment> segs;
			vector<Circle> cirs;
			bool flag = false;
			/*  1
				c - 100 4 1
			*/
			try {
				LoadFromFile(fin, lines, rays, segs, cirs);
			}
			catch (exception& e) {
				if ((string)e.what() == "typecode should be upppercase!") flag = true;
			}
			Assert::AreEqual(true, flag);
		}
	};
}

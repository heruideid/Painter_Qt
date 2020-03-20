#include "obj.h"
using namespace std;
#define DEBUG

inline void exception_if_samePoints(int x1,int y1,int x2,int y2) {
	if (x1 == x2 && y1 == y2) throw exception("定义线的两点不能相同!");
}

inline void exception_if_outBorder(int value) {
	if (value <= -100000 || value >= 100000) throw exception("点的数值超限,应为(-100000,100000)内整数!");
}

inline void exception_if_illegalRadius(int r) {
	if (r <= 0) throw exception("圆的半径<=0,非法!");
}

void LoadFromFile(ifstream& input, vector<Line>& lines, vector<Ray>& rays,
	vector<Segment>& segments, vector<Circle>& circles) {
	int n;
	char ch;
	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	double x0 = 0, y0 = 0, r = 0;

	input >> n;
	int num[5];
	if (n <= 0) throw exception("illegal n!");
	for (int i = 0; i < n; i++) {
		input >> ch;
		if (ch == 'L') {
			for (int i = 1; i <= 4; i++) {
				input >> num[i];
				exception_if_outBorder(num[i]);
			}
			exception_if_samePoints(num[1],num[2],num[3],num[4]);
			lines.push_back(Line(num[1],num[2],num[3],num[4]));
		}
		else if (ch == 'C') {
			for (int i = 1; i <= 3; i++) {
				input >> num[i];
				exception_if_outBorder(num[i]);
			}
			exception_if_illegalRadius(num[3]);
			circles.push_back(Circle(num[1], num[2], num[3]));
		}
		else if (ch == 'R') {
			for (int i = 1; i <= 4; i++) {
				input >> num[i];
				exception_if_outBorder(num[i]);
			}
			exception_if_samePoints(num[1], num[2], num[3], num[4]);
			rays.push_back(Ray(num[1], num[2], num[3], num[4]));
		}
		else if (ch == 'S') {
			for (int i = 1; i <= 4; i++) {
				input >> num[i];
				exception_if_outBorder(num[i]);
			}
			exception_if_samePoints(num[1], num[2], num[3], num[4]);
			segments.push_back(Segment(num[1], num[2], num[3], num[4]));
		}
		else if (ch == 's' || ch == 'l' || ch == 'r' || ch == 'c') throw exception("typecode should be upppercase!");
		else throw exception("illegal typecode!");
	}
}

void Calculate_Points(set<Point>& points, vector<Line>& lines, vector<Ray>& rays,
	vector<Segment>& segments, vector<Circle>& circles) {
	int lines_size = (int)lines.size();
	int circles_size = (int)circles.size();
	int segments_size = (int)segments.size();
	int rays_size = (int)rays.size();

	for (int i = 0; i < lines_size; i++) {
		for (int j = i + 1; j < lines_size; j++) 	lines.at(i).upgrade_points(points, lines.at(j));
		for (auto& ray : rays) lines.at(i).upgrade_points(points, ray);
		for (auto& segment : segments) lines.at(i).upgrade_points(points, segment);
		for (auto& circle : circles) lines.at(i).upgrade_points(points, circle);
	}

	for (int i = 0; i < rays_size; i++) {
		for (int j = i + 1; j < rays_size; j++) 	rays.at(i).upgrade_points(points, rays.at(j));
		for (auto& segment : segments) rays.at(i).upgrade_points(points, segment);
		for (auto& circle : circles) 	rays.at(i).upgrade_points(points, circle);
	}

	for (int i = 0; i < segments_size; i++) {
		for (int j = i + 1; j < segments_size; j++) segments.at(i).upgrade_points(points, segments.at(j));
		for (auto& circle : circles) segments.at(i).upgrade_points(points, circle);
	}

	for (int i = 0; i < circles_size; i++) {
		for (int j = i + 1; j < circles_size; j++) {
			circles.at(i).upgrade_points(points, circles.at(j));
		}
	}
}

int main(int argv, char** args)
{
	if (args == nullptr) throw exception();
	ifstream input;
	ofstream output;

	set<Point> points;
	vector<Line> lines;
	vector<Segment> segments;
	vector<Ray> rays;
	vector<Circle> circles;

#ifdef DEBUG
	input.open("input.txt");
	output.open("output.txt");
#else
	if (argv == 5 && strcmp(args[1], "-i") == 0 && strcmp(args[3], "-o") == 0) {
		input = ifstream(args[2]);
		output = ofstream(args[4]);
	}
	else {
		cout << "请按指定格式调用!\n";
		exit(0);
	}
#endif

	try {
		LoadFromFile(input, lines, rays, segments, circles);
		Calculate_Points(points, lines, rays, segments, circles);
	}
	catch(exception& error){
		cout << error.what() << endl;
	}
	
	/*cout << points.size()<<endl;
	for (auto& point : points)	cout << point.x << " " << point.y << endl;
*/
	/*
	for (auto& line : lines) cout<<line.display()<<endl;
	for (auto& ray : rays) cout << ray.display() << endl;
	for (auto& line : segments) cout << line.display() << endl;
	for (auto& line : circles) cout << line.display() << endl;*/

	output << points.size();
	input.close();
	output.close();
	return 0;
}
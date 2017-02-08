#ifndef _FT_MUCT_HPP_
#define _FT_MUCT_HPP_
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
using namespace std;
using namespace cv;
class muct_data{
public:
	string name;
	string index;
	vector<Point2f> points;
	muct_data(string str, string muct_dir);
};
#endif
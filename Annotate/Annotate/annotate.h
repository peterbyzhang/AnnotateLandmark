#ifndef _FT_ANNOTATE_HPP_
#define _FT_ANNOTATE_HPP_
#include <opencv2/highgui/highgui.hpp>
#include "ft_data.hpp"
#include <iostream>
#include <fstream>
#include "tinyxml.h"

using namespace std;
using namespace cv;
class Annotate{
public:
	int idx;                       //index of image to annotate
	int pidx;                      //index of point to manipulate
	Mat image;                     //current image to display 
	Mat image_zoom;                //image to display zoom
	Mat image_clean;               //clean image to display
	ft_data data;                  //annotation data
	const char* wname;             //display window name
	vector<string> instructions;   //annotation instructions
	vector<Point2f> points; // store point coordinates

	Annotate();
	int set_image(string path);
	int set_current_image(const int idx);
	void set_clean_image();
	void copy_clean_image();
	void copy_clean_image_zoom();
	void draw_instructions();
	void draw_points();
	void draw_chosen_point();
	void draw_connections();
	void draw_symmetry();
	void draw_landmarks();
	void draw_chosen_landmark();
	void draw_landmarks_zoom();
	void draw_chosen_landmark_zoom();
	void set_capture_instructions();
	void set_pick_points_instructions();
	void set_connectivity_instructions();
	void set_symmetry_instructions();
	void set_move_points_instructions();
	void initialise_symmetry(const int index);
	void replicate_annotations(const int index);
	void write_landmarks(TiXmlNode* partNode);
	int find_closest_point(const Point2f p, const double thresh = 10.0);
	int find_closest_landmark(const Point2f p, const double thresh = 10.0);
	int find_closest_eye_landmark(const Point2f p, const double thresh = 10.0);
protected:
	void draw_strings(Mat img, const vector<string> &text);
	void draw_string(Mat img, const string text, const int level);

};
#endif
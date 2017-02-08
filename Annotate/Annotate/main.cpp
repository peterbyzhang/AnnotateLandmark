#include <stdio.h> 
#include <vector>
#include "tinyxml.h"
#include <Windows.h>
#include "ft.hpp"
#include "ft_data.hpp"
#include "annotate.h"
#include "muct.h"
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>


using namespace std;
using namespace cv;

Annotate annotation;
Mat image9X;
Size size9X(459, 459);
int mouseX = 0;
int mouseY = 0;

string path; // = "C:/Users/User/Documents/Resources/AnotatedFaces/dlibDownlaod/ibug_300W_large_face_landmark_dataset/";
string fileName; // = "labels_ibug_300W_train_annotation_result.xml";
TiXmlDocument doc;



void displayZoomedImage(int x, int y)
{
	int width = min(51, annotation.image.cols);
	int height = min(51, annotation.image.rows);
	int left = x - 25;
	int top = y - 25;
	if (x - 25 < 0) left = 0;
	if (x + 25 >= annotation.image.cols) left = annotation.image.cols - width;
	if (y - 25 < 0) top = 0;
	if (y + 25 >= annotation.image.rows) top = annotation.image.rows - height;
	Rect roi(left, top, width, height); // x, y, width, height
	annotation.copy_clean_image_zoom();
	annotation.draw_landmarks_zoom();
	annotation.draw_chosen_landmark_zoom();
	image9X = annotation.image_zoom(roi);
	size9X.width = width * 9;
	size9X.height = height * 9;
	resize(image9X, image9X, size9X, 0, 0, INTER_NEAREST);

	// draw cursor
	int cursorX = 9 * (x - left) + 4;
	int cursorY = 9 * (y - top) + 4;;
	line(image9X, cv::Point2i(cursorX - 15, cursorY), cv::Point2i(cursorX + 15, cursorY), CV_RGB(0, 0, 0), 3);
	line(image9X, cv::Point2i(cursorX, cursorY - 15), cv::Point2i(cursorX, cursorY + 15), CV_RGB(0, 0, 0), 3);
	line(image9X, cv::Point2i(cursorX - 15, cursorY), cv::Point2i(cursorX + 15, cursorY), CV_RGB(255, 255, 255), 1);
	line(image9X, cv::Point2i(cursorX, cursorY - 15), cv::Point2i(cursorX, cursorY + 15), CV_RGB(255, 255, 255), 1);

	imshow("9X", image9X);
}


// handle mouse input
void mv_MouseCallback(int event, int x, int y, int /*flags*/, void* /*param*/)
{
	if (event == CV_EVENT_LBUTTONDOWN) {
		// if the eye pupil points are not annotated, add them.
		if (annotation.points[68].x == -1 || annotation.points[68].y == -1) {
			annotation.points[68] = Point2f(x, y);
			annotation.copy_clean_image();
			annotation.draw_landmarks();
			annotation.draw_chosen_landmark();
			imshow(annotation.wname, annotation.image);
		}
		else if (annotation.points[69].x == -1 || annotation.points[69].y == -1) {
			annotation.points[69] = Point2f(x, y);
			annotation.copy_clean_image();
			annotation.draw_landmarks();
			annotation.draw_chosen_landmark();
			imshow(annotation.wname, annotation.image);
		}
		// both eye points exists, move them
		else {
			if (annotation.pidx < 0) {
				annotation.pidx = annotation.find_closest_eye_landmark(Point2f(x, y), 30.0); // select point
			}
			else annotation.pidx = -1; // release point
			annotation.copy_clean_image();
			annotation.draw_landmarks();
			annotation.draw_chosen_landmark();
			imshow(annotation.wname, annotation.image);
		}
	}
	else if (event == CV_EVENT_MOUSEMOVE) { // move point
		if (annotation.pidx >= 0) {
			annotation.points[annotation.pidx] = Point2f(x, y);
			annotation.copy_clean_image();
			annotation.draw_landmarks();
			annotation.draw_chosen_landmark();
			imshow(annotation.wname, annotation.image);
		}
	}

	mouseX = x;
	mouseY = y;
	displayZoomedImage(x, y);
}


// save file when the program terminates by clicking the X at the upperright corner of the console
BOOL ctrl_handler(DWORD event)
{
	if (event == CTRL_CLOSE_EVENT) {
		doc.SaveFile((path + fileName).c_str());
	}
}



int main(int argc, char** argv)
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ctrl_handler), TRUE);

	int top;
	int left;
	int right;
	int bottom;
	int width;
	int height;
	int size;

	int keyInput = 0;

	int iVal;

	string imageName;

	cout << endl;
	// load file 
	if (argc > 1) {
		string argIn(argv[1]);
		path = argIn.substr(0, argIn.find_last_of("\\/") + 1);
		fileName = argIn.substr(argIn.find_last_of("\\/") + 1);
	}
	bool loadOkay = doc.LoadFile((path + fileName).c_str());
	if (!loadOkay)
	{
		printf("Could not load file '%s'. Error='%s'. Exiting.\n", (path + fileName).c_str(), doc.ErrorDesc());
		exit(1);
	}

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	// color code:  0 black, 7 white,  9 blue, 11 cyan
	SetConsoleTextAttribute(hConsole, 7);
	cout << "The ";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "yellow"; 
	SetConsoleTextAttribute(hConsole, 7);
	cout << " landmark is the eye on the user's right hand side. " << endl;
	cout << "The ";
	SetConsoleTextAttribute(hConsole, 11);
	cout << "cyan";
	SetConsoleTextAttribute(hConsole, 7);
	cout << " landmark is the eye on the user's left hand side. " << endl;
	cout << endl;

	cout << "MOUSE CONTROL: " << endl;
	cout << "Click mouse to add eye points, always add ";
	SetConsoleTextAttribute(hConsole, 14);
	cout << "yellow"; 
	SetConsoleTextAttribute(hConsole, 7);
	cout << " point before ";
	SetConsoleTextAttribute(hConsole, 11);
	cout << "cyan"; 
	SetConsoleTextAttribute(hConsole, 7);
	cout << "." << endl;
	cout << "After the eye points are added, click mouse to move them." << endl;
	cout << endl;

	cout << "KEYBOARD CONTROL: " << endl;
	cout << "D - next face" << endl;
	cout << "A - previous face" << endl;
	cout << "R - reload landmarks" << endl;
	cout << "Q - save and quit" << endl;
	cout << endl;

	// set color to black, conceal the error message about OpenCL
	SetConsoleTextAttribute(hConsole, 0);
	cout << "Please ignore the following error message, we do not need OpenCL. " << endl;

	cvNamedWindow(annotation.wname, CV_WINDOW_NORMAL);
	namedWindow("9X"); // TODO display next to the other window, so user does not have to adjust window position
	HWND h9X = FindWindow(0, L"9X");
	SetWindowPos(h9X, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); // set window 9X as the topmost window
	setMouseCallback(annotation.wname, mv_MouseCallback, 0);
	annotation.set_move_points_instructions();
	annotation.points.resize(70);

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	TiXmlNode* rootNode = 0;
	TiXmlNode* imagesNode = 0;
	TiXmlNode* imageNode = 0;
	TiXmlNode* boxNode = 0;
	TiXmlNode* partNode = 0;
	TiXmlElement* rootElement = 0;
	TiXmlElement* imagesElement = 0;
	TiXmlElement* imageElement = 0;
	TiXmlElement* boxElement = 0;
	TiXmlElement* partElement = 0;

	rootNode = doc.FirstChild("dataset");
	assert(rootNode);
	rootElement = rootNode->ToElement();
	assert(rootElement);

	imagesNode = rootElement->FirstChildElement("images");
	assert(imageNode);
	imagesElement = imagesNode->ToElement();
	assert(imagesElement);

	imageNode = imagesElement->FirstChildElement("image");
	assert(imageNode);
	imageElement = imageNode->ToElement();
	assert(imageElement);

	// TODO will program break down if there is no image, no part?
	boxNode = imageElement->FirstChildElement("box");
	assert(boxNode);
	boxElement = boxNode->ToElement();
	assert(boxElement);

	while (1) {
		// load image
		imageName = imageElement->Attribute("file");
		annotation.set_image(path + imageName);
		//annotation.draw_instructions();
		annotation.set_clean_image();

		// get the x and y coordinate of the 68 landmarks
		partNode = boxElement->FirstChildElement("part");
		assert(partNode);
		partElement = partNode->ToElement();
		assert(partElement);

		for (int i = 0; i < 70; i++) {
			annotation.points[i].x = -1;
			annotation.points[i].y = -1;
		}

		for (int i = 0; i < 70; i++) {
			partElement->QueryIntAttribute("name", &iVal);
			if (iVal != i) {
				partElement->SetAttribute("name", -1000);
				// TODO display error message
			}

			partElement->QueryIntAttribute("x", &iVal);
			annotation.points[i].x = iVal;
			partElement->QueryIntAttribute("y", &iVal);
			annotation.points[i].y = iVal;
			partElement = partElement->NextSiblingElement();
			if (partElement == nullptr) {
				break;
			}
		}

		annotation.copy_clean_image();
		annotation.draw_landmarks();
		imshow(annotation.wname, annotation.image);

		// resize window
		int windowWidth = annotation.image.cols;
		int windowHeight = annotation.image.rows;

		if (windowWidth > desktop.right) {
			windowHeight = windowHeight * desktop.right / windowWidth;
			windowWidth = desktop.right;
		}
		if (windowHeight > desktop.bottom) {
			windowWidth = windowWidth * desktop.bottom / windowHeight;
			windowHeight = desktop.bottom;
		}
		resizeWindow(annotation.wname, windowWidth, windowHeight);

		displayZoomedImage(mouseX, mouseY);
		keyInput = waitKey(0); // here perform mouse manipulation, and wait for key input

		if(keyInput == 'q') // quit and save
			break;
		else if(keyInput == 'a') // previous face
		{
			annotation.write_landmarks(partNode);

			if (boxNode->PreviousSibling()) {
				boxNode = boxNode->PreviousSibling();
				boxElement = boxNode->ToElement();
				annotation.pidx = -1;
			}
			else { // go to previous image
				if (imageNode->PreviousSibling()) {
					imageNode = imageNode->PreviousSibling();
					imageElement = imageNode->ToElement();

					boxNode = imageElement->LastChild("box");
					assert(boxNode);
					boxElement = boxNode->ToElement();
					assert(boxElement);
				}
			}
		}
		else if (keyInput == 'r') // reload face
		{
			annotation.pidx = -1;
		}
		else if(keyInput == 'd') // next face
		{
			annotation.write_landmarks(partNode);

			if (boxNode->NextSibling()) {
				boxNode = boxNode->NextSibling();
				boxElement = boxNode->ToElement();
				annotation.pidx = -1;
			}
			else { // go to next image
				if (imageNode->NextSibling()) {
					imageNode = imageNode->NextSibling();
					imageElement = imageNode->ToElement();

					boxNode = imageElement->FirstChildElement("box");
					assert(boxNode);
					boxElement = boxNode->ToElement();
					assert(boxElement);
				}
			}
		}
	}

	// restore test color to white
	SetConsoleTextAttribute(hConsole, 7);
	annotation.write_landmarks(partNode);
	doc.SaveFile((path + fileName).c_str());
	// TODO add line <?xml-stylesheet type='text/xsl' href='image_metadata_stylesheet.xsl'?>

	return 0;
}

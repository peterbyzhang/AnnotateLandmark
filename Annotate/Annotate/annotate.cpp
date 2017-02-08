#include "annotate.h"

Annotate::Annotate()
{
	wname = "Annotate"; 
	idx = 0; pidx = -1;
}

int Annotate::set_image(string path)
{
	if (path.empty())
		return 0;
	image = imread(path.c_str(), 1);
	return 1;
}

int Annotate::set_current_image(const int idx = 0)
{
	if((idx < 0) || (idx > int(data.imnames.size())))
		return 0;
	image = data.get_image(idx,2); 
	return 1;
}

void Annotate::set_clean_image(){
	image_clean = image.clone();
}

void Annotate::copy_clean_image(){
	image_clean.copyTo(image);
}

void Annotate::copy_clean_image_zoom() {
	image_clean.copyTo(image_zoom);
}

void Annotate::draw_instructions(){
	if(image.empty())
		return;
	this->draw_strings(image,instructions);
}

void Annotate::draw_points(){
	data.draw_points(image,idx);
}

void Annotate::draw_chosen_point(){
	if(pidx >= 0)
		circle(image,data.points[idx][pidx],1,CV_RGB(0,255,0),2,CV_AA);
}

void Annotate::draw_connections(){
	int m = data.connections.size();
	if(m == 0)
		this->draw_points();
	else{
		if(data.connections[m-1][1] < 0){
			int i = data.connections[m-1][0];
			data.connections[m-1][1] = i;
			data.draw_connect(image,idx); 
			this->draw_points();
			circle(image,data.points[idx][i],1,CV_RGB(0,255,0),2,CV_AA);
			data.connections[m-1][1] = -1;
		}
		else
		{
			data.draw_connect(image,idx);
			this->draw_points();
		}
	}
}

void Annotate::draw_symmetry()
{
	this->draw_points(); 
	this->draw_connections();
	for(int i = 0; i < int(data.symmetry.size()); i++)
	{
		int j = data.symmetry[i];
		if(j != i)
		{
			circle(image,data.points[idx][i],1,CV_RGB(255,255,0),2,CV_AA);
			circle(image,data.points[idx][j],1,CV_RGB(255,255,0),2,CV_AA);
		}
	}
	if(pidx >= 0)
		circle(image,data.points[idx][pidx],1,CV_RGB(0,255,0),2,CV_AA);
}

void Annotate::draw_landmarks()
{
	for (int i = 0; i < points.size(); i++) {
		circle(image, points[i], 2, CV_RGB(255, 0, 0), -1, CV_AA);
		if (i == 68)	circle(image, points[i], 2, CV_RGB(255, 255, 0), -1, CV_AA);
		if (i == 69)	circle(image, points[i], 2, CV_RGB(0, 255, 255), -1, CV_AA);
	}
}

void Annotate::draw_chosen_landmark() 
{
	if (pidx >= 0)
		circle(image, points[pidx], 2, CV_RGB(0, 255, 0), -1, CV_AA);
}

void Annotate::draw_landmarks_zoom()
{
	for (int i = 0; i < points.size(); i++) {
		line(image_zoom, points[i], points[i], CV_RGB(255, 0, 0), 1);
		if (i == 68)	line(image_zoom, points[i], points[i], CV_RGB(255, 255, 0), 1);
		if (i == 69)	line(image_zoom, points[i], points[i], CV_RGB(0, 255, 255), 1);
	}
}
void Annotate::draw_chosen_landmark_zoom()
{
	if (pidx >= 0)
		line(image_zoom, points[pidx], points[pidx], CV_RGB(0, 255, 0), 1);
}

void Annotate::set_capture_instructions(){
	instructions.clear();
	instructions.push_back(string("Select expressive frames."));
	instructions.push_back(string("s - use this frame"));
	instructions.push_back(string("n - to the next frame"));
	instructions.push_back(string("q - done"));
}

void Annotate::set_pick_points_instructions(){
	instructions.clear();
	instructions.push_back(string("Pick Points"));
	instructions.push_back(string("q - done"));
}

void Annotate::set_connectivity_instructions(){
	instructions.clear();
	instructions.push_back(string("Pick Connections"));
	instructions.push_back(string("q - done"));
}

void Annotate::set_symmetry_instructions(){
	instructions.clear();
	instructions.push_back(string("Pick Symmetric Points"));
	instructions.push_back(string("q - done"));
}

void Annotate::set_move_points_instructions(){
	instructions.clear();
	instructions.push_back(string("Move Points"));
	instructions.push_back(string("D - next face"));
	instructions.push_back(string("A - previous face"));
	instructions.push_back(string("R - reload landmarks"));
	instructions.push_back(string("Q - save and quit"));
}

void Annotate::initialise_symmetry(const int index){
	int n = data.points[index].size();
	data.symmetry.resize(n);
	for(int i = 0; i < n; i++)
		data.symmetry[i] = i;
}

void Annotate::replicate_annotations(const int index){
	if((index < 0) || (index >= int(data.points.size())))
		return;
	for(int i = 0; i < int(data.points.size()); i++)
	{
		if(i == index)
			continue;
		data.points[i] = data.points[index];
	}
}
/*
void Annotate::write_landmarks(TiXmlElement* boxElement) // TODO doesn't work
{
	// write results to points
	int iVal;
	TiXmlElement* partElement = partNode->ToElement();
	assert(partElement);

	for (int i = 0; i < 70; i++) {
		if (partElement != nullptr)
		{
			partElement->SetAttribute("x", points[i].x);
			partElement->SetAttribute("y", points[i].y);
			partElement = partElement->NextSiblingElement();
		}
		else {
			TiXmlElement * part = new TiXmlElement("part");
			partNode->LinkEndChild(part);
			part->SetAttribute("name", i);
			part->SetAttribute("x", points[i].x);
			part->SetAttribute("y", points[i].y);
		}
	}
}


*/


void Annotate::write_landmarks(TiXmlNode* partNode)
{
	// write results to points
	int iVal;
	TiXmlElement* partElement = partNode->ToElement();
	assert(partElement);
	TiXmlElement* boxElemet = partNode->Parent()->ToElement();

	for (int i = 0; i < 70; i++) { // TODO 70
		if (partElement != nullptr) {
			partElement->SetAttribute("x", points[i].x);
			partElement->SetAttribute("y", points[i].y);
			partElement = partElement->NextSiblingElement();
		}
		else {
			if (points[i].x != -1 && points[i].y != -1) {
				TiXmlElement * part = new TiXmlElement("part");
				boxElemet->LinkEndChild(part);
				part->SetAttribute("name", i);
				part->SetAttribute("x", points[i].x);
				part->SetAttribute("y", points[i].y);
			}
		}
	}
}




int Annotate::find_closest_point(const Point2f p, const double thresh)
{
   int n = data.points[idx].size(),imin = -1; 
   double dmin = -1;
   for(int i = 0; i < n; i++)
   {
	   double d = norm(p-data.points[idx][i]);
	   if((imin < 0) || (d < dmin))
	   {
		   imin = i; 
		   dmin = d;
	   }
   }
   if((dmin >= 0) && (dmin < thresh))
	   return imin; 
   else
	   return -1;
}

int Annotate::find_closest_landmark(const Point2f p, const double thresh)
{
	int n = points.size(), imin = -1;
	double dmin = -1;
	for (int i = 0; i < n; i++)
	{
		double d = norm(p - points[i]);
		if ((imin < 0) || (d < dmin))
		{
			imin = i;
			dmin = d;
		}
	}
	if ((dmin >= 0) && (dmin < thresh))
		return imin;
	else
		return -1;
}

int Annotate::find_closest_eye_landmark(const Point2f p, const double thresh)
{
	int n = points.size(), imin = -1;
	double dmin = -1;
	for (int i = 68; i < n; i++)
	{
		double d = norm(p - points[i]);
		if ((imin < 0) || (d < dmin))
		{
			imin = i;
			dmin = d;
		}
	}
	if ((dmin >= 0) && (dmin < thresh))
		return imin;
	else
		return -1;
}

void Annotate::draw_strings(Mat img, const vector<string> &text)
{
	for(int i = 0; i < int(text.size()); i++)
		this->draw_string(img,text[i],i+1);
}

void Annotate::draw_string(Mat img, 
			const string text,
			const int level)
{
	Size size = getTextSize(text,FONT_HERSHEY_COMPLEX,0.6f,1,NULL);
	putText(img,text,Point(0,level*size.height),FONT_HERSHEY_COMPLEX,0.6f,
		Scalar::all(0),1,CV_AA);
	putText(img,text,Point(1,level*size.height+1),FONT_HERSHEY_COMPLEX,0.6f,
		Scalar::all(255),1,CV_AA);
}





//#include "ft.hpp"
//#include "ft_data.hpp"
//#include "annotate.h"
//#include "muct.h"
//#include <opencv2/highgui/highgui.hpp>
//#include <iostream>
//#include <fstream>
//
//using namespace std;
//using namespace cv;
//
//Annotate annotation;
////==============================================================================  pick points
//void pp_MouseCallback(int event, int x, int y, int /*flags*/, void* /*param*/)
//{ 
//	if(event == CV_EVENT_LBUTTONDOWN){
//		annotation.data.points[0].push_back(Point2f(x,y));
//		annotation.draw_points(); imshow(annotation.wname,annotation.image); 
//	}
//}
//
////==============================================================================  pick connections
//void pc_MouseCallback(int event, int x, int y, int /*flags*/, void* /*param*/)
//{ 
//	if(event == CV_EVENT_LBUTTONDOWN){
//		int imin = annotation.find_closest_point(Point2f(x,y)); // find closest point, useful
//		if(imin >= 0){ //add connection
//			int m = annotation.data.connections.size();
//			if(m == 0)annotation.data.connections.push_back(Vec2i(imin,-1));
//			else{
//				if(annotation.data.connections[m-1][1] < 0)//1st connecting point chosen
//					annotation.data.connections[m-1][1] = imin;
//				else annotation.data.connections.push_back(Vec2i(imin,-1));
//			}
//			annotation.draw_connections(); 
//			imshow(annotation.wname,annotation.image); 
//		}
//	}
//}
//
////==============================================================================
//void ps_MouseCallback(int event, int x, int y, int /*flags*/, void* /*param*/)  // pick symmetry
//{ 
//	if(event == CV_EVENT_LBUTTONDOWN){
//		int imin = annotation.find_closest_point(Point2f(x,y));
//		if(imin >= 0){
//			if(annotation.pidx < 0)annotation.pidx = imin;
//			else{
//				annotation.data.symmetry[annotation.pidx] = imin;
//				annotation.data.symmetry[imin] = annotation.pidx;
//				annotation.pidx = -1;
//			}      
//			annotation.draw_symmetry(); 
//			imshow(annotation.wname,annotation.image); 
//		}
//	}
//}
//
////==============================================================================  move points
//void mv_MouseCallback(int event, int x, int y, int /*flags*/, void* /*param*/)
//{ 
//	if(event == CV_EVENT_LBUTTONDOWN){
//		if(annotation.pidx < 0){
//			annotation.pidx = annotation.find_closest_point(Point2f(x,y)); // select point
//		}else annotation.pidx = -1; // release point
//		annotation.copy_clean_image();
//		annotation.draw_connections();
//		annotation.draw_chosen_point();
//		imshow(annotation.wname,annotation.image); 
//	}else if(event == CV_EVENT_MOUSEMOVE){
//		if(annotation.pidx >= 0){
//			annotation.data.points[annotation.idx][annotation.pidx] = Point2f(x,y);
//			annotation.copy_clean_image();
//			annotation.draw_connections();
//			annotation.draw_chosen_point();
//			imshow(annotation.wname,annotation.image); 
//		}
//	}
//}
//
////==============================================================================
//string parse_odir(int argc,char** argv)
//{
//	string odir = "data/";
//	for(int i = 1; i < argc; i++){
//		string str = argv[i];
//		if(str.length() != 2)continue;
//		if(strcmp(str.c_str(),"-d") == 0){
//			if(argc > i+1){odir = argv[i+1]; break;}
//		}
//	}
//	if(odir[odir.length()-1] != '/')odir += "/";
//	return odir;
//}
//
////==============================================================================
//int parse_ifile(int argc,
//			char** argv,
//			string& ifile)
//{
//	for(int i = 1; i < argc; i++){
//		string str = argv[i];
//		if(str.length() != 2)continue;
//		if(strcmp(str.c_str(),"-m") == 0){ //MUCT data
//			if(argc > i+1){ifile = argv[i+1]; return 2;}
//		}
//		if(strcmp(str.c_str(),"-v") == 0){ //video file
//			if(argc > i+1){ifile = argv[i+1]; return 1;}
//		}
//	}
//	ifile = ""; return 0;
//}
//
////==============================================================================
//int mainless(int argc,char** argv)
//{	
//	Mat im,img; 
//	VideoCapture cam; 
//	char str[1024]; 
//	string ifile = "C:/Users/User/Documents/MyProjects/annotate tools/Annotate/Annotate/000.png"; 
//	string fname = "annotations.xml"; //file to save annotation data to
//	int idx = 0;
//	int flag = 1;
//	int c = 0;
//
//	//get data // TODO loadXml
//	namedWindow(annotation.wname);  
//	cam.open(ifile);
//	
//	if(!cam.isOpened()){
//		cout << "Failed opening video file." << endl
//			<< "usage: ./annotate [-v video] [-m muct_dir] [-d output_dir]" 
//			<< endl; 
//		return 0;
//	}
//	cam >> im;
//
//	//get images to annotate
//	annotation.set_capture_instructions();
//	annotation.image = im.clone(); 
//	annotation.draw_instructions(); 
//	imshow(annotation.wname,annotation.image); 
//	
//	while(im.empty()!= true && flag != 0)
//	{	
//		c = waitKey(0);
//		switch((char)c)
//		{
//			case 'q':
//				cout <<"Exiting ...\n";
//				flag = 0;
//				break;
//			case 's':
//				annotation.image = im.clone(); 
//				annotation.draw_instructions();
//				imshow(annotation.wname,annotation.image);
//				idx = annotation.data.imnames.size(); 
//				if(idx < 10)
//					sprintf(str,"00%d.png",idx);
//				else if(idx < 100)
//					sprintf(str,"0%d.png",idx);
//				else               
//					sprintf(str,"%d.png",idx);
//				imwrite(str,im); 
//				annotation.data.imnames.push_back(str);
//				cam >> im;
//				imshow(annotation.wname,annotation.image); 
//				break;
//			case 'n':
//				cam >> im;
//				annotation.image = im.clone(); 
//				annotation.draw_instructions();
//				imshow(annotation.wname,annotation.image);
//				break;
//		}		
//	}
//	if(annotation.data.imnames.size() == 0)
//		return 0;
//	annotation.data.points.resize(annotation.data.imnames.size());
//
//	//pick points  // TODO delete
//	setMouseCallback(annotation.wname,pp_MouseCallback,0);
//	annotation.set_pick_points_instructions();
//	annotation.set_current_image(0);
//	annotation.draw_instructions();
//	annotation.idx = 0;
//	while(1)
//	{ 
//		annotation.draw_points();
//		imshow(annotation.wname,annotation.image); 
//		if(waitKey(0) == 'q')
//			break;
//	}
//	if(annotation.data.points[0].size() == 0)
//		return 0;
//	annotation.replicate_annotations(0);
//
//	save_ft(fname.c_str(),annotation.data);
//
//	//annotate connections
//	setMouseCallback(annotation.wname,pc_MouseCallback,0);
//	annotation.set_connectivity_instructions();
//	annotation.set_current_image(0);
//	annotation.draw_instructions();
//	annotation.idx = 0;
//	while(1)
//	{
//		annotation.draw_connections(); 
//		imshow(annotation.wname,annotation.image); 
//		if(waitKey(0) == 'q')
//			break;
//	}
//	save_ft(fname.c_str(),annotation.data); 
//
//	//annotate symmetry
//	setMouseCallback(annotation.wname,ps_MouseCallback,0);
//	annotation.initialise_symmetry(0);
//	annotation.set_symmetry_instructions();
//	annotation.set_current_image(0);
//	annotation.draw_instructions();
//	annotation.idx = 0; 
//	annotation.pidx = -1;
//	while(1){ 
//		annotation.draw_symmetry(); 
//		imshow(annotation.wname,annotation.image); 
//		if(waitKey(0) == 'q')
//			break;
//	}
//	save_ft(fname.c_str(),annotation.data); 
//
//	//annotate the rest
//	setMouseCallback(annotation.wname,mv_MouseCallback,0);
//	annotation.set_move_points_instructions();
//	annotation.idx = 1; 
//	annotation.pidx = -1;
//	while(1)
//	{
//		annotation.set_current_image(annotation.idx);
//		annotation.draw_instructions();
//		annotation.set_clean_image();
//		annotation.draw_connections();
//		imshow(annotation.wname,annotation.image); 
//		c = waitKey(0);
//		
//		if(c == 'q')
//			break;
//		else if(c == 'p')
//		{
//			annotation.idx++;
//			annotation.pidx = -1;
//		}
//		else if(c == 'o')
//		{
//			annotation.idx--; 
//			annotation.pidx = -1;
//		}
//		if(annotation.idx < 0)
//			annotation.idx = 0;
//		if(annotation.idx >= int(annotation.data.imnames.size()))
//			annotation.idx = annotation.data.imnames.size()-1;
//	}
//	
//	save_ft(fname.c_str(),annotation.data); 
//	destroyWindow("Annotate"); 
//	return 0;
//}

#include "muct.h"
muct_data::muct_data(string str, string muct_dir)
{
  size_t p1 = 0,p2;

  //set image directory
  string idir = muct_dir; 
  if(idir[idir.length()-1] != '/')
	  idir += "/";
  idir += "jpg/";

  //get image name
  p2 = str.find(",");
  if(p2 == string::npos)
  {
	  cerr << "Invalid MUCT file" << endl; exit(0);
  }
  name = str.substr(p1,p2-p1);

  if((strcmp(name.c_str(),"i434xe-fn") == 0)||(name[1] == 'r')) //corrupted data ||ignore flipped images
	  name = "";                
  else
  {
	  name = idir + str.substr(p1,p2-p1) + ".jpg"; p1 = p2+1;

	  //get index
	  p2 = str.find(",",p1);
	  if(p2 == string::npos)
	  {
		  cerr << "Invalid MUCT file" << endl; exit(0);
	  }
	  index = str.substr(p1,p2-p1); p1 = p2+1;

	  //get points
	  for(int i = 0; i < 75; i++)
	  {
		  p2 = str.find(",",p1);
		  if(p2 == string::npos)
		  {
			  cerr << "Invalid MUCT file" << endl; exit(0);
		  }
		  string x = str.substr(p1,p2-p1); 
		  p1 = p2+1;
		  p2 = str.find(",",p1);
		  if(p2 == string::npos)
		  {
			  cerr << "Invalid MUCT file" << endl; exit(0);
		  }
		  string y = str.substr(p1,p2-p1); 
		  p1 = p2+1;
		  points.push_back(Point2f(atoi(x.c_str()),atoi(y.c_str())));
	  }
	  p2 = str.find(",",p1);
	  if(p2 == string::npos)
	  {
		  cerr << "Invalid MUCT file" << endl; exit(0);
	  }
	  string x = str.substr(p1,p2-p1); p1 = p2+1;
	  string y = str.substr(p1,str.length()-p1);
	  points.push_back(Point2f(atoi(x.c_str()),atoi(y.c_str())));
  }
}

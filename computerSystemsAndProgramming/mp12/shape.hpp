#ifndef SHAPE_H_
#define SHAPE_H_

#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip>    

using namespace std;

/*
Intro:
This file contains the classes Shape, Rectangle, Circle, and RectPrism. It also 
contains the functions createShapes, maxArea, and maxVolume. All of the shape classes 
are children of the Shape class and contain virtual functions to getArea and getVolume.
They also contain attrubtes to describe their size like width and length or radius. 
The function createShapes creates a list of Shape pointers based on an input file.
maxArea and maxVolume returns the max area and volume of an inputted Shape pointer list.
*/

//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initizlize Shape's private variable 
  	Shape(string name) {
		name_ = name;
	}
	
  	string getName() {
		return name_;
	}
	
  	virtual double getArea() const = 0;
  	virtual double getVolume() const = 0;
	
private:
  string name_;


};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
	//constructs the Rectangle class
  	Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle") {
		width_ = width;
		length_ = length;
	}
	//returns the area
  	double getArea() const {
		return width_*length_;
	}
	//returns the volume
  	double getVolume() const {
		return 0;
	}
	//overloads the + operator
	Rectangle<T> operator + (const Rectangle<T>& rec) {
		Rectangle<T> res;
		res.width_ = width_ + rec.width_;
		res.length_ = length_ + rec.length_;
		return res;
	}
	//overloads the - operator
	Rectangle<T> operator - (const Rectangle<T>& rec) {
		Rectangle<T> res;
		res.width_ = width_ - rec.width_;
		if(res.width_ < 0){
			res.width_ = 0;
		}
		res.length_ = length_ - rec.length_;
		if(res.length_ < 0){
			res.length_ = 0;
		}
		return res;
	} 
	
	//returns the width
	T getWidth() const { 
		return width_;
	}
	
	//returns the length
	T getLength() const { 
		return length_;
	}
private:
	T width_;
        T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
	//Constructs the circle class
  	Circle(double radius = 0):Shape("Circle") {
		radius_ = radius;
	}
	//returns the area
  	double getArea() const{
		return radius_*radius_*M_PI;
	}
	//returns the volume
 	double getVolume() const{
		return 0;
	}
	//Overloads the + operator
  	Circle operator + (const Circle& cir){
		Circle res;
		res.radius_ = radius_ + cir.radius_;
		return res;
	}
	//Overloads the - operator
	Circle operator - (const Circle& cir){
		Circle res;
		res.radius_ = radius_ - cir.radius_;
		if(res.radius_ < 0){
			res.radius_ = 0;
		}
		return res;
	} 
	//returns the radius
	double getRadius() const { 
		return radius_;
	}
	
private:
	double radius_;

};



//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
	//Constructs the Sphere
  	Sphere(double radius=0):Shape("Sphere") {
		radius_ = radius;
	}
	//returns the volume
  	double getVolume() const {
		return (4.0/3.0)*radius_*radius_*radius_*M_PI;
	}	
	//returns the area
  	double getArea() const {
		return radius_*radius_*4.0*M_PI;
	}
	//Overloads the + operator
	Sphere operator + (const Sphere& sph) {
		Sphere res;
		res.radius_ = radius_ + sph.radius_;
		return res;
	}
	//Overloads the - operator
	Sphere operator - (const Sphere& sph) {
		Sphere res;
		res.radius_ = radius_ - sph.radius_;
		if(res.radius_ < 0){
			res.radius_ = 0;
		}
		return res;
	} 
	//returns the radius
	double getRadius() const {
		return radius_;
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
class RectPrism : public Shape{
public:
	//Constructs the RectPrism class
  	RectPrism(double width=0, double length=0, double height=0):Shape("RectPrism") {
		width_ = width;
		length_ = length;
		height_ = height;
	}
	//returns the volume
  	double getVolume() const {
		return length_*width_*height_;
	}
  	//returns the area
	double getArea() const {
		return 2*((length_*width_) + (length_*height_) + (width_*height_));
	}
	//Overloads the + operator
	RectPrism operator + (const RectPrism& rectp){
		RectPrism res;
		res.width_ = width_ + rectp.width_;
		res.length_ = length_ + rectp.length_;
		res.height_ = height_ + rectp.height_;
		return res;
	}
	//Overloads the - operator
	RectPrism operator - (const RectPrism& rectp){
		RectPrism res;
		res.width_ = width_ - rectp.width_;
		if(res.width_ < 0){
			res.width_ = 0;
		}
		res.length_ = length_ - rectp.length_;
		if(res.length_ < 0){
			res.length_ = 0;
		}
		res.height_ = height_ - rectp.height_;
		if(res.height_ < 0){
			res.height_ = 0;
		}
		return res;
	}	
	//returns the width
	double getWidth() const { 
		return width_;
	}
	//returns the length
	double getLength() const { 
		return length_;
	}
	//returns the height
	double getHeight() const { 
		return height_;
	}
  
private:
  	double length_;
  	double width_;
  	double height_;

};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
static list<Shape*> CreateShapes(char* file_name) {
	//@@Insert your code here
	
  	string name;
  	int num;
  //char end;

	//creates strings to reference when comparing names pulled from the file
  	string rect("Rectangle");
  	string cir("Circle");
  	string sph("Sphere");
 	string pris("RectPrism");
  
	//starts the ifstream io
	ifstream ifs (file_name, std::ifstream::in);
	ifs >> num;

	//creates the return list
	list<Shape*> shapeList;
	list<Shape*>::iterator it = shapeList.begin();

	//loops through the file until all the shapes are extracted
	for (int i=0; i<num; i++){
		//extracts the next name from the file
		ifs >> name;
		Shape* next;
		//if the shape is a rectangle, width and length are extracted and assigned
		if (name.compare(rect) == 0){
			double w, l;
			ifs >> w >> l;       
	        next = new Rectangle<double>(w, l);
		}//if the shape is a circle, the radius is extracted and assigned 
		else if (name.compare(cir) == 0){
			double r;
			ifs >> r;
	        next = new Circle(r);
		}//if the shape is a sphere, the radius is extracted and assigned
		else if (name.compare(sph) == 0){
			double r;
			ifs >> r;
	       	next = new Sphere(r);
		}//if the shape is a RectPrism, the width, length, and height is extracted and assigned
		else if (name.compare(pris) == 0){
			double w, l, h;
			ifs >> w >> l >> h;
	        next = new RectPrism(w, l, h);
        }
		//the next shape is pushed to the return list
		shapeList.push_back(next);
		it++;
	}
	ifs.close();
	return shapeList;
}

// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
	//loops through the shapes list and if the area of the current shape is larger
	//than the saved max area, then the max area is reassiged
	for(auto i = shapes.begin(); i != shapes.end(); i++){
	  if(max_area < (**i).getArea()){
	    max_area = (**i).getArea();
		}
	}
	return max_area;
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here
	//loops through the shapes list and if the volume of the current shape is larger
	//than the saved max volume, then the max volume is reassiged
	for(auto i = shapes.begin(); i != shapes.end(); i++){
	  if(max_volume < (**i).getVolume()){
	    max_volume = (**i).getVolume();
		}
	}
	return max_volume;
}
#endif


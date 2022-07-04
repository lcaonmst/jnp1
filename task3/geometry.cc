/* Implementacja: geometry
 * Autorzy:
 * Kamil Zwierzchowski 418510
 * Konrad Korczyński 418331
 */ 

#include <utility>
#include <cassert>
#include "geometry.h"

namespace {

using fint = int_fast32_t;
	
void valid_args(fint width, fint height) {assert(width > 0); assert(height > 0);}
	
inline bool valid_vertically(const Rectangle& rec1, const Rectangle& rec2) {
	return rec1.height() == rec2.height() && rec1.pos().y() == rec2.pos().y() 
			&& rec1.pos().x() + rec1.width() == rec2.pos().x();
}
	
inline bool valid_horizontally(const Rectangle& rec1, const Rectangle& rec2) {
	return rec1.width() == rec2.width() && rec1.pos().x() == rec2.pos().x() 
			&& rec1.pos().y() + rec1.height() == rec2.pos().y();
}


//////////////////////POSITION I VECTOR////////////////////////////////
//Makro, ktore wykorzystuje podobienstwa w implementacjach position i vector

#define DECLARATION(A, B) 															\
const A A::center = A(0, 0);														\
																					\
A::A(fint x, fint y): posX(x), posY(y) {}											\
A::A(const B& v): A(v.x(), v.y()) {}												\
																					\
fint A::x() const {return posX;}													\
fint A::y() const {return posY;}													\
A A::reflection() const {return A(posY, posX);}										\
																					\
A& A::operator+= (const Vector& v) {posX += v.x(); posY += v.y(); return *this;}	\
A A::operator+ (const Vector& v) const {return A(*this) += v;}						\
bool A::operator== (const A& v) const {return x() == v.x() && y() == v.y();}		\
																					\
const A& A::origin() {return center;}	
											
}

DECLARATION(Position, Vector)
DECLARATION(Vector, Position)

/////////////////////////////RECTANGLE/////////////////////////////////

Rectangle::Rectangle(fint width, fint height): w(width), h(height), position(0, 0) {
	valid_args(width, height);
}
Rectangle::Rectangle(fint width, fint height, Position pos):  w(width), h(height), position(pos) {
	valid_args(width, height);
}

fint Rectangle::width() const {return w;}
fint Rectangle::height() const {return h;}
const Position& Rectangle::pos() const {return position;}

Rectangle Rectangle::reflection() const {return Rectangle(height(), width(), pos().reflection());}
fint Rectangle::area() const {return width() * height();}

Rectangle& Rectangle::operator+= (const Vector& vec) {position += vec; return *this;}
bool Rectangle::operator== (const Rectangle& rec) const {
	return rec.width() == width() && rec.height() == height() && rec.pos() == pos();
}

////////////////////////////RECTANGLES////////////////////////////////////

Rectangles::Rectangles(std::initializer_list<Rectangle> recs): rectans(recs) {}

size_t Rectangles::size() const {return rectans.size();}

Rectangle& Rectangles::operator[] (int index) {
	assert(index >= 0 && index < (int)rectans.size()); 
	return rectans[index];
}
const Rectangle& Rectangles::operator[] (int index) const {
	assert(index >= 0 && index < (int)rectans.size()); 
	return rectans[index];
}

bool Rectangles::operator== (const Rectangles& recs) const {
	if(recs.size() != rectans.size())
		return false;

	for(size_t i = 0; i < rectans.size(); ++i)
		if(!(recs[i] == rectans[i]))
			return false;
	return true;
}
Rectangles& Rectangles::operator+= (const Vector& vec) {
	for(Rectangle& rec: rectans)
		rec += vec;
	return *this;
}

/////////////////////////+ OPERATORS///////////////////////////////
//wszystkie, które nie zostały zadeklarowane przez #define DECLARAITON(A, B)

Position Vector::operator+ (const Position& pos) const {return Position(pos) += *this;}
Rectangle Vector::operator+ (const Rectangle& rec) const {return Rectangle(rec) += *this;}
Rectangle Rectangle::operator+ (const Vector& vec) const {return Rectangle(*this) += vec;}
Rectangles Vector::operator+ (const Rectangles& recs) const {return Rectangles(recs) += *this;}
Rectangles Rectangles::operator+ (const Vector& vec) const {return Rectangles(*this) += vec;}
Rectangles operator+ (Rectangles&& recs, const Vector& vec) {return Rectangles(std::move(recs)) += vec;}
Rectangles operator+ (const Vector& vec, Rectangles&& recs) {return std::move(recs) + vec;}

////////////////////////MERGE////////////////////////////////////////

Rectangle merge_vertically(const Rectangle& rec1, const Rectangle& rec2) {
	assert(valid_vertically(rec1, rec2));
	return Rectangle(rec1.width() + rec2.width(), rec1.height(), rec1.pos());
}

Rectangle merge_horizontally(const Rectangle& rec1, const Rectangle& rec2) {
	assert(valid_horizontally(rec1, rec2));
	return Rectangle(rec1.width(), rec1.height() + rec2.height(), rec1.pos());
}

Rectangle merge_all(const Rectangles& recs) {
	assert(recs.size() > 0);
	Rectangle merged = Rectangle(recs[0]);

	for(size_t i = 1; i < recs.size(); ++i) {
		if(valid_horizontally(merged, recs[i]))
			merged = merge_horizontally(merged, recs[i]);
		else if(valid_vertically(merged, recs[i]))
			merged = merge_vertically(merged, recs[i]);
		else
			throw;
	}

	return merged;
}

/* Interfejs: geometry
 * Autorzy:
 * Kamil Zwierzchowski 418510
 * Konrad Korczyński 418331
 */

#ifndef GEOMETRY_H 
#define GEOMETRY_H

#include <vector>
#include <initializer_list>
#include <cstdint>

class Vector;
class Position;
class Rectangle;
class Rectangles;

class Position {
		using fint = int_fast32_t;
		fint posX;
		fint posY;
		static const Position center;	
	public:
		Position() = delete;
		Position(fint x, fint y);
		explicit Position(const Vector&);
		Position(const Position&) = default;
		fint x() const;
		fint y() const;
		Position reflection() const;
		Position& operator+= (const Vector&);
		Position operator+ (const Vector&) const;
		bool operator== (const Position&) const;
		static const Position& origin();
};

class Vector { 
		using fint = int_fast32_t;
		fint posX;
		fint posY;
		static const Vector center;	
	public:
		Vector() = delete;
		Vector(fint x, fint y);
		explicit Vector(const Position&);
		Vector(const Vector&) = default;
		fint x() const;
		fint y() const;
		Vector reflection() const;
		Vector& operator+= (const Vector&);
		Vector operator+ (const Vector&) const;
		Position operator+ (const Position&) const;
		Rectangle operator+ (const Rectangle&) const;
		Rectangles operator+ (const Rectangles&) const;
		bool operator== (const Vector&) const;	
		static const Vector& origin();
};

class Rectangle
{
		using fint = int_fast32_t;
		fint w;
		fint h;
		Position position;
	public:
		Rectangle() = delete;
		Rectangle(fint width, fint height);
		Rectangle(fint width, fint height, Position pos);
		Rectangle(const Rectangle& rect) = default;
		Rectangle(Rectangle&& rec) = default;
		fint width() const;
		fint height() const;
		const Position& pos() const;
		Rectangle reflection() const;
		Rectangle& operator=(const Rectangle& rec) = default;
		Rectangle& operator=(Rectangle&& rec) = default;
		Rectangle& operator+=(const Vector& vec);
		Rectangle operator+(const Vector& vec) const;
		bool operator==(const Rectangle& rec) const;
		fint area() const;	
};


class Rectangles
{
		std::vector<Rectangle> rectans;
	public:
		Rectangles() = default;
		Rectangles(std::initializer_list<Rectangle> recs);
		Rectangles(const Rectangles& recs) = default;
		Rectangles(Rectangles&& recs) = default;
		size_t size() const;
		Rectangle& operator[](int index);
		const Rectangle& operator[](int index) const;
		Rectangles& operator=(const Rectangles& rec) = default;
		Rectangles& operator=(Rectangles&& rec) = default;
		bool operator==(const Rectangles& recs) const;
		Rectangles& operator+=(const Vector& vec);
		Rectangles operator+(const Vector& vec) const;
};

Rectangles operator+ (Rectangles&&, const Vector&);
Rectangles operator+ (const Vector&, Rectangles&&);
Rectangle merge_horizontally(const Rectangle& rec1, const Rectangle& rec2);
Rectangle merge_vertically(const Rectangle& rec1, const Rectangle& rec2);
Rectangle merge_all(const Rectangles& recs);

#endif


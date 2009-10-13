#ifndef _VECTOR2D_H
#define _VECTOR2D_H
#include <math.h>

struct Vector2D{

	double x;
	double y;


	Vector2D():x(0.0),y(0.0) {}
	Vector2D(double a, double b):x(a),y(b) {}

	Vector2D(const Vector2D& v) : x(v.x), y(v.y) {}
	
	void normalize(){
		double divisor = this->magnitude();
		if(divisor == 0)
			return;
		this->x = this->x/divisor;
		this->y = this->y/divisor;
	}


	double dot(const struct Vector2D &other) const{
		return (this->x * other.x + this->y * other.y);
	}


	double magnitude() const{
		return sqrt(this->x*this->x + this->y*this->y);
	}

	struct Vector2D operator-(const struct Vector2D &other) const {
		struct Vector2D subs = Vector2D(this->x-other.x, this->y-other.y);	
		return subs;
	}

	void operator-=(const struct Vector2D &other){
		this->x -= other.x;
		this->y -= other.y;
	}

	struct Vector2D operator+(const struct Vector2D &other) const{
		struct Vector2D sum = Vector2D(this->x+other.x, this->y+other.y);
		return sum;
	
	}
	
	void operator+=(const struct Vector2D &other){
		this->x += other.x;
		this->y += other.y;
	}

	struct Vector2D operator*(const double factor) const{
		struct Vector2D scal = Vector2D(this->x*factor, this->y*factor);
		return scal;
	}

	void operator*=(const double factor){
		this->x *= factor;
		this->y *= factor;
	}
};

#endif
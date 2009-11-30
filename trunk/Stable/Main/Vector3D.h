#pragma once
#include <math.h>

struct Vector3D{

	double x;
	double y;
	double z;

	Vector3D():x(0.0),y(0.0),z(0.0){}
	Vector3D(double a, double b, double c):x(a),y(b),z(c){}

	Vector3D(const Vector3D& v) : x(v.x), y(v.y), z(v.z){}
	
	void normalize(){
		double divisor = this->magnitude();
		if(divisor == 0)
			return;
		this->x = this->x/divisor;
		this->y = this->y/divisor;
		this->z = this->z/divisor;
	}

	double dot(const struct Vector3D &other) const
	{
		return (this->x * other.x + this->y * other.y + this->z * other.z);
	}


	double magnitude() const
	{
		return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
	}

	struct Vector3D operator-(const struct Vector3D &other) const 
	{
		struct Vector3D subs = Vector3D(this->x-other.x, this->y-other.y, this->z-other.z);	
		return subs;
	}

	void operator-=(const struct Vector3D &other)
	{
		this->x -= other.x;
		this->y -= other.y;
		this->z -= other.z;
	}

	struct Vector3D operator+(const struct Vector3D &other) const{
		struct Vector3D sum = Vector3D(this->x+other.x, this->y+other.y, this->z+other.z);
		return sum;
	
	}
	
	void operator+=(const struct Vector3D &other){
		this->x += other.x;
		this->y += other.y;
		this->z += other.z;
	}

	struct Vector3D operator*(const double factor) const
	{
		struct Vector3D scal = Vector3D(this->x*factor, this->y*factor, this->z*factor);
		return scal;
	}

	void operator*=(const double factor)
	{
		this->x *= factor;
		this->y *= factor;
		this->z *= factor;
	}

	bool operator== (const Vector3D& v2) const {
		return this->x == v2.x && this-> y == v2.y && this->z == v2.z;
	}
};

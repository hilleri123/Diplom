
#pragma once


#include <cstdarg>
#include <valarray>
#include <vector>
#include <algorithm>
#include <utility>
#include <stdexcept>

#include "point.h"
#include "vector.h"
#include "metrics.h"
#include "log.h"

class Polygon;
class BzCurve;
class Conversion;

class Matrix : public base_init
{
public:
	enum matrix {null, move, rotate, comp};
//	    x y z
//	 x (1 0 0 0)
//	 y (0 1 0 0)
//	 z (0 0 1 0)
//	   (0 0 0 1)
	explicit Matrix(matrix opt = null, const Vector* v = nullptr, double angle = 0);

	Matrix(const Matrix&) = default;
	Matrix(Matrix&&) = default;
	Matrix& operator=(const Matrix&) = default;
	Matrix& operator=(Matrix&&) = default;
	
	virtual bool init() override;

	bool operator==(const Matrix& vector) const;
	bool operator!=(const Matrix& vector) const;
	bool operator==(Matrix&& vector) const;
	bool operator!=(Matrix&& vector) const;

//	this = this * matrix
	Matrix& operator*=(const Matrix&);

	friend Matrix operator*(const Matrix&, const Matrix&);

	Point operator()(Point) const;
	Vector operator()(Vector) const;
	BzCurve operator()(BzCurve) const;
	Polygon operator()(Polygon) const;

	friend std::ostream& operator<<(std::ostream& stream, const Matrix& m);
	friend std::ostream& operator<<(std::ostream& stream, Matrix&& m);

	std::size_t size() const;

	//static Matrix&& transform(const Matrix& matrixs...);
	
	static bool multiplay_foreward_backward(Matrix& foreward, Matrix& backward, matrix opt = null, const Vector* vec_ptr = nullptr, double angle = 0);

	virtual ~Matrix() override;
protected:
	std::valarray<double> _matrix;

	std::exception* err = nullptr;
};



class Conversion : public base_init
{
public:
	explicit Conversion(const Matrix& to, const Matrix& from)
		: _to(to), _from(from)
	{}

	Conversion(const Point* pos = nullptr, const Vector* ox = nullptr, const Vector* oy = nullptr, const Vector* oz = nullptr);

	virtual bool init() override;

	Matrix& to_matrix() { return _to;}
	Matrix& from_matrix() { return _from;}

	template <class T>
	T to(T p) const { return _to(p);}
	template <class T>
	T from(T p) const { return _from(p);}

#if 0
	Point to(Point&& p) { return _to(p);}
	Point from(Point&& p) { return _from(p);}
	Vector to(Vector&& v) { return _to(v);}
	Vector from(Vector&& v) { return _from(v);}
#endif
	virtual ~Conversion() override;
protected:
	//std::exception* err = nullptr;
	
	Point const* pos;
	Vector const* ox;
	Vector const* oy;
	Vector const* oz;
	
	Matrix _to;
	Matrix _from;
};

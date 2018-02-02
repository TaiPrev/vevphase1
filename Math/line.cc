#include <cassert>
#include <cstdio>
#include "line.h"
#include "constants.h"
#include "tools.h"

const float epsilon = 0.0001f;

Line::Line() : m_O(Vector3::ZERO), m_d(Vector3::UNIT_Y) {}
Line::Line(const Vector3 & o, const Vector3 & d) : m_O(o), m_d(d) {}
Line::Line(const Line & line) : m_O(line.m_O), m_d(line.m_d) {}

Line & Line::operator=(const Line & line) {
	if (&line != this) {
		m_O = line.m_O;
		m_d = line.m_d;
	}
	return *this;
}

// @@ TODO: Set line to pass through two points A and B
//
// Note: Check than A and B are not too close!

void Line::setFromAtoB(const Vector3 & A, const Vector3 & B) {
	//Vector3 aux = new Vector3(0.0, 0.0, 0.0); 	//creación de un vector con todo a 0
	m_O = A;	//asignamos A como punto de origen de la linea
	Vector3 aux = B-A;
	float normal = aux.normalize();	//ya esta controlado que sea mayor que 0
	if (normal > epsilon){
		printf("Los puntos estan muy cercanos.\n");	//imprimimos en caso de que la normal sea 'igual' a 0
	}
	m_d = aux;
}

// @@ TODO: Give the point corresponding to parameter u

Vector3 Line::at(float u) const {
	Vector3 res;
	res = m_O + m_d*u;
	return res;
}

// @@ TODO: Calculate the parameter 'u0' of the line point nearest to P
//
// u0 = D*(P-O) / D*D , where * == dot product

float Line::paramDistance(const Vector3 & P) const {
	float res = 0.0f;
	float auxf = 0.0f;
	float divisor = m_d.dot(m_d);
	Vector3 aux = P-m_O;
	auxf = m_d.dot(aux);
	res = auxf/divisor;
	return res;
}

// @@ TODO: Calculate the minimum distance 'dist' from line to P
//
// dist = ||P - (O + uD)||
// Where u = paramDistance(P)

float Line::distance(const Vector3 & P) const {
	float res = 0.0f;
	float auxf = 0.0f;	//el float u
	Vector3 auxv = (P-m_O);
	auxf = m_d.dot(auxv);
	auxf /= m_d.dot(m_d);
	Vector3 p_2 = m_O+(m_d*auxf);	//punto 2, mas cercano a P pero dentro de la linea
	res = P.eucldist(p_2);
	return res;
}

void Line::print() const {
	printf("O:");
	m_O.print();
	printf(" d:");
	m_d.print();
	printf("\n");
}

#include <cmath>
#include "intersect.h"
#include "constants.h"
#include "tools.h"

/* | algo           | difficulty | */
/* |----------------+------------| */
/* | BSPhereBSPhere |          1 | */
/* | BSPherePlane   |          2 | */
/* | BBoxBBox       |          3 | */
/* | BSphereBBox    |          5 | */
/* | BBoxPlane      |          5 | */


// @@ TODO: test if two BBoxes intersect.
//! Returns :
//    IINTERSECT intersect
//    IREJECT don't intersect

int  BBoxBBoxIntersect(const BBox *bba, const BBox *bbb ) {
	if (bba->m_min[0] > bbb->m_max[0] || bbb->m_min[0] > bba->m_max[0]){
		return IREJECT;
	}
	else if (bba->m_min[1] > bbb->m_max[1] || bbb->m_min[1] > bba->m_max[1]){
		return IREJECT;
	}
	else if (bba->m_min[2] > bbb->m_max[2] || bbb->m_min[2] > bba->m_max[2]){
		return IREJECT;
	}
	return IINTERSECT;
}

// @@ TODO: test if a BBox and a plane intersect.
//! Returns :
//   +IREJECT outside
//   -IREJECT inside
//    IINTERSECT intersect

int  BBoxPlaneIntersect (const BBox *theBBox, Plane *thePlane) {
	int i = 0;

	float v_mi[3]; float v_ma[3];
	for (i=0; i<3; i++){
		if (thePlane->m_n[i] >= 0){
			v_mi[i] = theBBox->m_min[i];
			v_ma[i] = theBBox->m_max[i];
		}
		else{
			v_mi[i] = theBBox->m_max[i];
			v_ma[i] = theBBox->m_min[i];
		}
	}
	Vector3 *v_min = new Vector3(v_mi[0], v_mi[1], v_mi[2]);
	Vector3 *v_max = new Vector3(v_ma[0], v_ma[1], v_ma[2]);

	if (v_min->dot(thePlane->m_n) > 0){return -IREJECT;}
	else if (v_max->dot(thePlane->m_n) < 0){return IREJECT;}
	return IINTERSECT;
}


// @@ TODO: test if two BSpheres intersect.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int BSphereBSphereIntersect(const BSphere *bsa, const BSphere *bsb ) {
	float distance = bsa->m_centre.eucldist(bsb->m_centre);
	float radius = bsa->m_radius + bsb->m_radius;
	if (distance > radius){return IREJECT;}
	else{return IINTERSECT;}
}

// @@ TODO: test if a BSpheres intersects a plane.
//! Returns :
//   +IREJECT outside
//   -IREJECT inside
//    IINTERSECT intersect

int BSpherePlaneIntersect(const BSphere *bs, Plane *pl) {
	int side = pl->whichSide(bs->m_centre);
	if(pl->distance(bs->m_centre) > bs->m_radius){
		if (side < 0){return -IREJECT;}
		else{return IREJECT;}
	}
	else{return IINTERSECT;}
}

// @@ TODO: test if a BSpheres intersect a BBox.
//! Returns :
//    IREJECT don't intersect
//    IINTERSECT intersect

int BSphereBBoxIntersect(const BSphere *sphere, const BBox *box) {
	int d = 0;
	int i = 0;
	float e;
	float r = sphere->m_radius;
	for (i=0; i<3; i++){
		e = sphere->m_centre[i] - box->m_min[i];
		if (e < 0){
			if (e < -r){return IREJECT;}
			d+=pow(e , 2.0);
		}
		else{
			e = sphere->m_centre[i] - box->m_max[i];
			if (e > 0){
				if (e > r){return IREJECT;}
				d+=pow(e, 2.0);
			}
		}
	}
	if (d > pow(r, 2.0)){return IREJECT;}
	return IINTERSECT;
}


int IntersectTriangleRay(const Vector3 & P0,
						 const Vector3 & P1,
						 const Vector3 & P2,
						 const Line *l,
						 Vector3 & uvw) {
	Vector3 e1(P1 - P0);
	Vector3 e2(P2 - P0);
	Vector3 p(crossVectors(l->m_d, e2));
	float a = e1.dot(p);
	if (fabs(a) < Constants::distance_epsilon) return IREJECT;
	float f = 1.0f / a;
	// s = l->o - P0
	Vector3 s(l->m_O - P0);
	float lu = f * s.dot(p);
	if (lu < 0.0 || lu > 1.0) return IREJECT;
	Vector3 q(crossVectors(s, e1));
	float lv = f * q.dot(l->m_d);
	if (lv < 0.0 || lv > 1.0) return IREJECT;
	uvw[0] = lu;
	uvw[1] = lv;
	uvw[2] = f * e2.dot(q);
	return IINTERSECT;
}

/* IREJECT 1 */
/* IINTERSECT 0 */

const char *intersect_string(int intersect) {

	static const char *iint = "IINTERSECT";
	static const char *prej = "IREJECT";
	static const char *mrej = "-IREJECT";
	static const char *error = "IERROR";

	const char *result = error;

	switch (intersect) {
	case IINTERSECT:
		result = iint;
		break;
	case +IREJECT:
		result = prej;
		break;
	case -IREJECT:
		result = mrej;
		break;
	}
	return result;
}

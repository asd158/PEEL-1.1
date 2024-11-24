///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 *	Contains code for sphere-plane intersection
 *	\file		CTC_SpherePlaneOverlap.h
 *	\author		Pierre Terdiman
 *	\date		January, 13, 2000
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Include Guard
#ifndef CTCSPHEREPLANEOVERLAP_H
#define CTCSPHEREPLANEOVERLAP_H

	// Sphere-plane intersection
	CONTACT_API float SpherePlane(const Point& center, float radius, const Plane& plane, Point* contact_point=null, Point* contact_normal=null);

#endif // CTCSPHEREPLANEOVERLAP_H

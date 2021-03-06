#ifndef _DERIVATIVES_H_
#define _DERIVATIVES_H_

#include <math.h>
#include "../Definitions.h"
#include "../Parameters.h"

// Load the local velocity cube with relevant velocities of the 2D plane
inline void loadLocalVelocity2D(FlowField & flowField, FLOAT * const localVelocity, int i, int j){
    for (int row = -1; row <= 1; row++ ){
        for ( int column = -1; column <= 1; column ++ ){
            const FLOAT * const point = flowField.getVelocity().getVector(i + column, j + row);
            localVelocity[39 + 9*row + 3*column]     = point[0]; // x-component
            localVelocity[39 + 9*row + 3*column + 1] = point[1]; // y-component
        }
    }
}

// Load the local velocity cube with surrounding velocities
inline void loadLocalVelocity3D(FlowField & flowField, FLOAT * const localVelocity, int i, int j, int k){
    for ( int layer = -1; layer <= 1; layer ++ ){
        for ( int row = -1; row <= 1; row++ ){
            for ( int column = -1; column <= 1; column ++ ){
                const FLOAT * const point = flowField.getVelocity().getVector(i + column, j + row, k + layer);
                localVelocity[39 + 27*layer + 9*row + 3*column    ] = point[0]; // x-component
                localVelocity[39 + 27*layer + 9*row + 3*column + 1] = point[1]; // y-component
                localVelocity[39 + 27*layer + 9*row + 3*column + 2] = point[2]; // z-component
            }
        }
    }
}

inline void loadLocalViscosity2D(FlowField & flowField, FLOAT * const localViscosity, int i, int j){

        for ( int row = -1; row <= 1; row++ ){
            for ( int column = -1; column <= 1; column ++ ){
            	localViscosity[39 + 9*row + 3*column] = flowField.getViscosity().getScalar(i + column, j + row);

	    }
	}
}

// Load the local viscosity cube with surrounding viscosities
// array is almost empty but can be acessed with the same scheme as the other
inline void loadLocalViscosity3D(FlowField & flowField, FLOAT * const localViscosity, int i, int j, int k){
    for ( int layer = -1; layer <= 1; layer ++ ){
        for ( int row = -1; row <= 1; row++ ){
            for ( int column = -1; column <= 1; column ++ ){
            	localViscosity[39 + 27*layer + 9*row + 3*column] = flowField.getViscosity().getScalar(i + column, j + row, k + layer);

	    }
	}
    }
}


// load local meshsize for 2D -> same as loadLocalVelocity2D, but invoking call to meshsize-ptr
inline void loadLocalMeshsize2D(const Parameters& parameters, FLOAT * const localMeshsize, int i, int j){
    for (int row = -1; row <= 1; row++ ){
        for ( int column = -1; column <= 1; column ++ ){
            localMeshsize[39 + 9*row + 3*column]     = parameters.meshsize->getDx(i+column,j+row);
            localMeshsize[39 + 9*row + 3*column + 1] = parameters.meshsize->getDy(i+column,j+row);
        }
    }
}

// load local meshsize for 3D
inline void loadLocalMeshsize3D(const Parameters& parameters, FLOAT * const localMeshsize, int i, int j, int k){
    for ( int layer = -1; layer <= 1; layer ++ ){
        for ( int row = -1; row <= 1; row++ ){
            for ( int column = -1; column <= 1; column ++ ){
                localMeshsize[39 + 27*layer + 9*row + 3*column    ] = parameters.meshsize->getDx(i+column,j+row,k+layer);
                localMeshsize[39 + 27*layer + 9*row + 3*column + 1] = parameters.meshsize->getDy(i+column,j+row,k+layer);
                localMeshsize[39 + 27*layer + 9*row + 3*column + 2] = parameters.meshsize->getDz(i+column,j+row,k+layer);
            }
        }
    }
}


// Maps an index and a component to the corresponding value in the cube.
inline int mapd (int i, int j, int k, int component){
   return 39 + 27*k + 9*j + 3*i + component;
}

// Derivative functions. They are applied to a cube of 3x3x3 cells. lv stands for the local velocity, lm represents the local mesh sizes
// dudx <-> first derivative of u-component of velocity field w.r.t. x-direction
inline FLOAT dudx ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv [mapd(0,0,0,0)] - lv [mapd(-1,0,0,0)] ) / GeometricParameters::dx;

    // evaluate dudx in the cell center by a central difference
    const int index0 = mapd(0,0,0,0);
    const int index1 = mapd(-1,0,0,0);
    return  ( lv [index0] - lv [index1] ) / lm[index0];
    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "dudx");}

    return tmp2;*/
}

inline FLOAT dvdy ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv [mapd(0,0,0,1)] - lv [mapd(0,-1,0,1)] ) / GeometricParameters::dy;
    const int index0 = mapd(0, 0,0,1);
    const int index1 = mapd(0,-1,0,1);
    return ( lv [index0] - lv [index1] ) / lm[index0];

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "dvdy");}

    return tmp2;*/
}

inline FLOAT dwdz ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv [mapd(0,0,0,2)] - lv [mapd(0,0,-1,2)] ) / GeometricParameters::dz;
    const int index0 = mapd(0,0, 0,2);
    const int index1 = mapd(0,0,-1,2);
    return ( lv [index0] - lv [index1] ) / lm[index0];

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "dwdz");}

    return tmp2;*/
}

// first derivative of u-component w.r.t. y-direction, evaluated at the mid of the cell
inline FLOAT dudy ( const FLOAT * const lv, const FLOAT * const lm ) {
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong0 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,-1,0,1)]); // distance between center and west v-value
    const FLOAT hyLong1 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,1,0,1)]); // distance between center and east v-value

    const FLOAT u00   = lv[mapd( 0, 0, 0, 0)];
    const FLOAT u01   = lv[mapd( 0, 1, 0, 0)];
    const FLOAT u0M1  = lv[mapd( 0,-1, 0, 0)];

    const FLOAT uM10  = lv[mapd(-1, 0, 0, 0)];
    const FLOAT uM11  = lv[mapd(-1, 1, 0, 0)];
    const FLOAT uM1M1 = lv[mapd(-1,-1, 0, 0)];

    const FLOAT u00u01  = (hyLong1-hyShort)/hyLong1*u00+hyShort/hyLong1*u01;
    const FLOAT u00u0M1 = (hyLong0-hyShort)/hyLong0*u00+hyShort/hyLong0*u0M1;

    const FLOAT uM10uM11  = (hyLong1-hyShort)/hyLong1*uM10+hyShort/hyLong1*uM11;
    const FLOAT uM10uM1M1 = (hyLong0-hyShort)/hyLong0*uM10+hyShort/hyLong0*uM1M1;

    const FLOAT u0 = 0.5*(u00u01+uM10uM11);
    const FLOAT uM1 = 0.5*(u00u0M1+uM10uM1M1);

    return (u0-uM1)/(2.0*hyShort);
}

// first derivative of v-component w.r.t. x-direction, evaluated at the mid of the cell
inline FLOAT dvdx ( const FLOAT * const lv, const FLOAT * const lm ) {
	const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
	const FLOAT hxLong0 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd(-1,0,0,0)]); // distance between center and west v-value
	const FLOAT hxLong1 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd( 1,0,0,0)]); // distance between center and east v-value

	const FLOAT v00   = lv[mapd( 0, 0, 0, 1)];
	const FLOAT v10   = lv[mapd( 1, 0, 0, 1)];
	const FLOAT vM10  = lv[mapd(-1, 0, 0, 1)];

	const FLOAT v0M1  = lv[mapd( 0,-1, 0, 1)];
	const FLOAT v1M1  = lv[mapd( 1,-1, 0, 1)];
	const FLOAT vM1M1 = lv[mapd(-1,-1, 0, 1)];

	const FLOAT v00v10    = (hxLong1-hxShort)/hxLong1*v00+hxShort/hxLong1*v10;
	const FLOAT v00vM10   = (hxLong0-hxShort)/hxLong0*v00+hxShort/hxLong0*vM10;

	const FLOAT v0M1v1M1  = (hxLong1-hxShort)/hxLong1*v0M1+hxShort/hxLong1*v1M1;
	const FLOAT v0M1vM1M1 = (hxLong0-hxShort)/hxLong0*v0M1+hxShort/hxLong0*vM1M1;

	const FLOAT v0  = 0.5*(v00v10+v0M1v1M1);
    const FLOAT vM1 = 0.5*(v00vM10+v0M1vM1M1);

    return (v0-vM1)/(2.0*hxShort);
}

// first derivative of u-component w.r.t. z-direction, evaluated at the mid of the cell
inline FLOAT dudz( const FLOAT * const lv, const FLOAT * const lm ) {
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong0 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0,-1,2)]); // distance between center and west v-value
    const FLOAT hzLong1 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0, 1,2)]); // distance between center and east v-value

    const FLOAT u00  = lv[mapd( 0, 0, 0, 0)];
    const FLOAT u01  = lv[mapd( 0, 0, 1, 0)];
    const FLOAT u0M1 = lv[mapd( 0, 0,-1, 0)];

    const FLOAT uM10  = lv[mapd(-1, 0, 0, 0)];
    const FLOAT uM11  = lv[mapd(-1, 0, 1, 0)];
    const FLOAT uM1M1 = lv[mapd(-1, 0,-1, 0)];

    const FLOAT u00u01  = (hzLong1-hzShort)/hzLong1*u00+hzShort/hzLong1*u01;
    const FLOAT u00u0M1 = (hzLong0-hzShort)/hzLong0*u00+hzShort/hzLong0*u0M1;

    const FLOAT uM10uM11  = (hzLong1-hzShort)/hzLong1*uM10+hzShort/hzLong1*uM11;
    const FLOAT uM10uM1M1 = (hzLong0-hzShort)/hzLong0*uM10+hzShort/hzLong0*uM1M1;

    const FLOAT u0 = 0.5*(u00u01+uM10uM11);
    const FLOAT uM1 = 0.5*(u00u0M1+uM10uM1M1);

    return (u0-uM1)/(2.0*hzShort);
}

// first derivative of w-component w.r.t. x-direction, evaluated at the mid of the cell
inline FLOAT dwdx( const FLOAT * const lv, const FLOAT * const lm ) {
    const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hxLong0 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd(-1,0,0,0)]); // distance between center and west v-value
    const FLOAT hxLong1 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd( 1,0,0,0)]); // distance between center and east v-value

    const FLOAT w00   = lv[mapd( 0, 0, 0, 2)];
    const FLOAT w10   = lv[mapd( 1, 0, 0, 2)];
    const FLOAT wM10  = lv[mapd(-1, 0, 0, 2)];

    const FLOAT w0M1  = lv[mapd( 0, 0,-1, 2)];
    const FLOAT w1M1  = lv[mapd( 1, 0,-1, 2)];
    const FLOAT wM1M1 = lv[mapd(-1, 0,-1, 2)];

    const FLOAT w00w10    = (hxLong1-hxShort)/hxLong1*w00+hxShort/hxLong1*w10;
    const FLOAT w00wM10   = (hxLong0-hxShort)/hxLong0*w00+hxShort/hxLong0*wM10;

    const FLOAT w0M1w1M1  = (hxLong1-hxShort)/hxLong1*w0M1+hxShort/hxLong1*w1M1;
    const FLOAT w0M1wM1M1 = (hxLong0-hxShort)/hxLong0*w0M1+hxShort/hxLong0*wM1M1;

    const FLOAT w0 = 0.5*(w00w10+w0M1w1M1);
    const FLOAT wM1 = 0.5*(w00wM10+w0M1wM1M1);

    return (w0-wM1)/(2.0*hxShort);
}

// first derivative of v-component w.r.t. z-direction, evaluated at the mid of the cell
inline FLOAT dvdz( const FLOAT * const lv, const FLOAT * const lm ) {
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong0 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0,-1,2)]); // distance between center and west v-value
    const FLOAT hzLong1 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0, 1,2)]); // distance between center and east v-value

    const FLOAT v00    = lv[mapd( 0, 0, 0, 1)];
    const FLOAT v01    = lv[mapd( 0, 0, 1, 1)];
    const FLOAT v0M1   = lv[mapd( 0, 0,-1, 1)];

    const FLOAT vM10   = lv[mapd( 0,-1, 0, 1)];
    const FLOAT vM11   = lv[mapd( 0,-1, 1, 1)];
    const FLOAT vM1M1  = lv[mapd( 0,-1,-1, 1)];

    const FLOAT v00v01   = (hzLong1-hzShort)/hzLong1*v00+hzShort/hzLong1*v01;
    const FLOAT v00v0M1  = (hzLong0-hzShort)/hzLong0*v00+hzShort/hzLong0*v0M1;

    const FLOAT vM10vM11   = (hzLong1-hzShort)/hzLong1*vM10+hzShort/hzLong1*vM11;
    const FLOAT vM10vM1M1  = (hzLong0-hzShort)/hzLong0*vM10+hzShort/hzLong0*vM1M1;

    const FLOAT v0 = 0.5*(v00v01+vM10vM11);
    const FLOAT vM1 = 0.5*(v00v0M1+vM10vM1M1);

    return (v0-vM1)/(2.0*hzShort);
}

// first derivative of w-component w.r.t. y-direction, evaluated at the mid of the cell
inline FLOAT dwdy( const FLOAT * const lv, const FLOAT * const lm ) {
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong0 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd(0,-1,0,1)]); // distance between center and west v-value
    const FLOAT hyLong1 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,1,0,1)]); // distance between center and east v-value

    const FLOAT w00   = lv[mapd( 0, 0, 0, 2)];
    const FLOAT w10   = lv[mapd( 0, 1, 0, 2)];
    const FLOAT wM10  = lv[mapd( 0,-1, 0, 2)];

    const FLOAT w0M1  = lv[mapd( 0, 0,-1, 2)];
    const FLOAT w1M1  = lv[mapd( 0, 1,-1, 2)];
    const FLOAT wM1M1 = lv[mapd( 0,-1,-1, 2)];

    const FLOAT w00w10    = (hyLong1-hyShort)/hyLong1*w00+hyShort/hyLong1*w10;
    const FLOAT w00wM10   = (hyLong0-hyShort)/hyLong0*w00+hyShort/hyLong0*wM10;

    const FLOAT w0M1w1M1  = (hyLong1-hyShort)/hyLong1*w0M1+hyShort/hyLong1*w1M1;
    const FLOAT w0M1wM1M1 = (hyLong0-hyShort)/hyLong0*w0M1+hyShort/hyLong0*wM1M1;

    const FLOAT w0  = 0.5*(w00w10+w0M1w1M1);
    const FLOAT wM1 = 0.5*(w00wM10+w0M1wM1M1);

    return (w0-wM1)/(2.0*hyShort);
}

// second derivative of u-component w.r.t. x-direction, evaluated at the location of the u-component
inline FLOAT d2udx2 ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv[mapd(1,0,0,0)] - 2*lv[mapd(0,0,0,0)] + lv[mapd(-1,0,0,0)] )
    //    / ( GeometricParameters::dx * GeometricParameters::dx );

    // evaluate the second derivative at the location of the u-component of the velocity field;
    // we therefore use the two neighbouring u-components and assume arbitrary mesh sizes in both
    // directions -> the formula arises from a straight-forward taylor expansion.
    // -> for equal meshsizes, we obtain the usual [1 -2 1]-like stencil
    const int index_M1    = mapd(-1,0,0,0);
    const int index_0     = mapd(0,0,0,0);
    const int index_P1    = mapd(1,0,0,0);

    const FLOAT dx0   = lm[index_0];
    const FLOAT dx1   = lm[index_P1];
    const FLOAT dxSum = dx0+dx1;
    return 2.0*(lv[index_P1]/(dx1*dxSum) - lv[index_0]/(dx1*dx0) + lv[index_M1]/(dx0*dxSum) );

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "d2udx2");}

    return tmp2;*/
}

inline FLOAT d2udy2 ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1=( lv[mapd(0,1,0,0)] - 2*lv[mapd(0,0,0,0)] + lv[mapd(0,-1,0,0)] )
    //    / ( GeometricParameters::dy * GeometricParameters::dy );
    // average mesh sizes, since the component u is located in the middle of the cell's face
    const FLOAT dy_M1 = lm[mapd(0,-1,0,1)];
    const FLOAT dy_0  = lm[mapd(0, 0,0,1)];
    const FLOAT dy_P1 = lm[mapd(0, 1,0,1)];
    const FLOAT dy0 = 0.5*(dy_0+dy_M1);
    const FLOAT dy1 = 0.5*(dy_0+dy_P1);
    const FLOAT dySum = dy0+dy1;
    return 2.0*(lv[mapd(0,1,0,0)]/(dy1*dySum) - lv[mapd(0,0,0,0)]/(dy1*dy0) + lv[mapd(0,-1,0,0)]/(dy0*dySum) );

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "d2udy2");}

    return tmp2;*/
}

inline FLOAT d2udz2 ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv[mapd(0,0,1,0)] - 2*lv[mapd(0,0,0,0)] + lv[mapd(0,0,-1,0)] )
    //    / ( GeometricParameters::dz * GeometricParameters::dz );
    const FLOAT dz_M1 = lm[mapd(0, 0,-1,2)];
    const FLOAT dz_0  = lm[mapd(0, 0, 0,2)];
    const FLOAT dz_P1 = lm[mapd(0, 0 ,1,2)];
    const FLOAT dz0 = 0.5*(dz_0+dz_M1);
    const FLOAT dz1 = 0.5*(dz_0+dz_P1);
    const FLOAT dzSum = dz0+dz1;
    return 2.0*(lv[mapd(0,0,1,0)]/(dz1*dzSum) - lv[mapd(0,0,0,0)]/(dz1*dz0) + lv[mapd(0,0,-1,0)]/(dz0*dzSum) );
    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "d2udz2");}

    return tmp2;*/
}


/** second derivative of the v-component, evaluated at the location of the v-component */
inline FLOAT d2vdx2 ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv[mapd(1,0,0,1)] - 2*lv[mapd(0,0,0,1)] + lv[mapd(-1,0,0,1)] )
    //    / ( GeometricParameters::dx * GeometricParameters::dx );
    const FLOAT dx_M1 = lm[mapd(-1,0,0,0)];
    const FLOAT dx_0  = lm[mapd( 0,0,0,0)];
    const FLOAT dx_P1 = lm[mapd( 1,0,0,0)];
    const FLOAT dx0 = 0.5*(dx_0+dx_M1);
    const FLOAT dx1 = 0.5*(dx_0+dx_P1);
    const FLOAT dxSum = dx0+dx1;
    return 2.0*(lv[mapd(1,0,0,1)]/(dx1*dxSum) - lv[mapd(0,0,0,1)]/(dx1*dx0) + lv[mapd(-1,0,0,1)]/(dx0*dxSum) );

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "d2vdx2");}

    return tmp2;*/
}

inline FLOAT d2vdy2 ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv[mapd(0,1,0,1)] - 2*lv[mapd(0,0,0,1)] + lv[mapd(0,-1,0,1)] )
    //    / ( GeometricParameters::dy * GeometricParameters::dy );
    const int index_M1    = mapd(0,-1,0,1);
    const int index_0     = mapd(0, 0,0,1);
    const int index_P1    = mapd(0, 1,0,1);

    const FLOAT dy0   = lm[index_0];
    const FLOAT dy1   = lm[index_P1];
    const FLOAT dySum = dy0+dy1;
    return 2.0*(lv[index_P1]/(dy1*dySum) - lv[index_0]/(dy1*dy0) + lv[index_M1]/(dy0*dySum) );

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "d2vdy2");}

    return tmp2;*/
}

inline FLOAT d2vdz2 ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv[mapd(0,0,1,1)] - 2*lv[mapd(0,0,0,1)] + lv[mapd(0,0,-1,1)] )
    //    / ( GeometricParameters::dz * GeometricParameters::dz );
    const FLOAT dz_M1 = lm[mapd(0,0,-1,2)];
    const FLOAT dz_0  = lm[mapd(0,0, 0,2)];
    const FLOAT dz_P1 = lm[mapd(0,0, 1,2)];
    const FLOAT dz0 = 0.5*(dz_0+dz_M1);
    const FLOAT dz1 = 0.5*(dz_0+dz_P1);
    const FLOAT dzSum = dz0+dz1;
    return 2.0*(lv[mapd(0,0,1,1)]/(dz1*dzSum) - lv[mapd(0,0,0,1)]/(dz1*dz0) + lv[mapd(0,0,-1,1)]/(dz0*dzSum) );

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "d2vdz2");}

    return tmp2;*/
}


/** second derivative of the w-component, evaluated at the location of the w-component */
inline FLOAT d2wdx2 ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv[mapd(1,0,0,2)] - 2*lv[mapd(0,0,0,2)] + lv[mapd(-1,0,0,2)] )
    //    / ( GeometricParameters::dx * GeometricParameters::dx );
    const FLOAT dx_M1 = lm[mapd(-1,0,0,0)];
    const FLOAT dx_0  = lm[mapd( 0,0,0,0)];
    const FLOAT dx_P1 = lm[mapd( 1,0,0,0)];
    const FLOAT dx0 = 0.5*(dx_0+dx_M1);
    const FLOAT dx1 = 0.5*(dx_0+dx_P1);
    const FLOAT dxSum = dx0+dx1;
    return 2.0*(lv[mapd(1,0,0,2)]/(dx1*dxSum) - lv[mapd(0,0,0,2)]/(dx1*dx0) + lv[mapd(-1,0,0,2)]/(dx0*dxSum) );

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "d2wdx2");}

    return tmp2;*/
}

inline FLOAT d2wdy2 ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv[mapd(0,1,0,2)] - 2*lv[mapd(0,0,0,2)] + lv[mapd(0,-1,0,2)] )
    //    / ( GeometricParameters::dy * GeometricParameters::dy );
    const FLOAT dy_M1 = lm[mapd(0,-1,0,1)];
    const FLOAT dy_0  = lm[mapd(0, 0,0,1)];
    const FLOAT dy_P1 = lm[mapd(0, 1,0,1)];
    const FLOAT dy0 = 0.5*(dy_0+dy_M1);
    const FLOAT dy1 = 0.5*(dy_0+dy_P1);
    const FLOAT dySum = dy0+dy1;
    return 2.0*(lv[mapd(0,1,0,2)]/(dy1*dySum) - lv[mapd(0,0,0,2)]/(dy1*dy0) + lv[mapd(0,-1,0,2)]/(dy0*dySum) );

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "d2wdy2");}

    return tmp2;*/
}

inline FLOAT d2wdz2 ( const FLOAT * const lv, const FLOAT * const lm ) {
    //double tmp1= ( lv[mapd(0,0,1,2)] - 2*lv[mapd(0,0,0,2)] + lv[mapd(0,0,-1,2)] )
    //    / ( GeometricParameters::dz * GeometricParameters::dz );
    const int index_M1    = mapd(0,0,-1,2);
    const int index_0     = mapd(0,0, 0,2);
    const int index_P1    = mapd(0,0, 1,2);

    const FLOAT dz0   = lm[index_0];
    const FLOAT dz1   = lm[index_P1];
    const FLOAT dzSum = dz0+dz1;
    return 2.0*(lv[index_P1]/(dz1*dzSum) - lv[index_0]/(dz1*dz0) + lv[index_M1]/(dz0*dzSum) );

    /*if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "d2wdz2");}

    return tmp2;*/
}


/** first-derivative of product (u*v), evaluated at the location of the v-component */
inline FLOAT duvdx ( const FLOAT * const lv, const Parameters & parameters, const FLOAT * const lm ) {
/*
    const FLOAT tmp1= 1.0 /4.0 * ( ( ( ( lv [mapd(0,0,0,0)] + lv [mapd(0,1,0,0)] ) *
                         ( lv [mapd(0,0,0,1)] + lv [mapd(1,0,0,1)] ) ) -
                       ( ( lv [mapd(-1,0,0,0)] + lv [mapd(-1,1,0,0)] ) *
                         ( lv [mapd(-1,0,0,1)] + lv [mapd(0,0,0,1)] ) ) )
      + parameters.solver.gamma *( ( fabs ( lv [mapd(0,0,0,0)] + lv [mapd(0,1,0,0)] ) *
                              ( lv [mapd(0,0,0,1)] - lv [mapd(1,0,0,1)] ) ) -
                       ( fabs ( lv [mapd(-1,0,0,0)] + lv [mapd(-1,1,0,0)] ) *
                              ( lv [mapd(-1,0,0,1)] - lv [mapd(0,0,0,1)] ) ) )
                       ) / lm[mapd(0,0,0,0)];
*/

    const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hxLong0 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd(-1,0,0,0)]); // distance between center and west v-value
    const FLOAT hxLong1 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd( 1,0,0,0)]); // distance between center and east v-value
    const FLOAT hyShort = 0.5*lm[mapd(0,0,0,1)];                        // distance of center u-value from upper edge of cell
    const FLOAT hyLong  = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd(0,1,0,1)]);  // distance of north and center u-value

    const FLOAT u00  = lv[mapd( 0, 0, 0, 0)];
    const FLOAT u01  = lv[mapd( 0, 1, 0, 0)];
    const FLOAT v00  = lv[mapd( 0, 0, 0, 1)];
    const FLOAT v10  = lv[mapd( 1, 0, 0, 1)];

    const FLOAT uM10 = lv[mapd(-1, 0, 0, 0)];
    const FLOAT uM11 = lv[mapd(-1, 1, 0, 0)];
    const FLOAT vM10 = lv[mapd(-1, 0, 0, 1)];

    // this a central difference expression for the first-derivative. We therefore linearly interpolate u*v onto the surface of the 
    // current cell (in 2D: upper left and upper right corner) and then take the central difference
    const FLOAT secondOrder = (  ((hyLong-hyShort)/hyLong*u00 +hyShort/hyLong*u01) * ((hxLong1-hxShort)/hxLong1*v00+hxShort/hxLong1*v10)
                               - ((hyLong-hyShort)/hyLong*uM10+hyShort/hyLong*uM11) *((hxLong0-hxShort)/hxLong0*v00+hxShort/hxLong0*vM10) )/ (2.0*hxShort);


    // this is a forward-difference in donor-cell style. We apply donor cell and again interpolate the velocity values (u-comp.)
    // onto the surface of the cell. We then apply the standard donor cell scheme. This will, however, result in non-equal
    // mesh spacing evaluations (in case of stretched meshes)
    const FLOAT kr = (hyLong-hyShort)/hyLong*u00 +hyShort/hyLong*u01;
    const FLOAT kl = (hyLong-hyShort)/hyLong*uM10+hyShort/hyLong*uM11;

    const FLOAT firstOrder  = 1.0/(4.0*hxShort)* (
                                kr*(v00+v10) - kl*(vM10+v00) + fabs(kr)*(v00 - v10) - fabs(kl)*(vM10 - v00)
                              );

    // return linear combination of central and donor-cell difference
    const FLOAT tmp2 = (1.0-parameters.solver.gamma)*secondOrder + parameters.solver.gamma*firstOrder;

//    if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1, "Error duv_dx"); }
    return tmp2;
}


/** evaluates first derivative w.r.t. y for u*v at location of u-component. For details on implementation, see duvdx */
inline FLOAT duvdy ( const FLOAT * const lv, const Parameters & parameters, const FLOAT * const lm ) {
/*    const FLOAT tmp1 = 1.0 /4.0 * ( ( ( ( lv [mapd(0,0,0,1)] + lv [mapd(1,0,0,1)] ) *
                         ( lv [mapd(0,0,0,0)] + lv [mapd(0,1,0,0)] ) ) -
                       ( ( lv [mapd(0,-1,0,1)] + lv [mapd(1,-1,0,1)] ) *
                         ( lv [mapd(0,-1,0,0)] + lv [mapd(0,0,0,0)] ) ) ) +
      parameters.solver.gamma * ( ( fabs ( lv [mapd(0,0,0,1)] + lv [mapd(1,0,0,1)] ) *
                              ( lv [mapd(0,0,0,0)] - lv [mapd(0,1,0,0)] ) ) -
                       ( fabs ( lv [mapd(0,-1,0,1)] + lv [mapd(1,-1,0,1)] ) *
                              ( lv [mapd(0,-1,0,0)] - lv [mapd(0,0,0,0)] ) ) ) ) /
                       lm[mapd(0,0,0,1)];
*/
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong0 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,-1,0,1)]); // distance between center and west v-value
    const FLOAT hyLong1 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,1,0,1)]); // distance between center and east v-value
    const FLOAT hxShort = 0.5*lm[mapd(0,0,0,0)];                        // distance of center u-value from upper edge of cell
    const FLOAT hxLong  = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd(1,0,0,0)]);  // distance of north and center u-value

    const FLOAT v00  = lv[mapd( 0, 0, 0, 1)];
    const FLOAT v10  = lv[mapd( 1, 0, 0, 1)];
    const FLOAT u00  = lv[mapd( 0, 0, 0, 0)];
    const FLOAT u01  = lv[mapd( 0, 1, 0, 0)];

    const FLOAT v0M1 = lv[mapd( 0,-1, 0, 1)];
    const FLOAT v1M1 = lv[mapd( 1,-1, 0, 1)];
    const FLOAT u0M1 = lv[mapd( 0,-1, 0, 0)];

    const FLOAT secondOrder = (  ((hxLong-hxShort)/hxLong*v00 +hxShort/hxLong*v10) * ((hyLong1-hyShort)/hyLong1*u00+hyShort/hyLong1*u01)
                               - ((hxLong-hxShort)/hxLong*v0M1+hxShort/hxLong*v1M1) *((hyLong0-hyShort)/hyLong0*u00+hyShort/hyLong0*u0M1) )/ (2.0*hyShort);


    const FLOAT kr = (hxLong-hxShort)/hxLong*v00 +hxShort/hxLong*v10;
    const FLOAT kl = (hxLong-hxShort)/hxLong*v0M1+hxShort/hxLong*v1M1;

    const FLOAT firstOrder  = 1.0/(4.0*hyShort)* (
                                kr*(u00+u01) - kl*(u0M1+u00) + fabs(kr)*(u00 - u01) - fabs(kl)*(u0M1 - u00)
                              );
    const FLOAT tmp2 = (1.0-parameters.solver.gamma)*secondOrder + parameters.solver.gamma*firstOrder;
//    if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1,"Error duvdy"); }
    return tmp2;
}


/** evaluates first derivative w.r.t. x for u*w at location of w-component. For details on implementation, see duvdx */
inline FLOAT duwdx ( const FLOAT * const lv, const Parameters & parameters, const FLOAT * const lm ) {
/*    const FLOAT tmp1 = 1.0 /4.0 * ( ( ( ( lv [mapd(0,0,0,0)] + lv [mapd(0,0,1,0)] ) *
                         ( lv [mapd(0,0,0,2)] + lv [mapd(1,0,0,2)] ) ) -
                       ( ( lv [mapd(-1,0,0,0)] + lv [mapd(-1,0,1,0)] ) *
                         ( lv [mapd(-1,0,0,2)] + lv [mapd(0,0,0,2)] ) ) ) +
      parameters.solver.gamma * ( ( fabs ( lv [mapd(0,0,0,0)] + lv [mapd(0,0,1,0)] ) *
                              ( lv [mapd(0,0,0,2)] - lv [mapd(1,0,0,2)] ) ) -
                       ( fabs ( lv [mapd(-1,0,0,0)] + lv [mapd(-1,0,1,0)] ) *
                              ( lv [mapd(-1,0,0,2)] - lv [mapd(0,0,0,2)] ) ) ) ) /
                       lm[mapd(0,0,0,0)];
*/
    const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hxLong0 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd(-1,0,0,0)]); // distance between center and west v-value
    const FLOAT hxLong1 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd( 1,0,0,0)]); // distance between center and east v-value
    const FLOAT hzShort = 0.5*lm[mapd(0,0,0,2)];                        // distance of center u-value from upper edge of cell
    const FLOAT hzLong  = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd(0,0,1,2)]);  // distance of north and center u-value

    const FLOAT u00  = lv[mapd( 0, 0, 0, 0)];
    const FLOAT u01  = lv[mapd( 0, 0, 1, 0)];
    const FLOAT w00  = lv[mapd( 0, 0, 0, 2)];
    const FLOAT w10  = lv[mapd( 1, 0, 0, 2)];

    const FLOAT uM10 = lv[mapd(-1, 0, 0, 0)];
    const FLOAT uM11 = lv[mapd(-1, 0, 1, 0)];
    const FLOAT wM10 = lv[mapd(-1, 0, 0, 2)];

    const FLOAT secondOrder = (  ((hzLong-hzShort)/hzLong*u00 +hzShort/hzLong*u01) * ((hxLong1-hxShort)/hxLong1*w00+hxShort/hxLong1*w10)
                               - ((hzLong-hzShort)/hzLong*uM10+hzShort/hzLong*uM11) *((hxLong0-hxShort)/hxLong0*w00+hxShort/hxLong0*wM10) )/ (2.0*hxShort);


    const FLOAT kr = (hzLong-hzShort)/hzLong*u00 +hzShort/hzLong*u01;
    const FLOAT kl = (hzLong-hzShort)/hzLong*uM10+hzShort/hzLong*uM11;

    const FLOAT firstOrder  = 1.0/(4.0*hxShort)* (
                                kr*(w00+w10) - kl*(wM10+w00) + fabs(kr)*(w00 - w10) - fabs(kl)*(wM10 - w00)
                              );
    const FLOAT tmp2 = (1.0-parameters.solver.gamma)*secondOrder + parameters.solver.gamma*firstOrder;
//    if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1,"Error duwdx");}
    return tmp2;
}


/** evaluates first derivative w.r.t. z for u*w at location of u-component. For details on implementation, see duvdx */
inline FLOAT duwdz ( const FLOAT * const lv, const Parameters & parameters, const FLOAT * const lm ) {
/*    const FLOAT tmp1= 1.0 /4.0 * ( ( ( ( lv [mapd(0,0,0,2)] + lv [mapd(1,0,0,2)] ) *
                         ( lv [mapd(0,0,0,0)] + lv [mapd(0,0,1,0)] ) ) -
                       ( ( lv [mapd(0,0,-1,2)] + lv [mapd(1,0,-1,2)] ) *
                         ( lv [mapd(0,0,-1,0)] + lv [mapd(0,0,0,0)] ) ) ) +
      parameters.solver.gamma * ( ( fabs ( lv [mapd(0,0,0,2)] + lv [mapd(1,0,0,2)] ) *
                              ( lv [mapd(0,0,0,0)] - lv [mapd(0,0,1,0)] ) ) -
                       ( fabs ( lv [mapd(0,0,-1,2)] + lv [mapd(1,0,-1,2)] ) *
                              ( lv [mapd(0,0,-1,0)] - lv [mapd(0,0,0,0)] ) ) ) ) /
                       lm[mapd(0,0,0,2)];
*/
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong0 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0,-1,2)]); // distance between center and west v-value
    const FLOAT hzLong1 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0, 1,2)]); // distance between center and east v-value
    const FLOAT hxShort = 0.5*lm[mapd(0,0,0,0)];                        // distance of center u-value from upper edge of cell
    const FLOAT hxLong  = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd(1,0,0,0)]);  // distance of north and center u-value

    const FLOAT w00  = lv[mapd( 0, 0, 0, 2)];
    const FLOAT w10  = lv[mapd( 1, 0, 0, 2)];
    const FLOAT u00  = lv[mapd( 0, 0, 0, 0)];
    const FLOAT u01  = lv[mapd( 0, 0, 1, 0)];

    const FLOAT w0M1 = lv[mapd( 0, 0,-1, 2)];
    const FLOAT w1M1 = lv[mapd( 1, 0,-1, 2)];
    const FLOAT u0M1 = lv[mapd( 0, 0,-1, 0)];

    const FLOAT secondOrder = (  ((hxLong-hxShort)/hxLong*w00 +hxShort/hxLong*w10) * ((hzLong1-hzShort)/hzLong1*u00+hzShort/hzLong1*u01)
                               - ((hxLong-hxShort)/hxLong*w0M1+hxShort/hxLong*w1M1) *((hzLong0-hzShort)/hzLong0*u00+hzShort/hzLong0*u0M1) )/ (2.0*hzShort);


    const FLOAT kr = (hxLong-hxShort)/hxLong*w00 +hxShort/hxLong*w10;
    const FLOAT kl = (hxLong-hxShort)/hxLong*w0M1+hxShort/hxLong*w1M1;

    const FLOAT firstOrder  = 1.0/(4.0*hzShort)* (
                                kr*(u00+u01) - kl*(u0M1+u00) + fabs(kr)*(u00 - u01) - fabs(kl)*(u0M1 - u00)
                              );
    const FLOAT tmp2 = (1.0-parameters.solver.gamma)*secondOrder + parameters.solver.gamma*firstOrder;

//    if (fabs(tmp1-tmp2)> 1.0e-12){handleError(1,"duwdz");}
    return tmp2;
}


/** evaluates first derivative w.r.t. y for v*w at location of w-component. For details on implementation, see duvdx */
inline FLOAT dvwdy ( const FLOAT * const lv, const Parameters & parameters,const FLOAT * const lm ) {
/*    const FLOAT tmp1 =  1.0 /4.0 * ( ( ( ( lv [mapd(0,0,0,1)] + lv [mapd(0,0,1,1)] ) *
                         ( lv [mapd(0,0,0,2)] + lv [mapd(0,1,0,2)] ) ) -
                       ( ( lv [mapd(0,-1,0,1)] + lv [mapd(0,-1,1,1)] ) *
                         ( lv [mapd(0,-1,0,2)] + lv [mapd(0,0,0,2)] ) ) ) +
      parameters.solver.gamma * ( ( fabs ( lv [mapd(0,0,0,1)] + lv [mapd(0,0,1,1)] ) *
                              ( lv [mapd(0,0,0,2)] - lv [mapd(0,1,0,2)] ) ) -
                       ( fabs ( lv [mapd(0,-1,0,1)] + lv [mapd(0,-1,1,1)] ) *
                              ( lv [mapd(0,-1,0,2)] - lv [mapd(0,0,0,2)] ) ) ) ) /
                       lm[mapd(0,0,0,1)];
*/
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong0 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd(0,-1,0,1)]); // distance between center and west v-value
    const FLOAT hyLong1 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,1,0,1)]); // distance between center and east v-value
    const FLOAT hzShort = 0.5*lm[mapd(0,0,0,2)];                        // distance of center u-value from upper edge of cell
    const FLOAT hzLong  = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd(0,0,1,2)]);  // distance of north and center u-value

    const FLOAT v00  = lv[mapd( 0, 0, 0, 1)];
    const FLOAT v01  = lv[mapd( 0, 0, 1, 1)];
    const FLOAT w00  = lv[mapd( 0, 0, 0, 2)];
    const FLOAT w10  = lv[mapd( 0, 1, 0, 2)];

    const FLOAT vM10 = lv[mapd( 0,-1, 0, 1)];
    const FLOAT vM11 = lv[mapd( 0,-1, 1, 1)];
    const FLOAT wM10 = lv[mapd( 0,-1, 0, 2)];

    const FLOAT secondOrder = (  ((hzLong-hzShort)/hzLong*v00 +hzShort/hzLong*v01) * ((hyLong1-hyShort)/hyLong1*w00+hyShort/hyLong1*w10)
                               - ((hzLong-hzShort)/hzLong*vM10+hzShort/hzLong*vM11) *((hyLong0-hyShort)/hyLong0*w00+hyShort/hyLong0*wM10) )/ (2.0*hyShort);


    const FLOAT kr = (hzLong-hzShort)/hzLong*v00 +hzShort/hzLong*v01;
    const FLOAT kl = (hzLong-hzShort)/hzLong*vM10+hzShort/hzLong*vM11;

    const FLOAT firstOrder  = 1.0/(4.0*hyShort)* (
                                kr*(w00+w10) - kl*(wM10+w00) + fabs(kr)*(w00 - w10) - fabs(kl)*(wM10 - w00)
                              );
    const FLOAT tmp2 = (1.0-parameters.solver.gamma)*secondOrder + parameters.solver.gamma*firstOrder;
//    if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1,"dvwdy");}
    return tmp2;
}


/** evaluates first derivative w.r.t. z for v*w at location of v-component. For details on implementation, see duvdx */
inline FLOAT dvwdz ( const FLOAT * const lv, const Parameters & parameters, const FLOAT * const lm ) {
/*    const FLOAT tmp1 = 1.0 /4.0 * ( ( ( ( lv [mapd(0,0,0,2)] + lv [mapd(0,1,0,2)] ) *
                         ( lv [mapd(0,0,0,1)] + lv [mapd(0,0,1,1)] ) ) -
                       ( ( lv [mapd(0,0,-1,2)] + lv [mapd(0,1,-1,2)] ) *
                         ( lv [mapd(0,0,-1,1)] + lv [mapd(0,0,0,1)] ) ) ) +
      parameters.solver.gamma * ( ( fabs ( lv [mapd(0,0,0,2)] + lv [mapd(0,1,0,2)] ) *
                              ( lv [mapd(0,0,0,1)] - lv [mapd(0,0,1,1)] ) ) -
                       ( fabs ( lv [mapd(0,0,-1,2)] + lv [mapd(0,1,-1,2)] ) *
                              ( lv [mapd(0,0,-1,1)] - lv [mapd(0,0,0,1)] ) ) ) ) /
                       lm[mapd(0,0,0,2)];
*/
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong0 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0,-1,2)]); // distance between center and west v-value
    const FLOAT hzLong1 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0, 1,2)]); // distance between center and east v-value
    const FLOAT hyShort = 0.5*lm[mapd(0,0,0,1)];                        // distance of center u-value from upper edge of cell
    const FLOAT hyLong  = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd(0,1,0,1)]);  // distance of north and center u-value

    const FLOAT w00  = lv[mapd( 0, 0, 0, 2)];
    const FLOAT w10  = lv[mapd( 0, 1, 0, 2)];
    const FLOAT v00  = lv[mapd( 0, 0, 0, 1)];
    const FLOAT v01  = lv[mapd( 0, 0, 1, 1)];

    const FLOAT w0M1 = lv[mapd( 0, 0,-1, 2)];
    const FLOAT w1M1 = lv[mapd( 0, 1,-1, 2)];
    const FLOAT v0M1 = lv[mapd( 0, 0,-1, 1)];

    const FLOAT secondOrder = (  ((hyLong-hyShort)/hyLong*w00 +hyShort/hyLong*w10) * ((hzLong1-hzShort)/hzLong1*v00+hzShort/hzLong1*v01)
                               - ((hyLong-hyShort)/hyLong*w0M1+hyShort/hyLong*w1M1) *((hzLong0-hzShort)/hzLong0*v00+hzShort/hzLong0*v0M1) )/ (2.0*hzShort);


    const FLOAT kr = (hyLong-hyShort)/hyLong*w00 +hyShort/hyLong*w10;
    const FLOAT kl = (hyLong-hyShort)/hyLong*w0M1+hyShort/hyLong*w1M1;

    const FLOAT firstOrder  = 1.0/(4.0*hzShort)* (
                                kr*(v00+v01) - kl*(v0M1+v00) + fabs(kr)*(v00 - v01) - fabs(kl)*(v0M1 - v00)
                              );
    const FLOAT tmp2 = (1.0-parameters.solver.gamma)*secondOrder + parameters.solver.gamma*firstOrder;
//    if (fabs(tmp1-tmp2) > 1.0e-12){std::cout << tmp1 << ", " << tmp2 << std::endl;handleError(1,"dvwdz");}
    return tmp2;
}


/** first derivative of u*u w.r.t. x, evaluated at location of u-component. */
inline FLOAT du2dx ( const FLOAT * const lv, const Parameters & parameters, const FLOAT * const lm ) {
/*    const FLOAT tmp1 = 1.0 /4.0 * ( ( ( ( lv [mapd(0,0,0,0)] + lv [mapd(1,0,0,0)] ) *
                         ( lv [mapd(0,0,0,0)] + lv [mapd(1,0,0,0)] ) ) -
                       ( ( lv [mapd(-1,0,0,0)] + lv [mapd(0,0,0,0)] ) *
                         ( lv [mapd(-1,0,0,0)] + lv [mapd(0,0,0,0)] ) ) ) +
      parameters.solver.gamma * ( ( fabs ( lv [mapd(0,0,0,0)] + lv [mapd(1,0,0,0)] ) *
                              ( lv [mapd(0,0,0,0)] - lv [mapd(1,0,0,0)] ) ) -
                       ( fabs ( lv [mapd(-1,0,0,0)] + lv [mapd(0,0,0,0)] ) *
                              ( lv [mapd(-1,0,0,0)] - lv [mapd(0,0,0,0)] ) ) ) ) /
                       lm[mapd(0,0,0,0)];
*/
    const FLOAT dxShort = 0.5*lm[mapd(0,0,0,0)];
    const FLOAT dxLong0 = 0.5*(lm[mapd(-1,0,0,0)] + lm[mapd(0,0,0,0)]);
    const FLOAT dxLong1 = 0.5*(lm[mapd( 0,0,0,0)] + lm[mapd(1,0,0,0)]);

    const FLOAT u0 = lv[mapd(0,0,0,0)];
    const FLOAT uM1= lv[mapd(-1,0,0,0)];
    const FLOAT u1 = lv[mapd(1,0,0,0)];

    const FLOAT kr = (dxLong1-dxShort)/dxLong1*u0 + dxShort/dxLong1*u1;
    const FLOAT kl = (dxLong0-dxShort)/dxLong0*u0 + dxShort/dxLong0*uM1;

    // central difference expression which is second-order accurate for uniform meshes. We interpolate u half-way between
    // neighboured u-component values and afterwards build the central difference for u*u
    const FLOAT secondOrder = (  ((dxLong1-dxShort)/dxLong1*u0 + dxShort/dxLong1*u1 )*((dxLong1-dxShort)/dxLong1*u0 + dxShort/dxLong1*u1 )
                               - ((dxLong0-dxShort)/dxLong0*u0 + dxShort/dxLong0*uM1)*((dxLong0-dxShort)/dxLong0*u0 + dxShort/dxLong0*uM1)
                              )/(2.0*dxShort);

    // donor-cell like derivative expression. We evaluate u half-way between neighboured u-components and use this as a prediction
    // of the transport direction
    const FLOAT firstOrder = 1.0/(4.0*dxShort)* (
                               kr*(u0+u1) - kl*(uM1+u0) + fabs(kr)*(u0 - u1) - fabs(kl)*(uM1 - u0)
                             );

    // return linear combination of central- and upwind difference
    const FLOAT tmp2 = (1.0-parameters.solver.gamma)*secondOrder + parameters.solver.gamma*firstOrder;
//    if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1,"du2dx");}
    return tmp2;
}


/** first derivative of v*v w.r.t. y, evaluated at location of v-component; for details, see du2dx */
inline FLOAT dv2dy ( const FLOAT * const lv, const Parameters & parameters, const FLOAT* const lm ) {
/*    const FLOAT tmp1= 1.0 /4.0 * ( ( ( ( lv [mapd(0,0,0,1)] + lv [mapd(0,1,0,1)] ) *
                         ( lv [mapd(0,0,0,1)] + lv [mapd(0,1,0,1)] ) ) -
                       ( ( lv [mapd(0,-1,0,1)] + lv [mapd(0,0,0,1)] ) *
                         ( lv [mapd(0,-1,0,1)] + lv [mapd(0,0,0,1)] ) ) ) +
      parameters.solver.gamma * ( ( fabs ( lv [mapd(0,0,0,1)] + lv [mapd(0,1,0,1)] ) *
                              ( lv [mapd(0,0,0,1)] - lv [mapd(0,1,0,1)] ) ) -
                       ( fabs ( lv [mapd(0,-1,0,1)] + lv [mapd(0,0,0,1)] ) *
                              ( lv [mapd(0,-1,0,1)] - lv [mapd(0,0,0,1)] ) ) ) ) /
                       lm[mapd(0,0,0,1)];
*/
    const FLOAT dyShort = 0.5*lm[mapd(0,0,0,1)];
    const FLOAT dyLong0 = 0.5*(lm[mapd(0,-1,0,1)] + lm[mapd(0,0,0,1)]);
    const FLOAT dyLong1 = 0.5*(lm[mapd( 0,0,0,1)] + lm[mapd(0,1,0,1)]);

    const FLOAT v0 = lv[mapd(0,0,0,1)];
    const FLOAT vM1= lv[mapd(0,-1,0,1)];
    const FLOAT v1 = lv[mapd(0,1,0,1)];

    const FLOAT kr = (dyLong1-dyShort)/dyLong1*v0 + dyShort/dyLong1*v1;
    const FLOAT kl = (dyLong0-dyShort)/dyLong0*v0 + dyShort/dyLong0*vM1;

    const FLOAT secondOrder = (  ((dyLong1-dyShort)/dyLong1*v0 + dyShort/dyLong1*v1 )*((dyLong1-dyShort)/dyLong1*v0 + dyShort/dyLong1*v1 )
                               - ((dyLong0-dyShort)/dyLong0*v0 + dyShort/dyLong0*vM1)*((dyLong0-dyShort)/dyLong0*v0 + dyShort/dyLong0*vM1)
                              )/(2.0*dyShort);
    const FLOAT firstOrder = 1.0/(4.0*dyShort)* (
                               kr*(v0+v1) - kl*(vM1+v0) + fabs(kr)*(v0 - v1) - fabs(kl)*(vM1 - v0)
                             );
    const FLOAT tmp2 = (1.0-parameters.solver.gamma)*secondOrder + parameters.solver.gamma*firstOrder;
//    if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1,"dv2dy");}
    return tmp2;
}


/** first derivative of w*w w.r.t. z, evaluated at location of w-component; for details, see du2dx */
inline FLOAT dw2dz ( const FLOAT * const lv, const Parameters & parameters, const FLOAT* const lm ) {
/*    const FLOAT tmp1= 1.0 /4.0 * ( ( ( ( lv [mapd(0,0,0,2)] + lv [mapd(0,0,1,2)] ) *
                         ( lv [mapd(0,0,0,2)] + lv [mapd(0,0,1,2)] ) ) -
                       ( ( lv [mapd(0,0,-1,2)] + lv [mapd(0,0,0,2)] ) *
                         ( lv [mapd(0,0,-1,2)] + lv [mapd(0,0,0,2)] ) ) ) +
      parameters.solver.gamma * ( ( fabs ( lv [mapd(0,0,0,2)] + lv [mapd(0,0,1,2)] ) *
                              ( lv [mapd(0,0,0,2)] - lv [mapd(0,0,1,2)] ) ) -
                       ( fabs ( lv [mapd(0,0,-1,2)] + lv [mapd(0,0,0,2)] ) *
                              ( lv [mapd(0,0,-1,2)] - lv [mapd(0,0,0,2)] ) ) ) ) /
                       lm[mapd(0,0,0,2)];
*/
    const FLOAT dzShort = 0.5*lm[mapd(0,0,0,2)];
    const FLOAT dzLong0 = 0.5*(lm[mapd(0,0,-1,2)] + lm[mapd(0,0,0,2)]);
    const FLOAT dzLong1 = 0.5*(lm[mapd( 0,0,0,2)] + lm[mapd(0,0,1,2)]);

    const FLOAT w0 = lv[mapd(0,0,0,2)];
    const FLOAT wM1= lv[mapd(0,0,-1,2)];
    const FLOAT w1 = lv[mapd(0,0,1,2)];

    const FLOAT kr = (dzLong1-dzShort)/dzLong1*w0 + dzShort/dzLong1*w1;
    const FLOAT kl = (dzLong0-dzShort)/dzLong0*w0 + dzShort/dzLong0*wM1;

    const FLOAT secondOrder = (  ((dzLong1-dzShort)/dzLong1*w0 + dzShort/dzLong1*w1 )*((dzLong1-dzShort)/dzLong1*w0 + dzShort/dzLong1*w1 )
                               - ((dzLong0-dzShort)/dzLong0*w0 + dzShort/dzLong0*wM1)*((dzLong0-dzShort)/dzLong0*w0 + dzShort/dzLong0*wM1)
                              )/(2.0*dzShort);
    const FLOAT firstOrder = 1.0/(4.0*dzShort)* (
                               kr*(w0+w1) - kl*(wM1+w0) + fabs(kr)*(w0 - w1) - fabs(kl)*(wM1 - w0)
                             );
    const FLOAT tmp2 = (1.0-parameters.solver.gamma)*secondOrder + parameters.solver.gamma*firstOrder;
//    if (fabs(tmp1-tmp2) > 1.0e-12){handleError(1,"dw2dz");}
    return tmp2;
}

//helper functions for diffusive Part of NSE

inline FLOAT nuiPhalbjPhalb ( const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong1 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,1,0,1)]); // distance between center and east v-value
    const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hxLong1 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd( 1,0,0,0)]); // distance between center and east v-value

    const FLOAT lvis00 = lvis[mapd(0,0,0,0)];
    const FLOAT lvis01 = lvis[mapd(0,1,0,0)];
    const FLOAT lvis10 = lvis[mapd(1,0,0,0)];
    const FLOAT lvis11 = lvis[mapd(1,1,0,0)];

    const FLOAT lvis00lvis01  = (hyLong1-hyShort)/hyLong1*lvis00+hyShort/hyLong1*lvis01;
    const FLOAT lvis10lvis11  = (hyLong1-hyShort)/hyLong1*lvis10+hyShort/hyLong1*lvis11;

    return  (hxLong1-hxShort)/hxLong1*lvis00lvis01+hxShort/hxLong1*lvis10lvis11;

}

inline FLOAT nuiPhalbjMhalb ( const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong0 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,-1,0,1)]); // distance between center and west v-value
    const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hxLong1 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd( 1,0,0,0)]); // distance between center and east v-value

    const FLOAT lvis00  = lvis[mapd(0, 0,0,0)];
    const FLOAT lvis0M1 = lvis[mapd(0,-1,0,0)];
    const FLOAT lvis10  = lvis[mapd(1, 0,0,0)];
    const FLOAT lvis1M1 = lvis[mapd(1,-1,0,0)];
    
    const FLOAT lvis00lvis0M1  = (hyLong0-hyShort)/hyLong0*lvis00+hyShort/hyLong0*lvis0M1;
    const FLOAT lvis10lvis1M1  = (hyLong0-hyShort)/hyLong0*lvis10+hyShort/hyLong0*lvis1M1;
    
    return (hxLong1-hxShort)/hxLong1*lvis00lvis0M1+hxShort/hxLong1*lvis10lvis1M1;
}

inline FLOAT nuiPhalbkPhalb ( const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong1 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0, 1,2)]); // distance between center and east v-value
    const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hxLong1 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd( 1,0,0,0)]); // distance between center and east v-value
 
    const FLOAT lvis00 = lvis[mapd(0,0,0,0)];
    const FLOAT lvis01 = lvis[mapd(0,0,1,0)];
    const FLOAT lvis10 = lvis[mapd(1,0,0,0)];
    const FLOAT lvis11 = lvis[mapd(1,0,1,0)];
    
    const FLOAT lvis00lvis01 = (hzLong1-hzShort)/hzLong1*lvis00+hzShort/hzLong1*lvis01;
    const FLOAT lvis10lvis11 = (hzLong1-hzShort)/hzLong1*lvis10+hzShort/hzLong1*lvis11;
    
    return (hxLong1-hxShort)/hxLong1*lvis00lvis01+hxShort/hxLong1*lvis10lvis11;
}

inline FLOAT nuiPhalbkMhalb ( const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong0 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0,-1,2)]); // distance between center and west v-value
    const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hxLong1 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd( 1,0,0,0)]); // distance between center and east v-value
  
    const FLOAT lvis00  = lvis[mapd(0, 0,0,0)];
    const FLOAT lvis0M1 = lvis[mapd(0,0,-1,0)];
    const FLOAT lvis10  = lvis[mapd(1, 0,0,0)];
    const FLOAT lvis1M1 = lvis[mapd(1,0,-1,0)];
    
    const FLOAT lvis00lvis0M1 = (hzLong0-hzShort)/hzLong0*lvis00+hzShort/hzLong0*lvis0M1;
    const FLOAT lvis10lvis1M1 = (hzLong0-hzShort)/hzLong0*lvis10+hzShort/hzLong0*lvis1M1;
    
    return (hxLong1-hxShort)/hxLong1*lvis00lvis0M1+hxShort/hxLong1*lvis10lvis1M1;
}

inline FLOAT nuiMhalbjPhalb ( const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong1 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,1,0,1)]); // distance between center and east v-value
    const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hxLong0 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd(-1,0,0,0)]); // distance between center and east v-value

    const FLOAT lvis00  = lvis[mapd( 0,0,0,0)];
    const FLOAT lvis01  = lvis[mapd( 0,1,0,0)];
    const FLOAT lvisM10 = lvis[mapd(-1,0,0,0)];
    const FLOAT lvisM11 = lvis[mapd(-1,1,0,0)];
    
    const FLOAT lvis00lvis01  = (hyLong1-hyShort)/hyLong1*lvis00+hyShort/hyLong1*lvis01;
    const FLOAT lvisM10lvisM11  = (hyLong1-hyShort)/hyLong1*lvisM10+hyShort/hyLong1*lvisM11;
    
    return (hxLong0-hxShort)/hxLong0*lvis00lvis01+hxShort/hxLong0*lvisM10lvisM11;
}

inline FLOAT nujPhalbkPhalb ( const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong1 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0, 1,2)]); // distance between center and east v-value
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong1 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,1,0,1)]); // distance between center and east v-value

    const FLOAT lvis00 = lvis[mapd(0,0,0,0)];
    const FLOAT lvis01 = lvis[mapd(0,0,1,0)];
    const FLOAT lvis10 = lvis[mapd(0,1,0,0)];
    const FLOAT lvis11 = lvis[mapd(0,1,1,0)];
    
    const FLOAT lvis00lvis01 = (hzLong1-hzShort)/hzLong1*lvis00+hzShort/hzLong1*lvis01;
    const FLOAT lvis10lvis11 = (hzLong1-hzShort)/hzLong1*lvis10+hzShort/hzLong1*lvis11;
 
    return (hyLong1-hyShort)/hyLong1*lvis00lvis01+hyShort/hyLong1*lvis10lvis11;
}

inline FLOAT nujPhalbkMhalb ( const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong0 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0,0,-1,2)]); // distance between center and east v-value
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong1 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,1,0,1)]); // distance between center and east v-value

    const FLOAT lvis00  = lvis[mapd(0,0, 0,0)];
    const FLOAT lvis0M1 = lvis[mapd(0,0,-1,0)];
    const FLOAT lvis10  = lvis[mapd(0,1, 0,0)];
    const FLOAT lvis1M1 = lvis[mapd(0,1,-1,0)];
        
    const FLOAT lvis00lvis0M1 = (hzLong0-hzShort)/hzLong0*lvis00+hzShort/hzLong0*lvis0M1;
    const FLOAT lvis10lvis1M1 = (hzLong0-hzShort)/hzLong0*lvis10+hzShort/hzLong0*lvis1M1;
    
    return (hyLong1-hyShort)/hyLong1*lvis00lvis0M1+hyShort/hyLong1*lvis10lvis1M1;
}

inline FLOAT nujMhalbkPhalb ( const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong1 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0, 0, 1,2)]); // distance between center and east v-value
    const FLOAT hyShort = 0.5*lm[mapd( 0,0,0,1)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hyLong0 = 0.5*(lm[mapd(0,0,0,1)] + lm[mapd( 0,-1,0,1)]); // distance between center and east v-value

    const FLOAT lvis00  = lvis[mapd(0, 0,0,0)];
    const FLOAT lvis01  = lvis[mapd(0, 0,1,0)];
    const FLOAT lvisM10 = lvis[mapd(0,-1,0,0)];
    const FLOAT lvisM11 = lvis[mapd(0,-1,1,0)];
    
    const FLOAT lvis00lvis01 = (hzLong1-hzShort)/hzLong1*lvis00+hzShort/hzLong1*lvis01;
    const FLOAT lvisM10lvisM11 = (hzLong1-hzShort)/hzLong1*lvisM10+hzShort/hzLong1*lvisM11;
    
    return (hyLong0-hyShort)/hyLong0*lvis00lvis01+hyShort/hyLong0*lvisM10lvisM11;
}

inline FLOAT nuiMhalbkPhalb ( const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT hzShort = 0.5*lm[mapd( 0,0,0,2)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hzLong1 = 0.5*(lm[mapd(0,0,0,2)] + lm[mapd( 0, 0, 1,2)]); // distance between center and east v-value
    const FLOAT hxShort = 0.5*lm[mapd( 0,0,0,0)];                       // distance of corner points in x-direction from center v-value
    const FLOAT hxLong0 = 0.5*(lm[mapd(0,0,0,0)] + lm[mapd(-1,0,0,0)]); // distance between center and east v-value

    const FLOAT lvis00  = lvis[mapd( 0,0,0,0)];
    const FLOAT lvis01  = lvis[mapd( 0,0,1,0)];
    const FLOAT lvisM10 = lvis[mapd(-1,0,0,0)];
    const FLOAT lvisM11 = lvis[mapd(-1,0,1,0)];
    
    const FLOAT lvis00lvis01 = (hzLong1-hzShort)/hzLong1*lvis00+hzShort/hzLong1*lvis01;
    const FLOAT lvisM10lvisM11 = (hzLong1-hzShort)/hzLong1*lvisM10+hzShort/hzLong1*lvisM11;
       
    return (hxLong0-hxShort)/hxLong0*lvis00lvis01+hxShort/hxLong0*lvisM10lvisM11;
}

// New components for the diffusive Part of the Navier-Stokes-Equation
// d/dx(nu*du/dx)
inline FLOAT comp11 ( const FLOAT * const lv, const FLOAT * const lm, const FLOAT * const lvis, const Parameters & parameters ) {
    const int index_M1    = mapd(-1,0,0,0);
    const int index_0     = mapd(0,0,0,0);
    const int index_P1    = mapd(1,0,0,0);

    const FLOAT dx0   = lm[index_0];
    const FLOAT dx1   = lm[index_P1];
    const FLOAT dxSum = dx0+dx1;
    return 2.0*( ((lvis[index_P1] + 1 / parameters.flow.Re) * (lv[index_P1] - lv[index_0])) / (dxSum*dx1) - 
		 ((lvis[index_0] + 1 / parameters.flow.Re) * (lv[index_0] - lv[index_M1])) / (dxSum*dx0) );
}

// d/dy(nu*dv/dy)
inline FLOAT comp22 ( const FLOAT * const lv, const FLOAT * const lm, const FLOAT * const lvis, const Parameters & parameters ) {
    const int index_M1    = mapd(0,-1,0,1);
    const int index_0     = mapd(0, 0,0,1);
    const int index_P1    = mapd(0, 1,0,1);

    const FLOAT dy0   = lm[index_0];
    const FLOAT dy1   = lm[index_P1];
    const FLOAT dySum = dy0+dy1;
    return 2.0*( ((lvis[index_P1] + 1 / parameters.flow.Re) * (lv[index_P1] - lv[index_0])) / (dySum*dy1) - 
		 ((lvis[index_0] + 1 / parameters.flow.Re) * (lv[index_0] - lv[index_M1])) / (dySum*dy0) );
}

// d/dy(nu*dw/dz)
inline FLOAT comp33 ( const FLOAT * const lv, const FLOAT * const lm, const FLOAT * const lvis, const Parameters & parameters ) {
    const int index_M1    = mapd(0,0,-1,2);
    const int index_0     = mapd(0,0, 0,2);
    const int index_P1    = mapd(0,0, 1,2);

    const FLOAT dz0   = lm[index_0];
    const FLOAT dz1   = lm[index_P1];
    const FLOAT dzSum = dz0+dz1;
    return 2.0*( ((lvis[index_P1] + 1 / parameters.flow.Re) * (lv[index_P1] - lv[index_0])) / (dzSum*dz1) - 
		 ((lvis[index_0] + 1 / parameters.flow.Re) * (lv[index_0] - lv[index_M1])) / (dzSum*dz0) );
}

// d/dy(nu*(du/dy+dv/dx))
inline FLOAT comp12 ( const FLOAT * const lv, const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT v00   = lv[mapd(0, 0,0,1)];
    const FLOAT v10   = lv[mapd(1, 0,0,1)];
    const FLOAT v0M1  = lv[mapd(0,-1,0,1)];
    const FLOAT v1M1  = lv[mapd(1,-1,0,1)];
  
    const FLOAT u0M1  = lv[mapd(0,-1,0,0)];
    const FLOAT u00   = lv[mapd(0, 0,0,0)];
    const FLOAT u01   = lv[mapd(0, 1,0,0)];
    
    const FLOAT dy01  = lm[mapd(0, 1,0,1)];
    const FLOAT dy00  = lm[mapd(0, 0,0,1)];
    const FLOAT dy0M1 = lm[mapd(0,-1,0,1)];
    
    const FLOAT dx00  = lm[mapd(0, 0,0,0)];
    const FLOAT dx10  = lm[mapd(1, 0,0,0)];
    
    const FLOAT dy1 = 0.5*(dy01+dy00);
    const FLOAT dy0 = 0.5*(dy00+dy0M1);
    
    const FLOAT dx0 = 0.5*(dx10+dx00);
    
    return (1 / dy00)*( nuiPhalbjPhalb(lm, lvis)*((u01-u00)/dy1+(v10-v00)/dx0) - nuiPhalbjMhalb(lm, lvis)*((u00-u0M1)/dy0+(v1M1-v0M1)/dx0));
}

// d/dz(nu*(du/dz+dw/dx))
inline FLOAT comp13 ( const FLOAT * const lv, const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT w00   = lv[mapd(0,0, 0,2)];
    const FLOAT w10   = lv[mapd(1,0, 0,2)];
    const FLOAT w0M1  = lv[mapd(0,0,-1,2)];
    const FLOAT w1M1  = lv[mapd(1,0,-1,2)];
    
    const FLOAT u0M1  = lv[mapd(0,0,-1,0)];
    const FLOAT u00   = lv[mapd(0,0, 0,0)];
    const FLOAT u01   = lv[mapd(0,0, 1,0)];
    
    const FLOAT dz01  = lm[mapd(0,0, 1,2)];
    const FLOAT dz00  = lm[mapd(0,0, 0,2)];
    const FLOAT dz0M1 = lm[mapd(0,0,-1,2)];
    
    const FLOAT dx00  = lm[mapd(0, 0,0,0)];
    const FLOAT dx10  = lm[mapd(1, 0,0,0)];
    
    const FLOAT dz1 = 0.5*(dz01+dz00);
    const FLOAT dz0 = 0.5*(dz00+dz0M1);
    const FLOAT dx0 = 0.5*(dx10+dx00);
    return (1 / dz00)*( nuiPhalbkPhalb(lm, lvis)*((u01-u00)/dz1+(w10-w00)/dx0) - nuiPhalbkMhalb(lm, lvis)*((u00-u0M1)/dz0+(w1M1-w0M1)/dx0));
}

// d/dx(nu*(dv/dx+du/dy))
inline FLOAT comp21 ( const FLOAT * const lv, const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT u00   = lv[mapd( 0,0,0,0)];
    const FLOAT u01   = lv[mapd( 0,1,0,0)];
    const FLOAT uM10  = lv[mapd(-1,0,0,0)];
    const FLOAT uM11  = lv[mapd(-1,1,0,0)];
    
    const FLOAT v00   = lv[mapd( 0,0,0,1)];
    const FLOAT v10   = lv[mapd( 1,0,0,1)];
    const FLOAT vM10  = lv[mapd(-1,0,0,1)];
    
    const FLOAT dx10  = lm[mapd( 1,0,0,0)];
    const FLOAT dx00  = lm[mapd( 0,0,0,0)];
    const FLOAT dxM10 = lm[mapd(-1,0,0,0)];
    
    const FLOAT dy00  = lm[mapd( 0,0,0,1)];
    const FLOAT dy01  = lm[mapd( 0,1,0,1)];
    
    const FLOAT dx1 = 0.5*(dx10+dx00);
    const FLOAT dx0 = 0.5*(dx00+dxM10);
    
    const FLOAT dy0 = 0.5*(dy00+dy01);
    
    return (1 / dx00)*( nuiPhalbjPhalb(lm, lvis)*((v10-v00)/dx1+(u01-u00)/dy0) - nuiMhalbjPhalb(lm, lvis)*((v00-vM10)/dx0+(uM11-uM10)/dy0));
}


// d/dz(nu*(dv/dz+dw/dy)) similar to comp12
inline FLOAT comp23 ( const FLOAT * const lv, const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT w00   = lv[mapd(0,0, 0,2)];
    const FLOAT w10   = lv[mapd(0,1, 0,2)];
    const FLOAT w0M1  = lv[mapd(0,0,-1,2)];
    const FLOAT w1M1  = lv[mapd(0,1,-1,2)];
    
    const FLOAT v0M1  = lv[mapd(0,0,-1,1)];
    const FLOAT v00   = lv[mapd(0,0, 0,1)];
    const FLOAT v01   = lv[mapd(0,0, 1,1)];
    
    const FLOAT dz01  = lm[mapd(0,0, 1,2)];
    const FLOAT dz00  = lm[mapd(0,0, 0,2)];
    const FLOAT dz0M1 = lm[mapd(0,0,-1,2)];
    
    const FLOAT dy00  = lm[mapd(0,0,0,1)];
    const FLOAT dy10  = lm[mapd(0,1,0,1)];
    
    const FLOAT dz1  = 0.5*(dz01+dz00);
    const FLOAT dz0  = 0.5*(dz00+dz0M1);
    
    const FLOAT dy0  = 0.5*(dy00+dy10);
    
    return (1 / dz00)*( nujPhalbkPhalb(lm, lvis)*((v01-v00)/dz1+(w10-w00)/dy0) - nujPhalbkMhalb(lm, lvis)*((v00-v0M1)/dz0+(w1M1-w0M1)/dy0));

}

// d/dy(nu*(dw/dy+dv/dz)) implementation is similar to comp21
inline FLOAT comp32 ( const FLOAT * const lv, const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT v00   = lv[mapd(0, 0,0,1)];
    const FLOAT v01   = lv[mapd(0, 0,1,1)];
    const FLOAT vM10  = lv[mapd(0,-1,0,1)];
    const FLOAT vM11  = lv[mapd(0,-1,1,1)];
    
    const FLOAT w00   = lv[mapd(0, 0,0,2)];
    const FLOAT w10   = lv[mapd(0, 1,0,2)];
    const FLOAT wM10  = lv[mapd(0,-1,0,2)];
    
    const FLOAT dy10  = lm[mapd(0, 1,0,1)];
    const FLOAT dy00  = lm[mapd(0, 0,0,1)];
    const FLOAT dyM10 = lm[mapd(0,-1,0,1)];
    
    const FLOAT dz00  = lm[mapd(0, 0,0,2)];
    const FLOAT dz01  = lm[mapd(0, 0,1,2)];
    
    const FLOAT dy1   = 0.5*(dy10+dy00);
    const FLOAT dy0   = 0.5*(dy00+dyM10);
    
    const FLOAT dz0   = 0.5*(dz00+dz01);
    
    return (1 / dy00)*( nujPhalbkPhalb(lm, lvis)*((w10-w00)/dy1+(v01-v00)/dz0) - nujMhalbkPhalb(lm, lvis)*((w00-wM10)/dy0 + (vM11-vM10)/dz0));

}

// d/dx(nu*(dw/dx+du/dz)) similar to comp21
inline FLOAT comp31 ( const FLOAT * const lv, const FLOAT * const lm, const FLOAT * const lvis ) {
    const FLOAT u00   = lv[mapd( 0,0,0,0)];
    const FLOAT u01   = lv[mapd( 0,0,1,0)];
    const FLOAT uM10  = lv[mapd(-1,0,0,0)];
    const FLOAT uM11  = lv[mapd(-1,0,1,0)];
    
    const FLOAT w00   = lv[mapd( 0,0,0,2)];
    const FLOAT w10   = lv[mapd( 1,0,0,2)];
    const FLOAT wM10  = lv[mapd(-1,0,0,2)];
    
    const FLOAT dx10  = lm[mapd( 1,0,0,0)];
    const FLOAT dx00  = lm[mapd( 0,0,0,0)];
    const FLOAT dxM10 = lm[mapd(-1,0,0,0)];
    
    const FLOAT dz00  = lm[mapd( 0,0,0,2)];
    const FLOAT dz01  = lm[mapd( 0,0,1,2)];
    
    const FLOAT dx1 = 0.5*(dx10+dx00);
    const FLOAT dx0 = 0.5*(dx00+dxM10);
    
    const FLOAT dz0 = 0.5*(dz00+dz01);
    
    return (1 / dx00)*( nuiPhalbkPhalb(lm, lvis)*((w10-w00)/dx1+(u01-u00)/dz0) - nuiMhalbkPhalb(lm, lvis)*((w00-wM10)/dx0+(uM11-uM10)/dz0));
}


inline FLOAT computeF2D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,0)]
        + dt * ( 1 / parameters.flow.Re * ( d2udx2 ( localVelocity, localMeshsize )
                    + d2udy2(localVelocity, localMeshsize)) - du2dx (localVelocity, parameters, localMeshsize)
                    - duvdy (localVelocity, parameters, localMeshsize) + parameters.environment.gx);
}

inline FLOAT computeF2D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const FLOAT * const localViscosity, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,0)]
		+ dt * ( 2.0*comp11 ( localVelocity, localMeshsize, localViscosity, parameters )
		+ comp12 ( localVelocity, localMeshsize, localViscosity )
		- du2dx ( localVelocity, parameters, localMeshsize )
		- duvdy ( localVelocity, parameters, localMeshsize )
        + parameters.environment.gx );
}

inline FLOAT computeG2D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,1)] 
        + dt * ( 1 / parameters.flow.Re * ( d2vdx2 ( localVelocity, localMeshsize )
                    + d2vdy2(localVelocity, localMeshsize)) - duvdx (localVelocity, parameters, localMeshsize)
                    - dv2dy (localVelocity, parameters, localMeshsize) + parameters.environment.gy);
}

inline FLOAT computeG2D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const FLOAT * const localViscosity, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,1)]
		+ dt * ( comp21 ( localVelocity, localMeshsize, localViscosity )
		+ 2.0*comp22 ( localVelocity, localMeshsize, localViscosity, parameters )
        - dv2dy ( localVelocity, parameters, localMeshsize )
		- duvdx ( localVelocity, parameters, localMeshsize )
		+ parameters.environment.gy );
}

inline FLOAT computeF3D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,0)]
                +  dt * ( 1 / parameters.flow.Re * ( d2udx2 ( localVelocity, localMeshsize )
                + d2udy2 ( localVelocity, localMeshsize ) + d2udz2 ( localVelocity, localMeshsize ) )
                - du2dx ( localVelocity, parameters, localMeshsize ) - duvdy ( localVelocity, parameters, localMeshsize )
                - duwdz ( localVelocity, parameters, localMeshsize ) + parameters.environment.gx );
}

inline FLOAT computeF3D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const FLOAT * const localViscosity, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,0)]
		+ dt * ( 2.0*comp11 ( localVelocity, localMeshsize, localViscosity, parameters )
		+ comp12 ( localVelocity, localMeshsize, localViscosity )
		+ comp13 ( localVelocity, localMeshsize, localViscosity )
		- du2dx ( localVelocity, parameters, localMeshsize ) 
		- duvdy ( localVelocity, parameters, localMeshsize )
                - duwdz ( localVelocity, parameters, localMeshsize ) + parameters.environment.gx );
}


inline FLOAT computeG3D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,1)]
                +  dt * ( 1 / parameters.flow.Re * ( d2vdx2 ( localVelocity, localMeshsize )
                + d2vdy2 ( localVelocity, localMeshsize ) + d2vdz2 ( localVelocity, localMeshsize ) )
                - dv2dy ( localVelocity, parameters, localMeshsize ) - duvdx ( localVelocity, parameters, localMeshsize )
                - dvwdz ( localVelocity, parameters, localMeshsize ) + parameters.environment.gy );
}

inline FLOAT computeG3D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const FLOAT * const localViscosity, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,1)]
		+ dt * ( comp21 ( localVelocity, localMeshsize, localViscosity ) 
		+ 2.0*comp22 ( localVelocity, localMeshsize, localViscosity, parameters )
		+ comp23 ( localVelocity, localMeshsize, localViscosity )
                - dv2dy ( localVelocity, parameters, localMeshsize )
		- duvdx ( localVelocity, parameters, localMeshsize )
                - dvwdz ( localVelocity, parameters, localMeshsize ) + parameters.environment.gy );
}


inline FLOAT computeH3D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,2)]
                +  dt * ( 1 / parameters.flow.Re * ( d2wdx2 ( localVelocity, localMeshsize )
                + d2wdy2 ( localVelocity, localMeshsize ) + d2wdz2 ( localVelocity, localMeshsize ) )
                - dw2dz ( localVelocity, parameters, localMeshsize ) - duwdx ( localVelocity, parameters, localMeshsize )
                - dvwdy ( localVelocity, parameters, localMeshsize ) + parameters.environment.gz );
}

inline FLOAT computeH3D(const FLOAT * const localVelocity, const FLOAT * const localMeshsize, const FLOAT * const localViscosity, const Parameters & parameters, FLOAT dt){
    return localVelocity [mapd(0,0,0,2)]
		+ dt * ( comp31 ( localVelocity, localMeshsize, localViscosity ) 
		+ comp32 ( localVelocity, localMeshsize, localViscosity )
		+ 2.0*comp33 ( localVelocity, localMeshsize, localViscosity, parameters )
                - dw2dz ( localVelocity, parameters, localMeshsize )
		- duwdx ( localVelocity, parameters, localMeshsize )
                - dvwdy ( localVelocity, parameters, localMeshsize ) + parameters.environment.gz );
}



#endif

#ifndef _VISCOSITY_BUFFER_FILL_STENCIL_H_
#define _VISCOSITY_BUFFER_FILL_STENCIL_H_

#include "../Stencil.h"
#include "../Parameters.h"
#include "../FlowField.h"

/** Stencil to read the viscosity values and store it into a single 1-d array.
 */
class ViscosityBufferFillStencil : public BoundaryStencil<FlowField> {

 public:

  const int *localSize;

  FLOAT* leftViscosityFillBuffer;
  FLOAT* rightViscosityFillBuffer;
  FLOAT* bottomViscosityFillBuffer;
  FLOAT* topViscosityFillBuffer;
  FLOAT* frontViscosityFillBuffer;
  FLOAT* backViscosityFillBuffer;


  ViscosityBufferFillStencil(const Parameters & parameters);

        //@Brief Functions for the 2D problem. Coordinates entered in alphabetical order.
        //@{
        void applyLeftWall   ( FlowField & flowField, int i, int j );
        void applyRightWall  ( FlowField & flowField, int i, int j );
        void applyBottomWall ( FlowField & flowField, int i, int j );
        void applyTopWall    ( FlowField & flowField, int i, int j );
        //@}

        //@brief Functions for the 3D problem. Coordinates entered in alphabetical order.
        //@{
        void applyLeftWall   ( FlowField & flowField, int i, int j, int k );
        void applyRightWall  ( FlowField & flowField, int i, int j, int k );
        void applyBottomWall ( FlowField & flowField, int i, int j, int k );
        void applyTopWall    ( FlowField & flowField, int i, int j, int k );
        void applyFrontWall  ( FlowField & flowField, int i, int j, int k );
        void applyBackWall   ( FlowField & flowField, int i, int j, int k );
        //@}
};

#endif

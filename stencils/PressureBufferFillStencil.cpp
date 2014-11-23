#include "PressureBufferFillStencil.h"

PressureBufferFillStencil::PressureBufferFillStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftPressureFillBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
  rightPressureFillBuffer = new FLOAT[(localSize[1]) * (localSize[2])];

  bottomPressureFillBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
  topPressureFillBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

  frontPressureFillBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  backPressureFillBuffer = new FLOAT[(localSize[0]) * (localSize[1])];

}



void PressureBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset; 
   */
  leftPressureFillBuffer[(k-2)+(localSize[1]*(j-2))] = flowField.getPressure().getScalar(i,j,k);
}

void PressureBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
  rightPressureFillBuffer[(k-2)+(localSize[1]*(j-2))] = flowField.getPressure().getScalar(i,j,k);
}

void PressureBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  bottomPressureFillBuffer[(k-2)+(localSize[0]*(i-2))] = flowField.getPressure().getScalar(i,j,k);
}

void PressureBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */ 
  topPressureFillBuffer[(k-2)+(localSize[0]*(i-2))] = flowField.getPressure().getScalar(i,j,k);
}

void PressureBufferFillStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  frontPressureFillBuffer[(j-2)+(localSize[0]*(i-2))] = flowField.getPressure().getScalar(i,j,k);
}

void PressureBufferFillStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */   
  backPressureFillBuffer[(j-2)+(localSize[0]*(i-2))] = flowField.getPressure().getScalar(i,j,k);
}

/*2D
 */
void PressureBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j) {
}

void PressureBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j) {
}

void PressureBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j) {
}

void PressureBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j) {
}

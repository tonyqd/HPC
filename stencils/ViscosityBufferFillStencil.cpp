#include "ViscosityBufferFillStencil.h"

ViscosityBufferFillStencil::ViscosityBufferFillStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

  int dim = parameters.geometry.dim;

  leftViscosityFillBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
  rightViscosityFillBuffer = new FLOAT[(localSize[1]) * (localSize[2])];

  bottomViscosityFillBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
  topViscosityFillBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

  frontViscosityFillBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  backViscosityFillBuffer = new FLOAT[(localSize[0]) * (localSize[1])];

}



void ViscosityBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset; 
   */
  leftViscosityFillBuffer[(k-2)+(localSize[1]*(j-2))] = flowField.getViscosity().getScalar(i,j,k);
}

void ViscosityBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
  rightViscosityFillBuffer[(k-2)+(localSize[1]*(j-2))] = flowField.getViscosity().getScalar(i,j,k);
}

void ViscosityBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  bottomViscosityFillBuffer[(k-2)+(localSize[0]*(i-2))] = flowField.getViscosity().getScalar(i,j,k);
}

void ViscosityBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */ 
  topViscosityFillBuffer[(k-2)+(localSize[0]*(i-2))] = flowField.getViscosity().getScalar(i,j,k);
}

void ViscosityBufferFillStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  frontViscosityFillBuffer[(j-2)+(localSize[0]*(i-2))] = flowField.getViscosity().getScalar(i,j,k);
}

void ViscosityBufferFillStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */   
  backViscosityFillBuffer[(j-2)+(localSize[0]*(i-2))] = flowField.getViscosity().getScalar(i,j,k);
}

/*2D
 */
void ViscosityBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j) {
}

void ViscosityBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j) {
}

void ViscosityBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j) {
}

void ViscosityBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j) {
}

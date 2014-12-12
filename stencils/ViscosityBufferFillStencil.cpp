#include "ViscosityBufferFillStencil.h"

ViscosityBufferFillStencil::ViscosityBufferFillStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {


  leftViscosityFillBuffer = new FLOAT[(localSize[1]+3) * (localSize[2]+3)];
  rightViscosityFillBuffer = new FLOAT[(localSize[1]+3) * (localSize[2]+3)];

  bottomViscosityFillBuffer = new FLOAT[(localSize[0]+3) * (localSize[2]+3)];
  topViscosityFillBuffer = new FLOAT[(localSize[0]+3) * (localSize[2]+3)];

  frontViscosityFillBuffer = new FLOAT[(localSize[0]+3) * (localSize[1]+3)];
  backViscosityFillBuffer = new FLOAT[(localSize[0]+3) * (localSize[1]+3)];

}



void ViscosityBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset; 
   */
  leftViscosityFillBuffer[(k)+(localSize[2]+3)*j] = flowField.getViscosity().getScalar(i+2,j,k);
}

void ViscosityBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
  rightViscosityFillBuffer[(k)+(localSize[2]+3)*(j)] = flowField.getViscosity().getScalar(i-1,j,k);
}

void ViscosityBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  bottomViscosityFillBuffer[(k)+(localSize[2]+3)*(i)] = flowField.getViscosity().getScalar(i,j+2,k);
}

void ViscosityBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */ 
  topViscosityFillBuffer[(k)+(localSize[2]+3)*(i)] = flowField.getViscosity().getScalar(i,j-1,k);
}

void ViscosityBufferFillStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  frontViscosityFillBuffer[(j)+(localSize[1]+3)*(i)] = flowField.getViscosity().getScalar(i,j,k+2);
}

void ViscosityBufferFillStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */   
  backViscosityFillBuffer[(j)+(localSize[1]+3)*(i)] = flowField.getViscosity().getScalar(i,j,k-1);
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

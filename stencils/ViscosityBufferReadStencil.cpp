#include "ViscosityBufferReadStencil.h"

ViscosityBufferReadStencil::ViscosityBufferReadStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {


  leftViscosityReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
  rightViscosityReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];

  bottomViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
  topViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

  frontViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  backViscosityReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  
}


void ViscosityBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset; 
   */  
	if( _parameters.parallel.leftNb >= 0){
		flowField.getViscosity().getScalar(i-1,j,k) = leftViscosityReadBuffer[k+(localSize[2])*j];
	}
}

void ViscosityBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.rightNb >= 0){
		flowField.getViscosity().getScalar(i+1,j,k) = rightViscosityReadBuffer[(k)+(localSize[2])*(j)];
	}
}

void ViscosityBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.bottomNb >= 0){
		flowField.getViscosity().getScalar(i,j-1,k) = bottomViscosityReadBuffer[k+(localSize[2])*(i)];
	}
}

void ViscosityBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */ 
	if( _parameters.parallel.topNb >= 0){
		flowField.getViscosity().getScalar(i,j+1,k) = topViscosityReadBuffer[(k)+(localSize[2])*(i)];
	}
}

void ViscosityBufferReadStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.frontNb >= 0){
		flowField.getViscosity().getScalar(i,j,k-1) = frontViscosityReadBuffer[(j)+(localSize[1])*(i)];
	}
}

void ViscosityBufferReadStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */  
	if( _parameters.parallel.backNb >= 0){
		flowField.getViscosity().getScalar(i,j,k+1) = backViscosityReadBuffer[(j)+(localSize[1])*(i)];
	}
}

/*2D
 */
void ViscosityBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j) {
}

void ViscosityBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j) {
}

void ViscosityBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j) {
}

void ViscosityBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j) {
}


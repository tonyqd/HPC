#include "PressureBufferReadStencil.h"

PressureBufferReadStencil::PressureBufferReadStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {


  leftPressureReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];
  rightPressureReadBuffer = new FLOAT[(localSize[1]) * (localSize[2])];

  bottomPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];
  topPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[2])];

  frontPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  backPressureReadBuffer = new FLOAT[(localSize[0]) * (localSize[1])];
  
}


void PressureBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset; 
   */  
	if( _parameters.parallel.leftNb >= 0)
	flowField.getPressure().getScalar(i-1,j,k) = leftPressureReadBuffer[((k)+((localSize[2])*(j)))];
	}
}

void PressureBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = -0;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.rightNb >= 0){
		flowField.getPressure().getScalar(i+1,j,k) = rightPressureReadBuffer[(k)+((localSize[2])*(j))];
	}
}

void PressureBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = -0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.bottomNb >= 0){
		flowField.getPressure().getScalar(i,j-1,k) = bottomPressureReadBuffer[((k)+((localSize[2])*(i)))];
	}

}

void PressureBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */ 
	if( _parameters.parallel.topNb >= 0){
		flowField.getPressure().getScalar(i,j+1,k) = topPressureReadBuffer[(k)+((localSize[2])*(i))];
	}
}

void PressureBufferReadStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
	if( _parameters.parallel.frontNb >= 0){
		flowField.getPressure().getScalar(i,j,k-1) = frontPressureReadBuffer[((j)+((localSize[1])*(i)))];
	}

}

void PressureBufferReadStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */  
	if( _parameters.parallel.backNb >= 0){
		flowField.getPressure().getScalar(i,j,k+1) = backPressureReadBuffer[(j)+((localSize[1])*(i))];
	}
}

/*2D
 */
void PressureBufferReadStencil::applyLeftWall ( FlowField & flowField, int i, int j) {
}

void PressureBufferReadStencil::applyRightWall ( FlowField & flowField, int i, int j) {
}

void PressureBufferReadStencil::applyBottomWall ( FlowField & flowField, int i, int j) {
}

void PressureBufferReadStencil::applyTopWall ( FlowField & flowField, int i, int j) {
}


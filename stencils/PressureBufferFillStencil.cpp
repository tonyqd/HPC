#include "PressureBufferFillStencil.h"

PressureBufferFillStencil::PressureBufferFillStencil( const Parameters & parameters):
  BoundaryStencil<FlowField>(parameters), localSize(parameters.parallel.localSize) {

	if(parameters.geometry.dim ==3){

	  leftPressureFillBuffer = new FLOAT[(localSize[1]+3) * (localSize[2]+3)];
	  rightPressureFillBuffer = new FLOAT[(localSize[1]+3) * (localSize[2]+3)*2];

	  bottomPressureFillBuffer = new FLOAT[(localSize[0]+3) * (localSize[2]+3)];
	  topPressureFillBuffer = new FLOAT[(localSize[0]+3) * (localSize[2]+3)*2];

	  frontPressureFillBuffer = new FLOAT[(localSize[0]+3) * (localSize[1]+3)];
	  backPressureFillBuffer = new FLOAT[(localSize[0]+3) * (localSize[1]+3)*2];
	}

	if(parameters.geometry.dim ==2){
		leftPressureFillBuffer = new FLOAT[(localSize[1]+3)];
		rightPressureFillBuffer = new FLOAT[(localSize[1]+3)*2];

		bottomPressureFillBuffer = new FLOAT[(localSize[0]+3) ];
		topPressureFillBuffer = new FLOAT[(localSize[0]+3) *2];
	}

}



void PressureBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 0; _highOffset = 0;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset; 
   */
	leftPressureFillBuffer[(k)+((localSize[2]+3)*(j))] = flowField.getPressure().getScalar(i+2,j,k);
}

void PressureBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _flowField.getCellsX()+_highOffset-1 ; j = _lowOffset ; k = _lowOffset;
   */
  rightPressureFillBuffer[0+2*((k)+((localSize[2]+3)*(j)))] = flowField.getPressure().getScalar(i-2,j,k);
  rightPressureFillBuffer[1+2*((k)+((localSize[2]+3)*(j)))] = flowField.getPressure().getScalar(i-1,j,k);
}

void PressureBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  bottomPressureFillBuffer[(k)+((localSize[2]+3)*(i))] = flowField.getPressure().getScalar(i,j+2,k);
}

void PressureBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1 ; k = _lowOffset;
   */ 
  topPressureFillBuffer[0+2*((k)+((localSize[2]+3)*(i)))] = flowField.getPressure().getScalar(i,j-2,k);
  topPressureFillBuffer[1+2*((k)+((localSize[2]+3)*(i)))] = flowField.getPressure().getScalar(i,j-1,k);
}

void PressureBufferFillStencil::applyFrontWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = _lowOffset;
   */
  frontPressureFillBuffer[(j)+((localSize[1]+3)*(i))] = flowField.getPressure().getScalar(i,j,k+2);
}

void PressureBufferFillStencil::applyBackWall ( FlowField & flowField, int i, int j, int k) {
  /* _lowOffset = 2; _highOffset = -1;
   * from ParallelBoundaryIterater:  i = _lowOffset ; j = _lowOffset ; k = Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1 ;
   */   
  backPressureFillBuffer[0+2*((j)+((localSize[1]+3)*(i)))] = flowField.getPressure().getScalar(i,j,k-2);
  backPressureFillBuffer[1+2*((j)+((localSize[1]+3)*(i)))] = flowField.getPressure().getScalar(i,j,k-1);
}

/*2D
 */
void PressureBufferFillStencil::applyLeftWall ( FlowField & flowField, int i, int j) {
	leftPressureFillBuffer[j] = flowField.getPressure().getScalar(i+2,j);
}

void PressureBufferFillStencil::applyRightWall ( FlowField & flowField, int i, int j) {
	rightPressureFillBuffer[0+2*j] = flowField.getPressure().getScalar(i-2,j);
	rightPressureFillBuffer[1+2*j] = flowField.getPressure().getScalar(i-1,j);
}

void PressureBufferFillStencil::applyBottomWall ( FlowField & flowField, int i, int j) {
	bottomPressureFillBuffer[i] = flowField.getPressure().getScalar(i,j+2);
}

void PressureBufferFillStencil::applyTopWall ( FlowField & flowField, int i, int j) {
	topPressureFillBuffer[0+2*i] = flowField.getPressure().getScalar(i,j-2);
	topPressureFillBuffer[1+2*i] = flowField.getPressure().getScalar(i,j-1);
}

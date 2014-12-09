#ifndef _SIMULATION_H_
#define _SIMULATION_H_

#include <petscksp.h>
#include <float.h>
#include "FlowField.h"
#include "stencils/FGHStencil.h"
#include "stencils/MovingWallStencils.h"
#include "stencils/RHSStencil.h"
#include "stencils/VelocityStencil.h"
#include "stencils/VTKStencil.h"
#include "stencils/MaxUStencil.h"
#include "stencils/PeriodicBoundaryStencils.h"
#include "stencils/BFStepInitStencil.h"
#include "stencils/NeumannBoundaryStencils.h"
#include "stencils/BFInputStencils.h"
#include "stencils/InitTaylorGreenFlowFieldStencil.h"
#include "parallelManagers/PetscParallelManager.h"
#include "stencils/PressureBufferFillStencil.h"
#include "stencils/PressureBufferReadStencil.h"
#include "stencils/VelocityBufferFillStencil.h"
#include "stencils/VelocityBufferReadStencil.h"
#include "GlobalBoundaryFactory.h"
#include "Iterators.h"
#include "Definitions.h"

#include "LinearSolver.h"
#include "solvers/SORSolver.h"
#include "solvers/PetscSolver.h"



class Simulation {
  protected:
    Parameters &_parameters;

    FlowField &_flowField;

    MaxUStencil _maxUStencil;
    FieldIterator<FlowField> _maxUFieldIterator;
    GlobalBoundaryIterator<FlowField> _maxUBoundaryIterator;

    // Set up the boundary conditions
    GlobalBoundaryFactory _globalBoundaryFactory;
    GlobalBoundaryIterator<FlowField> _wallVelocityIterator;
    GlobalBoundaryIterator<FlowField> _wallFGHIterator;

    // Pressure and velocity communication
    PressureBufferFillStencil _pressureBufferFillStencil;
    PressureBufferReadStencil _pressureBufferReadStencil;
    VelocityBufferFillStencil _velocityBufferFillStencil;
    VelocityBufferReadStencil _velocityBufferReadStencil;
    ParallelBoundaryIterator<FlowField> _parallelPressureFillBoundaryIterator;
    ParallelBoundaryIterator<FlowField> _parallelPressureReadBoundaryIterator;
    ParallelBoundaryIterator<FlowField> _parallelVelocityFillBoundaryIterator;
    ParallelBoundaryIterator<FlowField> _parallelVelocityReadBoundaryIterator;
    PetscParallelManager<FlowField> _petscParallelManager;

    // Viscosity communication
    ViscosityBufferFillStencil _viscosityBufferFillStencil;
    ViscosityBufferReadStencil _viscosityBufferReadStencil;
    ParallelBoundaryIterator<FlowField> _parallelViscosityFillBoundaryIterator;
    ParallelBoundaryIterator<FlowField> _parallelViscosityReadBoundaryIterator;

    FGHStencil _fghStencil;
    FieldIterator<FlowField> _fghIterator;

    RHSStencil _rhsStencil;
    FieldIterator<FlowField> _rhsIterator;

    VelocityStencil _velocityStencil;
    FieldIterator<FlowField> _velocityIterator;

    VTKStencil _vtkStencil;
    FieldIterator<FlowField> _vtkIterator;

    PetscSolver _solver;


  public:
    Simulation(Parameters &parameters, FlowField &flowField):
       _parameters(parameters),
       _flowField(flowField),
       _maxUStencil(parameters),
       _maxUFieldIterator(_flowField,parameters,_maxUStencil),
       _maxUBoundaryIterator(_flowField,parameters,_maxUStencil),
       _globalBoundaryFactory(parameters),
       _wallVelocityIterator(_globalBoundaryFactory.getGlobalBoundaryVelocityIterator(_flowField)),
       _wallFGHIterator(_globalBoundaryFactory.getGlobalBoundaryFGHIterator(_flowField)),
		_pressureBufferFillStencil(parameters),
		_pressureBufferReadStencil(parameters),
		_velocityBufferFillStencil(parameters),
		_velocityBufferReadStencil(parameters),
		_viscosityBufferFillStencil(parameters),
		_viscosityBufferReadStencil(parameters),

		_parallelPressureFillBoundaryIterator(_flowField, parameters, _pressureBufferFillStencil, 0, 0),
		_parallelPressureReadBoundaryIterator(_flowField, parameters, _pressureBufferReadStencil, 0, 0),
		_parallelVelocityFillBoundaryIterator(_flowField, parameters, _velocityBufferFillStencil, 0, 0),
		_parallelVelocityReadBoundaryIterator(_flowField, parameters, _velocityBufferReadStencil, 0, 0),
		_parallelViscosityFillBoundaryIterator(_flowField, parameters, _viscosityBufferFillStencil, 0, 0),
		_parallelViscosityReadBoundaryIterator(_flowField, parameters, _viscosityBufferReadStencil, 0, 0),
		_petscParallelManager(parameters, _pressureBufferFillStencil, _pressureBufferReadStencil, _parallelPressureFillBoundaryIterator, _parallelPressureReadBoundaryIterator,
				_velocityBufferFillStencil, _velocityBufferReadStencil, _parallelVelocityFillBoundaryIterator, _parallelVelocityReadBoundaryIterator,
				_viscosityBufferFillStencil, _viscosityBufferReadStencil, _parallelViscosityFillBoundaryIterator, _parallelViscosityReadBoundaryIterator),
       _fghStencil(parameters),
       _fghIterator(_flowField,parameters,_fghStencil),
       _rhsStencil(parameters),
       _rhsIterator(_flowField,parameters,_rhsStencil),
       _velocityStencil(parameters),
       _velocityIterator(_flowField,parameters,_velocityStencil),
       _vtkStencil(parameters),
       _vtkIterator(_flowField,parameters,_vtkStencil,1,0),
       _solver(_flowField,parameters)
       {
       }

    virtual ~Simulation(){}

    /** initialises the flow field according to the scenario */
    virtual void initializeFlowField(){
      if (_parameters.simulation.scenario=="taylor-green"){
        // currently, a particular initialization is only requrid for the taylor-green vortex
        InitTaylorGreenFlowFieldStencil stencil(_parameters);
        FieldIterator<FlowField> iterator(_flowField,_parameters,stencil);
        iterator.iterate();
      } else if (_parameters.simulation.scenario=="channel"){
        BFStepInitStencil stencil(_parameters);
        FieldIterator<FlowField> iterator(_flowField,_parameters,stencil,0,1);
        iterator.iterate();
      } else if (_parameters.simulation.scenario=="pressure-channel"){
	    //set pressure boundaries here for left wall
        const FLOAT value = _parameters.walls.scalarLeft;
        ScalarField& rhs = _flowField.getRHS();

        if (_parameters.geometry.dim==2){
          const int sizey = _flowField.getNy();
          for (int i =0 ;i < sizey+3;i++) {
            rhs.getScalar(0,i) = value;
          }
        } else {
          const int sizey = _flowField.getNy();
          const int sizez = _flowField.getNz();
          for (int i=0;i<sizey+3;i++)
            for(int j=0;j<sizez + 3;j++)
              rhs.getScalar(0,i,j) =value;
	    }
	    
	    // do same procedure for domain flagging as for regular channel
	    BFStepInitStencil stencil(_parameters);
        FieldIterator<FlowField> iterator(_flowField,_parameters,stencil,0,1);
        iterator.iterate();
	  }
    }

    virtual void solveTimestep(){
        // determine and set max. timestep which is allowed in this simulation
        setTimeStep();
        // compute fgh
        _fghIterator.iterate();
        // set global boundary values
        _wallFGHIterator.iterate();
        // compute the right hand side
        _rhsIterator.iterate();
        // solve for pressure 
        _solver.solve();
        // TODO WS2: communicate pressure values
        _petscParallelManager.communicatePressure();
 /*int i=0; int j = 0;int k =0;
        for (k =0; k < _parameters.parallel.localSize[0] +3; k++ ){
        	for(j =0; j < _parameters.parallel.localSize[0] +3; j++){
        		for(i =0; i < _parameters.parallel.localSize[0]+3; i++){
        			printf("processor %d i %d j %d k %d = %f \n", _parameters.parallel.rank ,i,j,k,_flowField.getPressure().getScalar(i,j,k));
        		}
        	}
        }*/
        //_flowField.getFGH().show();

        // compute velocity
        _velocityIterator.iterate();
        // TODO WS2: communicate velocity values
        _petscParallelManager.communicateVelocities();
        // Iterate for velocities on the boundary
        _wallVelocityIterator.iterate();


    }

    /** TODO WS1: plots the flow field. */
    virtual void plotVTK(int timeStep){
      _vtkIterator.iterate();
      _vtkStencil.write(_flowField,timeStep);
      
      
      // TODO WS1: create VTKStencil and respective iterator; iterate stencil
      //           over _flowField and write flow field information to vtk file
    }

  protected:
    /** sets the time step*/
    virtual void setTimeStep(){

      FLOAT localMin, globalMin;
      assertion(_parameters.geometry.dim == 2 || _parameters.geometry.dim == 3);
      FLOAT factor = 1.0/(_parameters.meshsize->getDxMin() * _parameters.meshsize->getDxMin()) +
                     1.0/(_parameters.meshsize->getDyMin() * _parameters.meshsize->getDyMin());

      // determine maximum velocity
      _maxUStencil.reset();
      _maxUFieldIterator.iterate();
      _maxUBoundaryIterator.iterate();
      if (_parameters.geometry.dim == 3) {
        factor += 1.0/(_parameters.meshsize->getDzMin() * _parameters.meshsize->getDzMin());
        _parameters.timestep.dt = 1.0 / _maxUStencil.getMaxValues()[2];
      } else {
        _parameters.timestep.dt = 1.0 / _maxUStencil.getMaxValues()[0];
      }

      localMin = std::min(_parameters.timestep.dt,
                                        std::min(std::min(_parameters.flow.Re/(2*factor),
                                        1 / _maxUStencil.getMaxValues()[0]),
                                        1 / _maxUStencil.getMaxValues()[1]));

      // Here, we select the type of operation before compiling. This allows to use the correct
      // data type for MPI. Not a concern for small simulations, but useful if using heterogeneous
      // machines.

      globalMin = MY_FLOAT_MAX;
      MPI_Allreduce(&localMin, &globalMin, 1, MY_MPI_FLOAT, MPI_MIN, PETSC_COMM_WORLD);

      _parameters.timestep.dt = globalMin;
      _parameters.timestep.dt *= _parameters.timestep.tau;
    }

};

#endif // _SIMULATION_H_


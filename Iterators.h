#ifndef _ITERATORS_H_
#define _ITERATORS_H_

#include "Stencil.h"
#include "Parameters.h"


/** Iterator class
 *
 * Applies operations to a flow field
 */
template<class FlowField>
class Iterator {
    protected:

        FlowField & _flowField;     //! Reference to the flow field
        const Parameters & _parameters; //! Rerence to the parameters

    public:

        /** Constructor for the iterator
         *
         * Creates an iterator, given a flow field and a stencil instance
         *
         * @param flowField Flow field with the state of the flow
         * @param stencil Stencil defining an operation to be applied
         */
        Iterator ( FlowField & flowfield, const Parameters& parameters ): _flowField(flowfield), _parameters(parameters){}

        /** Perform the stencil operation on inner, non-ghost cells
         */
        virtual void iterate () = 0;
};

template<class FlowField>
class FieldIterator : public Iterator<FlowField> {

    private:

        FieldStencil<FlowField> & _stencil;         //! Reference to a stencil

        //@brief Define the iteration domain to include more or less layers
        // Added since the ability to select the iteration domain provides more flexibility
        //@{
        const int _lowOffset;
        const int _highOffset;
        //@}

    public:

        FieldIterator (FlowField & flowField, const Parameters& parameters, FieldStencil<FlowField> & stencil,
                       int lowOffset = 0, int highOffset = 0);

        /** Volume iteration over the field.
         *
         * Volume iteration. The stencil will be applied to all cells in the domain plus the upper
         * boundaries. Lower boundaries are not included.
         */
        void iterate ();
};


template<class FlowField>
class GlobalBoundaryIterator : public Iterator<FlowField> {

    private:

        const int _lowOffset;
        const int _highOffset;

        // This iterator has a reference to a stencil for each side, and will call its methods
        BoundaryStencil<FlowField> & _leftWallStencil;   //! Stencil used on the left wall
        BoundaryStencil<FlowField> & _rightWallStencil;  //! Stencil used on the right wall
        BoundaryStencil<FlowField> & _bottomWallStencil; //! Stencil used on the bottom wall
        BoundaryStencil<FlowField> & _topWallStencil;    //! Stencil used on the top wall
        BoundaryStencil<FlowField> & _frontWallStencil;  //! Stencil used on the front wall
        BoundaryStencil<FlowField> & _backWallStencil;   //! Stencil used on the back wall

    public:

        /** Constructor for a single stencil in all faces
         * @param flowField The flowfield information
         * @param stencil Stencil for all faces
         */
        GlobalBoundaryIterator (FlowField & flowField, const Parameters & parameters,
                                BoundaryStencil<FlowField> & stencil,
                                int lowOffset = 0, int highOffset = 0);

        /** Constructor with different stencils for each face. For the 2D case.
         * @param flowField Flow field information
         * @param <some>WallStencil Stencil used on <some> wall
         */
        GlobalBoundaryIterator ( FlowField & flowField, const Parameters & parameters,
                                 BoundaryStencil<FlowField> & leftWallStencil,
                                 BoundaryStencil<FlowField> & rightWallStencil,
                                 BoundaryStencil<FlowField> & bottomWallStencil,
                                 BoundaryStencil<FlowField> & topWallStencil,
                                 int lowOffset = 0, int highOffset = 0);

        /** Constructor with different stencils for each face. For the 3D case.
         * @param flowField Flow field information
         * @param <some>WallStencil Stencil used on <some> wall
         */
        GlobalBoundaryIterator ( FlowField & flowField, const Parameters & parameters,
                                 BoundaryStencil<FlowField> & leftWallStencil,
                                 BoundaryStencil<FlowField> & rightWallStencil,
                                 BoundaryStencil<FlowField> & bottomWallStencil,
                                 BoundaryStencil<FlowField> & topWallStencil,
                                 BoundaryStencil<FlowField> & frontWallStencil,
                                 BoundaryStencil<FlowField> & backWallStencil,
                                 int lowOffset = 0, int highOffset = 0);

        /** Surface iterator
         *
         * Iterates on the boundary cells. Only upper corners and edges are iterated.
         */
        void iterate ();
};

template <class FlowField>
class ParallelBoundaryIterator : public Iterator<FlowField> {

    private:
        BoundaryStencil<FlowField> & _stencil;

        const int _lowOffset;
        const int _highOffset;

    public:
        ParallelBoundaryIterator(FlowField & flowField,
                                 const Parameters & parameters,
                                 BoundaryStencil<FlowField> & stencil,
                                 int lowOffset = 0, int highOffset = 0);
        void iterate();
};
//#include "Iterators.cpph"


template<class FlowField>
FieldIterator<FlowField>::FieldIterator (FlowField & flowField, const Parameters& parameters, FieldStencil<FlowField> & stencil,
                               int lowOffset, int highOffset):
    Iterator<FlowField>(flowField,parameters), _stencil(stencil), _lowOffset(lowOffset), _highOffset(highOffset){}


template<class FlowField>
void FieldIterator<FlowField>::iterate (){

    const int cellsX = Iterator<FlowField>::_flowField.getCellsX();
    const int cellsY = Iterator<FlowField>::_flowField.getCellsY();
    const int cellsZ = Iterator<FlowField>::_flowField.getCellsZ();
    // The index k can be used for the 2D and 3D cases.

    if (Iterator<FlowField>::_parameters.geometry.dim == 2){

        // Loop without lower boundaries. These will be dealt with by the global boundary stencils
        // or by the subdomain boundary iterators.
        for (int j = 1 + _lowOffset; j < cellsY - 1 + _highOffset; j++){
            for (int i = 1 + _lowOffset; i < cellsX - 1 + _highOffset; i++){
                _stencil.apply ( Iterator<FlowField>::_flowField, i, j );
            }
        }
    }

    if (Iterator<FlowField>::_parameters.geometry.dim == 3){

        for (int k = 1 + _lowOffset; k < cellsZ - 1 + _highOffset; k++){
            for (int j = 1 + _lowOffset; j < cellsY - 1 + _highOffset; j++){
                for (int i = 1 + _lowOffset; i < cellsX - 1 + _highOffset; i++){
                    _stencil.apply ( Iterator<FlowField>::_flowField, i, j, k );
                }
            }
        }
    }
}


template<class FlowField>
GlobalBoundaryIterator<FlowField>::GlobalBoundaryIterator(FlowField & flowField,
                                               const Parameters & parameters,
                                               BoundaryStencil<FlowField> & stencil,
                                               int lowOffset, int highOffset):
    Iterator<FlowField> (flowField,parameters),
    _lowOffset(lowOffset), _highOffset(highOffset),
    _leftWallStencil(stencil), _rightWallStencil(stencil),
    _bottomWallStencil(stencil), _topWallStencil(stencil),
    _frontWallStencil(stencil), _backWallStencil(stencil){}


template<class FlowField>
GlobalBoundaryIterator<FlowField>::GlobalBoundaryIterator(FlowField & flowField,
                                               const Parameters & parameters,
                                               BoundaryStencil<FlowField> & leftWallStencil,
                                               BoundaryStencil<FlowField> & rightWallStencil,
                                               BoundaryStencil<FlowField> & bottomWallStencil,
                                               BoundaryStencil<FlowField> & topWallStencil,
                                               int lowOffset, int highOffset):
    Iterator<FlowField> (flowField,parameters),
    _lowOffset(lowOffset), _highOffset(highOffset),
    _leftWallStencil(leftWallStencil), _rightWallStencil(rightWallStencil),
    _bottomWallStencil(bottomWallStencil), _topWallStencil(topWallStencil),
    // This is plain bad, but it will work. The references had to be initialized somehow
    _frontWallStencil(leftWallStencil), _backWallStencil(leftWallStencil)
{

    if (Iterator<FlowField>::_parameters.geometry.dim == 3){
        handleError(1, "Trying to use 2D constructor for a 3D field");
    }

}


template<class FlowField>
GlobalBoundaryIterator<FlowField>::GlobalBoundaryIterator(FlowField & flowField,
                                               const Parameters & parameters,
                                               BoundaryStencil<FlowField> & leftWallStencil,
                                               BoundaryStencil<FlowField> & rightWallStencil,
                                               BoundaryStencil<FlowField> & bottomWallStencil,
                                               BoundaryStencil<FlowField> & topWallStencil,
                                               BoundaryStencil<FlowField> & frontWallStencil,
                                               BoundaryStencil<FlowField> & backWallStencil,
                                               int lowOffset, int highOffset):
    Iterator<FlowField> (flowField,parameters),
    _lowOffset(lowOffset), _highOffset(highOffset),
    _leftWallStencil(leftWallStencil), _rightWallStencil(rightWallStencil),
    _bottomWallStencil(bottomWallStencil), _topWallStencil(topWallStencil),
    _frontWallStencil(frontWallStencil), _backWallStencil(backWallStencil){}



template<class FlowField>
void GlobalBoundaryIterator<FlowField>::iterate () {

    if (Iterator<FlowField>::_parameters.geometry.dim == 2){

        if (Iterator<FlowField>::_parameters.parallel.leftNb < 0){
            for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY() + _highOffset; j++) {
                _leftWallStencil.applyLeftWall (Iterator<FlowField>::_flowField, _lowOffset, j);
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.rightNb < 0){
            for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY() + _highOffset; j++) {
                _rightWallStencil.applyRightWall (Iterator<FlowField>::_flowField, Iterator<FlowField>::_flowField.getCellsX()+_highOffset-1,j);
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.bottomNb < 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX() + _highOffset; i++) {
                _bottomWallStencil.applyBottomWall (Iterator<FlowField>::_flowField, i, _lowOffset);
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.topNb < 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX() + _highOffset; i++) {
                _topWallStencil.applyTopWall (Iterator<FlowField>::_flowField, i, Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1);
            }
        }
    }

    if (Iterator<FlowField>::_parameters.geometry.dim == 3){

        if (Iterator<FlowField>::_parameters.parallel.leftNb < 0){
            for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY()+_highOffset; j++) {
                for (int k = _lowOffset; k < Iterator<FlowField>::_flowField.getCellsZ()+_highOffset; k++) {
                    _leftWallStencil.applyLeftWall   ( Iterator<FlowField>::_flowField, _lowOffset, j, k );
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.rightNb < 0){
            for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY()+_highOffset; j++) {
                for (int k = _lowOffset; k < Iterator<FlowField>::_flowField.getCellsZ()+_highOffset; k++) {
                    _rightWallStencil.applyRightWall (Iterator<FlowField>::_flowField, Iterator<FlowField>::_flowField.getCellsX()+_highOffset-1, j, k);
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.bottomNb < 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX()+_highOffset; i++) {
                for (int k = _lowOffset; k < Iterator<FlowField>::_flowField.getCellsZ()+_highOffset; k++) {
                    _bottomWallStencil.applyBottomWall (Iterator<FlowField>::_flowField, i, _lowOffset, k);
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.topNb < 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX()+_highOffset; i++) {
                for (int k = _lowOffset; k < Iterator<FlowField>::_flowField.getCellsZ()+_highOffset; k++) {
                    _topWallStencil.applyTopWall (Iterator<FlowField>::_flowField, i, Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1, k);
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.frontNb < 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX()+_highOffset; i++) {
                for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY()+_highOffset; j++) {
                    _frontWallStencil.applyFrontWall (Iterator<FlowField>::_flowField, i, j, _lowOffset);
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.backNb < 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX()+_highOffset; i++) {
                for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY()+_highOffset; j++) {
                    _backWallStencil.applyBackWall (Iterator<FlowField>::_flowField, i, j, Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1);
                }
            }
        }
    }
}


template <class FlowField>
ParallelBoundaryIterator<FlowField>::ParallelBoundaryIterator (FlowField & flowField,
                                                               const Parameters & parameters,
                                                               BoundaryStencil<FlowField> & stencil,
                                                               int lowOffset, int highOffset):
    Iterator<FlowField>(flowField,parameters), _stencil(stencil),
    _lowOffset(lowOffset), _highOffset(highOffset){}


template<class FlowField>
void ParallelBoundaryIterator<FlowField>::iterate () {

    if (Iterator<FlowField>::_parameters.geometry.dim == 2){

        if (Iterator<FlowField>::_parameters.parallel.leftNb >= 0){
            for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY() + _highOffset; j++) {
                _stencil.applyLeftWall (Iterator<FlowField>::_flowField, _lowOffset, j);
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.rightNb >= 0){
            for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY() + _highOffset; j++) {
                _stencil.applyRightWall (Iterator<FlowField>::_flowField, Iterator<FlowField>::_flowField.getCellsX()+_highOffset-1,j);
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.bottomNb >= 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX() + _highOffset; i++) {
                _stencil.applyBottomWall (Iterator<FlowField>::_flowField, i, _lowOffset);
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.topNb >= 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX() + _highOffset; i++) {
                _stencil.applyTopWall (Iterator<FlowField>::_flowField, i, Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1);
            }
        }
    }

    if (Iterator<FlowField>::_parameters.geometry.dim == 3){

        if (Iterator<FlowField>::_parameters.parallel.leftNb >= 0){
            for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY()+_highOffset; j++) {
                for (int k = _lowOffset; k < Iterator<FlowField>::_flowField.getCellsZ()+_highOffset; k++) {
                    _stencil.applyLeftWall   ( Iterator<FlowField>::_flowField, _lowOffset, j, k );
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.rightNb >= 0){
            for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY()+_highOffset; j++) {
                for (int k = _lowOffset; k < Iterator<FlowField>::_flowField.getCellsZ()+_highOffset; k++) {
                    _stencil.applyRightWall (Iterator<FlowField>::_flowField, Iterator<FlowField>::_flowField.getCellsX()+_highOffset-1, j, k);
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.bottomNb >= 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX()+_highOffset; i++) {
                for (int k = _lowOffset; k < Iterator<FlowField>::_flowField.getCellsZ()+_highOffset; k++) {
                    _stencil.applyBottomWall (Iterator<FlowField>::_flowField, i, _lowOffset, k);
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.topNb >= 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX()+_highOffset; i++) {
                for (int k = _lowOffset; k < Iterator<FlowField>::_flowField.getCellsZ()+_highOffset; k++) {
                    _stencil.applyTopWall (Iterator<FlowField>::_flowField, i, Iterator<FlowField>::_flowField.getCellsY()+_highOffset-1, k);
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.frontNb >= 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX()+_highOffset; i++) {
                for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY()+_highOffset; j++) {
                    _stencil.applyFrontWall (Iterator<FlowField>::_flowField, i, j, _lowOffset);
                }
            }
        }

        if (Iterator<FlowField>::_parameters.parallel.backNb >= 0){
            for (int i = _lowOffset; i < Iterator<FlowField>::_flowField.getCellsX()+_highOffset; i++) {
                for (int j = _lowOffset; j < Iterator<FlowField>::_flowField.getCellsY()+_highOffset; j++) {
                    _stencil.applyBackWall (Iterator<FlowField>::_flowField, i, j, Iterator<FlowField>::_flowField.getCellsZ()+_highOffset-1);
                }
            }
        }
    }
}


#endif

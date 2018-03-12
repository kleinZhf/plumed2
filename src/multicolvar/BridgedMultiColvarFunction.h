/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
   Copyright (c) 2014-2018 The plumed team
   (see the PEOPLE file at the root of the distribution for a list of names)

   See http://www.plumed.org for more information.

   This file is part of plumed, version 2.

   plumed is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   plumed is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with plumed.  If not, see <http://www.gnu.org/licenses/>.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
#ifndef __PLUMED_multicolvar_BridgedMultiColvarFunction_h
#define __PLUMED_multicolvar_BridgedMultiColvarFunction_h

#include "vesselbase/BridgeVessel.h"
#include "MultiColvarBase.h"

namespace PLMD {
namespace multicolvar {

class BridgedMultiColvarFunction : public MultiColvarBase {
  friend class MultiColvarBase;
  friend class MultiColvarFunction;
private:
/// This is used for storing positions properly
  Vector tmp_p;
/// The action that is calculating the colvars of interest
  MultiColvarBase* mycolv;
/// The vessel that bridges
  vesselbase::BridgeVessel* myBridgeVessel;
/// Everything for controlling the updating of neighbor lists
  bool firsttime;
  int updateFreq;
protected:
/// Deactivate all the atoms in the list
  void deactivateAllAtoms();
/// Activate the nth atom in the list
  void setAtomActive( const unsigned& n );
public:
  static void registerKeywords( Keywords& keys );
  explicit BridgedMultiColvarFunction(const ActionOptions&);
/// Get a pointer to the base multicolvar
  MultiColvarBase* getPntrToMultiColvar() const ;
/// Don't actually clear the derivatives when this is called from plumed main.
/// They are calculated inside another action and clearing them would be bad
  void clearDerivatives() {}
/// Check nothing impossible being done with derivatives
  virtual void turnOnDerivatives();
/// Get the number of derivatives for this action
  unsigned getNumberOfDerivatives();
/// Get the size of the atoms with derivatives array
  unsigned getSizeOfAtomsWithDerivatives();
/// Is the output quantity periodic
  bool isPeriodic();
/// Routines that have to be defined so as not to have problems with virtual methods
  void deactivate_task( const unsigned& taskno );
  void calculate() {}
/// This does the task
  void transformBridgedDerivatives( const unsigned& current, MultiValue& invals, MultiValue& outvals ) const ;
  void performTask( const unsigned&, const unsigned&, MultiValue& ) const ;
  virtual void completeTask( const unsigned& curr, MultiValue& invals, MultiValue& outvals ) const=0;
/// Get the central atom position
  Vector retrieveCentralAtomPos();
/// Get the index of the central atom
  AtomNumber getAbsoluteIndexOfCentralAtom( const unsigned& i ) const ;
/// Get indicecs involved in this colvar
  const std::vector<AtomNumber> & getAbsoluteIndexes()const ;
/// We need our own calculate numerical derivatives here
  void calculateNumericalDerivatives( ActionWithValue* a=NULL );
  void apply() {};
/// Is this atom currently being copied
  bool isCurrentlyActive( const unsigned& );
/// This should not be called
  Vector calculateCentralAtomPosition() { plumed_error(); }
  double compute( const unsigned& tindex, AtomValuePack& myvals ) const { plumed_error(); }
  Vector getPositionOfAtomForLinkCells( const unsigned& iatom ) const ;
  void getIndexList( const unsigned& ntotal, const unsigned& jstore, const unsigned& maxder, std::vector<unsigned>& indices );
  void applyBridgeForces( const std::vector<double>& bb );
  Vector getCentralAtomPos( const unsigned& curr );
  CatomPack getCentralAtomPack( const unsigned& basn, const unsigned& curr );
};

inline
const std::vector<AtomNumber> & BridgedMultiColvarFunction::getAbsoluteIndexes() const {
  return mycolv->getAbsoluteIndexes();
}

inline
MultiColvarBase* BridgedMultiColvarFunction::getPntrToMultiColvar() const {
  return mycolv;
}

inline
unsigned BridgedMultiColvarFunction::getNumberOfDerivatives() {
  return mycolv->getNumberOfDerivatives() + 3*getNumberOfAtoms();
}

inline
bool BridgedMultiColvarFunction::isCurrentlyActive( const unsigned& code ) {
  return mycolv->isCurrentlyActive( code );
}

inline
unsigned BridgedMultiColvarFunction::getSizeOfAtomsWithDerivatives() {
  return mycolv->getNumberOfAtoms();
}

inline
Vector BridgedMultiColvarFunction::getPositionOfAtomForLinkCells( const unsigned& iatom ) const {
  return mycolv->getPositionOfAtomForLinkCells(iatom);
}

inline
Vector BridgedMultiColvarFunction::getCentralAtomPos( const unsigned& curr ) {
  return mycolv->getCentralAtomPos( curr );
}

inline
AtomNumber BridgedMultiColvarFunction::getAbsoluteIndexOfCentralAtom(const unsigned& i) const {
  return mycolv->getAbsoluteIndexOfCentralAtom(i);
}

}
}
#endif

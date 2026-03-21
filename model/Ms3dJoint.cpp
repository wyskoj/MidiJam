//
// Created by Jacob on 3/17/2026.
//

#include "Ms3dJoint.h"

// FUNCTION: MIDIJAM 0x43A060
Ms3dJoint::Ms3dJoint()
{
    worldTransformationMatrix = AffineTransform4f();
    localTransformationMatrix = AffineTransform4f();
    finalTransformationMatrix = AffineTransform4f();
}

// FUNCTION: MIDIJAM 0x43A0D0
Ms3dJoint::~Ms3dJoint()
{
    // No-op — compiler-generated base class destructor stub (noop_0).
}

// FUNCTION: MIDIJAM 0x439FF0
void Ms3dJointCollection_Delete(Ms3dJointCollection* collection)
{
    // Compiler-generated array destructor — equivalent to delete[] pJoints.
    delete[] collection->pJoints;
    collection->pJoints = nullptr;
    collection->nJoints = 0;
}
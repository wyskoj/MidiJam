//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_MS3DJOINT_H
#define MIDIJAM_MS3DJOINT_H

#include "math/MatrixMath.h"

/**
 * A translation keyframe, storing x, y, z position values.
 */
struct KeyframeLoc
{
    float x;
    float y;
    float z;
};

/**
 * A rotation keyframe, storing x, y, z Euler angle values.
 */
struct KeyframeRot
{
    float x;
    float y;
    float z;
};

/**
 * Union of a location and rotation keyframe.
 * Which member is valid depends on whether this keyframe belongs to
 * a translation or rotation track.
 */
union KeyframeData
{
    KeyframeLoc loc;
    KeyframeRot rot;
};

/**
 * A packed keyframe entry associating a joint index and timestamp
 * with either a translation or rotation value.
 *
 * Original struct layout (sizeof = 0x14):
 *   +0x00  int           jointIndex
 *   +0x04  float         timeMs
 *   +0x08  KeyframeData  data
 */
struct Ms3dKeyframePacked
{
    int          jointIndex;
    float        timeMs;
    KeyframeData data;
};

/**
 * A single joint (bone) in a Milkshape 3D skeletal hierarchy.
 * Stores local and world transformation matrices, keyframe arrays,
 * and a final transformation matrix used during skeletal animation.
 *
 * Original struct layout (sizeof = 0xF4):
 *   +0x00  float[3]              rotation
 *   +0x0C  float[3]              position
 *   +0x18  MatrixMath            worldTransformationMatrix
 *   +0x58  MatrixMath            localTransformationMatrix
 *   +0x98  int                   nKeyframesRot
 *   +0x9C  int                   nKeyframesTrans
 *   +0xA0  Ms3dKeyframePacked*   pKeyframesTrans
 *   +0xA4  Ms3dKeyframePacked*   pKeyFramesRot
 *   +0xA8  int                   currentRotKeyframe
 *   +0xAC  int                   currentTransKeyframe
 *   +0xB0  MatrixMath            finalTransformationMatrix
 *   +0xF0  int                   parentIndex
 */
struct Ms3dJoint
{
    float               rotation[3];
    float               position[3];
    MatrixMath          worldTransformationMatrix;
    MatrixMath          localTransformationMatrix;
    int                 nKeyframesRot;
    int                 nKeyframesTrans;
    Ms3dKeyframePacked* pKeyframesTrans;
    Ms3dKeyframePacked* pKeyFramesRot;
    int                 currentRotKeyframe;
    int                 currentTransKeyframe;
    MatrixMath          finalTransformationMatrix;
    int                 parentIndex;

    /**
     * Initializes all three transformation matrices to identity via MatrixMath::ctor.
     */
    Ms3dJoint();

    /**
     * No-op destructor — compiler-generated base class destructor stub.
     */
    ~Ms3dJoint();
};

static_assert(sizeof(Ms3dKeyframePacked) == 0x14, "Ms3dKeyframePacked layout mismatch");
static_assert(sizeof(Ms3dJoint) == 0xF4, "Ms3dJoint layout mismatch");

/**
 * A counted array of Ms3dJoint objects.
 *
 * Original struct layout (sizeof = 0x8):
 *   +0x00  int         nJoints
 *   +0x04  Ms3dJoint*  pJoints
 */
struct Ms3dJointCollection
{
    int        nJoints;
    Ms3dJoint* pJoints;
};

static_assert(sizeof(Ms3dJointCollection) == 0x8, "Ms3dJointCollection layout mismatch");

void Ms3dJointCollection_Delete(Ms3dJointCollection* collection);

#endif // MIDIJAM_MS3DJOINT_H
//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_MS3D_FORMAT_H
#define MIDIJAM_MS3D_FORMAT_H

#include <windows.h>

// ---------------------------------------------------------------------------
// Raw Milkshape 3D binary file format structs.
// These map directly to the on-disk layout and are used only during parsing.
// Do not use these in runtime code — convert to Ms3dBundle's internal types.
// ---------------------------------------------------------------------------

#pragma pack(push, 1)

/**
 * A single vertex in the MS3D binary format.
 * sizeof = 0x0F
 */
struct ms3d_vertex_t
{
    byte  flags;
    float vertex[3];
    char  boneId;
    byte  referenceCount;
};

static_assert(sizeof(ms3d_vertex_t) == 0x0F, "ms3d_vertex_t layout mismatch");

/**
 * A triangle in the MS3D binary format.
 * sizeof = 0x46
 */
struct ms3d_triangle_t
{
    short flags;
    short vertexIndices[3];
    float vertexNormals[3][3];
    float s[3];
    float t[3];
    byte  smoothingGroup;
    byte  groupIndex;
};

static_assert(sizeof(ms3d_triangle_t) == 0x46, "ms3d_triangle_t layout mismatch");

/**
 * A joint (bone) in the MS3D binary format.
 * sizeof = 0x5D
 */
struct ms3d_joint_t
{
    byte  flags;
    char  name[32];
    char  parentName[32];
    float rotation[3];
    float position[3];
    short numKeyFramesRot;
    short numKeyFramesTrans;
};

static_assert(sizeof(ms3d_joint_t) == 0x5D, "ms3d_joint_t layout mismatch");

/**
 * A rotation keyframe in the MS3D binary format.
 * sizeof = 0x10
 */
struct ms3d_keyframe_rot_t
{
    float time;
    float rotation[3];
};

static_assert(sizeof(ms3d_keyframe_rot_t) == 0x10, "ms3d_keyframe_rot_t layout mismatch");

/**
 * A translation keyframe in the MS3D binary format.
 * sizeof = 0x10
 */
struct ms3d_keyframe_pos_t
{
    float time;
    float position[3];
};

static_assert(sizeof(ms3d_keyframe_pos_t) == 0x10, "ms3d_keyframe_pos_t layout mismatch");

/**
 * A material definition in the MS3D binary format.
 * sizeof = 0x169
 */
struct ms3d_material_t
{
    char  name[32];
    float ambient[4];
    float diffuse[4];
    float specular[4];
    float emissive[4];
    float shininess;
    float transparency;
    char  mode;
    char  texture_name[128];
    char  alphamap_name[128];
};

static_assert(sizeof(ms3d_material_t) == 0x169, "ms3d_material_t layout mismatch");

#pragma pack(pop)

/**
 * Maps a joint index to its name pointer during MS3D parsing.
 * Used to resolve parent joint names to indices.
 * sizeof = 0x8
 */
struct Ms3dJointMapping
{
    int   index;
    char* name;
};

static_assert(sizeof(Ms3dJointMapping) == 0x8, "Ms3dJointMapping layout mismatch");

#endif // MIDIJAM_MS3D_FORMAT_H
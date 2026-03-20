//
// Created by Jacob on 3/17/2026.
//

#ifndef MIDIJAM_MS3DBUNDLE_H
#define MIDIJAM_MS3DBUNDLE_H

#include "Ms3dJoint.h"
#include "animation/AnimationController.h"

#include <cstdio>
#include <windows.h>
#include <GL/gl.h>

// ---------------------------------------------------------------------------
// Mesh data structs
// ---------------------------------------------------------------------------

/**
 * A single interleaved vertex containing position, texture coordinates,
 * and normal — laid out for use with OpenGL interleaved vertex arrays.
 *
 * Original struct layout (sizeof = 0x20):
 *   +0x00  float[3]  position
 *   +0x0C  float[2]  texCoord
 *   +0x14  float[3]  normal
 */
struct InterleavedVertex
{
    float position[3];
    float texCoord[2];
    float normal[3];
};

static_assert(sizeof(InterleavedVertex) == 0x20, "InterleavedVertex layout mismatch");

/**
 * Three interleaved vertices forming a triangle.
 *
 * Original struct layout (sizeof = 0x60):
 *   +0x00  InterleavedVertex[3]  vertices
 */
struct InterleavedTriangle
{
    InterleavedVertex vertices[3];
};

static_assert(sizeof(InterleavedTriangle) == 0x60, "InterleavedTriangle layout mismatch");

#pragma pack(push, 1)
/**
 * Per-material render state, including an optional compiled display list
 * and a pointer to a pre-built interleaved vertex buffer.
 *
 * Original struct layout (sizeof = 0xE):
 *   +0x00  byte                flags
 *   +0x01  InterleavedVertex*  vertexBuffer
 *   +0x05  GLuint              displayListId
 *   +0x09  short               vertexCount
 *   +0x0B  short               triangleCount
 *   +0x0D  char                field_D
 */
struct Ms3dRenderState
{
    byte flags;
    InterleavedVertex* vertexBuffer;
    GLuint displayListId;
    short vertexCount;
    short triangleCount;
    char field_D;
};

static_assert(sizeof(Ms3dRenderState) == 0xE, "Ms3dRenderState layout mismatch");

/**
 * A group of triangles sharing a material.
 *
 * Original struct layout (sizeof = 0xC):
 *   +0x00  int   materialIndex
 *   +0x04  int   nTris
 *   +0x08  int*  pTris
 */
struct Ms3dGroup
{
    int materialIndex;
    int nTris;
    int* pTris;
};

static_assert(sizeof(Ms3dGroup) == 0xC, "Ms3dGroup layout mismatch");

/**
 * A material definition containing OpenGL lighting parameters,
 * a texture, and an optional alpha map.
 *
 * Original struct layout (sizeof = 0x54):
 *   +0x00  GLfloat[4]  ambient
 *   +0x10  GLfloat[4]  diffuse
 *   +0x20  GLfloat[4]  specular
 *   +0x30  GLfloat[4]  emissive
 *   +0x40  GLfloat     shininess
 *   +0x44  GLuint      texture
 *   +0x48  char*       pTextureName
 *   +0x4C  GLuint      alphamap
 *   +0x50  char*       pAlphaMapName
 */
struct Ms3dMaterial
{
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat emissive[4];
    GLfloat shininess;
    GLuint texture;
    char* pTextureName;
    GLuint alphamap;
    char* pAlphaMapName;
};

static_assert(sizeof(Ms3dMaterial) == 0x54, "Ms3dMaterial layout mismatch");

/**
 * A triangle defined by vertex indices, per-vertex normals, and UV coordinates.
 *
 * Original struct layout (sizeof = 0x48):
 *   +0x00  float[3][3]  vertex_normals
 *   +0x24  float[3]     s
 *   +0x30  float[3]     t
 *   +0x3C  int[3]       vertex_indices
 */
struct Ms3dTriangle
{
    float vertex_normals[3][3];
    float s[3];
    float t[3];
    int vertex_indices[3];
};

static_assert(sizeof(Ms3dTriangle) == 0x48, "Ms3dTriangle layout mismatch");

/**
 * A single mesh vertex with a bone assignment and position.
 *
 * Original struct layout (sizeof = 0xD):
 *   +0x00  char      boneId
 *   +0x01  float[3]  position
 */
struct Ms3dVertex
{
    char boneId;
    float position[3];
};

static_assert(sizeof(Ms3dVertex) == 0xD, "Ms3dVertex layout mismatch");

#pragma pack(pop)

// ---------------------------------------------------------------------------
// Ms3dBundle
// ---------------------------------------------------------------------------

/**
 * A loaded Milkshape 3D model bundle. Contains all mesh data, materials,
 * joints, animation state, and render state for a single model.
 *
 * The 64-byte block from +0x04 to +0x43 contains fields not yet identified
 * from decompilation. They are represented as an opaque byte array until
 * the methods that access them are transcribed.
 *
 * Original struct layout (sizeof = 0x9E):
 *   +0x00  Ms3dBundle_VTable*        vtable
 *   +0x04  byte[64]                  _unknown_04         (unidentified)
 *   +0x44  char                      hasSkeletalAnimation
 *   +0x45  float                     magX
 *   +0x49  float                     magY
 *   +0x4D  float                     magZ
 *   +0x51  int                       nGroups
 *   +0x55  Ms3dGroup*                pGroups
 *   +0x59  int                       nMaterials
 *   +0x5D  Ms3dMaterial*             pMaterials
 *   +0x61  int                       nTris
 *   +0x65  Ms3dTriangle*             pTris
 *   +0x69  int                       nVerts
 *   +0x6D  Ms3dVertex*               pVerts
 *   +0x71  int                       nJoints
 *   +0x75  Ms3dJoint*                vJoints
 *   +0x79  IAnimationController*     pAnimationController
 *   +0x7D  double                    animationDurationMs
 *   +0x85  float                     fAnimationFps
 *   +0x89  int                       iTotalFrames
 *   +0x8D  char                      isLoopAnimation
 *   +0x8E  byte[2]                   _padding_8E
 *   +0x90  short                     rightWristJointIndex
 *   +0x92  Ms3dRenderState*          pRenderState
 *   +0x96  char*                     materialVisibility
 *   +0x9A  short*                    nTrianglesPerMaterial
 */
class Ms3dBundle
{
public:
    byte _unknown_04[64];
    char hasSkeletalAnimation;
    float magX;
    float magY;
    float magZ;
    int nGroups;
    Ms3dGroup* pGroups;
    int nMaterials;
    Ms3dMaterial* pMaterials;
    int nTris;
    Ms3dTriangle* pTris;
    int nVerts;
    Ms3dVertex* pVerts;
    int nJoints;
    Ms3dJoint* vJoints;
    IAnimationController* pAnimationController;
    double animationDurationMs;
    float fAnimationFps;
    int iTotalFrames;
    char isLoopAnimation;
    byte _padding_8E[2];
    short rightWristJointIndex;
    Ms3dRenderState* pRenderState;
    char* materialVisibility;
    short* nTrianglesPerMaterial;

    Ms3dBundle();
    ~Ms3dBundle();

    void Deconstruct();
    void Init();
    void Delete(bool free);
    void Erase(bool free);

    bool LoadFromResource(LPCSTR resourceId, LPCSTR resourceType);
    bool LoadFromHWF(const char* asset_name, FILE* hwfStream, void* hwfAppendix, unsigned int hwfAppendixEntriesCount);
    bool ParseMilkshape(const byte* data, const char* modelName);

    void Render();
    void RenderModel();
    void RenderModelShiny();
    void RenderModelComplex();
    void RenderModelSimple();
    void RenderModelGroup(short groupIndex);
    void RenderImmediate();
    void RenderCompiled();

    GLboolean ApplyMaterial();
    void ApplyTextures(FILE* hwfStream, void* hwfAppendix, unsigned int hwfAppendixEntriesCount);
    void ReplaceTexture(const char* oldTexture, const char* newTexture);

    int BuildVertexBuffer(Ms3dRenderState* materialInfo, short materialSlot);
    void CompileMaterialDisplayLists();
    void* StoreKeyframe(int jointIndex, int keyframeIndex, float keyframeTimeMs, float (*data)[3], bool isRotation);
    void SkinVerticesToSkeleton();
    void ResetJoints();
    void UpdateSkeletalAnimation();
};

#endif // MIDIJAM_MS3DBUNDLE_H

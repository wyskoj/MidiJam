//
// Created by Jacob on 11/15/2025.
//

#ifndef MIDIJAM_MS3DBUNDLE_H
#define MIDIJAM_MS3DBUNDLE_H
#include <cstdio>
#include <vector>
#include <windows.h>
#include <GL/gl.h>
#include "AnimationController.h"
#include "MatrixMath.h"

struct Ms3dJointMapping // sizeof=0x8
{
    int index;
    char *name;
};

struct InterleavedVertex // sizeof=0x20
{
    float position[3];
    float texCoord[2];
    float normal[3];
};

struct InterleavedTriangle // sizeof=0x60
{
    InterleavedVertex vertices[3];
};

struct Ms3dRenderState // sizeof=0xE
{
    byte flags;
    InterleavedVertex *vertexBuffer;
    GLuint displayListId;
    __int16 vertexCount;
    __int16 triangleCount;
    char field_D;
};

union KeyframeData // sizeof=0xC
{
    float rotation[3];
    float position[3];
};

struct Ms3dKeyframePacked // sizeof=0x14
{
    int jointIndex;
    float timeMs;
    KeyframeData data;
};

struct Ms3dVertex {
    char boneId;
    float position[3];
};

struct Ms3dTriangle {
    float vertex_normals[3][3];
    float s[3];
    float t[3];
    int vertex_indices[3];
};

struct Ms3dGroup {
    int materialIndex;
    int nTris;
    int *pTris;
};

struct Ms3dMaterial // sizeof=0x54
{
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat emissive[4];
    GLfloat shininess;
    GLuint texture;
    char *pTextureName;
    GLuint alphamap;
    char *pAlphaMapName;
};

struct Ms3dJoint // sizeof=0xF4
{
    float rotation[3];
    float position[3];
    MatrixMath worldTransformationMatrix;
    MatrixMath localTransformationMatrix;
    int nKeyframesRot;
    int nKeyframesTrans;
    Ms3dKeyframePacked *pKeyframesTrans;
    Ms3dKeyframePacked *pKeyFramesRot;
    int currentRotKeyframe;
    int currentTransKeyframe;
    MatrixMath finalTransformationMatrix;
    int parentIndex;
};

class Ms3dBundle {
public:
    float magX;
    float magY;
    float magZ;
    int nGroups;
    Ms3dGroup *pGroups;
    int nMaterials;
    Ms3dMaterial *pMaterials;
    int nTris;
    Ms3dTriangle *pTris;
    int nVerts;
    Ms3dVertex *pVerts;
    int nJoints;
    std::vector<Ms3dJoint> vJoints;
    AnimationController *pAnimationController;
    double animationDurationMs;
    float fAnimationFps;
    int iTotalFrames;
    bool isLoopAnimation;
    bool hasSkeletalAnimation;
    short rightWristJointIndex;
    Ms3dRenderState *pRenderState;
    char *materialVisibility;
    __int16 *nTrianglesPerMaterial;

    Ms3dBundle();

    ~Ms3dBundle();

    void Init();

    GLboolean ApplyMaterial() const;

    void ApplyTextures(FILE *hwfStream, void *hwfAppendix, unsigned int hwfAppendixEntriesCount) const;

    int BuildVertexBuffer(Ms3dRenderState *materialInfo, __int16 materialSlot) const;

    void CompileMaterialDisplayLists() const;

    void Deconstruct();

    bool LoadFromHWF(const char *asset_name, FILE *hwfStream, void *hwfAppendix, unsigned int hwfAppendixEntriesCount);

    bool LoadFromResource(LPCSTR resourceId, LPCSTR resourceType);

    bool ParseMilkshape(char *ms3d_file_data, char *model_name);

    void Render();

    void RenderCompiled();

    void RenderImmediate();

    void RenderModel();

    void RenderModelComplex();

    void RenderModelGroup(short groupIndex);

    void RenderModelShiny();

    void RenderModelSimple();

    void ReplaceTexture(const char *old_texture, const char *new_texture);

    void ResetJoints();

    void SkinVerticesToSkeleton();

    void *StoreKeyframe(int jointIndex,
        int keyframeIndex,
        float keyframeTimeMs,
        float (*data)[3],
        bool isRotation);

    void UpdateSkeletalAnimation();
};

void __stdcall OnFailLoadMilkshape(const char *modelName);


#endif //MIDIJAM_MS3DBUNDLE_H

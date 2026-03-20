//
// Created by Jacob on 3/17/2026.
//

#include "Ms3dBundle.h"

#include "ms3d_format.h"
#include "math/scalar.h"
#include "math/Quaternion.h"
#include "animation/AnimationController.h"

#include <GL/gl.h>
#include <cstring>
#include <cstdio>


// Forward declarations for render/texture functions not yet transcribed.
GLuint LoadTextureFromHwf(
    const char*  textureName,
    FILE*        hwfStream,
    void*        hwfAppendix,
    unsigned int hwfAppendixEntriesCount);

void OnFailLoadMilkshape(const char* modelName);

// FUNCTION: MIDIJAM 0x43A130
Ms3dBundle::Ms3dBundle()
{
    nGroups              = 0;
    pGroups              = nullptr;
    nMaterials           = 0;
    pMaterials           = nullptr;
    nTris                = 0;
    pTris                = nullptr;
    nVerts               = 0;
    pVerts               = nullptr;
    nJoints              = 0;
    vJoints              = nullptr;
    pAnimationController = CreateAnimationController();
    isLoopAnimation      = 0;
    hasSkeletalAnimation = 0;
    rightWristJointIndex = -1;
    ResetJoints();
}

// FUNCTION: MIDIJAM 0x438EE0
Ms3dBundle::~Ms3dBundle()
{
    Deconstruct();
}

// FUNCTION: MIDIJAM 0x43A1E0
void Ms3dBundle::Deconstruct()
{
    for (int i = 0; i < nGroups; ++i)
        free(pGroups[i].pTris);

    for (int j = 0; j < nMaterials; ++j)
    {
        free(pMaterials[j].pTextureName);
        free(pMaterials[j].pAlphaMapName);
    }

    nGroups = 0;
    if (pGroups)
    {
        free(pGroups);
        pGroups = nullptr;
    }

    nMaterials = 0;
    if (pMaterials)
    {
        free(pMaterials);
        pMaterials = nullptr;
    }

    nTris = 0;
    if (pTris)
    {
        free(pTris);
        pTris = nullptr;
    }

    nVerts = 0;
    if (pVerts)
    {
        free(pVerts);
        pVerts = nullptr;
    }

    for (int k = 0; k < nJoints; ++k)
    {
        free(vJoints[k].pKeyFramesRot);
        free(vJoints[k].pKeyframesTrans);
    }

    nJoints = 0;
    if (vJoints)
    {
        Ms3dJointCollection_Delete(
            reinterpret_cast<Ms3dJointCollection*>(&vJoints));
        vJoints = nullptr;
    }

    // TODO: the animation controller is allocated with a header block preceding
    // the pointer — HIDWORD(pAnimationController[-1].elapsed) checks a field
    // in that header. This destruction pattern is not fully understood.
    if (pAnimationController)
    {
        pAnimationController->Destroy();
        pAnimationController = nullptr;
    }
}

// FUNCTION: MIDIJAM 0x439FC0
void Ms3dBundle::Delete(bool free)
{
    Deconstruct();
    if (free)
        ::free(this);
}

// FUNCTION: MIDIJAM 0x43C1A0
void Ms3dBundle::Erase(bool free)
{
    Deconstruct();
    if (free)
        ::free(this);
}

// FUNCTION: MIDIJAM 0x438F60
bool Ms3dBundle::LoadFromResource(LPCSTR resourceId, LPCSTR resourceType)
{
    HRSRC  hResourceInfo = FindResourceA(nullptr, resourceId, resourceType);
    DWORD  resourceSize  = SizeofResource(nullptr, hResourceInfo);
    HGLOBAL hResourceData = LoadResource(nullptr, hResourceInfo);
    void*  pResourceData  = LockResource(hResourceData);

    void* pBuffer = malloc(resourceSize);
    memcpy(pBuffer, pResourceData, resourceSize);
    FreeResource(hResourceData);

    char resourceName[200];
    sprintf(resourceName, "RESOURCE NUMBER:%d", reinterpret_cast<int>(resourceId));

    ParseMilkshape(static_cast<const byte*>(pBuffer), resourceName);
    free(pBuffer);

    return true;
}

// FUNCTION: MIDIJAM 0x439070
bool Ms3dBundle::LoadFromHWF(
    const char* asset_name,
    FILE*        hwfStream,
    void*        hwfAppendix,
    unsigned int hwfAppendixEntriesCount)
{
    void *asset_buffer; // [esp+Ch] [ebp-8h]
    unsigned int i; // [esp+10h] [ebp-4h]

    fseek(hwfStream, 0, 0);
    for (i = 0; i < hwfAppendixEntriesCount; ++i) {
        // Find asset in appendix
        if (!_stricmp(asset_name, (const char *) hwfAppendix + 264 * i)) {
            // Allocate memory for asset
            asset_buffer = operator new(*((int *) hwfAppendix + 0x42 * i + 0x41));
            // Read into buffer
            fread(asset_buffer, 1u, *((int *) hwfAppendix + 0x42 * i + 0x41), hwfStream);
            // Process data
            ParseMilkshape(
                reinterpret_cast<const byte*>((char *) asset_buffer),
                (char *) hwfAppendix + 264 * i);
            // Free buffer
            free(asset_buffer);
            return TRUE;
        }
        fseek(hwfStream, *((int *) hwfAppendix + 66 * i + 65), 1);
    }
    return FALSE;
}

// FUNCTION: MIDIJAM 0x43AC00
GLboolean Ms3dBundle::ApplyMaterial()
{
    GLboolean isEnabled = glIsEnabled(GL_TEXTURE_2D);

    for (short i = 0; i < nMaterials + 1; ++i)
    {
        if (materialVisibility[i] == 1)
        {
            int materialIndex = i - 1;
            if (materialIndex >= 0)
            {
                glMaterialfv(GL_FRONT, GL_AMBIENT,   pMaterials[materialIndex].ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE,   pMaterials[materialIndex].diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR,  pMaterials[materialIndex].specular);
                glMaterialfv(GL_FRONT, GL_EMISSION,  pMaterials[materialIndex].emissive);
                glMaterialf(GL_FRONT,  GL_SHININESS, pMaterials[materialIndex].shininess);
                glBindTexture(GL_TEXTURE_2D,          pMaterials[materialIndex].texture);
                glEnable(GL_TEXTURE_2D);
                return isEnabled;
            }
        }
    }

    return isEnabled;
}

// FUNCTION: MIDIJAM 0x43B180
void Ms3dBundle::ApplyTextures(
    FILE*        hwfStream,
    void*        hwfAppendix,
    unsigned int hwfAppendixEntriesCount)
{
    for (int i = 0; i < nMaterials; ++i)
    {
        if (strlen(pMaterials[i].pTextureName))
            pMaterials[i].texture = LoadTextureFromHwf(
                pMaterials[i].pTextureName,
                hwfStream,
                hwfAppendix,
                hwfAppendixEntriesCount);
        else
            pMaterials[i].texture = 0;

        if (strlen(pMaterials[i].pAlphaMapName))
            pMaterials[i].alphamap = LoadTextureFromHwf(
                pMaterials[i].pAlphaMapName,
                hwfStream,
                hwfAppendix,
                hwfAppendixEntriesCount);
        else
            pMaterials[i].alphamap = 0;
    }
}

// FUNCTION: MIDIJAM 0x43BE80
int Ms3dBundle::BuildVertexBuffer(Ms3dRenderState* materialInfo, short materialSlot)
{
    // --- Step 1: Count total triangles for this material slot ---
    int totalTriangles = 0;
    for (int groupIndex = 0; groupIndex < nGroups; ++groupIndex)
    {
        int materialIndex = pGroups[groupIndex].materialIndex;
        if ((materialIndex >= 0 && materialSlot - 1 == materialIndex)
            || (materialIndex < 0 && !materialSlot))
        {
            totalTriangles += pGroups[groupIndex].nTris;
        }
    }

    // --- Step 2: Allocate interleaved vertex buffer ---
    InterleavedVertex* vertexBuffer = static_cast<InterleavedVertex*>(
        malloc(sizeof(InterleavedTriangle) * totalTriangles));
    materialInfo->vertexBuffer = vertexBuffer;

    // --- Step 3: Fill vertex buffer ---
    for (int groupIndex = 0; groupIndex < nGroups; ++groupIndex)
    {
        int groupMaterialIndex = pGroups[groupIndex].materialIndex;
        if ((groupMaterialIndex >= 0 && materialSlot - 1 == groupMaterialIndex)
            || (groupMaterialIndex < 0 && !materialSlot))
        {
            for (int triIdx = 0; triIdx < pGroups[groupIndex].nTris; ++triIdx)
            {
                int globalTriIdx       = pGroups[groupIndex].pTris[triIdx];
                Ms3dTriangle* pTriangle = &pTris[globalTriIdx];

                for (int vertexCorner = 0; vertexCorner < 3; ++vertexCorner)
                {
                    int vertexIndex = pTriangle->vertex_indices[vertexCorner];

                    if (pVerts[vertexIndex].boneId == -1)
                    {
                        // Static vertex — copy directly without transformation.
                        memcpy(vertexBuffer->position,
                               pVerts[vertexIndex].position,
                               sizeof(vertexBuffer->position));
                        memcpy(vertexBuffer->normal,
                               pTriangle->vertex_normals[vertexCorner],
                               sizeof(vertexBuffer->normal));
                        vertexBuffer->texCoord[0] = pTriangle->s[vertexCorner];
                        vertexBuffer->texCoord[1] = pTriangle->t[vertexCorner];
                    }
                    else
                    {
                        // Skinned vertex — transform by bone matrix.
                        MatrixMath* boneMatrix =
                            &vJoints[pVerts[vertexIndex].boneId].finalTransformationMatrix;

                        Quaternion tempPosition;
                        tempPosition.FromVector(pVerts[vertexIndex].position);
                        tempPosition.TransformByMatrix(boneMatrix);
                        memcpy(vertexBuffer->position, &tempPosition, sizeof(vertexBuffer->position));

                        Quaternion normal;
                        normal.FromVector(pTriangle->vertex_normals[vertexCorner]);
                        normal.TransformByMatrixRotation(boneMatrix);
                        normal.Normalize();
                        memcpy(vertexBuffer->normal, &normal, sizeof(vertexBuffer->normal));

                        vertexBuffer->texCoord[0] = pTriangle->s[vertexCorner];
                        vertexBuffer->texCoord[1] = pTriangle->t[vertexCorner];
                    }

                    ++vertexBuffer;
                }
            }
        }
    }

    materialInfo->flags = 1;
    return totalTriangles;
}

// FUNCTION: MIDIJAM 0x43BCD0
void Ms3dBundle::CompileMaterialDisplayLists()
{
    for (int materialIndex = 0; materialIndex < nMaterials + 1; ++materialIndex)
    {
        if (materialVisibility[materialIndex] != 1)
            continue;

        nTrianglesPerMaterial[materialIndex] = static_cast<short>(
            BuildVertexBuffer(&pRenderState[materialIndex],
                              static_cast<short>(materialIndex)));

        pRenderState[materialIndex].displayListId = glGenLists(1);
        if (!pRenderState[materialIndex].displayListId)
            return;

        glNewList(pRenderState[materialIndex].displayListId, GL_COMPILE);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);

        glVertexPointer(
            3, GL_FLOAT, sizeof(InterleavedVertex),
            pRenderState[materialIndex].vertexBuffer);
        glTexCoordPointer(
            2, GL_FLOAT, sizeof(InterleavedVertex),
            pRenderState[materialIndex].vertexBuffer->texCoord);
        glNormalPointer(
            GL_FLOAT, sizeof(InterleavedVertex),
            pRenderState[materialIndex].vertexBuffer->normal);

        glDrawArrays(GL_TRIANGLES, 0, 3 * nTrianglesPerMaterial[materialIndex]);

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glEndList();
    }
}

// FUNCTION: MIDIJAM 0x43A470
void Ms3dBundle::RenderModelShiny()
{
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    RenderModel();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
}

// FUNCTION: MIDIJAM 0x43A4B0
void Ms3dBundle::RenderModel()
{
    short canUseSimpleRendering = 1;
    for (short i = 0; i < nMaterials + 1; ++i)
    {
        if (materialVisibility[i] == 1 && !pRenderState[i].displayListId)
            canUseSimpleRendering = 0;
    }

    if (canUseSimpleRendering && !hasSkeletalAnimation)
        RenderModelSimple();
    else
        RenderModelComplex();
}

// FUNCTION: MIDIJAM 0x43A550
void Ms3dBundle::Render()
{
    short canUseDisplayLists = 1;
    for (short i = 0; i < nMaterials + 1; ++i)
    {
        if (materialVisibility[i] == 1 && !pRenderState[i].displayListId)
            canUseDisplayLists = 0;
    }

    if (canUseDisplayLists && !hasSkeletalAnimation)
        RenderCompiled();
    else
        RenderImmediate();
}

// FUNCTION: MIDIJAM 0x43ADC0
void Ms3dBundle::RenderCompiled()
{
    for (short i = 0; i < nMaterials + 1; ++i)
        glCallList(pRenderState[i].displayListId);
}

// FUNCTION: MIDIJAM 0x43AD70
void Ms3dBundle::RenderImmediate()
{
    if (hasSkeletalAnimation)
        UpdateSkeletalAnimation();

    for (int groupIndex = 0; groupIndex < nGroups; ++groupIndex)
        RenderModelGroup(static_cast<short>(groupIndex));
}

// FUNCTION: MIDIJAM 0x43A5F0
void Ms3dBundle::RenderModelComplex()
{
    if (hasSkeletalAnimation)
        UpdateSkeletalAnimation();

    GLboolean wasTexturingEnabled = glIsEnabled(GL_TEXTURE_2D);

    for (short materialSlot = 0; materialSlot < nMaterials + 1; ++materialSlot)
    {
        if (materialVisibility[materialSlot] != 1)
            continue;

        int materialIndex = materialSlot - 1;

        if (materialIndex < 0)
        {
            if (*materialVisibility > 0)
            {
                glDisable(GL_TEXTURE_2D);
                for (int i = 0; i < nGroups; ++i)
                {
                    if (pGroups[i].materialIndex < 0)
                        RenderModelGroup(static_cast<short>(i));
                }
            }
        }
        else
        {
            glMaterialfv(GL_FRONT, GL_AMBIENT,    pMaterials[materialIndex].ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE,    pMaterials[materialIndex].diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR,   pMaterials[materialIndex].specular);
            glMaterialfv(GL_FRONT, GL_EMISSION,   pMaterials[materialIndex].emissive);
            glMaterialf(GL_FRONT,  GL_SHININESS,  pMaterials[materialIndex].shininess);

            if (pMaterials[materialIndex].alphamap)
            {
                for (int groupIndex = 0; groupIndex < nGroups; ++groupIndex)
                {
                    if (pGroups[groupIndex].materialIndex != materialIndex)
                        continue;

                    glDisable(GL_DEPTH_TEST);
                    glBlendFunc(GL_DST_COLOR, 0);
                    glEnable(GL_BLEND);
                    glBindTexture(GL_TEXTURE_2D, pMaterials[materialIndex].alphamap);
                    RenderModelGroup(static_cast<short>(groupIndex));
                    glBlendFunc(GL_ONE, GL_ONE);
                    glBindTexture(GL_TEXTURE_2D, pMaterials[materialIndex].texture);
                    RenderModelGroup(static_cast<short>(groupIndex));
                    glDisable(GL_BLEND);
                    glEnable(GL_DEPTH_TEST);
                }
            }
            else if (pMaterials[materialIndex].texture)
            {
                glBindTexture(GL_TEXTURE_2D, pMaterials[materialIndex].texture);
                glEnable(GL_TEXTURE_2D);
                for (int j = 0; j < nGroups; ++j)
                {
                    if (pGroups[j].materialIndex == materialIndex)
                        RenderModelGroup(static_cast<short>(j));
                }
            }
            else
            {
                glDisable(GL_TEXTURE_2D);
                for (int k = 0; k < nGroups; ++k)
                {
                    if (pGroups[k].materialIndex == materialIndex)
                        RenderModelGroup(static_cast<short>(k));
                }
            }
        }
    }

    if (wasTexturingEnabled)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);
}

// FUNCTION: MIDIJAM 0x43A950
void Ms3dBundle::RenderModelSimple()
{
    GLboolean isEnabled = glIsEnabled(GL_TEXTURE_2D);

    for (short materialIndex = 0; materialIndex < nMaterials + 1; ++materialIndex)
    {
        if (materialVisibility[materialIndex] != 1)
            continue;

        int materialPropertiesIndex = materialIndex - 1;

        if (materialPropertiesIndex < 0)
        {
            if (*materialVisibility > 0)
            {
                glDisable(GL_TEXTURE_2D);
                glCallList(pRenderState->displayListId);
            }
        }
        else
        {
            glMaterialfv(GL_FRONT, GL_AMBIENT,    pMaterials[materialPropertiesIndex].ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE,    pMaterials[materialPropertiesIndex].diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR,   pMaterials[materialPropertiesIndex].specular);
            glMaterialfv(GL_FRONT, GL_EMISSION,   pMaterials[materialPropertiesIndex].emissive);
            glMaterialf(GL_FRONT,  GL_SHININESS,  pMaterials[materialPropertiesIndex].shininess);

            if (pMaterials[materialPropertiesIndex].alphamap)
            {
                glDisable(GL_DEPTH_TEST);
                glBlendFunc(GL_DST_COLOR, 0);
                glEnable(GL_BLEND);
                glBindTexture(GL_TEXTURE_2D, pMaterials[materialPropertiesIndex].alphamap);
                glCallList(pRenderState[materialIndex].displayListId);
                glBlendFunc(GL_ONE, GL_ONE);
                glBindTexture(GL_TEXTURE_2D, pMaterials[materialPropertiesIndex].texture);
                glCallList(pRenderState[materialIndex].displayListId);
                glDisable(GL_BLEND);
                glEnable(GL_DEPTH_TEST);
            }
            else
            {
                if (pMaterials[materialPropertiesIndex].texture)
                {
                    glBindTexture(GL_TEXTURE_2D, pMaterials[materialPropertiesIndex].texture);
                    glEnable(GL_TEXTURE_2D);
                }
                else
                {
                    glDisable(GL_TEXTURE_2D);
                }
                glCallList(pRenderState[materialIndex].displayListId);
            }
        }
    }

    if (isEnabled)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);
}

// FUNCTION: MIDIJAM 0x43AE10
void Ms3dBundle::RenderModelGroup(short groupIndex)
{
    glBegin(GL_TRIANGLES);

    for (int triangleInGroup = 0; triangleInGroup < pGroups[groupIndex].nTris; ++triangleInGroup)
    {
        int triangleIndex      = pGroups[groupIndex].pTris[triangleInGroup];
        Ms3dTriangle* pTriangle = &pTris[triangleIndex];

        for (int vertexInTriangle = 0; vertexInTriangle < 3; ++vertexInTriangle)
        {
            int vertexIndex = pTriangle->vertex_indices[vertexInTriangle];

            glTexCoord2f(pTriangle->s[vertexInTriangle], pTriangle->t[vertexInTriangle]);

            if (pVerts[vertexIndex].boneId == -1)
            {
                // Static vertex — no skeletal transformation needed.
                glNormal3fv(pTriangle->vertex_normals[vertexInTriangle]);
                glVertex3fv(pVerts[vertexIndex].position);
            }
            else
            {
                // Skinned vertex — transform by bone matrix.
                MatrixMath* pJointMatrix =
                    &vJoints[pVerts[vertexIndex].boneId].finalTransformationMatrix;

                Quaternion transformedNormal;
                transformedNormal.FromVector(pTriangle->vertex_normals[vertexInTriangle]);
                transformedNormal.TransformByMatrixRotation(pJointMatrix);
                transformedNormal.Normalize();
                glNormal3fv(reinterpret_cast<const GLfloat*>(&transformedNormal));

                Quaternion transformedVertex;
                transformedVertex.FromVector(pVerts[vertexIndex].position);
                transformedVertex.TransformByMatrix(pJointMatrix);
                glVertex3fv(reinterpret_cast<const GLfloat*>(&transformedVertex));
            }
        }
    }

    glEnd();
}

// FUNCTION: MIDIJAM 0x43B290
void Ms3dBundle::ReplaceTexture(const char* oldTexture, const char* newTexture)
{
    for (int i = 0; i < nMaterials; ++i)
    {
        if (!strlen(pMaterials[i].pTextureName))
            continue;

        if (_stricmp(pMaterials[i].pTextureName, oldTexture) != 0)
            continue;

        free(pMaterials[i].pTextureName);
        size_t newLen = strlen(newTexture);
        pMaterials[i].pTextureName = static_cast<char*>(malloc(newLen + 1));
        strcpy(pMaterials[i].pTextureName, newTexture);
    }
}

// FUNCTION: MIDIJAM 0x43B730
void Ms3dBundle::ResetJoints()
{
    for (int i = 0; i < nJoints; ++i)
    {
        vJoints[i].currentRotKeyframe   = 0;
        vJoints[i].currentTransKeyframe = 0;
        vJoints[i].finalTransformationMatrix.Copy(&vJoints[i].worldTransformationMatrix);
    }
    pAnimationController->Update();
}

// FUNCTION: MIDIJAM 0x43B500
void Ms3dBundle::SkinVerticesToSkeleton()
{
    // --- Step 1: Calculate bone world transformation matrices ---
    for (int boneIndex = 0; boneIndex < nJoints; ++boneIndex)
    {
        Ms3dJoint* bone = &vJoints[boneIndex];
        bone->localTransformationMatrix.SetTranslation(bone->position);

        if (bone->parentIndex == -1)
        {
            bone->worldTransformationMatrix.Copy(&bone->localTransformationMatrix);
        }
        else
        {
            bone->worldTransformationMatrix.Copy(
                &vJoints[bone->parentIndex].worldTransformationMatrix);
            bone->worldTransformationMatrix.Multiply(&bone->localTransformationMatrix);
        }
    }

    // --- Step 2: Transform vertices by their assigned bone ---
    for (int vertexIndex = 0; vertexIndex < nVerts; ++vertexIndex)
    {
        Ms3dVertex* vertex = &pVerts[vertexIndex];
        if (vertex->boneId != -1)
        {
            MatrixMath* boneMatrix = &vJoints[vertex->boneId].worldTransformationMatrix;
            boneMatrix->RemoveTranslationFromVector(&vertex->position);
            boneMatrix->TransformVectorByMatrixRotation(&vertex->position);
        }
    }

    // --- Step 3: Transform vertex normals (rotation only) ---
    for (int triangleIndex = 0; triangleIndex < nTris; ++triangleIndex)
    {
        Ms3dTriangle* triangle = &pTris[triangleIndex];
        for (int i = 0; i < 3; ++i)
        {
            Ms3dVertex* vertexTri = &pVerts[triangle->vertex_indices[i]];
            if (vertexTri->boneId != -1)
            {
                vJoints[vertexTri->boneId].worldTransformationMatrix.TransformVectorByMatrixRotation(&triangle->vertex_normals[i]);
            }
        }
    }
}

// FUNCTION: MIDIJAM 0x43B470
void* Ms3dBundle::StoreKeyframe(
    int    jointIndex,
    int    keyframeIndex,
    float  keyframeTimeMs,
    float  (*data)[3],
    bool   isRotation)
{
    Ms3dKeyframePacked* destination;
    if (isRotation)
        destination = &vJoints[jointIndex].pKeyFramesRot[keyframeIndex];
    else
        destination = &vJoints[jointIndex].pKeyframesTrans[keyframeIndex];

    destination->jointIndex = jointIndex;
    destination->timeMs     = keyframeTimeMs;
    return memcpy(&destination->data, data, sizeof(destination->data));
}

// FUNCTION: MIDIJAM 0x43B7E0
void Ms3dBundle::UpdateSkeletalAnimation()
{
    double animationTime = pAnimationController->ComputeElapsed();

    if (animationTime > animationDurationMs)
    {
        if (isLoopAnimation)
        {
            ResetJoints();
            animationTime = 0.0;
        }
        else
        {
            animationTime = animationDurationMs;
        }
    }

    for (int jointIndex = 0; jointIndex < nJoints; ++jointIndex)
    {
        MatrixMath rotationMatrix;
        Ms3dJoint* pJoint = &vJoints[jointIndex];

        if (!pJoint->nKeyframesRot && !pJoint->nKeyframesTrans)
        {
            // No keyframes — use world transformation as final.
            pJoint->finalTransformationMatrix.Copy(&pJoint->worldTransformationMatrix);
            continue;
        }

        // --- Translation interpolation ---
        float translation[3];
        int keyframeIndex;

        for (keyframeIndex = pJoint->currentRotKeyframe;
             keyframeIndex < pJoint->nKeyframesTrans
                 && pJoint->pKeyframesTrans[keyframeIndex].timeMs < animationTime;
             ++keyframeIndex)
        {
        }
        pJoint->currentRotKeyframe = keyframeIndex;

        if (keyframeIndex == 0)
        {
            memcpy(translation, &pJoint->pKeyframesTrans->data, sizeof(translation));
        }
        else if (keyframeIndex == pJoint->nKeyframesTrans)
        {
            memcpy(translation, &pJoint->pKeyframesTrans[keyframeIndex - 1].data, sizeof(translation));
        }
        else
        {
            Ms3dKeyframePacked* nextKey = &pJoint->pKeyframesTrans[keyframeIndex];
            Ms3dKeyframePacked* prevKey = &pJoint->pKeyframesTrans[keyframeIndex - 1];
            float timeDelta             = nextKey->timeMs - prevKey->timeMs;
            float factor                = static_cast<float>((animationTime - prevKey->timeMs) / timeDelta);
            translation[0] = (nextKey->data.loc.x - prevKey->data.loc.x) * factor + prevKey->data.loc.x;
            translation[1] = (nextKey->data.loc.y - prevKey->data.loc.y) * factor + prevKey->data.loc.y;
            translation[2] = (nextKey->data.loc.z - prevKey->data.loc.z) * factor + prevKey->data.loc.z;
        }

        // --- Rotation interpolation ---
        for (keyframeIndex = pJoint->currentTransKeyframe;
             keyframeIndex < pJoint->nKeyframesRot
                 && pJoint->pKeyFramesRot[keyframeIndex].timeMs < animationTime;
             ++keyframeIndex)
        {
        }
        pJoint->currentTransKeyframe = keyframeIndex;

        if (keyframeIndex == 0)
        {
            rotationMatrix.CreateRotationMatrixEulerZYX(
                reinterpret_cast<const float(*)[3]>(&pJoint->pKeyFramesRot->data));
        }
        else if (keyframeIndex == pJoint->nKeyframesRot)
        {
            rotationMatrix.CreateRotationMatrixEulerZYX(
                reinterpret_cast<const float(*)[3]>(
                    &pJoint->pKeyFramesRot[keyframeIndex - 1].data));
        }
        else
        {
            Ms3dKeyframePacked* nextKey = &pJoint->pKeyFramesRot[keyframeIndex];
            Ms3dKeyframePacked* prevKey = &pJoint->pKeyFramesRot[keyframeIndex - 1];
            float timeDelta             = nextKey->timeMs - prevKey->timeMs;
            float factor                = static_cast<float>((animationTime - prevKey->timeMs) / timeDelta);
            float interpolatedRotation[3];
            interpolatedRotation[0] = (nextKey->data.rot.x - prevKey->data.rot.x) * factor + prevKey->data.rot.x;
            interpolatedRotation[1] = (nextKey->data.rot.y - prevKey->data.rot.y) * factor + prevKey->data.rot.y;
            interpolatedRotation[2] = (nextKey->data.rot.z - prevKey->data.rot.z) * factor + prevKey->data.rot.z;
            rotationMatrix.CreateRotationMatrixEulerZYX(
                reinterpret_cast<const float(*)[3]>(interpolatedRotation));
        }

        rotationMatrix.SetTranslation(translation);

        // --- Build final transformation matrix ---
        MatrixMath localTransform;
        localTransform.Copy(&pJoint->localTransformationMatrix);
        localTransform.Multiply(&rotationMatrix);

        if (pJoint->parentIndex == -1)
        {
            pJoint->finalTransformationMatrix.Copy(&localTransform);
        }
        else
        {
            pJoint->finalTransformationMatrix.Copy(
                &vJoints[pJoint->parentIndex].finalTransformationMatrix);
            pJoint->finalTransformationMatrix.Multiply(&localTransform);
        }
    }
}

// FUNCTION: MIDIJAM 0x438F00
void OnFailLoadMilkshape(const char* modelName)
{
    // Error handler for failed Milkshape model loads.
    // Displays an error message and terminates the application.
    // Full implementation transcribed separately.
}

// FUNCTION: MIDIJAM 0x439170
bool Ms3dBundle::ParseMilkshape(const byte* data, const char* modelName)
{
    // ParseMilkshape takes model_name as a second parameter in the original.
    // The reconstruction uses the resource name passed through LoadFromResource/LoadFromHWF.
    // TODO: reconcile model_name parameter — currently unused in this stub.
    const char* ms3d_cursor = reinterpret_cast<const char*>(data);
    const char* ms3d_data_1 = ms3d_cursor;

    // Validate MS3D header — must start with "MS3D000000" and be at least version 3.
    if (strncmp(ms3d_data_1, "MS3D000000", 10) != 0
        || ms3d_data_1[10] < 3)
    {
        OnFailLoadMilkshape("<unknown>");
        return false;
    }

    ms3d_cursor += 14; // skip header (10 bytes magic + 4 bytes version)

    // --- Vertices ---
    nVerts = *reinterpret_cast<const short*>(ms3d_cursor);
    pVerts = static_cast<Ms3dVertex*>(malloc(sizeof(Ms3dVertex) * nVerts));
    ms3d_cursor += 2;

    for (int i = 0; i < nVerts; ++i)
    {
        const ms3d_vertex_t* vert = reinterpret_cast<const ms3d_vertex_t*>(ms3d_cursor);
        pVerts[i].boneId = vert->boneId;
        memcpy(pVerts[i].position, vert->vertex, sizeof(pVerts[i].position));

        if (i == 0)
        {
            magX = fAbs(pVerts[0].position[0]);
            magY = fAbs(pVerts[0].position[1]);
            magZ = fAbs(pVerts[0].position[2]);
        }
        else
        {
            if (fAbs(pVerts[i].position[0]) > magX) magX = fAbs(pVerts[i].position[0]);
            if (fAbs(pVerts[i].position[1]) > magY) magY = fAbs(pVerts[i].position[1]);
            if (fAbs(pVerts[i].position[2]) > magZ) magZ = fAbs(pVerts[i].position[2]);
        }

        ms3d_cursor += sizeof(ms3d_vertex_t);
    }

    // --- Triangles ---
    nTris = *reinterpret_cast<const short*>(ms3d_cursor);
    pTris = static_cast<Ms3dTriangle*>(malloc(sizeof(Ms3dTriangle) * nTris));
    ms3d_cursor += 2;

    for (int i = 0; i < nTris; ++i)
    {
        const ms3d_triangle_t* tri = reinterpret_cast<const ms3d_triangle_t*>(ms3d_cursor);

        int vertexIndices[3];
        vertexIndices[0] = tri->vertexIndices[0];
        vertexIndices[1] = tri->vertexIndices[1];
        vertexIndices[2] = tri->vertexIndices[2];

        // MS3D stores t coordinates top-down; invert for OpenGL.
        float t[3];
        t[0] = 1.0f - tri->t[0];
        t[1] = 1.0f - tri->t[1];
        t[2] = 1.0f - tri->t[2];

        memcpy(&pTris[i], &tri->vertexNormals, 36);  // vertex normals
        memcpy(pTris[i].s, tri->s, sizeof(pTris[i].s));
        memcpy(pTris[i].t, t, sizeof(pTris[i].t));
        memcpy(pTris[i].vertex_indices, vertexIndices, sizeof(pTris[i].vertex_indices));

        ms3d_cursor += sizeof(ms3d_triangle_t);
    }

    // --- Groups ---
    nGroups = *reinterpret_cast<const short*>(ms3d_cursor);
    pGroups = static_cast<Ms3dGroup*>(malloc(sizeof(Ms3dGroup) * nGroups));
    ms3d_cursor += 2;

    for (int i = 0; i < nGroups; ++i)
    {
        ++ms3d_cursor;          // skip flags
        ms3d_cursor += 32;      // skip name

        unsigned short nGroupTris = *reinterpret_cast<const unsigned short*>(ms3d_cursor);
        ms3d_cursor += 2;

        int* pGroupTris = static_cast<int*>(malloc(sizeof(int) * nGroupTris));
        for (int j = 0; j < nGroupTris; ++j)
        {
            pGroupTris[j] = *reinterpret_cast<const short*>(ms3d_cursor);
            ms3d_cursor += 2;
        }

        char materialIndex = *ms3d_cursor++;
        pGroups[i].materialIndex = materialIndex;
        pGroups[i].nTris         = nGroupTris;
        pGroups[i].pTris         = pGroupTris;
    }

    // --- Materials ---
    nMaterials = *reinterpret_cast<const short*>(ms3d_cursor);
    pMaterials = static_cast<Ms3dMaterial*>(malloc(sizeof(Ms3dMaterial) * nMaterials));
    ms3d_cursor += 2;

    for (int i = 0; i < nMaterials; ++i)
    {
        const ms3d_material_t* mat = reinterpret_cast<const ms3d_material_t*>(ms3d_cursor);

        memcpy(&pMaterials[i].ambient,   mat->ambient,   sizeof(pMaterials[i].ambient));
        memcpy(&pMaterials[i].diffuse,   mat->diffuse,   sizeof(pMaterials[i].diffuse));
        memcpy(&pMaterials[i].specular,  mat->specular,  sizeof(pMaterials[i].specular));
        // emissive + shininess copied together (20 bytes) as the original does.
        memcpy(&pMaterials[i].emissive,  mat->emissive,  20);

        // Strip leading path separators and dots from texture name.
        short y = 0;
        while (mat->texture_name[y] == '.' || mat->texture_name[y] == '\\')
        {
            if (!mat->texture_name[++y]) { y = 0; break; }
        }
        size_t texLen = strlen(&mat->texture_name[y]);
        pMaterials[i].pTextureName = static_cast<char*>(malloc(texLen + 1));
        strcpy(pMaterials[i].pTextureName, &mat->texture_name[y]);

        // Strip leading path separators and dots from alpha map name.
        short l = 0;
        while (mat->alphamap_name[l] == '.' || mat->alphamap_name[l] == '\\')
        {
            if (!mat->alphamap_name[++l]) { l = 0; break; }
        }
        size_t alphaLen = strlen(&mat->alphamap_name[l]);
        pMaterials[i].pAlphaMapName = static_cast<char*>(malloc(alphaLen + 1));
        strcpy(pMaterials[i].pAlphaMapName, &mat->alphamap_name[l]);

        ms3d_cursor += sizeof(ms3d_material_t);
    }

    // Allocate render state and visibility arrays (nMaterials + 1 for the
    // default/no-material slot at index 0).
    pRenderState = static_cast<Ms3dRenderState*>(
        malloc(sizeof(Ms3dRenderState) * (nMaterials + 1)));
    materialVisibility    = static_cast<char*>(malloc(nMaterials + 1));
    nTrianglesPerMaterial = static_cast<short*>(malloc(sizeof(short) * (nMaterials + 1)));

    for (int i = 0; i < nMaterials + 1; ++i)
    {
        pRenderState[i].flags         = 0;
        pRenderState[i].vertexBuffer  = nullptr;
        pRenderState[i].vertexCount   = 0;
        pRenderState[i].triangleCount = 0;
        pRenderState[i].displayListId = 0;
        materialVisibility[i]         = 0;
        nTrianglesPerMaterial[i]      = 0;
    }

    // Mark which material slots are actually used by groups.
    for (int i = 0; i < nGroups; ++i)
    {
        if (pGroups[i].materialIndex >= 0 && pGroups[i].materialIndex < nMaterials)
            materialVisibility[pGroups[i].materialIndex + 1] = 1;
        else if (pGroups[i].materialIndex < 0)
            materialVisibility[0] = 1;
    }

    // --- Animation header ---
    fAnimationFps    = *reinterpret_cast<const float*>(ms3d_cursor);
    ms3d_cursor += 4;
    ms3d_cursor += 4; // skip currentTime
    iTotalFrames     = *reinterpret_cast<const int*>(ms3d_cursor);
    ms3d_cursor += 4;
    animationDurationMs = iTotalFrames * 1000.0 / fAnimationFps;

    // --- Joints ---
    nJoints  = *reinterpret_cast<const short*>(ms3d_cursor);
    ms3d_cursor += 2;

    vJoints = new Ms3dJoint[nJoints];

    // Pass 1: Build name-to-index mapping and find the right wrist joint.
    Ms3dJointMapping* pJointMappings = static_cast<Ms3dJointMapping*>(
        malloc(sizeof(Ms3dJointMapping) * nJoints));

    const char* ms3d_joints = ms3d_cursor;
    const char* ms3d_joints_cursor = ms3d_joints;

    for (int i = 0; i < nJoints; ++i)
    {
        const ms3d_joint_t* joint = reinterpret_cast<const ms3d_joint_t*>(ms3d_joints_cursor);
        pJointMappings[i].index = i;
        pJointMappings[i].name  = const_cast<char*>(joint->name);

        if (!strcmp(joint->name, "right wrist"))
            rightWristJointIndex = static_cast<short>(i);

        ms3d_joints_cursor += sizeof(ms3d_joint_t)
            + sizeof(ms3d_keyframe_rot_t) * joint->numKeyFramesRot
            + sizeof(ms3d_keyframe_pos_t) * joint->numKeyFramesTrans;
    }

    // Pass 2: Parse joint data, resolve parent hierarchy, and store keyframes.
    for (int i = 0; i < nJoints; ++i)
    {
        const ms3d_joint_t* sourceJoint = reinterpret_cast<const ms3d_joint_t*>(ms3d_cursor);
        ms3d_cursor += sizeof(ms3d_joint_t);

        int parentIndex = -1;
        if (strlen(sourceJoint->parentName))
        {
            for (int k = 0; k < nJoints; ++k)
            {
                if (!_stricmp(pJointMappings[k].name, sourceJoint->parentName))
                {
                    parentIndex = pJointMappings[k].index;
                    break;
                }
            }
            if (parentIndex == -1)
            {
                OnFailLoadMilkshape("<unknown>");
                free(pJointMappings);
                return false;
            }
        }

        memcpy(&vJoints[i].rotation,  sourceJoint->rotation,  sizeof(vJoints[i].rotation));
        memcpy(&vJoints[i].position,  sourceJoint->position,  sizeof(vJoints[i].position));
        vJoints[i].parentIndex    = parentIndex;
        vJoints[i].nKeyframesRot  = sourceJoint->numKeyFramesRot;
        vJoints[i].nKeyframesTrans = sourceJoint->numKeyFramesTrans;
        vJoints[i].pKeyFramesRot   = static_cast<Ms3dKeyframePacked*>(
            malloc(sizeof(Ms3dKeyframePacked) * sourceJoint->numKeyFramesRot));
        vJoints[i].pKeyframesTrans = static_cast<Ms3dKeyframePacked*>(
            malloc(sizeof(Ms3dKeyframePacked) * sourceJoint->numKeyFramesTrans));

        for (int k = 0; k < sourceJoint->numKeyFramesRot; ++k)
        {
            const ms3d_keyframe_rot_t* rotKey =
                reinterpret_cast<const ms3d_keyframe_rot_t*>(ms3d_cursor);
            ms3d_cursor += sizeof(ms3d_keyframe_rot_t);
            float timeMs = rotKey->time * 1000.0f;
            StoreKeyframe(i, k, timeMs,
                reinterpret_cast<float(*)[3]>(
                    const_cast<float*>(rotKey->rotation)), true);
        }

        for (int k = 0; k < sourceJoint->numKeyFramesTrans; ++k)
        {
            const ms3d_keyframe_pos_t* posKey =
                reinterpret_cast<const ms3d_keyframe_pos_t*>(ms3d_cursor);
            ms3d_cursor += sizeof(ms3d_keyframe_pos_t);
            float timeMs = posKey->time * 1000.0f;
            StoreKeyframe(i, k, timeMs,
                reinterpret_cast<float(*)[3]>(
                    const_cast<float*>(posKey->position)), false);
        }
    }

    free(pJointMappings);

    SkinVerticesToSkeleton();
    ResetJoints();
    CompileMaterialDisplayLists();

    return true;
}
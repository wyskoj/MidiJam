//
// Created by Jacob on 11/15/2025.
//

#include "Ms3dBundle.h"

#include <cstdlib>

#include "MatrixMath.h"
#include "ObjectConstructor.h"
#include "Quaternion.h"
#include "textures.h"
#include "Vector3.h"

Ms3dBundle::Ms3dBundle() {
    Init();
}

Ms3dBundle::~Ms3dBundle() {
    Deconstruct();
}

void Ms3dBundle::Init() {
    this->nGroups = 0;
    this->pGroups = nullptr;
    this->nMaterials = 0;
    this->pMaterials = nullptr;
    this->nTris = 0;
    this->pTris = nullptr;
    this->nVerts = 0;
    this->pVerts = nullptr;
    this->nJoints = 0;
    // this->vJoints = nullptr;
    this->pAnimationController = CreateAnimationController();
    this->isLoopAnimation = false;
    this->hasSkeletalAnimation = false;
    this->rightWristJointIndex = -1;
    ResetJoints();
}

GLboolean Ms3dBundle::ApplyMaterial() const {
    int material_index; // [esp+4h] [ebp-Ch]
    __int16 i; // [esp+8h] [ebp-8h]
    GLboolean isEnabled; // [esp+Fh] [ebp-1h]

    isEnabled = glIsEnabled(GL_TEXTURE_2D);
    for (i = 0; i < this->nMaterials + 1; ++i) {
        if (this->materialVisibility[i] == 1) {
            material_index = i - 1;
            if (material_index >= 0) {
                glMaterialfv(GL_FRONT, GL_AMBIENT, this->pMaterials[material_index].ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, this->pMaterials[material_index].diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, this->pMaterials[material_index].specular);
                glMaterialfv(GL_FRONT, GL_EMISSION, this->pMaterials[material_index].emissive);
                glMaterialf(GL_FRONT, GL_SHININESS, this->pMaterials[material_index].shininess);
                glBindTexture(GL_TEXTURE_2D, this->pMaterials[material_index].texture);
                glEnable(GL_TEXTURE_2D);
                return isEnabled;
            }
        }
    }
    return isEnabled;
}

void Ms3dBundle::ApplyTextures(FILE *hwfStream, void *hwfAppendix, const unsigned int hwfAppendixEntriesCount) const {
    int i; // [esp+4h] [ebp-4h]

    for (i = 0; i < this->nMaterials; ++i) {
        if (strlen(this->pMaterials[i].pTextureName))
            this->pMaterials[i].texture = LoadTextureFromHwf(
                this->pMaterials[i].pTextureName,
                hwfStream,
                hwfAppendix,
                hwfAppendixEntriesCount);
        else
            this->pMaterials[i].texture = 0;
        if (strlen(this->pMaterials[i].pAlphaMapName))
            this->pMaterials[i].alphamap = LoadTextureFromHwf(
                this->pMaterials[i].pAlphaMapName,
                hwfStream,
                hwfAppendix,
                hwfAppendixEntriesCount);
        else
            this->pMaterials[i].alphamap = 0;
    }
}

int Ms3dBundle::BuildVertexBuffer(Ms3dRenderState *materialInfo, const short materialSlot) const {
    Quaternion *pPosition; // eax
    const void *normal_0; // eax
    Quaternion tempPosition; // [esp+8h] [ebp-5Ch] BYREF
    Quaternion normal; // [esp+18h] [ebp-4Ch] BYREF
    const MatrixMath *boneMatrix; // [esp+28h] [ebp-3Ch]
    int vertexIndex; // [esp+2Ch] [ebp-38h]
    Ms3dTriangle *pTriangle; // [esp+30h] [ebp-34h]
    int global_triangle_index; // [esp+34h] [ebp-30h]
    int group_material_index; // [esp+38h] [ebp-2Ch]
    int materialIndex; // [esp+3Ch] [ebp-28h]
    int triangle_index_in_group; // [esp+40h] [ebp-24h]
    int totalTriangles; // [esp+44h] [ebp-20h]
    int vertexCorner; // [esp+48h] [ebp-1Ch]
    int totalVertices; // [esp+4Ch] [ebp-18h]
    InterleavedVertex *vertexBuffer; // [esp+50h] [ebp-14h]
    int group_index; // [esp+54h] [ebp-10h]
    int _; // [esp+60h] [ebp-4h]

    totalTriangles = 0;
    // STEP 1: Count total triangles that use this material
    for (group_index = 0; group_index < this->nGroups; ++group_index) {
        materialIndex = this->pGroups[group_index].materialIndex;
        // Check if this group belongs to our target material
        // materialSlot 0 = default material, 1+ = regular materials
        if (materialIndex >= 0 && materialSlot - 1 == materialIndex || materialIndex < 0 && !materialSlot)
            totalTriangles += this->pGroups[group_index].nTris;
    }
    // STEP 2: Allocate interleaved vertex buffer
    totalVertices = 3 * totalTriangles;
    vertexBuffer = (InterleavedVertex *) operator new(sizeof(InterleavedTriangle) * totalTriangles);
    materialInfo->vertexBuffer = vertexBuffer;
    for (group_index = 0; group_index < this->nGroups; ++group_index) {
        group_material_index = this->pGroups[group_index].materialIndex;
        if (group_material_index >= 0 && materialSlot - 1 == group_material_index
            || group_material_index < 0 && !materialSlot) {
            for (triangle_index_in_group = 0;
                 triangle_index_in_group < this->pGroups[group_index].nTris;
                 ++triangle_index_in_group) {
                global_triangle_index = this->pGroups[group_index].pTris[triangle_index_in_group];
                pTriangle = &this->pTris[global_triangle_index];
                for (vertexCorner = 0; vertexCorner < 3; ++vertexCorner) {
                    vertexIndex = pTriangle->vertex_indices[vertexCorner];
                    if (this->pVerts[vertexIndex].boneId == -1) {
                        // Static vertex - copy directly without transformation
                        memcpy(vertexBuffer, this->pVerts[vertexIndex].position, 12u);
                        memcpy(vertexBuffer->normal, pTriangle->vertex_normals[vertexCorner],
                               sizeof(vertexBuffer->normal));
                        vertexBuffer->texCoord[0] = pTriangle->s[vertexCorner];
                        vertexBuffer->texCoord[1] = pTriangle->t[vertexCorner];
                    } else {
                        // Skinned vertex - transform by bone matrix
                        boneMatrix = &this->vJoints[this->pVerts[vertexIndex].boneId].finalTransformationMatrix;
                        tempPosition.FromVector(this->pVerts[vertexIndex].position);
                        tempPosition.TransformByMatrix(boneMatrix);
                        pPosition = (Quaternion *) ObjectConstructor(&tempPosition);
                        memcpy(vertexBuffer, pPosition, 12u);
                        normal.FromVector(pTriangle->vertex_normals[vertexCorner]);
                        normal.TransformByMatrixRotation(boneMatrix);
                        normal.Normalize();
                        normal_0 = ObjectConstructor(&normal);
                        memcpy(vertexBuffer->normal, normal_0, sizeof(vertexBuffer->normal));
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

void Ms3dBundle::CompileMaterialDisplayLists() const {
    int materialIndex; // [esp+4h] [ebp-4h]

    for (materialIndex = 0; materialIndex < this->nMaterials + 1; ++materialIndex) {
        if (this->materialVisibility[materialIndex] == 1) {
            this->nTrianglesPerMaterial[materialIndex] = BuildVertexBuffer(

                &this->pRenderState[materialIndex],
                materialIndex);
            this->pRenderState[materialIndex].displayListId = glGenLists(1);
            if (!this->pRenderState[materialIndex].displayListId)
                return;
            glNewList(this->pRenderState[materialIndex].displayListId, GL_COMPILE);
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glVertexPointer(3, GL_FLOAT, sizeof(InterleavedVertex), this->pRenderState[materialIndex].vertexBuffer);
            glTexCoordPointer(
                2,
                GL_FLOAT,
                sizeof(InterleavedVertex),
                this->pRenderState[materialIndex].vertexBuffer->texCoord);
            glNormalPointer(GL_FLOAT, sizeof(InterleavedVertex),
                            this->pRenderState[materialIndex].vertexBuffer->normal);
            glDrawArrays(GL_TRIANGLES, 0, 3 * this->nTrianglesPerMaterial[materialIndex]);
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
            glEndList();
        }
    }
}

void Ms3dBundle::Deconstruct() {
    for (int i = 0; i < this->nGroups; ++i)
        free(this->pGroups[i].pTris);

    for (int j = 0; j < this->nMaterials; ++j) {
        free(this->pMaterials[j].pTextureName);
        free(this->pMaterials[j].pAlphaMapName);
    }

    this->nGroups = 0;
    if (this->pGroups) {
        free(this->pGroups);
        this->pGroups = nullptr;
    }

    this->nMaterials = 0;
    if (this->pMaterials) {
        free(this->pMaterials);
        this->pMaterials = nullptr;
    }

    this->nTris = 0;
    if (this->pTris) {
        free(this->pTris);
        this->pTris = nullptr;
    }

    this->nVerts = 0;
    if (this->pVerts) {
        free(this->pVerts);
        this->pVerts = nullptr;
    }

    for (int k = 0; k < this->nJoints; ++k) {
        free(this->vJoints[k].pKeyFramesRot);
        free(this->vJoints[k].pKeyframesTrans);
    }

    this->nJoints = 0;
    this->vJoints.clear();

    if (pAnimationController) {
        if (pAnimationController[-1].elapsed)
            delete pAnimationController;
        else
            free(&pAnimationController[-1].elapsed + 4);
    }
}

bool Ms3dBundle::LoadFromHWF(const char *asset_name, FILE *hwfStream, void *hwfAppendix,
                             const unsigned int hwfAppendixEntriesCount) {
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
                (char *) (asset_buffer),
                (char *) hwfAppendix + 264 * i);
            // Free buffer
            free(asset_buffer);
            return TRUE;
        }
        fseek(hwfStream, *((int *) hwfAppendix + 66 * i + 65), 1);
    }
    return FALSE;
}

bool Ms3dBundle::LoadFromResource(const LPCSTR resourceId, const LPCSTR resourceType) {
    DWORD resourceSize; // [esp+Ch] [ebp-E0h]
    HGLOBAL hResourceData; // [esp+10h] [ebp-DCh]
    HRSRC hResourceInfo; // [esp+14h] [ebp-D8h]
    char resourceName[200]; // [esp+1Ch] [ebp-D0h] BYREF
    void *pResourceData; // [esp+E4h] [ebp-8h]
    void *pBuffer; // [esp+E8h] [ebp-4h]

    hResourceInfo = FindResourceA(0, (LPCSTR) (unsigned __int16) resourceId, resourceType);
    resourceSize = SizeofResource(0, hResourceInfo);
    hResourceData = LoadResource(0, hResourceInfo);
    pResourceData = LockResource(hResourceData);
    pBuffer = operator new(resourceSize);
    memcpy(pBuffer, pResourceData, resourceSize);
    FreeResource(hResourceData);
    sprintf(resourceName, "RESOURCE NUMBER:%d", (unsigned __int16) (uintptr_t) resourceId);
    ParseMilkshape(static_cast<char *>(pBuffer), resourceName);
    free(pBuffer);
    return TRUE;
}

bool Ms3dBundle::ParseMilkshape(char *ms3d_file_data, char *model_name) {
    size_t texture_strlen; // eax
    size_t alphamap_strlen; // eax
    float rotKeyframeTimeMs; // [esp+0h] [ebp-D8h]
    float posKeyframeTimeMs; // [esp+0h] [ebp-D8h]
    Ms3dJoint **vJoints; // [esp+Ch] [ebp-CCh]
    std::vector<Ms3dJoint> pJointCollection; // [esp+24h] [ebp-B4h]
    int nJoints; // [esp+2Ch] [ebp-ACh]
    char *pAlphaMapName; // [esp+3Ch] [ebp-9Ch]
    char *pTextureName; // [esp+40h] [ebp-98h]
    int keyframeIndex; // [esp+60h] [ebp-78h]
    int keyframeIndexa; // [esp+60h] [ebp-78h]
    int keyframeIndexb; // [esp+60h] [ebp-78h]
    int parentIndex; // [esp+64h] [ebp-74h]
    __int16 y; // [esp+70h] [ebp-68h]
    __int16 l; // [esp+70h] [ebp-68h]
    int i; // [esp+78h] [ebp-60h]
    int *pGroupTris; // [esp+7Ch] [ebp-5Ch]
    char materialIndex; // [esp+83h] [ebp-55h]
    unsigned __int16 nGroupTris; // [esp+84h] [ebp-54h]
    float t[3]; // [esp+8Ch] [ebp-4Ch] BYREF
    int tri_vertexIndicies[3]; // [esp+98h] [ebp-40h] BYREF
    int nGroups; // [esp+A8h] [ebp-30h]
    int nTris; // [esp+ACh] [ebp-2Ch]
    char *ms3d_cursor; // [esp+B0h] [ebp-28h]
    int nMaterials; // [esp+B4h] [ebp-24h]
    int nVerts; // [esp+B8h] [ebp-20h]
    char *ms3d_data_1; // [esp+BCh] [ebp-1Ch]
    int jointIndex; // [esp+C4h] [ebp-14h]
    Ms3dJointMapping *pJointMappings; // [esp+C8h] [ebp-10h]
    int _; // [esp+D4h] [ebp-4h]

    ms3d_data_1 = ms3d_file_data;
    ms3d_cursor = ms3d_file_data + 14;
    if (!strncmp(ms3d_file_data, "MS3D000000", 10u) && *(ms3d_data_1 + 10) >= 3) // must be MS3D and at least version 3
    {
        // === VERTICES ===
        nVerts = *reinterpret_cast<unsigned __int16 *>(ms3d_cursor);
        this->nVerts = nVerts;
        this->pVerts = static_cast<Ms3dVertex *>(operator new(sizeof(Ms3dVertex) * nVerts));
        ms3d_cursor += 2;
        // For each vertex ...
        for (jointIndex = 0; jointIndex < nVerts; ++jointIndex) {
            this->pVerts[jointIndex].boneId = ms3d_cursor[0xD];
            memcpy(this->pVerts[jointIndex].position, ms3d_cursor + 1, sizeof(this->pVerts[jointIndex].position));
            // Copy position floats
            // try to find maximum value for vertex positions (?)
            if (jointIndex) {
                // check if larger for index 1
                if (abs(this->pVerts[jointIndex].position[0]) > this->magX)
                    this->magX = abs(this->pVerts[jointIndex].position[0]);
                if (abs(this->pVerts[jointIndex].position[1]) > this->magY)
                    this->magY = abs(this->pVerts[jointIndex].position[1]);
                if (abs(this->pVerts[jointIndex].position[2]) > this->magZ)
                    this->magZ = abs(this->pVerts[jointIndex].position[2]);
            } else {
                // first vertex only
                this->magX = abs(this->pVerts->position[0]);
                this->magY = abs(this->pVerts[jointIndex].position[1]);
                this->magZ = abs(this->pVerts[jointIndex].position[2]);
            }
            ms3d_cursor += 15; // Jump to next vertex
        }
        // === TRIANGLES ===
        nTris = *reinterpret_cast<unsigned __int16 *>(ms3d_cursor);
        this->nTris = nTris;
        this->pTris = static_cast<Ms3dTriangle *>(operator new(sizeof(Ms3dTriangle) * nTris));
        ms3d_cursor += 2; // advance to start of tri data
        // For each triangle ...
        for (jointIndex = 0; jointIndex < nTris; ++jointIndex) {
            tri_vertexIndicies[0] = *((unsigned __int16 *) ms3d_cursor + 1);
            tri_vertexIndicies[1] = *((unsigned __int16 *) ms3d_cursor + 2);
            tri_vertexIndicies[2] = *((unsigned __int16 *) ms3d_cursor + 3);
            t[0] = 1.0 - *((float *) ms3d_cursor + 14);
            t[1] = 1.0 - *((float *) ms3d_cursor + 15);
            t[2] = 1.0 - *((float *) ms3d_cursor + 16);
            memcpy(this->pTris[jointIndex].vertex_normals, ms3d_cursor + 0x8, 36u); // vertex normals
            memcpy(this->pTris[jointIndex].s, ms3d_cursor + 0x2C, 12u); // s
            memcpy(this->pTris[jointIndex].t, t, 12u); // t
            memcpy(this->pTris[jointIndex].vertex_indices, tri_vertexIndicies,
                   sizeof(this->pTris[jointIndex].vertex_indices)); // vertex indices
            ms3d_cursor += 70; // Jump to next tri
        }
        // === GROUPS ===
        nGroups = *(unsigned __int16 *) ms3d_cursor;
        this->nGroups = nGroups;
        this->pGroups = static_cast<Ms3dGroup *>(operator new(sizeof(Ms3dGroup) * nGroups));
        ms3d_cursor += 2; // Advance to group data
        for (jointIndex = 0; jointIndex < nGroups; ++jointIndex) {
            ++ms3d_cursor; // skip flags
            ms3d_cursor += 32; // skip name
            nGroupTris = *(unsigned __int16 *) ms3d_cursor;
            ms3d_cursor += 2; // advance to list of triangle indices
            pGroupTris = static_cast<int *>(operator new(4 * nGroupTris));
            // read list of triangles
            for (i = 0; i < nGroupTris; ++i) {
                pGroupTris[i] = *(unsigned __int16 *) ms3d_cursor;
                ms3d_cursor += 2;
            }
            materialIndex = *ms3d_cursor++;
            this->pGroups[jointIndex].materialIndex = materialIndex;
            this->pGroups[jointIndex].nTris = nGroupTris;
            this->pGroups[jointIndex].pTris = pGroupTris;
        }
        // === MATERIALS ===
        nMaterials = *(unsigned __int16 *) ms3d_cursor;
        this->nMaterials = nMaterials;
        this->pMaterials = static_cast<Ms3dMaterial *>(operator new(sizeof(Ms3dMaterial) * nMaterials));
        ms3d_cursor += 2; // advance to material data
        for (jointIndex = 0; jointIndex < nMaterials; ++jointIndex) {
            //  ... skip 32 bytes of name ...
            memcpy(this->pMaterials[jointIndex].ambient, ms3d_cursor + 0x20, 16u);
            memcpy(this->pMaterials[jointIndex].diffuse, ms3d_cursor + 0x30, 16u);
            memcpy(this->pMaterials[jointIndex].specular, ms3d_cursor + 0x40, 16u);
            memcpy(this->pMaterials[jointIndex].emissive, ms3d_cursor + 0x50, 16u);
            this->pMaterials[jointIndex].shininess = *(float *) (ms3d_cursor + 0x60);
            // skipping path prefixes and extensions
            y = 0;
            while ((ms3d_cursor + 0x69)[y] == '.' ||(ms3d_cursor + 0x69)[y] == '\\') {
                if (!(ms3d_cursor + 0x69)[++y]) {
                    y = 0;
                    break;
                }
            }
            texture_strlen = strlen(&(ms3d_cursor + 0x69)[y]);
            pTextureName = static_cast<char *>(operator new(texture_strlen + 1));
            this->pMaterials[jointIndex].pTextureName = pTextureName; // set the pointer
            strcpy(this->pMaterials[jointIndex].pTextureName, &(ms3d_cursor + 0x69)[y]); // copy the characters

            l = 0;
            // skip path prefixes and extensions
            while ((ms3d_cursor + 0xE9)[l] == '.' || (ms3d_cursor + 0xE9)[l] == '\\') {
                if (!(ms3d_cursor + 0xE9)[++l]) {
                    l = 0;
                    break;
                }
            }
            alphamap_strlen = strlen(&(ms3d_cursor + 0xE9)[l]);
            pAlphaMapName = static_cast<char *>(operator new(alphamap_strlen + 1));
            this->pMaterials[jointIndex].pAlphaMapName = pAlphaMapName;
            strcpy(this->pMaterials[jointIndex].pAlphaMapName, &(ms3d_cursor + 0xE9)[l]);
            ms3d_cursor += 361; // skip to next material
        }
        this->pRenderState = static_cast<Ms3dRenderState *>(operator new(sizeof(Ms3dRenderState) * (nMaterials + 1)));
        for (jointIndex = 0; jointIndex < nMaterials + 1; ++jointIndex) {
            this->pRenderState[jointIndex].flags = 0;
            this->pRenderState[jointIndex].vertexBuffer = 0;
            this->pRenderState[jointIndex].vertexCount = 0;
            *(int *) ((char *) &this->pRenderState[jointIndex].vertexCount + 1) = 0;
            this->pRenderState[jointIndex].displayListId = 0;
        }
        this->materialVisibility = static_cast<char *>(operator new(nMaterials + 1));
        this->nTrianglesPerMaterial = static_cast<short *>(operator new(2 * nMaterials + 2));
        for (jointIndex = 0; jointIndex < nMaterials + 1; ++jointIndex) {
            this->materialVisibility[jointIndex] = 0;
            this->nTrianglesPerMaterial[jointIndex] = 0;
        }
        for (jointIndex = 0; jointIndex < nGroups; ++jointIndex) {
            if (this->pGroups[jointIndex].materialIndex >= 0) {
                if (this->pGroups[jointIndex].materialIndex < nMaterials)
                    this->materialVisibility[this->pGroups[jointIndex].materialIndex + 1] = 1;
            } else {
                *this->materialVisibility = 1;
            }
        }
        this->fAnimationFps = *(float *) ms3d_cursor;
        ms3d_cursor += 4;
        ms3d_cursor += 4; // Skip currentTime
        this->iTotalFrames = *(int *) ms3d_cursor;
        ms3d_cursor += 4;
        this->animationDurationMs = this->iTotalFrames * 1000.0 / this->fAnimationFps;
        this->nJoints = *(unsigned __int16 *) ms3d_cursor;
        ms3d_cursor += 2;
        nJoints = this->nJoints;
        this->vJoints = std::vector<Ms3dJoint>();
        this->vJoints.reserve(nJoints);
        // Part 1: Building Joint Name Mapping
        pJointMappings = static_cast<Ms3dJointMapping *>(operator new(sizeof(Ms3dJointMapping) * this->nJoints));
        // for (jointIndex = 0; jointIndex < this->nJoints; ++jointIndex) {
        //     ms3d_joints_cursor = ms3d_joints++;
        //     ms3d_joints = (ms3d_joints
        //                    + sizeof(ms3d_keyframe_pos_t) * (unsigned __int16) ms3d_joints_cursor->numKeyFramesTrans
        //                    + sizeof(ms3d_keyframe_pos_t) * (unsigned __int16) ms3d_joints_cursor->numKeyFramesRot);
        //     pJointMappings[jointIndex].index = jointIndex;
        //     pJointMappings[jointIndex].name = ms3d_joints_cursor->name;
        //     if (!strcmp(ms3d_joints_cursor->name, "right wrist"))
        //         this->rightWristJointIndex = jointIndex;
        // }
        // // Part 2: Parsing Joint Data & Hierarchy
        // for (jointIndex = 0; jointIndex < this->nJoints; ++jointIndex) {
        //     sourceJoint = reinterpret_cast<ms3d_joint_t *>(ms3d_cursor);
        //     ms3d_cursor += sizeof(ms3d_joint_t);
        //     parentIndex = -1;
        //     if (strlen(sourceJoint->parentName)) {
        //         for (keyframeIndex = 0; keyframeIndex < this->nJoints; ++keyframeIndex) {
        //             if (!_stricmp(pJointMappings[keyframeIndex].name, sourceJoint->parentName)) {
        //                 parentIndex = pJointMappings[keyframeIndex].index;
        //                 break;
        //             }
        //         }
        //         if (parentIndex == -1) {
        //             OnFailLoadMilkshape(model_name);
        //             return FALSE; // Parent not found - bad file
        //         }
        //     }
        //     memcpy(&this->vJoints[jointIndex], sourceJoint->rotation, 12u);
        //     memcpy(this->vJoints[jointIndex].position, sourceJoint->position,
        //            sizeof(this->vJoints[jointIndex].position));
        //     this->vJoints[jointIndex].parentIndex = parentIndex;
        //     this->vJoints[jointIndex].nKeyframesRot = (unsigned __int16) sourceJoint->numKeyFramesRot;
        //     this->vJoints[jointIndex].pKeyFramesRot = static_cast<Ms3dKeyframePacked *>(operator new(
        //         sizeof(Ms3dKeyframePacked) * (unsigned __int16) sourceJoint->numKeyFramesRot));
        //     this->vJoints[jointIndex].nKeyframesTrans = sourceJoint->numKeyFramesTrans;
        //     this->vJoints[jointIndex].pKeyframesTrans = static_cast<Ms3dKeyframePacked *>(operator new(
        //         sizeof(Ms3dKeyframePacked) * (unsigned __int16) sourceJoint->numKeyFramesTrans));
        //     for (keyframeIndexa = 0; keyframeIndexa < (unsigned __int16) sourceJoint->numKeyFramesRot; ++
        //          keyframeIndexa) {
        //         ms3d_keyframe_rot = reinterpret_cast<ms3d_keyframe_rot_t *>(ms3d_cursor);
        //         ms3d_cursor += 16;
        //         rotKeyframeTimeMs = ms3d_keyframe_rot->time * 1000.0;
        //         StoreKeyframe(jointIndex, keyframeIndexa, rotKeyframeTimeMs, &ms3d_keyframe_rot->rotation, true);
        //     }
        //     for (keyframeIndexb = 0; keyframeIndexb < (unsigned __int16) sourceJoint->numKeyFramesTrans; ++
        //          keyframeIndexb) {
        //         ms3d_keyframe_pos = reinterpret_cast<ms3d_keyframe_pos_t *>(ms3d_cursor);
        //         ms3d_cursor += 16;
        //         posKeyframeTimeMs = ms3d_keyframe_pos->time * 1000.0;
        //         StoreKeyframe(jointIndex, keyframeIndexb, posKeyframeTimeMs, &ms3d_keyframe_pos->position, false);
        //     }
        // }
        free(pJointMappings);
        this->SkinVerticesToSkeleton();
        this->ResetJoints();
        this->CompileMaterialDisplayLists();
        return TRUE;
    }

    OnFailLoadMilkshape(model_name);
    return FALSE;
}

void Ms3dBundle::Render() {
    __int16 i; // [esp+4h] [ebp-8h]
    __int16 canUseDisplayLists; // [esp+8h] [ebp-4h]

    canUseDisplayLists = 1;
    for (i = 0; i < this->nMaterials + 1; ++i) {
        if (this->materialVisibility[i] == 1 && !this->pRenderState[i].displayListId)
            canUseDisplayLists = 0;
    }
    if (canUseDisplayLists && !this->hasSkeletalAnimation)
        RenderCompiled();
    else
        RenderImmediate();
}

void Ms3dBundle::RenderCompiled() {
    __int16 i; // [esp+4h] [ebp-4h]

    for (i = 0; i < this->nMaterials + 1; ++i)
        glCallList(this->pRenderState[i].displayListId);
}

void Ms3dBundle::RenderImmediate() {
    int groupIndex; // [esp+4h] [ebp-4h]

    if (this->hasSkeletalAnimation)
        UpdateSkeletalAnimation();
    for (groupIndex = 0; groupIndex < this->nGroups; ++groupIndex)
        RenderModelGroup(groupIndex);
}

void Ms3dBundle::RenderModel() {
    __int16 i; // [esp+4h] [ebp-8h]
    __int16 canUseSimpleRendering; // [esp+8h] [ebp-4h]

    canUseSimpleRendering = 1;
    for (i = 0; i < this->nMaterials + 1; ++i) {
        if (this->materialVisibility[i] == 1 && !this->pRenderState[i].displayListId)
            canUseSimpleRendering = 0;
    }
    if (canUseSimpleRendering && !this->hasSkeletalAnimation)
        RenderModelSimple();
    else
        RenderModelComplex();
}

void Ms3dBundle::RenderModelSimple() {
    int material_properties_index; // [esp+4h] [ebp-Ch]
    __int16 material_index; // [esp+8h] [ebp-8h]
    GLboolean IsEnabled; // [esp+Fh] [ebp-1h]

    IsEnabled = glIsEnabled(GL_TEXTURE_2D);
    for (material_index = 0; material_index < this->nMaterials + 1; ++material_index) {
        if (this->materialVisibility[material_index] == 1) {
            material_properties_index = material_index - 1;
            if (material_properties_index < 0) {
                if (*this->materialVisibility > 0) {
                    glDisable(GL_TEXTURE_2D);
                    glCallList(this->pRenderState->displayListId);
                }
            } else {
                glMaterialfv(GL_FRONT, GL_AMBIENT, this->pMaterials[material_properties_index].ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, this->pMaterials[material_properties_index].diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, this->pMaterials[material_properties_index].specular);
                glMaterialfv(GL_FRONT, GL_EMISSION, this->pMaterials[material_properties_index].emissive);
                glMaterialf(GL_FRONT, GL_SHININESS, this->pMaterials[material_properties_index].shininess);
                if (this->pMaterials[material_properties_index].alphamap) {
                    glDisable(GL_DEPTH_TEST);
                    glBlendFunc(GL_DST_COLOR, 0);
                    glEnable(GL_BLEND);
                    glBindTexture(GL_TEXTURE_2D, this->pMaterials[material_properties_index].alphamap);
                    glCallList(this->pRenderState[material_index].displayListId);
                    glBlendFunc(1u, 1u);
                    glBindTexture(GL_TEXTURE_2D, this->pMaterials[material_properties_index].texture);
                    glCallList(this->pRenderState[material_index].displayListId);
                    glDisable(GL_BLEND);
                    glEnable(GL_DEPTH_TEST);
                } else {
                    if (this->pMaterials[material_properties_index].texture) {
                        glBindTexture(GL_TEXTURE_2D, this->pMaterials[material_properties_index].texture);
                        glEnable(GL_TEXTURE_2D);
                    } else {
                        glDisable(GL_TEXTURE_2D);
                    }
                    glCallList(this->pRenderState[material_index].displayListId);
                }
            }
        }
    }
    if (IsEnabled)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);
}

void Ms3dBundle::RenderModelComplex() {
    int i; // [esp+4h] [ebp-1Ch]
    int k; // [esp+8h] [ebp-18h]
    int j; // [esp+Ch] [ebp-14h]
    int groupIndex; // [esp+10h] [ebp-10h]
    int materialIndex; // [esp+14h] [ebp-Ch]
    __int16 materialSlot; // [esp+18h] [ebp-8h]
    GLboolean wasTexturingEnabled; // [esp+1Fh] [ebp-1h]

    if (this->hasSkeletalAnimation)
        UpdateSkeletalAnimation();
    wasTexturingEnabled = glIsEnabled(GL_TEXTURE_2D);
    for (materialSlot = 0; materialSlot < this->nMaterials + 1; ++materialSlot) {
        if (this->materialVisibility[materialSlot] == 1) {
            materialIndex = materialSlot - 1;
            if (materialIndex < 0) {
                if (*this->materialVisibility > 0) {
                    glDisable(GL_TEXTURE_2D);
                    for (i = 0; i < this->nGroups; ++i) {
                        if (this->pGroups[i].materialIndex < 0)
                            RenderModelGroup(i);
                    }
                }
            } else {
                glMaterialfv(GL_FRONT, GL_AMBIENT, this->pMaterials[materialIndex].ambient);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, this->pMaterials[materialIndex].diffuse);
                glMaterialfv(GL_FRONT, GL_SPECULAR, this->pMaterials[materialIndex].specular);
                glMaterialfv(GL_FRONT, GL_EMISSION, this->pMaterials[materialIndex].emissive);
                glMaterialf(GL_FRONT, GL_SHININESS, this->pMaterials[materialIndex].shininess);
                if (this->pMaterials[materialIndex].alphamap) {
                    for (groupIndex = 0; groupIndex < this->nGroups; ++groupIndex) {
                        if (this->pGroups[groupIndex].materialIndex == materialIndex) {
                            glDisable(GL_DEPTH_TEST);
                            glBlendFunc(GL_DST_COLOR, 0);
                            glEnable(GL_BLEND);
                            glBindTexture(GL_TEXTURE_2D, this->pMaterials[materialIndex].alphamap);
                            RenderModelGroup(groupIndex);
                            glBlendFunc(GL_ONE, GL_ONE);
                            glBindTexture(GL_TEXTURE_2D, this->pMaterials[materialIndex].texture);
                            RenderModelGroup(groupIndex);
                            glDisable(GL_BLEND);
                            glEnable(GL_DEPTH_TEST);
                        }
                    }
                } else if (this->pMaterials[materialIndex].texture) {
                    glBindTexture(GL_TEXTURE_2D, this->pMaterials[materialIndex].texture);
                    glEnable(GL_TEXTURE_2D);
                    for (j = 0; j < this->nGroups; ++j) {
                        if (this->pGroups[j].materialIndex == materialIndex)
                            RenderModelGroup(j);
                    }
                } else {
                    glDisable(GL_TEXTURE_2D);
                    for (k = 0; k < this->nGroups; ++k) {
                        if (this->pGroups[k].materialIndex == materialIndex)
                            RenderModelGroup(k);
                    }
                }
            }
        }
    }
    if (wasTexturingEnabled)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);
}

void Ms3dBundle::RenderModelGroup(short groupIndex) {
    const GLfloat *pTransformedVector; // eax
    const GLfloat *pTransformedNormal; // eax
    Quaternion transformedVertex; // [esp+4h] [ebp-44h] BYREF
    Quaternion transformedNormal; // [esp+14h] [ebp-34h] BYREF
    const MatrixMath *pJointMatrix; // [esp+24h] [ebp-24h]
    int vertexIndex; // [esp+28h] [ebp-20h]
    int vertexInTriangle; // [esp+2Ch] [ebp-1Ch]
    Ms3dTriangle *pTriangle; // [esp+30h] [ebp-18h]
    int triangleIndex; // [esp+34h] [ebp-14h]
    int triangleInGroup; // [esp+38h] [ebp-10h]
    int cleanupFlag; // [esp+44h] [ebp-4h]

    glBegin(GL_TRIANGLES);
    for (triangleInGroup = 0; triangleInGroup < this->pGroups[groupIndex].nTris; ++triangleInGroup) {
        triangleIndex = this->pGroups[groupIndex].pTris[triangleInGroup];
        pTriangle = &this->pTris[triangleIndex];
        for (vertexInTriangle = 0; vertexInTriangle < 3; ++vertexInTriangle) {
            vertexIndex = pTriangle->vertex_indices[vertexInTriangle];
            if (this->pVerts[vertexIndex].boneId == -1) {
                // Static vertex (no skeletal animation)
                glTexCoord2f(pTriangle->s[vertexInTriangle], pTriangle->t[vertexInTriangle]);
                glNormal3fv(pTriangle->vertex_normals[vertexInTriangle]);
                glVertex3fv(this->pVerts[vertexIndex].position);
            } else {
                // Skinned vertex - transform by bone matrix
                pJointMatrix = &this->vJoints[this->pVerts[vertexIndex].boneId].finalTransformationMatrix;
                glTexCoord2f(pTriangle->s[vertexInTriangle], pTriangle->t[vertexInTriangle]);

                // Transform normal by joint matrix (rotation only)
                transformedNormal.FromVector(pTriangle->vertex_normals[vertexInTriangle]);
                // cleanupFlag = 0;
                transformedNormal.TransformByMatrixRotation(pJointMatrix);
                transformedNormal.Normalize();
                pTransformedVector = (const GLfloat *) ObjectConstructor(&transformedNormal);
                glNormal3fv(pTransformedVector);

                // Transform vertex position by joint matrix (full transformation)
                transformedVertex.FromVector(this->pVerts[vertexIndex].position);
                // LOBYTE(cleanupFlag) = 1;
                transformedVertex.TransformByMatrix(pJointMatrix);
                pTransformedNormal = (const GLfloat *) ObjectConstructor(&transformedVertex);
                glVertex3fv(pTransformedNormal);

                // Cleanup
                // LOBYTE(cleanupFlag) = 0;
                // noop();
                // cleanupFlag = -1;
                // noop();
            }
        }
    }
    glEnd();
}

void Ms3dBundle::RenderModelShiny() {
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
    RenderModel();
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
}

void Ms3dBundle::ReplaceTexture(const char *old_texture, const char *new_texture) {
    size_t new_texture_str_len; // eax
    int i; // [esp+Ch] [ebp-4h]

    for (i = 0; i < this->nMaterials; ++i) {
        if (strlen(this->pMaterials[i].pTextureName)) {
            if (!_stricmp(this->pMaterials[i].pTextureName, old_texture)) {
                free(this->pMaterials[i].pTextureName);
                new_texture_str_len = strlen(new_texture);
                this->pMaterials[i].pTextureName = (char *) operator new(new_texture_str_len + 1);
                strcpy(this->pMaterials[i].pTextureName, new_texture);
            }
        }
    }
}

void Ms3dBundle::ResetJoints() {
    const MatrixMath *pMatrix; // eax
    int i; // [esp+4h] [ebp-4h]

    for (i = 0; i < this->nJoints; ++i) {
        this->vJoints[i].currentRotKeyframe = 0;
        this->vJoints[i].currentTransKeyframe = 0;
        pMatrix = static_cast<const MatrixMath *>(ObjectConstructor(&this->vJoints[i].worldTransformationMatrix));
        // MatrixMath::Copy((MatrixMath *)this->vJoints[i].finalTransformationMatrix, pMatrix);
        this->vJoints[i].finalTransformationMatrix.Copy(pMatrix);
    }
    return pAnimationController->Update();
}

void Ms3dBundle::SkinVerticesToSkeleton() {
    // STEP 1: Calculate bone transformation matrices
    for (int boneIndex = 0; boneIndex < this->nJoints; ++boneIndex) {
        Ms3dJoint *bone = &this->vJoints[boneIndex];
        // MatrixMath::SetTranslation(bone->localTransformationMatrix, bone->position);
        bone->localTransformationMatrix.SetTranslation(bone->position);
        if (bone->parentIndex == -1) {
            // void *localTransformationMatrix = ObjectConstructor(bone->localTransformationMatrix);
            // MatrixMath::Copy(bone->worldTransformationMatrix, localTransformationMatrix);
            bone->worldTransformationMatrix.Copy(&bone->localTransformationMatrix);
        } else {
            // void *worldTransformationMatrix = ObjectConstructor(
            //     this->vJoints[bone->parentIndex].worldTransformationMatrix);
            // MatrixMath::Copy(bone->worldTransformationMatrix, worldTransformationMatrix);
            // MatrixMath::Multiply(bone->worldTransformationMatrix, bone->localTransformationMatrix);
            bone->worldTransformationMatrix.Copy(&this->vJoints[bone->parentIndex].worldTransformationMatrix);
            bone->worldTransformationMatrix.Multiply(&bone->localTransformationMatrix);
        }
    }
    // STEP 2: Transform vertices by their assigned bones
    for (int vertexIndex = 0; vertexIndex < this->nVerts; ++vertexIndex) {
        Ms3dVertex *vertex = &this->pVerts[vertexIndex];
        if (vertex->boneId != -1) {
            const MatrixMath *boneMatrix = &this->vJoints[vertex->boneId].worldTransformationMatrix;
            boneMatrix->RemoveTranslationFromVector(&vertex->position);
            boneMatrix->TransformVectorByMatrixRotation(&vertex->position);
        }
    }
    // STEP 3: Transform vertex normals (using 3x3 rotation part only)
    for (int triangleIndex = 0; triangleIndex < this->nTris; ++triangleIndex) {
        Ms3dTriangle *triangle = &this->pTris[triangleIndex];
        for (int i = 0; i < 3; ++i) {
            Ms3dVertex *vertexTri = &this->pVerts[triangle->vertex_indices[i]];
            if (vertexTri->boneId != -1)
                // MatrixMath::TransformVectorByMatrixRotation(
                //     this->vJoints[vertexTri->boneId].worldTransformationMatrix,
                //     triangle->vertex_normals[i]
                // );
                this->vJoints[vertexTri->boneId].worldTransformationMatrix.TransformVectorByMatrixRotation(
                    &triangle->vertex_normals[i]);
        }
    }
}

void *Ms3dBundle::StoreKeyframe(int jointIndex,
                                int keyframeIndex,
                                float keyframeTimeMs,
                                float (*data)[3],
                                bool isRotation) {
    Ms3dKeyframePacked *destination; // [esp+0h] [ebp-10h]

    if (isRotation)
        destination = &this->vJoints[jointIndex].pKeyFramesRot[keyframeIndex];
    else
        destination = &this->vJoints[jointIndex].pKeyframesTrans[keyframeIndex];
    destination->jointIndex = jointIndex;
    destination->timeMs = keyframeTimeMs;
    return memcpy(&destination->data, data, sizeof(destination->data));
}

void Ms3dBundle::UpdateSkeletalAnimation() {
    void *pIdentityMatrix; // eax
    void *pLocalTransform; // eax
    void *pParentMatrix; // eax
    float rotationTimeDelta; // [esp+Ch] [ebp-D8h]
    float interpolationFactor_Rot; // [esp+10h] [ebp-D4h]
    Ms3dKeyframePacked *nextRotationKey; // [esp+14h] [ebp-D0h]
    float interpolatedRotation[3]; // [esp+18h] [ebp-CCh] BYREF
    Ms3dKeyframePacked *prevRotationKey; // [esp+24h] [ebp-C0h]
    float positionTimeDelta; // [esp+28h] [ebp-BCh]
    float interpolationFactor_Loc; // [esp+2Ch] [ebp-B8h]
    Ms3dKeyframePacked *nextPositionKey; // [esp+30h] [ebp-B4h]
    Ms3dKeyframePacked *prevPositionKey; // [esp+34h] [ebp-B0h]
    MatrixMath rotationMatrix; // [esp+38h] [ebp-ACh] BYREF
    int keyframeIndex; // [esp+78h] [ebp-6Ch]
    float translation[3]; // [esp+7Ch] [ebp-68h] BYREF
    MatrixMath localTransform; // [esp+88h] [ebp-5Ch] BYREF
    Ms3dJoint *pJoint; // [esp+C8h] [ebp-1Ch]
    int jointIndex; // [esp+CCh] [ebp-18h]
    double animationTime; // [esp+D0h] [ebp-14h]
    int _; // [esp+E0h] [ebp-4h]

    // animationTime = ((double (__thiscall *)(IAnimationController *))this->pAnimationController->vtable->ComputeElapsed)(this->pAnimationController);
    animationTime = pAnimationController->ComputeElapsed();
    if (animationTime > this->animationDurationMs) {
        if (this->isLoopAnimation) {
            ResetJoints();
            animationTime = 0.0;
        } else {
            animationTime = this->animationDurationMs;
        }
    }
    for (jointIndex = 0; jointIndex < this->nJoints; ++jointIndex) {
        // MatrixMath::ctor(&rotationMatrix);
        rotationMatrix = MatrixMath();
        // _ = 0;
        pJoint = &this->vJoints[jointIndex];
        if (pJoint->nKeyframesRot || pJoint->nKeyframesTrans) {
            for (keyframeIndex = pJoint->currentRotKeyframe;
                 keyframeIndex < pJoint->nKeyframesTrans && pJoint->pKeyframesTrans[keyframeIndex].timeMs <
                 animationTime;
                 ++keyframeIndex) {
                ;
            }
            pJoint->currentRotKeyframe = keyframeIndex;
            if (keyframeIndex) {
                if (keyframeIndex == pJoint->nKeyframesTrans) {
                    memcpy(translation, &pJoint->pKeyframesTrans[keyframeIndex - 1].data, sizeof(translation));
                } else {
                    nextPositionKey = &pJoint->pKeyframesTrans[keyframeIndex];
                    prevPositionKey = &pJoint->pKeyframesTrans[keyframeIndex - 1];
                    positionTimeDelta = nextPositionKey->timeMs - prevPositionKey->timeMs;
                    interpolationFactor_Loc = (animationTime - prevPositionKey->timeMs) / positionTimeDelta;
                    translation[0] = (nextPositionKey->data.position[0] - prevPositionKey->data.position[0]) *
                                     interpolationFactor_Loc
                                     + prevPositionKey->data.position[0];
                    translation[1] = (nextPositionKey->data.position[1] - prevPositionKey->data.position[1]) *
                                     interpolationFactor_Loc
                                     + prevPositionKey->data.position[1];
                    translation[2] = (nextPositionKey->data.position[2] - prevPositionKey->data.position[2]) *
                                     interpolationFactor_Loc
                                     + prevPositionKey->data.position[2];
                }
            } else {
                memcpy(translation, &pJoint->pKeyframesTrans->data, sizeof(translation));
            }
            for (keyframeIndex = pJoint->currentTransKeyframe;
                 keyframeIndex < pJoint->nKeyframesRot && pJoint->pKeyFramesRot[keyframeIndex].timeMs < animationTime;
                 ++keyframeIndex) {
                ;
            }
            pJoint->currentTransKeyframe = keyframeIndex;
            if (keyframeIndex) {
                if (keyframeIndex == pJoint->nKeyframesRot) {
                    // MatrixMath::CreateRotationMatrixEulerZYX(
                    //     &rotationMatrix,
                    //     (float (*)[3]) &pJoint->pKeyFramesRot[keyframeIndex - 1].data);
                    rotationMatrix.CreateRotationMatrixEulerZYX(pJoint->pKeyFramesRot[keyframeIndex - 1].data.position);
                } else {
                    nextRotationKey = &pJoint->pKeyFramesRot[keyframeIndex];
                    prevRotationKey = &pJoint->pKeyFramesRot[keyframeIndex - 1];
                    rotationTimeDelta = nextRotationKey->timeMs - prevRotationKey->timeMs;
                    interpolationFactor_Rot = (animationTime - prevRotationKey->timeMs) / rotationTimeDelta;
                    interpolatedRotation[0] = (nextRotationKey->data.rotation[2] - prevRotationKey->data.rotation[2])
                                              * interpolationFactor_Rot
                                              + prevRotationKey->data.rotation[2];
                    interpolatedRotation[1] = (nextRotationKey->data.rotation[1] - prevRotationKey->data.rotation[1])
                                              * interpolationFactor_Rot
                                              + prevRotationKey->data.rotation[1];
                    interpolatedRotation[2] = (nextRotationKey->data.rotation[2] - prevRotationKey->data.rotation[2])
                                              * interpolationFactor_Rot
                                              + prevRotationKey->data.rotation[2];
                    // MatrixMath::CreateRotationMatrixEulerZYX(&rotationMatrix, (float (*)[3]) interpolatedRotation);
                    rotationMatrix.CreateRotationMatrixEulerZYX(interpolatedRotation);
                }
            } else {
                // MatrixMath::CreateRotationMatrixEulerZYX(&rotationMatrix, (float (*)[3]) &pJoint->pKeyFramesRot->data);
                rotationMatrix.CreateRotationMatrixEulerZYX(pJoint->pKeyFramesRot->data.rotation);
            }
            // MatrixMath::SetTranslation(&rotationMatrix, translation);
            rotationMatrix.SetTranslation(translation);
            memcpy(&localTransform, &pJoint->localTransformationMatrix, sizeof(localTransform));
            // LOBYTE(_) = 1;
            // MatrixMath::Multiply(&localTransform, &rotationMatrix);
            localTransform.Multiply(&rotationMatrix);
            if (pJoint->parentIndex == -1) {
                // pLocalTransform = ObjectConstructor(&localTransform);
                // MatrixMath::Copy(&pJoint->finalTransformationMatrix, pLocalTransform);
                pJoint->finalTransformationMatrix.Copy(&localTransform);
            } else {
                // pParentMatrix = ObjectConstructor(&this->vJoints[pJoint->parentIndex].finalTransformationMatrix);
                // MatrixMath::Copy(&pJoint->finalTransformationMatrix, pParentMatrix);
                // MatrixMath::Multiply(&pJoint->finalTransformationMatrix, &localTransform);
                pJoint->finalTransformationMatrix.Copy(&this->vJoints[pJoint->parentIndex].finalTransformationMatrix);
                pJoint->finalTransformationMatrix.Multiply(&localTransform);
            }
            // LOBYTE(_) = 0;
            // noop_0();
            // _ = -1;
            // noop_0();
        } else {
            // pIdentityMatrix = ObjectConstructor(&pJoint->worldTransformationMatrix);
            // MatrixMath::Copy(&pJoint->finalTransformationMatrix, pIdentityMatrix);
            pJoint->finalTransformationMatrix.Copy(&pJoint->worldTransformationMatrix);
            // _ = -1;
            // noop_0();
        }
    }
}

void __stdcall OnFailLoadMilkshape(const char *modelName) {
    CHAR Text[200]; // [esp+4h] [ebp-C8h] BYREF

    sprintf(Text, "Couldn't load the model %s", modelName);
    MessageBoxA(0, Text, "Error", MB_ICONERROR);
    PostQuitMessage(0);
}

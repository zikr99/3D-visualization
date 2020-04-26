#ifndef __CBSPMESH_H__
#define __CBSPMESH_H__

#include <windows.h>
#include "irrArray.h"
#include "CMesh.h"
#include "SMaterial.h"
#include "aabbox.h"

enum eBSPLump
{
     kEntities = 0,  // Stores player/object positions, etc...
     kTextures, 
     kPlanes, 
     kNodes,
     kLeafs,       
     kLeafFaces,     // Stores the leaf's indices into the faces
     kLeafBrushes,   // Stores the leaf's indices into the brushes
     kModels,        // Stores the info of world models
     kBrushes,       
     kBrushSides,    
     kVertices,      
     kMeshVerts,     // Stores the model vertices offsets
     kShaders,       
     kFaces,         
     kLightmaps,     
     kLightVolumes,  // Stores extra world lighting information
     kVisData,       // Stores PVS and cluster info (visibility)
     kMaxLumps       
};

struct tBSPHeader
{
     int strID;      // This should always be 'IBSP'
     int version;    // This should be 0x2e for Quake 3 files
}; 

struct tBSPLump
{
     int offset;
     int length;
}; 

struct tBSPVertex
{
     float vPosition[3];      
     float vTextureCoord[2];  
     float vLightmapCoord[2]; 
     float vNormal[3];        
     byte color[4];     // RGBA color for the vertex 
};

struct tBSPFace
{
     int textureID;        
     int effect;           // The index for the effects (or -1 = n/a) 
     int type;             // 1=polygon, 2=patch, 3=mesh, 4=billboard 
     int vertexIndex;      // The index into this face's first vertex 
     int numOfVerts;       // The number of vertices for this face 
     int meshVertIndex;    // The index into the first meshvertex 
     int numMeshVerts;     // The number of mesh vertices 
     int lightmapID;       
     int lMapCorner[2];    // The face's lightmap corner in the image 
     int lMapSize[2];      // The size of the lightmap section 
     float lMapPos[3];     // The 3D origin of lightmap. 
     float lMapBitsets[2][3]; // The 3D space for s and t unit vectors. 
     float vNormal[3];     // The face normal. 
     int size[2];          // The bezier patch dimensions. 
};

struct tBSPPlane
{
     float vNormal[3];     // Plane normal. 
     float d;              // The plane distance from origin 
};

struct tBSPBrushSide 
{
     int plane;              // The plane index
     int textureID;          // The texture index
};

struct tBSPBrush 
{
     int brushSide;           // The starting brush side for the brush 
     int numOfBrushSides;     // Number of brush sides for the brush
     int textureID;           // The texture index for the brush
};

struct tBSPNode
{
     int plane;      // The index into the planes array 
     int front;      // The child index for the front node 
     int back;       // The child index for the back node 
     int mins[3];    // The bounding box min position. 
     int maxs[3];    // The bounding box max position. 
};

struct tBSPLeaf
{
     int cluster;           // The visibility cluster 
     int area;              // The area portal 
     int mins[3];           // The bounding box min position 
     int maxs[3];           // The bounding box max position 
     int leafface;          // The first index into the face array 
     int numOfLeafFaces;    // The number of faces for this leaf 
     int leafBrush;         // The first index for into the brushes 
     int numOfLeafBrushes;  // The number of brushes for this leaf 
};

struct tBSPVisData
{
     int numOfClusters;   // The number of clusters
     int bytesPerCluster; // Bytes (8 bits) in the cluster's bitset
     byte *pBitsets;      // Array of bytes holding the cluster vis.
};

struct tBSPTexture
{
     char strName[64];         // The name of the texture w/o the extension 
     unsigned int flags;       // The surface flags (unknown) 
     unsigned int contents;    // The content flags (unknown)
};

struct tBSPLightmap
{
     byte imageBits[128][128][3];   // The RGB data in a 128x128 image
};

struct tBSPModel 
{
     float min[3];           // The min position for the bounding box
     float max[3];           // The max position for the bounding box. 
     int faceIndex;          // The first face index in the model 
     int numOfFaces;         // The number of faces in the model 
     int brushIndex;         // The first brush index in the model 
     int numOfBrushes;       // The number brushes for the model
}; 

struct tBSPLights
{
     byte ambient[3];     // This is the ambient color in RGB
     byte directional[3]; // This is the directional color in RGB
     byte direction[2];   // The direction of the light: [phi,theta] 
};

struct tBSPShader
{
     byte strName[64];     // The name of the shader file 
     int brushIndex;       // The brush index for this shader 
     int unknown;          // This is 99% of the time 5
};

class CBSPBezier 
{
protected:     
     int unused0, unused1, unused2;
     int unused3, unused4, unused5;
     
     tBSPVertex modulateBSPVertices(tBSPVertex *controls[], float s[3], float t[3]);
     void tesselate(tBSPVertex *controls[], int L);
     
public:
     int level;    
     tBSPVertex *vertices;
     unsigned int *indices;   
         
     CBSPBezier(tBSPVertex *controls[], int level): level(0), vertices(NULL), indices(NULL)
     {    
          tesselate(controls, level);        
     }
     
     virtual ~CBSPBezier()
     {
          if (level > 0)
          {   
                delete [] vertices;
                delete [] indices;    
          }   
     }    
         
};

class CBSPMesh: public CMesh
{        
protected:
     byte *m_Bitsets;     
          
     void FreeBuffer();     
     virtual bool loadMaterials();
     virtual void addBeziers();     
          
public:
     tBSPLump Lumps[kMaxLumps];

     tBSPVertex* Vertices;
     int NumVertices;
     
     tBSPFace* Faces;
     int NumFaces;
     
     tBSPPlane* Planes;
     int NumPlanes;
     
     tBSPBrushSide* BrushSides;
     int NumBrushSides;
     
     tBSPBrush* Brushes;
     int NumBrushes;
     
     tBSPNode* Nodes;
     int NumNodes;
     
     tBSPLeaf* Leafs;
     int NumLeafs;
     
     int *LeafFaces;
     int NumLeafFaces;
     
     int *LeafBrushes;
     int NumLeafBrushes;

     tBSPTexture* Textures;
     int NumTextures;

     tBSPLightmap* Lightmaps;
     int NumLightmaps;

     int *MeshVerts;     // The vertex offsets for a mesh 
     int NumMeshVerts;
     
     tBSPVisData VisData;
     
     int *FaceFirstPatch;
     int *FaceNumPatches;
     
     array<CBSPBezier*> Beziers;
     array<SMaterial*> Materials;
     aabbox BBox;
  
     CBSPMesh(CSceneManager *mngr);
     CBSPMesh(CSceneManager *mngr, char *filename);
     virtual ~CBSPMesh();
     
     virtual bool loadFile(char *filename); 
     
};

#endif

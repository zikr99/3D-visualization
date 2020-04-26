#ifndef __CMD2MESH_H__
#define __CMD2MESH_H__

#include "CMesh.h"
#include "irrArray.h"
#include "SMaterial.h"
#include "aabbox.h"

struct modelHeader_t
{
      int ident;		// identifies as MD2 file "IDP2"
      int version;		// mine is 8
      int skinwidth;    // width of texture
      int skinheight;   // height of texture
      int framesize;    // number of bytes per frame
      int numSkins;     // number of textures
      int numXYZ;       // number of points
      int numST;        // number of texture
      int numTris;      // number of triangles
      int numGLcmds;
      int numFrames;    // total number of frames
      int offsetSkins;  // offset to skin names (64 bytes each)
      int offsetST;     // offset of texture s-t values
      int offsetTris;   // offset of triangle mesh
      int offsetFrames; // offset of frame data (points)
      int offsetGLcmds; // type of OpenGL commands to use
      int offsetEnd;    // end of file
};
  
// texture coordinate index
struct stIndex_t
{
      short s;
      short t;
};

// texture coordinate
struct texCoord_t
{
      float s;
      float t;
};

// info for a single frame point
struct framePoint_t
{
      unsigned char v[3];
      unsigned char normalIndex;
};

// information for a single frame
struct frame_t
{
      float scale[3];
      float translate[3];
      char name[16];
      framePoint_t fp[1];
};

// data for a single triangle
struct mesh_t
{
      unsigned short meshIndex[3];		// vertex indices
      unsigned short stIndex[3];		// texture coordinate indices
};

//single point
struct vector_t
{
      float point[3];
};

class CMD2Mesh: public CMesh
{
protected:      
      void FreeBuffer();
      virtual void loadSkin(char *skinfilename);
          
public:
      int NumFrames;
      int FrameSize;
          
      vector_t *Vertices;    
      int NumVertices;
      
      texCoord_t *STs;
      int NumSTs; 
      
      mesh_t *Triangles;
      int NumTriangles;
      
      SMaterial Material;
      array<aabbox> BBoxes;
       
      CMD2Mesh(CSceneManager *mngr);
      CMD2Mesh(CSceneManager *mngr, char *filename, char *skinfilename);
      virtual ~CMD2Mesh() { FreeBuffer(); } 
      
      virtual bool loadFile(char *filename, char *skinfilename);
};

#endif

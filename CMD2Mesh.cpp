#include "CMD2Mesh.h"
#include "CSceneManager.h"
#include <stdio.h>

CMD2Mesh::CMD2Mesh(CSceneManager *mngr): CMesh(mngr), NumFrames(0), FrameSize(0),
      NumVertices(0), NumSTs(0), NumTriangles(0), Vertices(NULL), STs(NULL), 
      Triangles(NULL)
{
      m_meshtype = EMST_MD2;                           
}

CMD2Mesh::CMD2Mesh(CSceneManager *mngr, char *filename, char* skinfilename): 
      CMesh(mngr), NumFrames(0), FrameSize(0), NumVertices(0), NumSTs(0), 
      NumTriangles(0), Vertices(NULL), STs(NULL), Triangles(NULL)
{
      m_meshtype = EMST_MD2;
      
      if ((filename != NULL) && (skinfilename != NULL)) loadFile(filename, skinfilename);                                 
}

bool CMD2Mesh::loadFile(char *filename, char *skinfilename)
{
      FILE *filePtr; 
      int fileLen; 
      char *buffer; 
      
      modelHeader_t *modelHeader;
      vector_t *vertexList;
      frame_t *frame;
      vector_t *vertexListPtr;
      float tempv;
      
      texCoord_t *st; 
      stIndex_t *stPtr;  
      mesh_t *bufIndexPtr;
      mesh_t *triIndex;
       
      int i, j, numread; 
      
      filePtr = fopen(filename, "rb");
      if (filePtr == NULL)
           return false;

      fseek(filePtr, 0, SEEK_END);  // find length of file
      fileLen = ftell(filePtr);
      fseek(filePtr, 0, SEEK_SET);

      buffer = new char [fileLen + 1];  //(char*)malloc(fileLen + 1);
      if (buffer == NULL)
      {
           fclose(filePtr);
           return false;
      } 
    
      numread = fread(buffer, sizeof(char), fileLen, filePtr);
      if (numread < fileLen)
      {
           delete [] buffer;
           fclose(filePtr);
           return false;
      }
  
      modelHeader = (modelHeader_t*)buffer;  // extract model file header from buffer
      vertexList = new vector_t [modelHeader->numXYZ*modelHeader->numFrames];
      if (vertexList == NULL)
      {
           delete [] buffer;
           fclose(filePtr);
           return false;
      }
  
      for (j = 0; j < modelHeader->numFrames; j++)
      {
           frame = (frame_t*)&buffer[modelHeader->offsetFrames + modelHeader->framesize*j];
           vertexListPtr = (vector_t*)&vertexList[modelHeader->numXYZ*j];
           
           for (i = 0; i < modelHeader->numXYZ; i++)
           {
                vertexListPtr[i].point[0] = frame->fp[i].v[0]*frame->scale[0] + frame->translate[0];
                vertexListPtr[i].point[1] = frame->fp[i].v[1]*frame->scale[1] + frame->translate[1];
                vertexListPtr[i].point[2] = frame->fp[i].v[2]*frame->scale[2] + frame->translate[2];
           }
           
           for (i = 0; i < modelHeader->numXYZ; i++)
           {
                tempv = vertexListPtr[i].point[1];
                vertexListPtr[i].point[1] = vertexListPtr[i].point[2];
                vertexListPtr[i].point[2] = -tempv;
           }
      }
  
      st = new texCoord_t [modelHeader->numST];
      if (st == NULL)
      {
           delete [] vertexList;
           delete [] buffer;
           fclose(filePtr);
           return false;
      }
  
      stPtr = (stIndex_t*)&buffer[modelHeader->offsetST];
      for (i = 0; i < modelHeader->numST; i++)
      {
           st[i].s = (float)stPtr[i].s/modelHeader->skinwidth;
           st[i].t = (float)stPtr[i].t/modelHeader->skinheight;
      }
  
      triIndex = new mesh_t [modelHeader->numTris];
      if (triIndex == NULL)
      {
           delete [] st;
           delete [] vertexList;
           delete [] buffer;
           fclose(filePtr);
           return false;
      }
  
      bufIndexPtr = (mesh_t*)&buffer[modelHeader->offsetTris];
      for(i = 0; i < modelHeader->numTris; i++) // for all triangles in a frame
      {
           triIndex[i].meshIndex[0] = bufIndexPtr[i].meshIndex[0];
           triIndex[i].meshIndex[1] = bufIndexPtr[i].meshIndex[1];
           triIndex[i].meshIndex[2] = bufIndexPtr[i].meshIndex[2];
           triIndex[i].stIndex[0] = bufIndexPtr[i].stIndex[0];
           triIndex[i].stIndex[1] = bufIndexPtr[i].stIndex[1];
           triIndex[i].stIndex[2] = bufIndexPtr[i].stIndex[2];
      }
      
      FreeBuffer();
      CMesh::loadFile(filename);
  
      NumFrames = modelHeader->numFrames;   
      FrameSize = modelHeader->framesize;
      
      NumVertices = modelHeader->numXYZ;
      Vertices = vertexList;  
      
      NumSTs = modelHeader->numST; 
      STs = st;
      
      NumTriangles = modelHeader->numTris; 
      Triangles = triIndex;
      
      aabbox BBox;
      
      for (j = 0; j < NumFrames; j++)
      {
           vertexListPtr = (vector_t*)&Vertices[NumVertices*j];
           
           BBox.reset(vertexListPtr[0].point[0], vertexListPtr[0].point[1], 
                vertexListPtr[0].point[2]);
           
           for (i = 0; i < NumVertices; i++)
                BBox.addInternalPoint(vertexListPtr[i].point[0], vertexListPtr[i].point[1], 
                     vertexListPtr[i].point[2]);
           
           BBoxes.push_back(BBox);
      }
      
      delete [] buffer;
      fclose(filePtr);
      
      loadSkin(skinfilename);
      return true;
}

void CMD2Mesh::loadSkin(char *skinfilename)
{     
      CTexture *tx;
       
      tx = new CTexture(); 
      tx->buildTextureBMP24(skinfilename);
      m_scenemanager->addTexture(tx);
      
      Material.MaterialType = EMT_TEXTURE;
      Material.Texture1 = tx;      
}

void CMD2Mesh::FreeBuffer()
{
      if (Vertices) delete [] Vertices;
      if (STs) delete [] STs;
      if (Triangles) delete [] Triangles;
      BBoxes.clear();
}

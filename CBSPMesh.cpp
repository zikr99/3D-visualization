#include "CBSPMesh.h"
#include "CSceneManager.h"
#include <stdio.h>
#include <string.h>

CBSPMesh::CBSPMesh(CSceneManager *mngr): CMesh(mngr),
      Vertices(NULL), Faces(NULL), Planes(NULL), BrushSides(NULL), Brushes(NULL),
      Nodes(NULL), Leafs(NULL), LeafFaces(NULL), LeafBrushes(NULL),
      Textures(NULL), Lightmaps(NULL), MeshVerts(NULL),
      m_Bitsets(NULL), FaceFirstPatch(NULL), FaceNumPatches(NULL),
      NumVertices(0), NumFaces(0), NumPlanes(0), NumBrushSides(0), NumBrushes(0),
      NumNodes(0), NumLeafs(0), NumLeafFaces(0), NumLeafBrushes(0),
      NumTextures(0), NumLightmaps(0), NumMeshVerts(0)
{
      m_meshtype = EMST_BSP;
      VisData.pBitsets = NULL;
      BBox.reset(0.0f, 0.0f, 0.0f);
}

CBSPMesh::CBSPMesh(CSceneManager *mngr, char *filename): CMesh(mngr),
      Vertices(NULL), Faces(NULL), Planes(NULL), BrushSides(NULL), Brushes(NULL),
      Nodes(NULL), Leafs(NULL), LeafFaces(NULL), LeafBrushes(NULL),
      Textures(NULL), Lightmaps(NULL), MeshVerts(NULL),
      m_Bitsets(NULL), FaceFirstPatch(NULL), FaceNumPatches(NULL),
      NumVertices(0), NumFaces(0), NumPlanes(0), NumBrushSides(0), NumBrushes(0),
      NumNodes(0), NumLeafs(0), NumLeafFaces(0), NumLeafBrushes(0),
      NumTextures(0), NumLightmaps(0), NumMeshVerts(0)
{
      m_meshtype = EMST_BSP;
      VisData.pBitsets = NULL;
      BBox.reset(0.0f, 0.0f, 0.0f);

      if (filename)
            loadFile(filename);
}

CBSPMesh::~CBSPMesh()
{
      FreeBuffer();
}

void CBSPMesh::FreeBuffer()
{
      if (Vertices) delete [] Vertices;
      if (Faces) delete [] Faces;
      if (Planes) delete [] Planes;
      if (BrushSides) delete [] BrushSides;
      if (Brushes) delete [] Brushes;
      if (Nodes) delete [] Nodes;
      if (Leafs) delete [] Leafs;
      if (LeafFaces) delete [] LeafFaces;
      if (LeafBrushes) delete [] LeafBrushes;
      if (Textures) delete [] Textures;
      if (Lightmaps) delete [] Lightmaps;
      if (MeshVerts) delete [] MeshVerts;
      if (m_Bitsets) delete [] m_Bitsets;
      if (FaceFirstPatch) delete [] FaceFirstPatch;
      if (FaceNumPatches) delete [] FaceNumPatches;

      for (int i = 0; i < Beziers.size(); i++) delete Beziers[i];
      Beziers.clear();

      for (int i = 0; i < Materials.size(); i++) delete Materials[i];
      Materials.clear();
}

bool CBSPMesh::loadFile(char *filename)
{
      FILE *filePtr;
      tBSPHeader header;
      tBSPLump lumps[kMaxLumps];
      float temp;
      int tempi;

      filePtr = fopen(filename, "rb");
      if (filePtr == NULL)
            return FALSE;

      fread(&header, sizeof(tBSPHeader), 1, filePtr);
      if (header.strID != 0x50534249 || header.version != 0x2e)
      {
            MessageBox(0, "Error", "BSP Load Error", 0);
            return FALSE;
      }

      CMesh::loadFile(filename);
      FreeBuffer();

      fread(lumps, kMaxLumps*sizeof(tBSPLump), 1, filePtr);

      Vertices = (tBSPVertex*)malloc(lumps[kVertices].length);
      Faces = (tBSPFace*)malloc(lumps[kFaces].length);
      Planes = (tBSPPlane*)malloc(lumps[kPlanes].length);
      BrushSides = (tBSPBrushSide*)malloc(lumps[kBrushSides].length);
      Brushes = (tBSPBrush*)malloc(lumps[kBrushes].length);
      Nodes = (tBSPNode*)malloc(lumps[kNodes].length);
      Leafs = (tBSPLeaf*)malloc(lumps[kLeafs].length);
      LeafFaces = (int*)malloc(lumps[kLeafFaces].length);
      LeafBrushes = (int*)malloc(lumps[kLeafBrushes].length);
      Textures = (tBSPTexture*)malloc(lumps[kTextures].length);
      Lightmaps = (tBSPLightmap*)malloc(lumps[kLightmaps].length);
      MeshVerts = (int*)malloc(lumps[kMeshVerts].length);
      m_Bitsets = (byte*)malloc(lumps[kVisData].length - 2*sizeof(int));

      NumVertices = lumps[kVertices].length/sizeof(tBSPVertex);
      NumFaces = lumps[kFaces].length/sizeof(tBSPFace);
      NumPlanes = lumps[kPlanes].length/sizeof(tBSPPlane);
      NumBrushSides = lumps[kBrushSides].length/sizeof(tBSPBrushSide);
      NumBrushes = lumps[kBrushes].length/sizeof(tBSPBrush);
      NumNodes = lumps[kNodes].length/sizeof(tBSPNode);
      NumLeafs = lumps[kLeafs].length/sizeof(tBSPLeaf);
      NumLeafFaces = lumps[kLeafFaces].length/sizeof(int);
      NumLeafBrushes = lumps[kLeafBrushes].length/sizeof(int);
      NumTextures = lumps[kTextures].length/sizeof(tBSPTexture);
      NumLightmaps = lumps[kLightmaps].length/sizeof(tBSPLightmap);
      NumMeshVerts = lumps[kMeshVerts].length/sizeof(int);

      fseek(filePtr, lumps[kVertices].offset, SEEK_SET);
      fread(Vertices, lumps[kVertices].length, 1, filePtr);
      fseek(filePtr, lumps[kFaces].offset, SEEK_SET);
      fread(Faces, lumps[kFaces].length, 1, filePtr);
      fseek(filePtr, lumps[kPlanes].offset, SEEK_SET);
      fread(Planes, lumps[kPlanes].length, 1, filePtr);
      fseek(filePtr, lumps[kBrushSides].offset, SEEK_SET);
      fread(BrushSides, lumps[kBrushSides].length, 1, filePtr);
      fseek(filePtr, lumps[kBrushes].offset, SEEK_SET);
      fread(Brushes, lumps[kBrushes].length, 1, filePtr);
      fseek(filePtr, lumps[kNodes].offset, SEEK_SET);
      fread(Nodes, lumps[kNodes].length, 1, filePtr);
      fseek(filePtr, lumps[kLeafs].offset, SEEK_SET);
      fread(Leafs, lumps[kLeafs].length, 1, filePtr);
      fseek(filePtr, lumps[kLeafFaces].offset, SEEK_SET);
      fread(LeafFaces, lumps[kLeafFaces].length, 1, filePtr);
      fseek(filePtr, lumps[kLeafBrushes].offset, SEEK_SET);
      fread(LeafBrushes, lumps[kLeafBrushes].length, 1, filePtr);
      fseek(filePtr, lumps[kTextures].offset, SEEK_SET);
      fread(Textures, lumps[kTextures].length, 1, filePtr);
      fseek(filePtr, lumps[kLightmaps].offset, SEEK_SET);
      fread(Lightmaps, lumps[kLightmaps].length, 1, filePtr);
      fseek(filePtr, lumps[kMeshVerts].offset, SEEK_SET);
      fread(MeshVerts, lumps[kMeshVerts].length, 1, filePtr);
      fseek(filePtr, lumps[kVisData].offset, SEEK_SET);
      fread(&(VisData.numOfClusters), sizeof(int), 1, filePtr);
      fread(&(VisData.bytesPerCluster), sizeof(int), 1, filePtr);
      fread(m_Bitsets, lumps[kVisData].length - 2*sizeof(int), 1, filePtr);
      VisData.pBitsets = m_Bitsets;

      fclose(filePtr);

      for (int i = 0; i < NumVertices; i++)
      {
             temp = Vertices[i].vPosition[1];
             Vertices[i].vPosition[1] = Vertices[i].vPosition[2];
             Vertices[i].vPosition[2] = -temp;

             temp = Vertices[i].vNormal[1];
             Vertices[i].vNormal[1] = Vertices[i].vNormal[2];
             Vertices[i].vNormal[2] = -temp;

             Vertices[i].vTextureCoord[1] = -Vertices[i].vTextureCoord[1];
      }

      BBox.reset(Vertices[0].vPosition[0], Vertices[0].vPosition[1], Vertices[0].vPosition[2]);
      for (int i = 0; i < NumVertices; i++)
             BBox.addInternalPoint(Vertices[i].vPosition[0], Vertices[i].vPosition[1],
                  Vertices[i].vPosition[2]);

      for (int i = 0; i < NumFaces; i++)
      {
             temp = Faces[i].vNormal[1];
             Faces[i].vNormal[1] = Faces[i].vNormal[2];
             Faces[i].vNormal[2] = -temp;
      }

      for (int i = 0; i < NumPlanes; i++)
      {
             temp = Planes[i].vNormal[1];
             Planes[i].vNormal[1] = Planes[i].vNormal[2];
             Planes[i].vNormal[2] = -temp;
      }

      for (int i = 0; i < NumNodes; i++)
      {
             tempi = Nodes[i].mins[1];
             Nodes[i].mins[1] = Nodes[i].mins[2];
             Nodes[i].mins[2] = -tempi;

             tempi = Nodes[i].maxs[1];
             Nodes[i].maxs[1] = Nodes[i].maxs[2];
             Nodes[i].maxs[2] = -tempi;
      }

      for (int i = 0; i < NumLeafs; i++)
      {
             tempi = Leafs[i].mins[1];
             Leafs[i].mins[1] = Leafs[i].mins[2];
             Leafs[i].mins[2] = -tempi;

             tempi = Leafs[i].maxs[1];
             Leafs[i].maxs[1] = Leafs[i].maxs[2];
             Leafs[i].maxs[2] = -tempi;
      }

      addBeziers();

      int numlv;
      tBSPVertex *vlv;

      for (int i = 0; i < Beziers.size(); i++)
      {
             numlv = Beziers[i]->level*Beziers[i]->level*4;
             vlv = Beziers[i]->vertices;

             for (int j = 0; j < numlv; j++)
                  BBox.addInternalPoint(vlv[j].vPosition[0], vlv[j].vPosition[1],
                        vlv[j].vPosition[2]);
      }

      loadMaterials();
      return TRUE;
}

void CBSPMesh::addBeziers()
{
      int cpidx[9];
      tBSPVertex *cts[9];
      CBSPBezier *tempBzr;

      if (NumFaces > 0)
      {
             FaceFirstPatch = (int*)malloc(NumFaces*sizeof(int));
             FaceNumPatches = (int*)malloc(NumFaces*sizeof(int));
      }

      for (int i = 0; i < NumFaces; i++)
      {
             if (Faces[i].type == 2)
             {
                  FaceFirstPatch[i] = Beziers.size();
                  FaceNumPatches[i] = ((Faces[i].size[1] - 1)/2)*((Faces[i].size[0] - 1)/2);

                  for (int r = 0; r < Faces[i].size[1] - 2; r+=2)
                        for (int c = 0; c < Faces[i].size[0] - 2; c+=2)
                        {
                              cpidx[0] = (r*Faces[i].size[0] + c) + Faces[i].vertexIndex;
			                  cpidx[1] = (r*Faces[i].size[0] + c + 1) + Faces[i].vertexIndex;
			                  cpidx[2] = (r*Faces[i].size[0] + c + 2) + Faces[i].vertexIndex;
			                  cpidx[3] = ((r + 1)*Faces[i].size[0] + c) + Faces[i].vertexIndex;
			                  cpidx[4] = ((r + 1)*Faces[i].size[0] + c + 1) + Faces[i].vertexIndex;
			                  cpidx[5] = ((r + 1)*Faces[i].size[0] + c + 2) + Faces[i].vertexIndex;
			                  cpidx[6] = ((r + 2)*Faces[i].size[0] + c) + Faces[i].vertexIndex;
			                  cpidx[7] = ((r + 2)*Faces[i].size[0] + c + 1) + Faces[i].vertexIndex;
			                  cpidx[8] = ((r + 2)*Faces[i].size[0] + c + 2) + Faces[i].vertexIndex;

			                  for (int j = 0; j < 9; j++) cts[j] = &Vertices[cpidx[j]];

			                  tempBzr = new CBSPBezier(cts, 3);
			                  Beziers.push_back(tempBzr);
                        }
             }
      }
}

bool CBSPMesh::loadMaterials()
{
      char *strNameBmp = new char [1];
      CTexture *tx;
      array<CTexture*> texs;
      array<CTexture*> lmtexs;
      SMaterial *mat;

      tx = new CTexture();
      texs.push_back(tx);
      m_scenemanager->addTexture(tx);

      for (int i = 0; i < NumTextures; i++)
      {
            delete [] strNameBmp;
            strNameBmp = new char [strlen(Textures[i].strName) + 4];
            strcpy(strNameBmp, Textures[i].strName);
            strcat(strNameBmp, ".bmp"); //MessageBox(0, strNameBmp, strNameBmp, 0);

            tx = new CTexture();
            tx->buildTextureBMP24(strNameBmp);

            texs.push_back(tx);
            m_scenemanager->addTexture(tx);
      }

      tx = new CTexture();
      lmtexs.push_back(tx);
      m_scenemanager->addTexture(tx);

      for (int i = 0; i < NumLightmaps; i++)
      {
            tx = new CTexture();
            tx->buildTexture((byte*)(Lightmaps[i].imageBits), 128, 128, GL_RGB);

            lmtexs.push_back(tx);
            m_scenemanager->addTexture(tx);
      }

      for (int i = 0; i < NumTextures + 1; i++)
      {
            for (int j = 0; j < NumLightmaps + 1; j++)
            {
                  mat = new SMaterial();
                  mat->Texture1 = texs[i];
                  mat->Texture2 = lmtexs[j];
                  mat->MaterialType = EMT_TEXTURE_LIGHTMAP;

                  Materials.push_back(mat);
            }
      }

      delete [] strNameBmp;
}

tBSPVertex CBSPBezier::modulateBSPVertices(tBSPVertex *controls[], float s[3], float t[3])
{
      tBSPVertex temp;

      temp.vPosition[0] = 0;
      temp.vPosition[1] = 0;
      temp.vPosition[2] = 0;
      temp.vNormal[0] = 0;
      temp.vNormal[1] = 0;
      temp.vNormal[2] = 0;
      temp.vTextureCoord[0] = 0;
      temp.vTextureCoord[1] = 0;
      temp.vLightmapCoord[0] = 0;
      temp.vLightmapCoord[1] = 0;

      for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                  temp.vPosition[0] = temp.vPosition[0] + controls[i*3 + j]->vPosition[0]*s[i]*t[j];
                  temp.vPosition[1] = temp.vPosition[1] + controls[i*3 + j]->vPosition[1]*s[i]*t[j];
                  temp.vPosition[2] = temp.vPosition[2] + controls[i*3 + j]->vPosition[2]*s[i]*t[j];
                  temp.vNormal[0] = temp.vNormal[0] + controls[i*3 + j]->vNormal[0]*s[i]*t[j];
                  temp.vNormal[1] = temp.vNormal[1] + controls[i*3 + j]->vNormal[1]*s[i]*t[j];
                  temp.vNormal[2] = temp.vNormal[2] + controls[i*3 + j]->vNormal[2]*s[i]*t[j];
                  temp.vTextureCoord[0] = temp.vTextureCoord[0] + controls[i*3 + j]->vTextureCoord[0]*s[i]*t[j];
                  temp.vTextureCoord[1] = temp.vTextureCoord[1] + controls[i*3 + j]->vTextureCoord[1]*s[i]*t[j];
                  temp.vLightmapCoord[0] = temp.vLightmapCoord[0] + controls[i*3 + j]->vLightmapCoord[0]*s[i]*t[j];
                  temp.vLightmapCoord[1] = temp.vLightmapCoord[1] + controls[i*3 + j]->vLightmapCoord[1]*s[i]*t[j];
            }

      return temp;
}

void CBSPBezier::tesselate(tBSPVertex *controls[], int L)
{
      if (!(L > 0))
            return;

      level = L;

      vertices = (tBSPVertex*)malloc(L*L*4*sizeof(tBSPVertex));
      indices = (unsigned int*)malloc(L*L*6*sizeof(int));

      float cs[3], nxs[3], ct[3], nxt[3];
      float ss = 0.0f;
      float ts;
      float sstep = 1.0f/L;
      float tstep = 1.0f/L;

      for (int i = 0; i < L; i++)
      {
            cs[0] = (1.0f - ss)*(1.0f - ss);
			cs[1] = 2.0f*(1.0f - ss)*ss;
			cs[2] = ss*ss;
			nxs[0] = (1.0f - ss - sstep)*(1.0f - ss - sstep);
			nxs[1] = 2.0f*(1.0f - ss - sstep)*(ss + sstep);
			nxs[2] = (ss + sstep)*(ss + sstep);
			ss = ss + sstep;
            ts = 0.0f;

            for (int j = 0; j < L; j++)
            {
                  ct[0] = (1.0f - ts)*(1.0f - ts);
				  ct[1] = 2.0f*(1.0f - ts)*ts;
				  ct[2] = ts*ts;
				  nxt[0] = (1.0f - ts - tstep)*(1.0f - ts - tstep);
				  nxt[1] = 2.0f*(1.0f - ts - tstep)*(ts + tstep);
				  nxt[2] = (ts + tstep)*(ts + tstep);
                  ts = ts + tstep;

                  vertices[(i*L + j)*4] = modulateBSPVertices(controls, cs, ct);
                  vertices[(i*L + j)*4 + 1] = modulateBSPVertices(controls, cs, nxt);
                  vertices[(i*L + j)*4 + 2] = modulateBSPVertices(controls, nxs, ct);
                  vertices[(i*L + j)*4 + 3] = modulateBSPVertices(controls, nxs, nxt);
            }
      }

      for (int i = 0; i < L; i++)
      {
            for (int j = 0; j < L; j++)
            {
                  indices[(i*L + j)*6] = (i*L + j)*4;
                  indices[(i*L + j)*6 + 1] = (i*L + j)*4 + 2;
                  indices[(i*L + j)*6 + 2] = (i*L + j)*4 + 1;
                  indices[(i*L + j)*6 + 3] = (i*L + j)*4 + 1;
                  indices[(i*L + j)*6 + 4] = (i*L + j)*4 + 2;
                  indices[(i*L + j)*6 + 5] = (i*L + j)*4 + 3;
            }
      }
}

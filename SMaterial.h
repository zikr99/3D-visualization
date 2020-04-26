#ifndef __SMATERIAL_H__
#define __SMATERIAL_H__

#define MATERIAL_MAX_TEXTURES 2

#include "SColor4f.h"
#include "CTexture.h"
#include "DriverExtension.h"

enum E_MATERIAL_TYPE
{
      EMT_COLOR = 0,
      EMT_LIGHTING,
      EMT_TEXTURE,
      EMT_COLOR_LIGHTING,
      EMT_COLOR_TEXTURE,
      EMT_LIGHTING_TEXTURE,
      EMT_COLOR_LIGHTING_TEXTURE,
      EMT_TEXTURE_LIGHTMAP,
      EMT_COLOR_TEXTURE_LIGHTMAP,
      EMT_LIGHTING_TEXTURE_LIGHTMAP,
      EMT_COLOR_LIGHTING_TEXTURE_LIGHTMAP,
      EMT_MATERIAL_TYPE_COUNT,

      EMT_FORCE_32BIT = 0x7fffffff	
};

enum E_MATERIAL_FLAG
{
     EMF_BLEND_ENABLE = 0,
     EMF_FOG_ENABLE,
     EMF_MATERIAL_FLAG_COUNT 
};

struct SMaterial
{ 
      E_MATERIAL_TYPE MaterialType; 
      SColor4f AmbientColor;		
	  SColor4f DiffuseColor;		
      SColor4f SpecularColor;
	  SColor4f EmissiveColor;	
	  float Shininess;
	  
	  union
      {
            struct    
            {
                   CTexture* Texture1;	
                   CTexture* Texture2;	
            };

            CTexture* Textures[MATERIAL_MAX_TEXTURES];
      };
      
      union
      {
            struct
			{
                  bool BlendEnable;						
                  bool FogEnable;
            };

            bool Flags[EMF_MATERIAL_FLAG_COUNT];
      };
      
      SMaterial(): MaterialType(EMT_COLOR), AmbientColor(1.0f, 1.0f, 1.0f, 1.0f),
            DiffuseColor(1.0f, 1.0f, 1.0f, 1.0f), SpecularColor(0.0f, 0.0f, 0.0f, 0.0f), 
            EmissiveColor(0.0f, 0.0f, 0.0f, 0.0f), Shininess(10.0f), 
            Texture1(NULL), Texture2(NULL), BlendEnable(FALSE), FogEnable(FALSE) 
      {}
      
      inline bool operator!=(const SMaterial &b)
	  {
  	        return MaterialType != b.MaterialType ||
                   AmbientColor != b.AmbientColor ||
                   SpecularColor != b.SpecularColor ||
	               DiffuseColor != b.DiffuseColor ||
	               EmissiveColor != b.EmissiveColor ||
	               Shininess != b.Shininess ||
	               BlendEnable != b.BlendEnable ||
	               FogEnable != b.FogEnable;
      }
      
      inline bool operator==(const SMaterial &b)
      {
            return !((*this) != b);   
      }
      
      inline void applyLightingMaterial()
      {
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, AmbientColor.rgba);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, DiffuseColor.rgba);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, SpecularColor.rgba);
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, EmissiveColor.rgba);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Shininess);
      }
      
      inline void resetMaterialMode(E_MATERIAL_TYPE mode)
      {
            switch (mode)
            {
            case EMT_COLOR:
                   break;
            case EMT_LIGHTING:
                   glDisable(GL_COLOR_MATERIAL);
                   break;
            case EMT_TEXTURE:
                   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
                   break;
            case EMT_COLOR_LIGHTING:
                   glEnable(GL_COLOR_MATERIAL);
                   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
                   break;
            case EMT_COLOR_TEXTURE:
                   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
                   break;
            case EMT_LIGHTING_TEXTURE:
                   glDisable(GL_COLOR_MATERIAL);
                   glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
                   break;
            case EMT_COLOR_LIGHTING_TEXTURE:
                   glEnable(GL_COLOR_MATERIAL);
                   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
                   glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
                   break;
            case EMT_TEXTURE_LIGHTMAP:
                   break;
            case EMT_COLOR_TEXTURE_LIGHTMAP:
                   break;
            case EMT_LIGHTING_TEXTURE_LIGHTMAP:
                   glDisable(GL_COLOR_MATERIAL);
                   glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
            case EMT_COLOR_LIGHTING_TEXTURE_LIGHTMAP:  
                   glEnable(GL_COLOR_MATERIAL);
                   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE); 
                   glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);    
            } 
      }
      
      inline void resetMaterialMode(E_MATERIAL_TYPE mode, GLint param)
      {
            switch (mode)
            {
            case EMT_TEXTURE:
                   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param);
                   break;
            case EMT_COLOR_TEXTURE:
                   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, param);
                   break;    
            } 
      }
      
      inline void activate(GLint param = -1)
      {
            if (BlendEnable)
            {
                   //
                            
                   if (FogEnable)
                   {
                          //       
                   }
                   else
                   {
                          //
                   }        
            }
            else
            {
                   //         
            } 
             
            switch (MaterialType)
            {
            case EMT_COLOR:
                 glDisable(GL_LIGHTING);
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glDisable(GL_TEXTURE_2D);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
                 glDisable(GL_TEXTURE_2D);
                 resetMaterialMode(EMT_COLOR);
                 break;
                 
            case EMT_LIGHTING:
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glDisable(GL_TEXTURE_2D);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
                 glDisable(GL_TEXTURE_2D);
                 glEnable(GL_LIGHTING);
                 applyLightingMaterial();
                 resetMaterialMode(EMT_LIGHTING);
                 break;
                 
            case EMT_TEXTURE:
                 glDisable(GL_LIGHTING);
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glEnable(GL_TEXTURE_2D);
                 if (Texture1) Texture1->activate();
                 if (param < 0) resetMaterialMode(EMT_TEXTURE);
                 else resetMaterialMode(EMT_TEXTURE, param);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
                 glDisable(GL_TEXTURE_2D);
                 break;
                 
            case EMT_COLOR_LIGHTING:
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glDisable(GL_TEXTURE_2D);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
                 glDisable(GL_TEXTURE_2D);
                 glEnable(GL_LIGHTING);
                 applyLightingMaterial();
                 resetMaterialMode(EMT_COLOR_LIGHTING);
                 break;
                 
            case EMT_COLOR_TEXTURE:
                 glDisable(GL_LIGHTING);
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glEnable(GL_TEXTURE_2D);
                 if (Texture1) Texture1->activate();
                 if (param < 0) resetMaterialMode(EMT_COLOR_TEXTURE);
                 else resetMaterialMode(EMT_COLOR_TEXTURE, param);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
                 glDisable(GL_TEXTURE_2D);
                 break;
                 
            case EMT_LIGHTING_TEXTURE:
                 glEnable(GL_LIGHTING);
                 applyLightingMaterial();
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glEnable(GL_TEXTURE_2D);
                 if (Texture1) Texture1->activate();
                 resetMaterialMode(EMT_LIGHTING_TEXTURE);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
                 glDisable(GL_TEXTURE_2D);
                 break;
                 
            case EMT_COLOR_LIGHTING_TEXTURE:
                 glEnable(GL_LIGHTING);
                 applyLightingMaterial();
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glEnable(GL_TEXTURE_2D);
                 if (Texture1) Texture1->activate();
                 resetMaterialMode(EMT_COLOR_LIGHTING_TEXTURE);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
                 glDisable(GL_TEXTURE_2D);
                 break;
                 
            case EMT_TEXTURE_LIGHTMAP:
                 glDisable(GL_LIGHTING);
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glEnable(GL_TEXTURE_2D);
                 if (Texture1) Texture1->activate();
	             glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
	             glEnable(GL_TEXTURE_2D);
	             if (Texture2) Texture2->activate();
		         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	             glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
		         glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	             glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
	             glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
		         glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);
                 glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 4.0f); 
                 resetMaterialMode(EMT_TEXTURE_LIGHTMAP);
                 break;
                 
            case EMT_COLOR_TEXTURE_LIGHTMAP:
                 glDisable(GL_LIGHTING);
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glEnable(GL_TEXTURE_2D);
                 if (Texture1) Texture1->activate();
	             glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
	             glEnable(GL_TEXTURE_2D);
	             if (Texture2) Texture2->activate();
		         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	             glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
		         glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	             glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
	             glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
		         glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);
                 glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 4.0f);
                 resetMaterialMode(EMT_COLOR_TEXTURE_LIGHTMAP);
                 break;
                 
            case EMT_LIGHTING_TEXTURE_LIGHTMAP:
                 glEnable(GL_LIGHTING);
                 applyLightingMaterial();
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glEnable(GL_TEXTURE_2D);
                 if (Texture1) Texture1->activate();
	             glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
	             glEnable(GL_TEXTURE_2D);
	             if (Texture2) Texture2->activate();
		         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	             glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
		         glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	             glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
	             glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
		         glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);
                 glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 4.0f);
                 resetMaterialMode(EMT_LIGHTING_TEXTURE_LIGHTMAP);
                 break;
                 
            case EMT_COLOR_LIGHTING_TEXTURE_LIGHTMAP:       
                 glEnable(GL_LIGHTING);
                 applyLightingMaterial();
                 glActiveTextureARB(GL_TEXTURE0_ARB);
                 glEnable(GL_TEXTURE_2D);
                 if (Texture1) Texture1->activate();
	             glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
                 glActiveTextureARB(GL_TEXTURE1_ARB);
	             glEnable(GL_TEXTURE_2D);
	             if (Texture2) Texture2->activate();
		         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	             glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);
		         glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_EXT, GL_SRC_COLOR);
	             glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
	             glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_EXT, GL_TEXTURE);
		         glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_EXT, GL_SRC_COLOR);
                 glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 4.0f);
                 resetMaterialMode(EMT_COLOR_LIGHTING_TEXTURE_LIGHTMAP);
                 break;
                 
            default:        
                 break;
            }
      }
		
};

#endif

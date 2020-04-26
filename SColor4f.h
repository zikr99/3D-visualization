#ifndef __SCOLOR_H__
#define __SCOLOR_H__

class SColor4f
{
public:
      union
      {     
            struct
            { 
                  float r;
	              float g;
	              float b;
	              float a;
            };
            
            float rgba[4];
      };
       
	  SColor4f(): r(0.0f), g(0.0f), b(0.0f), a(0.0f) {};
	  SColor4f(float rc, float gc, float bc): r(rc), g(gc), b(bc), a(1.0f) {};
	  SColor4f(float rc, float gc, float bc, float ac) : r(rc), g(gc), b(bc), a(ac) {};
	  
	  void set(float rr, float gg, float bb) { r = rr; g = gg; b = bb; };
	  void set(float rr, float gg, float bb, float aa) { r = rr; g = gg; b = bb; a = aa; };
	  
      inline SColor4f getInterpolated(SColor4f other, float d) const
	  {
			float inv = 1.0f - d;
			return SColor4f(other.r*inv + r*d,
				other.g*inv + g*d, other.b*inv + b*d, other.a*inv + a*d);
	  }
	  
	  inline bool operator!=(const SColor4f &other) const 
      { 
            return (r != other.r ||
                    g != other.g ||
                    b != other.b ||
                    a != other.a);
      }
      
      inline bool operator==(const SColor4f &other) const
      {
            return !((*this) != other); 
      }
	  
};

#endif

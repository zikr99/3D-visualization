#ifndef __CNODE_H__
#define __CNODE_H__

#include <windows.h>

class CNode
{
public:
      CNode *parentNode;
      CNode *childNode;
      CNode *prevNode;
      CNode *nextNode;
      
      CNode()	
	  {
           parentNode = childNode = NULL;
           prevNode = nextNode = this;
      } 

      CNode(CNode *node)
	  {
           parentNode = childNode = NULL;
           prevNode = nextNode = this;
           attachTo(node);
	  }
	  
	  virtual ~CNode()
	  {
           detach();
           clear();
	  }
	  
      bool hasParent() { return (parentNode != NULL); }
      bool hasChild() { return (childNode != NULL); }
      
      bool isFirstChild()
      {
           if (parentNode)
                return (parentNode->childNode == this);
           else
                return FALSE;
      }

      bool isLastChild()
      {
           if (parentNode)
                return (parentNode->childNode->prevNode == this);
           else
                return FALSE;
      }
	  
      void detach()
      {
           if (parentNode && parentNode->childNode == this)
           {
                if (nextNode != this)
                      parentNode->childNode = nextNode;
                else
                      parentNode->childNode = NULL;	
           }

           prevNode->nextNode = nextNode;
           nextNode->prevNode = prevNode;

           prevNode = this;
           nextNode = this;
           parentNode = NULL;
      }
	  
	  void attachTo(CNode *newParent)
      {
           if (parentNode)
                detach();
           
           if (!(newParent != NULL)) return;
           
           parentNode = newParent;

           if (parentNode->childNode)
           {
                prevNode = parentNode->childNode->prevNode;
                nextNode = parentNode->childNode;
                parentNode->childNode->prevNode->nextNode = this;
                parentNode->childNode->prevNode = this;
           }
           else
           {
                parentNode->childNode = this;
           }
      }
      
      void attach(CNode *newChild)
      {
           if (newChild->hasParent())
                newChild->detach();

           newChild->parentNode = this;

           if (childNode)
           {
                newChild->prevNode = childNode->prevNode;
                newChild->nextNode = childNode;
                childNode->prevNode->nextNode = newChild;
                childNode->prevNode = newChild;
           }
           else
                childNode = newChild;
      }
      
      void clear()
      {
           while (childNode)
                delete childNode; 
      }

};

#endif

#ifndef OCTREE_H
#define OCTREE_H


#include "ItemsManager.h"
#include "../!!adGlobals/vector_math.h"
#include <conio.h>
#include <list>

#define STATISTICS_ENABLED


extern float fBoxCollisionTimeCheck;
extern float fTrianCollisionTimeCheck;

extern DWORD gBoxCountProcessed;
extern DWORD gTrianCountChecked;

extern LARGE_INTEGER ticksPerSecond;

class Octree;

class OctreeNode
{
public:
	friend class Octree;

	bool bDrawLeavesOnly;
	static unsigned int nMinVerticesInNode;

	void Draw(bool bDrawMarks);

	void __DebugDrawAllTriangles();

	//constructor of root node
	OctreeNode(Octree* _carrier);
	~OctreeNode();

	void RayIntersect(std::list<OctreeNode*>* lHit, Vec3 ray_orig, Vec3 ray_dir);

	unsigned int GetPrivateIndicesBufferedItemsNumber();
	void GetPrivateIndicesItem(unsigned int ind, TriVertexIndex* tri_vertex_ind);

	void ClearMarkedNodes();

protected:
private:
	// constructor of nodes with level > 0
	OctreeNode(Octree* _carrier, Vec3 _ptMin, Vec3 _ptMax);

	// every node should has pointer to Octree mega-structure
	Octree *carrier;

	//private indices current node possesses
	ItemsManager<TriVertexIndex> privateIndices;

	// pointer to structures of type "us" depicting 8 subspaces
	// structures are 'non-null' in case [this node is collapsed] AND [particular node space contains vertices]
	OctreeNode* nodes[8];

	//depicter of current node position in space
	Vec3 ptMin, ptMax;

	//flags whether node is collapsed and has propagated private indices to its 8 nodes
	bool bCollapsed;
	
	unsigned short int iLevel;
	bool bMarked;

	//every call to collapse divides every node space into 8 nodes
	void Collapse();

	OctreeNode* GetNode(unsigned char ind, Vec3 ptMin, Vec3 ptMax);

	void __DebugDrawPrivateTriangles();
};


class Octree
{
friend class OctreeNode;

public:
	Octree(ItemsManager<TriVertex>* _vertices, ItemsManager<TriVertexIndex>* _indices);
	~Octree();
	
	OctreeNode* root_node;

	void Collapse(unsigned short int n)
	{
		for (unsigned short int i=0; i<n; i++)
			root_node->Collapse();
	}

	unsigned int m_uiNodesCount;
	unsigned int m_uiLeavesCount;
	DWORD gTrianColReqCount;

	void GetgVerticesItem(unsigned int ind, TriVertex* tri_vertex);

protected:
private:
	ItemsManager<TriVertex>* m_vertices;
	ItemsManager<TriVertexIndex>* m_indices;
};


// Utilities
bool ClosestTriangleOnRay(Octree* octree, Vec3 ray_orig, Vec3 ray_dir, Triangle& triangleOut, Vec3& ptIntersect, Vec3& vNormalIntersect);
void TestOctree(Octree* octree, Vec3 ptOrigin);


#endif
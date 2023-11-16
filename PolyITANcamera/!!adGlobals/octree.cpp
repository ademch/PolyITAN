#include "stdafx.h"
#include "octree.h"
#include <gl/GL.h>
#include <MMSystem.h>

// static octreeNode variables
unsigned int OctreeNode::nMinVerticesInNode = 10;

// global variables
DWORD gBoxCountProcessed = 0;
DWORD gTrianCountChecked = 0;
float fBoxCollisionTimeCheck   = 0;
float fTrianCollisionTimeCheck = 0;

LARGE_INTEGER _dCurTime;
LARGE_INTEGER _dCurTime2;


LARGE_INTEGER ticksPerSecond;


Octree::Octree(ItemsManager<TriVertex>* _vertices, ItemsManager<TriVertexIndex>* _indices): m_vertices(_vertices), m_indices(_indices)
{
	m_uiNodesCount  = 0;
	m_uiLeavesCount = 0;
	
	gTrianColReqCount = 0;

	root_node = new OctreeNode(this);
}

Octree::~Octree()
{
	delete root_node;
}

void Octree::GetgVerticesItem(unsigned int ind, TriVertex* tri_vertex)
{	ASSERT(ind < m_vertices->m_uiBufferedItemsNumber);
	*tri_vertex = m_vertices->items[ind];
}


OctreeNode::OctreeNode(Octree* _carrier): carrier(_carrier)
{
	ZeroMemory(nodes, sizeof(OctreeNode*)*8);
	bCollapsed      = false;
	bDrawLeavesOnly = true;
	bMarked         = false;
	iLevel          = 0;

	// copy all index class data (with user defined	operator=)
	privateIndices = *carrier->m_indices;

	ptMin = Vecc3( 10e4, 10e4, 10e4);
	ptMax = Vecc3(-10e4,-10e4,-10e4);

	// just iterate through all vertices not regarding indices
	// and initialize ptMin and ptMax for root node
	for (unsigned int i=0; i<carrier->m_vertices->m_uiBufferedItemsNumber; i++)
	{
		if (carrier->m_vertices->items[i].v.X < ptMin.X )
			ptMin.X = carrier->m_vertices->items[i].v.X;
		else if (carrier->m_vertices->items[i].v.X > ptMax.X )
			ptMax.X = carrier->m_vertices->items[i].v.X;

		if (carrier->m_vertices->items[i].v.Y < ptMin.Y )
			ptMin.Y = carrier->m_vertices->items[i].v.Y;
		else if (carrier->m_vertices->items[i].v.Y > ptMax.Y )
			ptMax.Y = carrier->m_vertices->items[i].v.Y;

		if (carrier->m_vertices->items[i].v.Z < ptMin.Z )
			ptMin.Z = carrier->m_vertices->items[i].v.Z;
		else if (carrier->m_vertices->items[i].v.Z > ptMax.Z )
			ptMax.Z = carrier->m_vertices->items[i].v.Z;
	}

	// expand root_node on eps value to fix numeric errors in case of node has zero thickness in some direction
	ptMin = ptMin - Vecc3(1e-3, 1e-3, 1e-3);
	ptMax = ptMax + Vecc3(1e-3, 1e-3, 1e-3);

	carrier->m_uiNodesCount++;
	carrier->m_uiLeavesCount++;
}


OctreeNode::OctreeNode(Octree* _carrier, Vec3 _ptMin, Vec3 _ptMax): carrier(_carrier), ptMin(_ptMin), ptMax(_ptMax)
{	ZeroMemory(nodes, sizeof(OctreeNode*)*8);
	bCollapsed = false;
	carrier->m_uiNodesCount++;
	bMarked = false;
	QueryPerformanceFrequency(&ticksPerSecond);
}

OctreeNode::~OctreeNode()
{
	//printf("Deleting nodes on level %d\n", iLevel);
	for (unsigned char i=0;i<8;i++)
	{	//printf("   Deleting node %d on level %d\n", i, level);
		if (nodes[i] != NULL)
		{	delete nodes[i];
			nodes[i] = NULL;
		}
	}
}


void OctreeNode::Draw(bool bDrawMarks)
{
	if (!(bDrawLeavesOnly && bCollapsed))
	{	//glColor3f(0.0,0.75,0.0);
		glColor3f(0.0,0.00,0.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f (ptMin.X, ptMin.Y, ptMin.Z);
			glVertex3f (ptMax.X, ptMin.Y, ptMin.Z);
			glVertex3f (ptMax.X, ptMax.Y, ptMin.Z);
			glVertex3f (ptMin.X, ptMax.Y, ptMin.Z);
			glVertex3f (ptMin.X, ptMin.Y, ptMin.Z);
			glVertex3f (ptMin.X, ptMin.Y, ptMax.Z);
			glVertex3f (ptMax.X, ptMin.Y, ptMax.Z);
			glVertex3f (ptMax.X, ptMin.Y, ptMin.Z);
			glVertex3f (ptMax.X, ptMin.Y, ptMax.Z);
			glVertex3f (ptMax.X, ptMax.Y, ptMax.Z);
			glVertex3f (ptMax.X, ptMax.Y, ptMin.Z);
			glVertex3f (ptMax.X, ptMax.Y, ptMax.Z);
			glVertex3f (ptMin.X, ptMax.Y, ptMax.Z);
			glVertex3f (ptMin.X, ptMax.Y, ptMin.Z);
			glVertex3f (ptMin.X, ptMax.Y, ptMax.Z);
			glVertex3f (ptMin.X, ptMin.Y, ptMax.Z);
		glEnd ();

		if ( bDrawMarks && bMarked )
		{	glPointSize(5.0);
			glColor3f(0.75,0.75,0.0);
			glBegin(GL_POINTS);
				glVertex3f((ptMin.X+ptMax.X)/2.0f, (ptMin.Y+ptMax.Y)/2.0f, (ptMin.Z+ptMax.Z)/2.0f);
			glEnd();
		}
	}

	// draw child nodes recursively
	for (unsigned char i=0; i<8; i++)
	{	//if octant is not empty descend down
		if (nodes[i] != NULL)
			nodes[i]->Draw(bDrawMarks);
	}

}

void OctreeNode::__DebugDrawAllTriangles()
{
	__DebugDrawPrivateTriangles();

	// draw child nodes recursively
	for (unsigned char i=0; i<8; i++)
	{	//if octant is not empty
		if (nodes[i] != NULL)
			nodes[i]->__DebugDrawAllTriangles();
	}

}

void OctreeNode::__DebugDrawPrivateTriangles()
{
TriVertex v0,v1,v2;

//	if (bMarked)
	{
		glBegin(GL_TRIANGLES);
		for (unsigned int ind=0; ind<privateIndices.m_uiBufferedItemsNumber; ind++)
		{
			carrier->GetgVerticesItem(privateIndices.items[ind].ind[A], &v0);
			carrier->GetgVerticesItem(privateIndices.items[ind].ind[B], &v1);
			carrier->GetgVerticesItem(privateIndices.items[ind].ind[C], &v2);

			glNormal3fv(&v0.n.X);	glVertex3fv(&v0.v.X);
			glNormal3fv(&v1.n.X);	glVertex3fv(&v1.v.X);
			glNormal3fv(&v2.n.X);	glVertex3fv(&v2.v.X);
		}
		glEnd();
	}
}


void OctreeNode::Collapse()
{
	// collapse current node
	if (!bCollapsed)
	{
		// do not perform collapse if number of private indices is too low
		// (NB: node remains marked as non-collapsed)
		if (privateIndices.m_uiBufferedItemsNumber < OctreeNode::nMinVerticesInNode)
			return;
		
		//code of the octant node
		unsigned char bit_index[3] = {0,0,0};
		Vec3 ptCenter = (ptMin + ptMax)/2.0;
		Vec3 BoxDim4 = (ptMax-ptMin)/4.0;
		Vec3 vDisp[3];

		Vec3 ptMinLocal = ptCenter - BoxDim4;
		Vec3 ptMaxLocal = ptCenter + BoxDim4;

		unsigned int i;
		unsigned int index;

		for (i=0;i<privateIndices.m_uiBufferedItemsNumber; i++)
		{
			vDisp[A] = BoxDim4; vDisp[B] = BoxDim4; vDisp[C] = BoxDim4;
			bit_index[A] = 0; bit_index[B] = 0; bit_index[C] = 0;
			//iterate and check every vertex from triangle on presence in specific octant
			for (unsigned char iVertex=A; iVertex<=C; iVertex++)
			{
				index = privateIndices.items[i].ind[iVertex];
				if (carrier->m_vertices->items[index].v.X <= ptCenter.X)
				{	bit_index[iVertex] |= 1;
					vDisp[iVertex].X *= -1.;
				}

				if (carrier->m_vertices->items[index].v.Y <= ptCenter.Y)
				{	bit_index[iVertex] |= 2;
					vDisp[iVertex].Y *= -1.;
				}

				if (carrier->m_vertices->items[index].v.Z <= ptCenter.Z)
				{	bit_index[iVertex] |= 4;
					vDisp[iVertex].Z *= -1.;
				}
			}
			//check whether all triangle vertices belong to one octant
			if ( (bit_index[A]==bit_index[B]) && (bit_index[B]==bit_index[C]) )
			{
				GetNode(bit_index[A], ptMinLocal+vDisp[A], ptMaxLocal+vDisp[A])->
					    privateIndices.AddItem(&privateIndices.items[i]);
			}
			else if ( (bit_index[A]==bit_index[B]) )
			{
				GetNode(bit_index[A], ptMinLocal+vDisp[A], ptMaxLocal+vDisp[A])->
					    privateIndices.AddItem(&privateIndices.items[i]);
				GetNode(bit_index[C],	ptMinLocal+vDisp[C], ptMaxLocal+vDisp[C])->
					    privateIndices.AddItem(&privateIndices.items[i]);
			}
			else if ( (bit_index[A]==bit_index[C]) )
			{
				GetNode(bit_index[A],	ptMinLocal+vDisp[A], ptMaxLocal+vDisp[A])->
					    privateIndices.AddItem(&privateIndices.items[i]);
				GetNode(bit_index[B],	ptMinLocal+vDisp[B], ptMaxLocal+vDisp[B])->
					    privateIndices.AddItem(&privateIndices.items[i]);
			}
			else if ( (bit_index[B]==bit_index[C]) )
			{
				GetNode(bit_index[A],	ptMinLocal+vDisp[A], ptMaxLocal+vDisp[A])->
					    privateIndices.AddItem(&privateIndices.items[i]);
				GetNode(bit_index[C],	ptMinLocal+vDisp[C], ptMaxLocal+vDisp[C])->
					    privateIndices.AddItem(&privateIndices.items[i]);
			}
			else //all three vertices belong to different octants
			{
				GetNode(bit_index[A], ptMinLocal+vDisp[A], ptMaxLocal+vDisp[A])->
					    privateIndices.AddItem(&privateIndices.items[i]);
				GetNode(bit_index[B], ptMinLocal+vDisp[B], ptMaxLocal+vDisp[B])->
					    privateIndices.AddItem(&privateIndices.items[i]);
				GetNode(bit_index[C], ptMinLocal+vDisp[C], ptMaxLocal+vDisp[C])->
					    privateIndices.AddItem(&privateIndices.items[i]);
			}
		}
		privateIndices.Clear();
		bCollapsed = true;
		carrier->m_uiLeavesCount--;
	}
	else
	{
		// we are collapsed already propagate collapse to child nodes
		for (unsigned char i=0; i<8; i++)
		{	//if octant is not empty collapse it
			if (nodes[i] != NULL)
				nodes[i]->Collapse();
		}
	}
}

void OctreeNode::RayIntersect(std::list<OctreeNode*>* lHit, Vec3 ray_orig, Vec3 ray_dir)
{
float tIn, tOut;
	if ( intersectAABoxRay(ptMin, ptMax, ray_orig, ray_dir, tIn, tOut) )
	{	
		if (!bCollapsed)	//i.e. leave
		{
			lHit->push_back(this);
			bMarked=true;				//mark ourselves
		}
		else
		{	// propagate function call to child nodes
			for (unsigned char i=0; i<8; i++)
			{	if (nodes[i] != NULL)
				nodes[i]->RayIntersect(lHit, ray_orig, ray_dir);
			}
		}
	}
}

OctreeNode* OctreeNode::GetNode(unsigned char ind, Vec3 ptMin, Vec3 ptMax)
{	ASSERT(ind <= 8);
	if (nodes[ind] == NULL)
	{
		nodes[ind] = new OctreeNode(carrier, ptMin, ptMax);
		nodes[ind]->iLevel = iLevel+1;
		nodes[ind]->bDrawLeavesOnly = bDrawLeavesOnly;
		carrier->m_uiLeavesCount++;
	}
return nodes[ind];
}

void OctreeNode::ClearMarkedNodes()
{
	bMarked = false;
	// propagate function call to child nodes
	for (unsigned char i=0; i<8; i++)
	{	if (nodes[i] != NULL)
		nodes[i]->bMarked = false;
	}
}

unsigned int OctreeNode::GetPrivateIndicesBufferedItemsNumber()
{	return privateIndices.m_uiBufferedItemsNumber;
}

void OctreeNode::GetPrivateIndicesItem(unsigned int ind, TriVertexIndex* tri_vertex_ind )
{	ASSERT(ind < privateIndices.m_uiBufferedItemsNumber);
	*tri_vertex_ind = privateIndices.items[ind];
}

// Returns: true- in case of collision, false- otherwise
bool ClosestTriangleOnRay(Octree* octree, Vec3 ray_orig, Vec3 ray_dir, Triangle& triangleOut, Vec3& ptIntersect, Vec3& vNormalIntersect)
{
float fDistClosest = 10e9;
float fDistCurrent;
std::list<OctreeNode*> lHitNodes;
std::list<OctreeNode*>::iterator iNode;
TriVertexIndex current_triangleInd;
TriVertex v0, v1, v2;
float u_cur,v_cur,t,u,v;
OctreeNode* current_node;
unsigned int uiPrivateIndicesBufferedItemsNumber;
TriVertexIndex HitTriangleInd;
Vec3 ptIntersectCurrent;
bool bResult;

#ifdef STATISTICS_ENABLED
	octree->gTrianColReqCount++;
	QueryPerformanceCounter(&_dCurTime);
#endif

	octree->root_node->RayIntersect(&lHitNodes, ray_orig, ray_dir);

#ifdef STATISTICS_ENABLED
	QueryPerformanceCounter(&_dCurTime2);
	fBoxCollisionTimeCheck  +=  (float(_dCurTime2.QuadPart) - float(_dCurTime.QuadPart))/float(ticksPerSecond.QuadPart);

	gBoxCountProcessed    = gBoxCountProcessed + lHitNodes.size();
	QueryPerformanceCounter(&_dCurTime);
#endif

	if (lHitNodes.size() > 0)
	{	
		for ( iNode = lHitNodes.begin(); iNode != lHitNodes.end(); iNode++)
		{	
			current_node = *iNode;
			uiPrivateIndicesBufferedItemsNumber = current_node->GetPrivateIndicesBufferedItemsNumber();
			for (unsigned int ind=0; ind < uiPrivateIndicesBufferedItemsNumber; ind++)
			{
				current_node->GetPrivateIndicesItem(ind, &current_triangleInd);
				octree->GetgVerticesItem(current_triangleInd.ind[A], &v0);
				octree->GetgVerticesItem(current_triangleInd.ind[B], &v1);
				octree->GetgVerticesItem(current_triangleInd.ind[C], &v2);

				#ifdef STATISTICS_ENABLED
					gTrianCountChecked++;
				#endif

				if (!intersectRayTriangle( v0.v, v1.v, v2.v, ray_orig, ray_dir, u_cur, v_cur, t ))
					continue;
				
				ptIntersectCurrent = ray_dir * t + ray_orig;
				fDistCurrent= VecLengthSqr(ptIntersectCurrent - ray_orig);
				if (fDistCurrent < 0.0001*0.0001)
					//we hit ourselves
					continue;
				if (fDistCurrent < fDistClosest)
				{	fDistClosest = fDistCurrent;
					HitTriangleInd = current_triangleInd;
					ptIntersect = ptIntersectCurrent;
					u = u_cur; v = v_cur;
				}
			}
		}
	}
	if (fDistClosest < 10e9 - 1e-9)
	{
		// convert structure of indices into Triangle structure (just making use of GetgVerticesItem)
		octree->GetgVerticesItem(HitTriangleInd.ind[A], &triangleOut.v0);
		octree->GetgVerticesItem(HitTriangleInd.ind[B], &triangleOut.v1);
		octree->GetgVerticesItem(HitTriangleInd.ind[C], &triangleOut.v2);

		vNormalIntersect = TriangleLinearInterpolate(triangleOut.v0.n,triangleOut.v1.n,triangleOut.v2.n, u, v);
		VecNormalizeP(vNormalIntersect);

		bResult = true;
	}
	else
		bResult = false;

	#ifdef STATISTICS_ENABLED
		QueryPerformanceCounter(&_dCurTime2);
		fTrianCollisionTimeCheck += (float(_dCurTime2.QuadPart) - float(_dCurTime.QuadPart))/float(ticksPerSecond.QuadPart);
	#endif
	return bResult;
}


void TestOctree(Octree* octree, Vec3 ptOrigin)
{
Triangle triangle;
float angle = 0.0;
Vec3 ptIsect;
Vec3 vNormal;
DWORD ff;

	ff = timeGetTime();
	printf("Testing octree working speed on 10000 requests...");

	while(angle < 100.)
	{
		ClosestTriangleOnRay(octree,ptOrigin, Vecc3(sin(angle),cos(angle),0.), triangle, ptIsect, vNormal);
		angle+=0.01f;
	}

	printf("done in %lu ms\n", timeGetTime() - ff);
}



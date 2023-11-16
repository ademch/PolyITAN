
#include "stdafx.h"
#include "SimpleModel.h"
#include "../!!adExtensions/extensions.h"
#include <Mmsystem.h>
#include <gl/glut.h>


SimpleModel::SimpleModel()
{
	m_VertexBufferObjectID  = -1;
	m_ElementBufferObjectID = -1;
	octree = NULL;
}

SimpleModel::~SimpleModel()
{
	DestroyOctree();
}

Octree* SimpleModel::GetOctree()
{
DWORD ff;

	if (octree == NULL)
	{
		ff = timeGetTime();
		printf("Constructing octree...");
			octree = new Octree(&vertices, &indices);
			octree->root_node->bDrawLeavesOnly = false;
			octree->Collapse(5);	//use 5- other values are experimental
		printf("done in %lu ms\n", timeGetTime() - ff);
		printf("\tReport: nodes count: %d\n", octree->m_uiNodesCount);
		printf("\t        leaves count: %d\n", octree->m_uiLeavesCount);
	}
	return octree;
}

void SimpleModel::DestroyOctree()
{
	delete octree;
	octree = NULL;
}

void SimpleModel::UploadDataToGraphicsCard()
{
int nParam_ArrayObjectSize = 0;
int nArrayObjectSize;

	nArrayObjectSize = sizeof(TriVertex) * vertices.m_uiBufferedItemsNumber;

	glGenBuffersARB( 1, &m_VertexBufferObjectID );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_VertexBufferObjectID );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, nArrayObjectSize, &vertices.items[0], GL_STATIC_DRAW_ARB );

	glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

	printf("VBO: allocated %d Kbyte in graphics card for vertex buffer\n", nParam_ArrayObjectSize >> 10);
	if( nParam_ArrayObjectSize <= 0 )
	{	printf("glBufferDataARB failed to allocate memory!");
	}

	nArrayObjectSize = sizeof(TriVertexIndex) * indices.m_uiBufferedItemsNumber;
	nParam_ArrayObjectSize = 0;

	glGenBuffersARB( 1, &m_ElementBufferObjectID );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_ElementBufferObjectID );
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nArrayObjectSize, &indices.items[0], GL_STATIC_DRAW_ARB );

	glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

	printf("VBO: allocated %d Kbyte in graphics card for index buffer\n", nParam_ArrayObjectSize >> 10);
	if( nParam_ArrayObjectSize <= 0 )
	{	printf("glBufferDataARB failed to allocate memory!");
	}

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
}

void SimpleModel::Render()
{
	if (vertices.m_uiBufferedItemsNumber == 0) return;
	
	ASSERT(m_VertexBufferObjectID != -1);
	ASSERT(m_ElementBufferObjectID != -1);

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_VertexBufferObjectID );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_ElementBufferObjectID );

	//glVertexPointer (3, GL_FLOAT, sizeof(TriVertex), &VertexManager.items[0].v);
	glVertexPointer (3, GL_FLOAT, sizeof(TriVertex), GRAPHICS_BUFFER_OFFSET(sizeof(Vec3)));
	glNormalPointer (GL_FLOAT, sizeof(TriVertex), GRAPHICS_BUFFER_OFFSET(0));

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );

	//glDrawElements(GL_TRIANGLES, IndexManager.m_uiBufferedItemsNumber*3, GL_UNSIGNED_INT, &IndexManager.items[0]);
	glDrawElements(GL_TRIANGLES, indices.m_uiBufferedItemsNumber*3, GL_UNSIGNED_INT, GRAPHICS_BUFFER_OFFSET(0));

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
}


void SimpleModel::DrawVertexNormals()
{
Vec3 vNormal;
const float NORMAL_LENGTH = 5.0;

	glColor3f(0,1,1);
	glBegin(GL_LINES);
	for (unsigned int i=0; i<vertices.m_uiBufferedItemsNumber; i++)
	{
		vNormal = vertices.items[i].v + vertices.items[i].n*NORMAL_LENGTH; 

		glVertex3fv(&vertices.items[i].v.X);
		glVertex3fv(&vNormal.X);
	}
	glEnd();
}

void SimpleModel::SaveToFile(char* strFileName)
{
FILE* file;
	
	fopen_s(&file, strFileName, "w+");
	fprintf_s(file, "Simple model v1.0\n");
	fprintf_s(file, "Vertices: [count: %u]\n", vertices.m_uiBufferedItemsNumber);
	for (unsigned int i=0; i<vertices.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%f %f %f\n", vertices.items[i].v.X, vertices.items[i].v.Y, vertices.items[i].v.Z);

	fprintf_s(file, "\n");

	fprintf_s(file, "Indices: [count: %u]\n", indices.m_uiBufferedItemsNumber);
	for (unsigned int i=0; i<indices.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%u %u %u\n", indices.items[i].ind[0], indices.items[i].ind[1], indices.items[i].ind[2]);

	fprintf_s(file, "\n");

	fprintf_s(file, "Normals: [count: %u]\n", vertices.m_uiBufferedItemsNumber);
	for (unsigned int i=0; i<vertices.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%f %f %f\n", vertices.items[i].n.X, vertices.items[i].n.Y, vertices.items[i].n.Z);

	fprintf_s(file, "\n");

	fclose(file);
}

void SimpleModel::LoadFromFile(const char* strFileName)
{
FILE* file;
char str[80];
unsigned int uiItemsNumber;

	Clear();

	fopen_s(&file, strFileName, "r");
	if (file == NULL) return;

	fgets(str, 80, file);
	if (_stricmp(str, "Simple model v1.0\n"))
		return;

	fscanf_s(file, "Vertices: [count: %u]\n", &uiItemsNumber);
	vertices.AllocateMemory(uiItemsNumber);
	for (unsigned int i=0; i<uiItemsNumber; i++)
	{
		fscanf_s(file, "%f %f %f\n", &vertices.items[i].v.X, &vertices.items[i].v.Y, &vertices.items[i].v.Z);
		vertices.m_uiBufferedItemsNumber++;
	}

	fscanf_s(file, "\n");

	fscanf_s(file, "Indices: [count: %u]\n", &uiItemsNumber);
	indices.AllocateMemory(uiItemsNumber);
	for (unsigned int i=0; i<uiItemsNumber; i++)
	{
		fscanf_s(file, "%u %u %u\n", &indices.items[i].ind[0], &indices.items[i].ind[1], &indices.items[i].ind[2]);
		indices.m_uiBufferedItemsNumber++;
	}

	fscanf_s(file, "\n");

	fscanf_s(file, "Normals: [count: %u]\n", &uiItemsNumber);
	for (unsigned int i=0; i<uiItemsNumber; i++)
		fscanf_s(file, "%f %f %f\n", &vertices.items[i].n.X, &vertices.items[i].n.Y, &vertices.items[i].n.Z);

	fclose(file);
}

void SimpleModel::Clear()
{
	DestroyOctree();

	vertices.Clear();
	indices.Clear();

	if (m_ElementBufferObjectID != -1)
	{	glDeleteBuffersARB(1, &m_ElementBufferObjectID);
		m_ElementBufferObjectID = -1;
	}
	if (m_VertexBufferObjectID != -1)
	{
		glDeleteBuffersARB(1, &m_VertexBufferObjectID);
		m_VertexBufferObjectID = -1;
	}
}

void SimpleModel::Transform(Matr4* mTransf)
{
	for (unsigned int i=0; i<vertices.m_uiBufferedItemsNumber; i++)
		vertices.items[i].v = (*mTransf)^vertices.items[i].v;
}



SimplePointsModel::SimplePointsModel()
{
	m_VertexBufferObjectID  = -1;
	m_ElementBufferObjectID = -1;
}


void SimplePointsModel::UploadDataToGraphicsCard()
{
int nParam_ArrayObjectSize = 0;
int nArrayObjectSize;

	nArrayObjectSize = sizeof(PointVertex) * vertices.m_uiBufferedItemsNumber;

	glGenBuffersARB( 1, &m_VertexBufferObjectID );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_VertexBufferObjectID );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, nArrayObjectSize, &vertices.items[0], GL_STATIC_DRAW_ARB );

	glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

	printf("VBO: Allocated %d Kbyte in graphics card for vertex buffer\n", nParam_ArrayObjectSize >> 10);
	if( nParam_ArrayObjectSize <= 0 )
	{	printf("glBufferDataARB failed to allocate memory!");
	}

	//nArrayObjectSize = sizeof(PointVertexIndex) * IndexManager.m_uiBufferedItemsNumber;
	//nParam_ArrayObjectSize = 0;

	//glGenBuffersARB( 1, &m_ElementBufferObjectID );
	//glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_ElementBufferObjectID );
	//	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nArrayObjectSize, &indices.items[0], GL_STATIC_DRAW_ARB );

	//	glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

	//	printf("Allocated %d Kbyte in graphics card for index buffer\n", nParam_ArrayObjectSize >> 10);
	//	if( nParam_ArrayObjectSize <= 0 )
	//	{	MessageBox(NULL,"glBufferDataARB failed to allocate memory!", "ERROR", MB_OK|MB_ICONEXCLAMATION);
	//	}

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
	//	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
}

void SimplePointsModel::Render()
{
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_VertexBufferObjectID );

	glVertexPointer (3, GL_FLOAT, sizeof(PointVertex), GRAPHICS_BUFFER_OFFSET(sizeof(VecUB4)));
	glColorPointer (4, GL_UNSIGNED_BYTE, sizeof(PointVertex), GRAPHICS_BUFFER_OFFSET(0));

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

	glDrawElements(GL_POINTS, indices.m_uiBufferedItemsNumber, GL_UNSIGNED_INT, &indices.items[0]);

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}


// SimpleLinesModel


SimpleLinesModel::SimpleLinesModel()
{
	m_VertexBufferObjectID  = -1;
	m_ElementBufferObjectID = -1;
}


void SimpleLinesModel::UploadDataToGraphicsCard()
{
int nParam_ArrayObjectSize = 0;
int nArrayObjectSize;

	nArrayObjectSize = sizeof(PointVertex) * vertices.m_uiBufferedItemsNumber;

	glGenBuffersARB( 1, &m_VertexBufferObjectID );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_VertexBufferObjectID );
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, nArrayObjectSize, &vertices.items[0], GL_STATIC_DRAW_ARB );

	glGetBufferParameterivARB( GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

	printf("VBO: Allocated %d Kbyte in graphics card for vertex buffer\n", nParam_ArrayObjectSize >> 10);
	if( nParam_ArrayObjectSize <= 0 )
	{	printf("glBufferDataARB failed to allocate memory!");
	}

	nArrayObjectSize = sizeof(PointVertexIndex) * indices.m_uiBufferedItemsNumber;
	nParam_ArrayObjectSize = 0;

	glGenBuffersARB( 1, &m_ElementBufferObjectID );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_ElementBufferObjectID );
	glBufferDataARB( GL_ELEMENT_ARRAY_BUFFER_ARB, nArrayObjectSize, &indices.items[0], GL_STATIC_DRAW_ARB );

	glGetBufferParameterivARB( GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &nParam_ArrayObjectSize );

	printf("Allocated %d Kbyte in graphics card for index buffer\n", nParam_ArrayObjectSize >> 10);
	if( nParam_ArrayObjectSize <= 0 )
	{	printf("glBufferDataARB failed to allocate memory!");
	}

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, NULL );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, NULL );
}

void SimpleLinesModel::Render()
{
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, m_VertexBufferObjectID );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, m_ElementBufferObjectID );

	glVertexPointer (3, GL_FLOAT, sizeof(PointVertex), GRAPHICS_BUFFER_OFFSET(sizeof(VecUB4)));
	glColorPointer (4, GL_UNSIGNED_BYTE, sizeof(PointVertex), GRAPHICS_BUFFER_OFFSET(0));

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );

//	glDrawElements(GL_LINES, indices.m_uiBufferedItemsNumber, GL_UNSIGNED_INT, &indices.items[0]);
	glDrawElements(GL_LINES, indices.m_uiBufferedItemsNumber, GL_UNSIGNED_INT, GRAPHICS_BUFFER_OFFSET(0));

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
	glBindBufferARB( GL_ELEMENT_ARRAY_BUFFER_ARB, 0 );
}

// SimplePointCloudModel

void SimplePointCloudModel::Render()
{
unsigned int i;

	glVertexPointer (3, GL_FLOAT, sizeof(PointCloudVertex), &vertices.items[0].v);

	glEnableClientState( GL_VERTEX_ARRAY );

	glDrawArrays(GL_POINTS, 0, vertices.m_uiBufferedItemsNumber);

	glDisableClientState( GL_VERTEX_ARRAY );

	glColor3f(1,1,0);
	glLineWidth(1.0);
	for (i=0; i<vertices.m_uiBufferedItemsNumber; i++)
	{	glBegin(GL_LINES);
			glVertex3fv( &vertices.items[i].v.X );
			glVertex3fv( &(vertices.items[i].v + vertices.items[i].n*10.0).X );
		glEnd();
	}

}

int SimplePointCloudModel::SaveToFile(const char* strFileName)
{
FILE* file;

	fopen_s(&file, strFileName, "w+");
	if (file == NULL) return 1;

	fprintf_s(file, "Point cloud model v1.0\n");
	fprintf_s(file, "Vertices: [count: %u]\n", vertices.m_uiBufferedItemsNumber);
	for (unsigned int i=0; i<vertices.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%f %f %f\n", vertices.items[i].v.X, vertices.items[i].v.Y, vertices.items[i].v.Z);

	fprintf_s(file, "\n");

	fprintf_s(file, "Normals: [count: %u]\n", vertices.m_uiBufferedItemsNumber);
	for (unsigned int i=0; i<vertices.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%f %f %f\n", vertices.items[i].n.X, vertices.items[i].n.Y, vertices.items[i].n.Z);

	fprintf_s(file, "\n");

	fprintf_s(file, "fMisc: [count: %u]\n", vertices.m_uiBufferedItemsNumber);
	for (unsigned int i=0; i<vertices.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%f\n", vertices.items[i].fMisc);

	fprintf_s(file, "\n");

	fclose(file);

	return 0;
}


void SimplePointCloudModel::Clear()
{
	vertices.Clear();
}


void SimplePointCloudModel::Transform(Matr4* mTransf)
{
	for (unsigned int i=0; i<vertices.m_uiBufferedItemsNumber; i++)
		vertices.items[i].v = (*mTransf)^vertices.items[i].v;
}


// SimplePointPairsModel

void SimplePointPairsModel::Render(bool bRenderPair1)
{
unsigned int i;
ItemsManager<PointVertexExt>* vertices;

	if (bRenderPair1)
		vertices = &verticesPair1;
	else
		vertices = &verticesPair2;

	// make sure number of points is even
	ASSERT(vertices->m_uiBufferedItemsNumber % 2 == 0);

	glPointSize(13);

	glEnable(GL_LIGHTING);	// it is unknown how to calculate correct normal at each point

	glColorPointer (3, GL_UNSIGNED_BYTE, sizeof(PointVertexExt), &vertices->items[0].c);	// can be 4
	glNormalPointer (GL_FLOAT, sizeof(PointVertexExt), &vertices->items[0].n);
	glVertexPointer (3, GL_FLOAT, sizeof(PointVertexExt), &vertices->items[0].v);

	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	glEnableClientState( GL_VERTEX_ARRAY );

	glDrawArrays(GL_POINTS, 0, vertices->m_uiBufferedItemsNumber);

	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor4f(1,1,0, 0.3);
	glLineWidth(1.0);
	for (i=0; i<vertices->m_uiBufferedItemsNumber; i+=2)
	{	
		glBegin(GL_LINES);
			glVertex3fv( &vertices->items[i].v.X );
			glVertex3fv( &vertices->items[i+1].v.X );
		glEnd();
	}
}

int SimplePointPairsModel::SaveToFile(const char* strFileName)
{
FILE* file;

	fopen_s(&file, strFileName, "w+");
	if (file == NULL) return 1;

	fprintf_s(file, "Point pairs model v1.0\n\n");
	
	// Pair 1
	fprintf_s(file, "Vertices: [count: %u]\n", verticesPair1.m_uiBufferedItemsNumber);
	for (unsigned int i=0; i<verticesPair1.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%f %f %f\n", verticesPair1.items[i].v.X, verticesPair1.items[i].v.Y, verticesPair1.items[i].v.Z);

	fprintf_s(file, "\n");

	fprintf_s(file, "Normals:\n");
	for (unsigned int i=0; i<verticesPair1.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%f %f %f\n", verticesPair1.items[i].n.X, verticesPair1.items[i].n.Y, verticesPair1.items[i].n.Z);

	fprintf_s(file, "\n");

	fprintf_s(file, "Colors:\n");
	for (unsigned int i=0; i<verticesPair1.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%u %u %u %u\n", verticesPair1.items[i].c.R, verticesPair1.items[i].c.G, verticesPair1.items[i].c.B, verticesPair1.items[i].c.A);

	fprintf_s(file, "\n");

	// Pair 2
	fprintf_s(file, "Vertices: [count: %u]\n", verticesPair2.m_uiBufferedItemsNumber);
	for (unsigned int i=0; i<verticesPair2.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%f %f %f\n", verticesPair2.items[i].v.X, verticesPair2.items[i].v.Y, verticesPair2.items[i].v.Z);

	fprintf_s(file, "\n");

	fprintf_s(file, "Normals:\n");
	for (unsigned int i=0; i<verticesPair2.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%f %f %f\n", verticesPair2.items[i].n.X, verticesPair2.items[i].n.Y, verticesPair2.items[i].n.Z);

	fprintf_s(file, "\n");

	fprintf_s(file, "Colors:\n");
	for (unsigned int i=0; i<verticesPair2.m_uiBufferedItemsNumber; i++)
		fprintf_s(file, "%u %u %u %u\n", verticesPair2.items[i].c.R, verticesPair2.items[i].c.G, verticesPair2.items[i].c.B, verticesPair1.items[i].c.A);

	fprintf_s(file, "\n");


	fclose(file);

return 0;
}


int SimplePointPairsModel::LoadFromFile(const char* strFileName)
{
FILE* file;
char str[80];
unsigned int uiItemsNumber;

	Clear();

	fopen_s(&file, strFileName, "r");
	if (file == NULL) return 1;

	fgets(str, 80, file);
	if (_stricmp(str, "Point pairs model v1.0\n"))
		return 1;

	fscanf_s(file, "\n");

	// Pair 1
	fscanf_s(file, "Vertices: [count: %u]\n", &uiItemsNumber);
	verticesPair1.AllocateMemory(uiItemsNumber);
	for (unsigned int i=0; i<uiItemsNumber; i++)
	{
		fscanf_s(file, "%f %f %f\n", &verticesPair1.items[i].v.X, &verticesPair1.items[i].v.Y, &verticesPair1.items[i].v.Z);
		verticesPair1.m_uiBufferedItemsNumber++;
	}

	fscanf_s(file, "\n");

	fscanf_s(file, "Normals:\n");
	for (unsigned int i=0; i<uiItemsNumber; i++)
		fscanf_s(file, "%f %f %f\n", &verticesPair1.items[i].n.X, &verticesPair1.items[i].n.Y, &verticesPair1.items[i].n.Z);

	fscanf_s(file, "\n");

	fscanf_s(file, "Colors:\n");
	for (unsigned int i=0; i<uiItemsNumber; i++)
		fscanf_s(file, "%u %u %u %u\n", &verticesPair1.items[i].c.R, &verticesPair1.items[i].c.G, &verticesPair1.items[i].c.B, &verticesPair1.items[i].c.A);

	fscanf_s(file, "\n");

	// Pair 2
	fscanf_s(file, "Vertices: [count: %u]\n", &uiItemsNumber);
	verticesPair2.AllocateMemory(uiItemsNumber);
	for (unsigned int i=0; i<uiItemsNumber; i++)
	{
		fscanf_s(file, "%f %f %f\n", &verticesPair2.items[i].v.X, &verticesPair2.items[i].v.Y, &verticesPair2.items[i].v.Z);
		verticesPair2.m_uiBufferedItemsNumber++;
	}

	fscanf_s(file, "\n");

	fscanf_s(file, "Normals:\n");
	for (unsigned int i=0; i<uiItemsNumber; i++)
		fscanf_s(file, "%f %f %f\n", &verticesPair2.items[i].n.X, &verticesPair2.items[i].n.Y, &verticesPair2.items[i].n.Z);

	fscanf_s(file, "\n");

	fscanf_s(file, "Colors:\n");
	for (unsigned int i=0; i<uiItemsNumber; i++)
		fscanf_s(file, "%u %u %u %u\n", &verticesPair2.items[i].c.R, &verticesPair2.items[i].c.G, &verticesPair2.items[i].c.B, &verticesPair1.items[i].c.A);

	fscanf_s(file, "\n");


	fclose(file);

	return 0;
}



void SimplePointPairsModel::Clear()
{
	verticesPair1.Clear();
	verticesPair2.Clear();
}


void SimplePointPairsModel::Transform(Matr4* mTransf)
{
	for (unsigned int i=0; i<verticesPair1.m_uiBufferedItemsNumber; i++)
		verticesPair1.items[i].v = (*mTransf)^verticesPair1.items[i].v;
	for (unsigned int i=0; i<verticesPair2.m_uiBufferedItemsNumber; i++)
		verticesPair2.items[i].v = (*mTransf)^verticesPair2.items[i].v;
}

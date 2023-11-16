#ifndef SIMPLEMODEL_H
#define SIMPLEMODEL_H


#include "vector_math.h"
#include "ItemsManager.h"
#include "octree.h"

class SimpleModel
{	
public:
	SimpleModel();
	~SimpleModel();

	ItemsManager<TriVertexIndex> indices;
	ItemsManager<TriVertex> vertices;

	//VBO handles
	unsigned int m_VertexBufferObjectID;
	unsigned int m_ElementBufferObjectID;

	void UploadDataToGraphicsCard();

	void Render();
	void DrawVertexNormals();
	
	Octree* GetOctree();
	void DestroyOctree();

	void SaveToFile(char* strFileName);
	void LoadFromFile(const char* strFileName);

	void Clear();

	void Transform(Matr4* mTransf);

private:
	Octree* octree;
};


class SimplePointsModel
{	
public:
	SimplePointsModel();
	~SimplePointsModel() {}

	ItemsManager<PointVertexIndex> indices;
	ItemsManager<PointVertex> vertices;

	//VBO handles
	unsigned int m_VertexBufferObjectID;
	unsigned int m_ElementBufferObjectID;

	void UploadDataToGraphicsCard();
	void Render();

private:
};

class SimpleLinesModel
{	
public:
	SimpleLinesModel();
	~SimpleLinesModel() {}

	ItemsManager<PointVertexIndex> indices;
	ItemsManager<PointVertex> vertices;

	//VBO handles
	unsigned int m_VertexBufferObjectID;
	unsigned int m_ElementBufferObjectID;

	void UploadDataToGraphicsCard();
	void Render();

private:
};

class SimplePointCloudModel
{	
public:
	SimplePointCloudModel() {}
	~SimplePointCloudModel() {}

	ItemsManager<PointCloudVertex> vertices;

	void Render();

	void Clear();
	void Transform(Matr4* mTransf);

	int SaveToFile(const char* strFileName);

private:
};


class SimplePointPairsModel
{	
public:
	SimplePointPairsModel() {}
	~SimplePointPairsModel() {}

	ItemsManager<PointVertexExt> verticesPair1;
	ItemsManager<PointVertexExt> verticesPair2;

	void Render(bool bRenderPair1);

	void Clear();
	void Transform(Matr4* mTransf);

	int SaveToFile(const char* strFileName);
	int LoadFromFile(const char* strFileName);

private:
};

#endif /* SIMPLEMODEL_H */
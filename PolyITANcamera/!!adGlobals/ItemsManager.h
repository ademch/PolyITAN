#ifndef ITEMS_MANAGER_H
#define ITEMS_MANAGER_H

template<class T>
class ItemsManager
{
private:
	//number of allocated slots of type T
	unsigned int m_uiAllocatedItemsNumber;

public:
	//number of items added with AddItem 
	unsigned int m_uiBufferedItemsNumber;
	T* items;

	void AddItem(T* input_item)
	{
		//alloc/re-allocated memory for vertices
		ASSERT(m_uiAllocatedItemsNumber < unsigned int (-1) - 128*2);
		if (m_uiBufferedItemsNumber >= m_uiAllocatedItemsNumber)
		{	items = (T*) realloc(items, sizeof(T)*(m_uiAllocatedItemsNumber + 128));
			m_uiAllocatedItemsNumber += 128;
		}
		memcpy(&items[m_uiBufferedItemsNumber],input_item, sizeof(T));
		m_uiBufferedItemsNumber++;
	}

	void AllocateMemory(unsigned int uiUnitsNumber)
	{
		ASSERT(uiUnitsNumber < unsigned int (-1) - 128*2);
		items = (T*) realloc(items, sizeof(T)*uiUnitsNumber);
		m_uiAllocatedItemsNumber = uiUnitsNumber;
	}

	ItemsManager()
	{   m_uiBufferedItemsNumber = 0;
		m_uiAllocatedItemsNumber = 0;
		items = NULL;
	}
	
	~ItemsManager()
	{		Clear();	}

	void Clear()
	{	free(items);
		items = NULL;

		m_uiBufferedItemsNumber  = 0;
		m_uiAllocatedItemsNumber = 0;
	}

	T* Last()
	{
		if (m_uiBufferedItemsNumber == 0) return 0;

		return &items[m_uiBufferedItemsNumber-1];
	}

	ItemsManager& operator=(const ItemsManager& rhs)
	{
		// prevent to copy object to itself
		if (this == &rhs) return *this;

		m_uiBufferedItemsNumber = rhs.m_uiBufferedItemsNumber;
		m_uiAllocatedItemsNumber = rhs.m_uiAllocatedItemsNumber;

		items = (T*) realloc(items, sizeof(T)*m_uiAllocatedItemsNumber);

		memcpy(items,rhs.items, sizeof(T)*m_uiAllocatedItemsNumber);

		return *this;
	}
};

#endif
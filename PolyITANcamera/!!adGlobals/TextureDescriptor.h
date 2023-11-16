
#ifndef TEXTURE_DESCRIPTOR_H
#define TEXTURE_DESCRIPTOR_H

#include <map>
#include <string>

class TextureDescriptor
{
public:
	TextureDescriptor(unsigned int uiTexID, int w, int h)
	{
		m_uiTextureID = uiTexID;
		m_width  = w;
		m_height = h;
	}
	void SetDimensions(int iW, int iH) {
		m_width  = iW;
		m_height = iH;
	}

	unsigned int m_uiTextureID;
	int m_width;
	int m_height;
};

typedef std::map<std::string, TextureDescriptor*>::iterator typeTexBankIter;

class TextureBank
{
public:
	~TextureBank()
	{
		std::map<std::string, TextureDescriptor*>::iterator iter;
		for (iter = bank.begin(); iter != bank.end(); ++iter)
		{
			delete iter->second;
		}

		bank.clear();
	}

	TextureDescriptor* operator[](std::string name)
	{
		std::map<std::string, TextureDescriptor*>::iterator iter;

		iter = bank.find(name);
		if (iter == bank.end())	return NULL;

		return iter->second;
	}


	std::map<std::string, TextureDescriptor*> bank;

protected:


};

#endif

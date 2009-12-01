#pragma once

class InfluenceMap;
class CObjectMesh;
class Controller{

public:
	enum category_t {BAD_GUY, GOOD_GUY};

	unsigned int getInfluenceRadius(){
			return this->m_iInfluenceRadius;
	}
	category_t getCategory(){
			return this->m_Category;
	}
Controller(CObjectMesh* owner);
static InfluenceMap* s_InfluenceMap;
private:
	category_t m_Category;
	unsigned int m_iInfluenceRadius; //Chevychev Distance


};
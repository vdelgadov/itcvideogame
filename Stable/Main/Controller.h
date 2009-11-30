#pragma once
class Controller{

public:
	enum category_t {BAD_GUY, GOOD_GUY};



	unsigned int getInfluenceRadius(){
			return this->m_iInfluenceRadius;
	}
	category_t getCategory(){
			return this->m_Category;
	}

private:
	category_t m_Category=GOOD_GUY;
	unsigned int m_iInfluenceRadius=1; //Chevychev Distance


};
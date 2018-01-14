#include "GameSituation.h"

zero::GameSituation::GameSituation()
{
	this->init();
}

zero::GameSituation::~GameSituation()
{
}

void zero::GameSituation::init()
{
	nDiZhuID = -1;
	nLandScore = 0;
	nNowDiZhuID = -1;
	nNowLandScore = 0;

	this->DiPai.setDimension(3);
	this->DiPai.InitValue(0);
	this->value_aAllOutCardList.setDimension(18);
	this->value_aAllOutCardList.InitValue(0);
	this->value_aUnitOutCardList.setDimension(3, 18);
	this->value_aUnitOutCardList.InitValue(0);
	this->value_aUnitHandCardCount.setDimension(3);
	this->value_aUnitHandCardCount.InitValue(0);

	//本局当前底分倍数  
	nMultiple = 0;
	//当前控手对象（用于区分是否可以自身任意出牌以及是否地主已经放弃出牌从而不去管队友）  
	nCardDroit = 0;

	//本局游戏是否结束  
	Over = false;
}

#ifndef __GAME_SITUATION_H__
#define __GAME_SITUATION_H__

#include "DataStruct.h"
#include "Array.hpp"

//最多手牌  
#define HandCardMaxLen 20  
//价值最小值  
#define MinCardsValue -25  
//价值最大值  
#define MaxCardsValue 106  

namespace zero {

	//游戏全局类  
	class GameSituation
	{
	public:

		//构造函数  
		GameSituation();
		
		//析构函数  
		virtual ~GameSituation();


	public:
		//初始化
		void init();

		//地主玩家  
		int nDiZhuID ;
		//本局叫分  
		int nLandScore;

		//当前地主玩家——还未确定  
		int nNowDiZhuID;
		//当前本局叫分——还未确定  
		int nNowLandScore;

 
		//三张底牌 int DiPai[3]; 地主比农民多三张牌
		zero::Array<int> DiPai;

		//已经打出的牌——状态记录，便于一些计算，值域为该index牌对应的数量0~4   int value_aAllOutCardList[18] = { 0 };
		zero::Array<int> value_aAllOutCardList;
 
		//三名玩家已经打出的手牌记录  int value_aUnitOutCardList[3][18] = { 0 };
		zero::MostArray<int> value_aUnitOutCardList;

		//三名玩家已经剩余手牌个数  int value_aUnitHandCardCount[3] = { 0 };
		zero::Array<int> value_aUnitHandCardCount;


		//本局当前底分倍数  
		int nMultiple = 0;
		//当前控手对象（用于区分是否可以自身任意出牌以及是否地主已经放弃出牌从而不去管队友）  
		int nCardDroit = 0;
		//当前打出牌的类型数据，被动出牌时玩家根据这里做出筛选  
		CardGroupData uctNowCardGroup;

		//本局游戏是否结束  
		bool Over;
	};
}


#endif

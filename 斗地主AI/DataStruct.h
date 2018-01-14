#ifndef __DATA_STRUCT_H__
#define __DATA_STRUCT_H__

#include "config.h"

namespace zero {
	//基本数据
	//手牌组合枚举  
	enum CardGroupType
	{
		cgERROR = -1,                                   //错误类型  
		cgZERO = 0,                                     //不出类型  
		cgSINGLE = 1,                                   //单牌类型  
		cgDOUBLE = 2,                                   //对牌类型  
		cgTHREE = 3,                                    //三条类型  
		cgSINGLE_LINE = 4,                              //单连类型  
		cgDOUBLE_LINE = 5,                              //对连类型  
		cgTHREE_LINE = 6,                               //三连类型  
		cgTHREE_TAKE_ONE = 7,                           //三带一单  
		cgTHREE_TAKE_TWO = 8,                           //三带一对  
		cgTHREE_TAKE_ONE_LINE = 9,                      //三带一单连  
		cgTHREE_TAKE_TWO_LINE = 10,                     //三带一对连  
		cgFOUR_TAKE_ONE = 11,                           //四带两单  
		cgFOUR_TAKE_TWO = 12,                           //四带两对  
		cgBOMB_CARD = 13,                               //炸弹类型  
		cgKING_CARD = 14                                //王炸类型  
	};
	
	//手牌权值结构  
	struct HandCardValue
	{
		int SumValue;        //手牌总价值  
		int NeedRound;       // 需要打几手牌  
	};

	//牌型组合数据结构  
	struct CardGroupData
	{
		//枚举类型  
		CardGroupType cgType = cgERROR;
		//该牌的价值  
		int  nValue = 0;
		//含牌的个数  
		int  nCount = 0;
		//牌中决定大小的牌值，用于对比  
		int nMaxCard = 0;

	};
#ifdef __DEBUG__

	//下发到三名玩家的手牌序列，此数据只用于测试，作为AI时不会获取  
	struct ALLCardsList
	{
		vector <int>  arrCardsList[3];
	};

#endif
  
	

}
#endif



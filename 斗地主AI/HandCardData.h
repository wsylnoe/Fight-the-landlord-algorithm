#include "GameSituation.h"
#include <vector>
#include "DataStruct.h"
#include "Array.hpp"
#include "config.h"

namespace zero {

	class HandCardData
	{
	public:
		//构造函数
		HandCardData();

		//析构函数
		virtual ~HandCardData();

	public:
		//手牌序列——无花色，值域3~17
		std::vector <int> value_nHandCardList;

		//int[18] 手牌序列——状态记录，便于一些计算，值域为该index牌对应的数量0~4
		zero::Array<int> value_aHandCardList;

		//手牌序列——有花色，按照从大到小的排列  56，52：大王小王……4~0：红3黑3方3花3
		std::vector <int> color_nHandCardList;

		//手牌个数 初始 = 17
		int nHandCardCount ;

		//玩家角色地位       0：地主    1：农民——地主下家   2：农民——地主上家
		int nGameRole;

		//玩家座位ID 
		int nOwnIndex;

		//玩家要打出去的牌类型
		zero::CardGroupData uctPutCardType;
		//要打出去的牌——无花色(按数值进行排序)
		std::vector <int> value_nPutCardList;
		//要打出去的牌——有花色(按花色进行排序)
		std::vector <int> color_nPutCardList;
		//当前打出牌的类型数据，被动出牌时玩家根据这里做出筛选  
		HandCardValue uctHandCardValue;


	public:

		//要打出的牌序列清空
		void ClearPutCardList();

		//手牌排序，大牌靠前(队列排序功能 从大到小)
		void SortAsList(std::vector <int> &arr);

		//出一张牌，返回操作是否合法  //提供给PutCards 使用
		bool PutOneCard(int value_nCard, int &clear_nCard);

		//出一组牌，返回操作是否合法
		bool PutCards();

		/**
		 * 通过有花色手牌获取无花色手牌（包含两种结构）
		 * color_nHandCardList链表中数据 转换为value_nHandCardList、value_aHandCardList数据
		 */
		void get_valueHandCardList();

		//初始化
		void Init();

#ifdef  __DEBUG__

		//输出所有成员变量，用于测试
		void PrintAll();

#endif

	};
}
/**
 color_nHandCardList 数据结构 
	00:A方块 01:A草花 02:A红心 03:A黑桃 04:2方块 05:2草花 06:2红心 07:2黑桃 08:3方块 09:3草花 10:3红心 11:3黑桃 .....

 value_nHandCardList 数据结构
	0:空 1:空 2:空 3:A(方块、草花 、红心、黑桃 合并) 4:1(方块、草花 、红心、黑桃合并)  ......

 value_aHandCardList  数据结构 
	(0:A)<=4 (1:2)<=4 (2:3)<=4

 */
#include "HandCardData.h"
#include<algorithm>

zero::HandCardData::HandCardData():nHandCardCount(17), nGameRole(-1), nOwnIndex(-1)
{
	value_aHandCardList.setDimension(18).InitValue(0);
}

zero::HandCardData::~HandCardData()
{
}

void zero::HandCardData::ClearPutCardList()
{
	//清空出牌队列
	color_nPutCardList.clear();		//要打出去的牌——有花色 出牌队列
	value_nPutCardList.clear();		//要打出去的牌——无花色 出牌队列

	//初始化数据结构
	uctPutCardType.cgType = cgERROR;	//牌枚举类型  
	uctPutCardType.nCount = 0;			//含牌的个数  
	uctPutCardType.nMaxCard = -1;		//牌中决定大小的牌值，用于对比 
	uctPutCardType.nValue = 0;			//该牌的价值

	return;
}

/*降序排序对比*/
int cmp(int a, int b) { return a > b ? 1 : 0; }


void zero::HandCardData::SortAsList(std::vector <int> & arr)
{
	std::sort(arr.begin(), arr.end(), cmp);
	return;
}

void zero::HandCardData::get_valueHandCardList()
{
 
	value_nHandCardList.clear(); //手牌序列——无花色 清空

	//初始化 value_aHandCardList 队列
	memset(value_aHandCardList.GetPointer(), 0, value_aHandCardList.GetBytesSize());

	//拷贝 手牌序列——有花色
	for (std::vector<int>::iterator iter = color_nHandCardList.begin(); iter != color_nHandCardList.end(); iter++)
	{
		auto index = (*iter / 4) + 3;
		value_nHandCardList.push_back(index);
		value_aHandCardList[index]++;
	}


}

void zero::HandCardData::Init()
{
	//根据花色手牌获取权值手牌  
	get_valueHandCardList();

	//手牌 排序  
	SortAsList(color_nHandCardList);
	SortAsList(value_nHandCardList);

	//当前手牌个数  
	nHandCardCount = value_nHandCardList.size();

}


#ifdef  __DEBUG__
/*
char* get_CardsName(int type) {
	//返回每张牌的名称
	return "assa  ";
}*/

void zero::HandCardData::PrintAll()
{


	std::cout << "color_nHandCardList:" << std::endl;

	for (std::vector<int>::iterator iter = color_nHandCardList.begin(); iter != color_nHandCardList.end(); iter++)
		std::cout << /*get_CardsName(*iter)*/" " << (iter == color_nHandCardList.end() - 1 ? '\n' : ',');

	std::cout << std::endl;


	std::cout << "value_nHandCardList：" << std::endl;
	for (std::vector<int>::iterator iter = value_nHandCardList.begin(); iter != value_nHandCardList.end(); iter++)
		std::cout << *iter << (iter == value_nHandCardList.end() - 1 ? '\n' : ',');

	std::cout << std::endl;

	std::cout << "value_aHandCardList：" << std::endl;
	for (int i = 0; i < 18; i++)
	{
		std::cout << value_aHandCardList[i] << (i == 17 ? '\n' : ',');
	}

	std::cout << std::endl;


	std::cout << "nHandCardCount:" << nHandCardCount << std::endl;

	std::cout << std::endl;

	std::cout << "nGameRole:" << nGameRole << std::endl;

	std::cout << std::endl;
}

#endif



bool  zero::HandCardData::PutCards()
{
	// value_nPutCardList   遍历   ---数据结构0:空 1:空 2:空 3:A(方块、草花 、红心、黑桃 合并) 4:1(方块、草花 、红心、黑桃合并)  ......
	for (std::vector<int>::iterator iter = value_nPutCardList.begin(); iter != value_nPutCardList.end(); iter++)
	{
		int color_nCard = -1;
		if (PutOneCard(*iter, color_nCard))
		{
			//可出牌 队列
			color_nPutCardList.push_back(color_nCard);
		}
		else
		{
			return false;
		}
	}

	nHandCardCount -= value_nPutCardList.size();
	return true;
}

bool  zero::HandCardData::PutOneCard(int value_nCard, int &color_nCard)
{
	bool ret = false;

	//value状态数组处理  
	//去掉一张牌
	value_aHandCardList[value_nCard]= value_aHandCardList[value_nCard]-1;

	//检测是否 剩余牌数是否 >= 0
	if (value_aHandCardList[value_nCard] < 0)
	{
		//没有此张牌 直接返回false
		return false;
	}

	//value_nHandCardList列表数组处理  
	for (std::vector<int>::iterator iter = value_nHandCardList.begin(); iter != value_nHandCardList.end(); iter++)
	{
		if (*iter == value_nCard)
		{
			value_nHandCardList.erase(iter);
			ret = true;
			break;
		}
	}


	// color列表数组处理  
	int k = (value_nCard - 3) * 4;      //数值转换  

	//删除有花色 列表中的这张牌
	for (std::vector<int>::iterator iter = color_nHandCardList.begin(); iter != color_nHandCardList.end(); iter++)
	{

		for (int i = k; i < k + 4; i++)
		{
			if (*iter == i)
			{
				color_nCard = i;
				color_nHandCardList.erase(iter);
				return ret;

			}
		}
	}
	return false;
}
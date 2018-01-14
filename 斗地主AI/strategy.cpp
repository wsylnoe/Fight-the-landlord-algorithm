#include "strategy.h"

using std::vector;
using std::cout;
using std::endl;


/*
	封装好的获取各类牌型组合结构函数
	@prarm  {CardGroupType}	cgType：牌型
	@prarm  {int}			MaxCard：决定大小的牌值（最大价值）
	@prarm  {int}			Count：牌数
	@return {CardGroupData} 牌型组合数据结构
*/
zero::CardGroupData zero::get_GroupData(zero::CardGroupType cgType, int MaxCard, int Count)
{
	//牌型组合数据结构 创建
	CardGroupData uctCardGroupData;

	uctCardGroupData.cgType = cgType;		//牌型
	uctCardGroupData.nCount = Count;		//决定大小的牌值牌数
	uctCardGroupData.nMaxCard = MaxCard;	//决定大小的牌值

	//nValue 该类型牌价值

	//不出牌型  
	if (cgType == cgZERO)
		uctCardGroupData.nValue = 0;
	//单牌类型  
	else if (cgType == cgSINGLE)
		uctCardGroupData.nValue = MaxCard - 10;
	//对牌类型  
	else if (cgType == cgDOUBLE)
		uctCardGroupData.nValue = MaxCard - 10;
	//三条类型  
	else if (cgType == cgTHREE)
		uctCardGroupData.nValue = MaxCard - 10;
	//单连类型  
	else if (cgType == cgSINGLE_LINE)
		uctCardGroupData.nValue = MaxCard - 10 + 1;
	//对连类型  
	else if (cgType == cgDOUBLE_LINE)
		uctCardGroupData.nValue = MaxCard - 10 + 1;
	//三连类型  
	else if (cgType == cgTHREE_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//三带一单  
	else if (cgType == cgTHREE_TAKE_ONE)
		uctCardGroupData.nValue = MaxCard - 10;
	//三带一对  
	else if (cgType == cgTHREE_TAKE_TWO)
		uctCardGroupData.nValue = MaxCard - 10;
	//三带一单连  
	else if (cgType == cgTHREE_TAKE_ONE_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//三带一对连  
	else if (cgType == cgTHREE_TAKE_TWO_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//四带两单  
	else if (cgType == cgFOUR_TAKE_ONE)
		uctCardGroupData.nValue = (MaxCard - 3) / 2;
	//四带两对  
	else if (cgType == cgFOUR_TAKE_TWO)
		uctCardGroupData.nValue = (MaxCard - 3) / 2;
	//炸弹类型   
	else if (cgType == cgBOMB_CARD)
		uctCardGroupData.nValue = MaxCard - 3 + 7;
	//王炸类型   
	else if (cgType == cgKING_CARD)
		uctCardGroupData.nValue = 20;
	//错误牌型  
	else
		uctCardGroupData.nValue = 0;


	return uctCardGroupData;
}

zero::HandCardValue zero::get_HandCardValue(HandCardData & clsHandCardData)
{
	//首先清空出牌队列，因为剪枝时是不调用get_PutCardList的  
	clsHandCardData.ClearPutCardList();

	HandCardValue uctHandCardValue;
	//出完牌了，其实这种情况只限于手中剩下四带二且被动出牌的情况，因为四带二剪枝做了特殊处理。  
	if (clsHandCardData.nHandCardCount == 0)
	{
		uctHandCardValue.SumValue = 0;
		uctHandCardValue.NeedRound = 0;
		return uctHandCardValue;
	}
	//————以下为剪枝：判断是否可以一手出完牌  
	CardGroupData uctCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
	//————不到万不得已我们都不会出四带二，都尽量保炸弹  
	if (uctCardGroupData.cgType != cgERROR && uctCardGroupData.cgType != cgFOUR_TAKE_ONE && uctCardGroupData.cgType != cgFOUR_TAKE_TWO)
	{
		uctHandCardValue.SumValue = uctCardGroupData.nValue;
		uctHandCardValue.NeedRound = 1;
		return uctHandCardValue;
	}

	//非剪枝操作，即非一手能出完的牌  

	/*只是获取出牌的序列，即clsHandCardData.value_nPutCardList及clsHandCardData.uctPutCardType
	其他成员均无改变，也不会调用出牌函数，get_PutCardList返回最优方案*/
	get_PutCardList_2(clsHandCardData);

	//要保存当前的clsHandCardData.value_nPutCardList及clsHandCardData.uctPutCardType用于回溯  
	CardGroupData NowPutCardType = clsHandCardData.uctPutCardType;
	vector<int> NowPutCardList = clsHandCardData.value_nPutCardList;

	if (clsHandCardData.uctPutCardType.cgType == cgERROR)
	{
		cout << "PutCardType ERROR!" << endl;
	}



	//---回溯↓  
	for (vector<int>::iterator iter = NowPutCardList.begin();
		iter != NowPutCardList.end(); iter++)
	{
		clsHandCardData.value_aHandCardList[*iter]--;
	}
	clsHandCardData.nHandCardCount -= NowPutCardType.nCount;
	//---回溯↑  
	HandCardValue tmp_SurValue = get_HandCardValue(clsHandCardData);//递归剩余牌价值  

																	//---回溯↓      
	for (vector<int>::iterator iter = NowPutCardList.begin();
		iter != NowPutCardList.end(); iter++)
	{
		clsHandCardData.value_aHandCardList[*iter]++;
	}
	clsHandCardData.nHandCardCount += NowPutCardType.nCount;
	//---回溯↑  

	uctHandCardValue.SumValue = NowPutCardType.nValue + tmp_SurValue.SumValue;
	uctHandCardValue.NeedRound = tmp_SurValue.NeedRound + 1;





	return uctHandCardValue;
}

zero::CardGroupData zero::ins_SurCardsType(int arr[])
{
	int nCount = 0;
	for (int i = 3; i < 18; i++)
	{
		nCount += arr[i];
	}

	CardGroupData retCardGroupData;
	retCardGroupData.nCount = nCount;


	//单牌类型  
	if (nCount == 1)
	{
		//用于验证的变量  
		int prov = 0;
		int SumValue = 0;
		for (int i = 3; i < 18; i++)
		{
			if (arr[i] == 1)
			{
				SumValue = i - 10;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgSINGLE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//对牌类型  
	if (nCount == 2)
	{
		//用于验证的变量  
		int prov = 0;
		int SumValue = 0;
		int i = 0;
		for (i = 3; i < 16; i++)
		{
			if (arr[i] == 2)
			{
				SumValue = i - 10;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgDOUBLE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三条类型  
	if (nCount == 3)
	{
		//用于验证的变量  
		int prov = 0;
		int SumValue = 0;
		int i = 0;
		for (i = 3; i < 16; i++)
		{
			if (arr[i] == 3)
			{
				SumValue = i - 10;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgTHREE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三带一单  
	if (nCount == 4)
	{
		//用于验证的变量  
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 18; i++)
		{
			if (arr[i] == 3)
			{
				SumValue = i - 10;
				prov1++;
				retCardGroupData.nMaxCard = i;

			}
			if (arr[i] == 1)
			{
				prov2++;
			}

		}
		if (prov1 == 1 && prov2 == 1)
		{
			retCardGroupData.cgType = cgTHREE_TAKE_ONE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三带一对  
	if (nCount == 5)
	{
		//用于验证的变量  
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 16; i++)
		{
			if (arr[i] == 3)
			{
				SumValue = i - 10;
				prov1++;
				retCardGroupData.nMaxCard = i;

			}
			if (arr[i] == 2)
			{
				prov2++;

			}
		}
		if (prov1 == 1 && prov2 == 1)
		{
			retCardGroupData.cgType = cgTHREE_TAKE_TWO;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//四带两单  
	if (nCount == 6)
	{
		//用于验证的变量  
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 18; i++)
		{
			if (arr[i] == 4)
			{
				SumValue = (i - 3) / 2;
				prov1++;
				retCardGroupData.nMaxCard = i;

			}
			if (arr[i] == 1 || arr[i] == 2)
			{
				prov2 += arr[i];
			}
		}

		if (prov1 == 1 && prov2 == 2)
		{
			retCardGroupData.cgType = cgFOUR_TAKE_ONE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//四带两对  
	if (nCount == 8)
	{
		//用于验证的变量  
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 16; i++)
		{
			if (arr[i] == 4)
			{
				SumValue = (i - 3) / 2;

				prov1++;
				retCardGroupData.nMaxCard = i;
			}
			if (arr[i] == 2 || arr[i] == 4)
			{
				prov2 += arr[i] / 2;

			}
		}
		//注意这里prov2==4因为四牌也是两个对  
		if (prov1 == 1 && prov2 == 4)
		{
			retCardGroupData.cgType = cgFOUR_TAKE_TWO;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//炸弹类型  
	if (nCount == 4)
	{
		//用于验证的变量  
		int prov = 0;
		int SumValue = 0;
		for (int i = 3; i < 16; i++)
		{
			if (arr[i] == 4)
			{
				SumValue += i - 3 + 7;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgBOMB_CARD;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//王炸类型  
	if (nCount == 2)
	{
		int SumValue = 0;
		if (arr[17] > 0 && arr[16] > 0)
		{
			SumValue = 20;
			retCardGroupData.nMaxCard = 17;
			retCardGroupData.cgType = cgKING_CARD;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//单连类型  
	if (nCount >= 5)
	{
		//用于验证的变量  
		int prov = 0;
		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 1)
			{
				prov++;
			}
			else
			{
				if (prov != 0)
				{
					break;
				}

			}
		}
		SumValue = i - 10;

		if (prov == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgSINGLE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//对连类型  
	if (nCount >= 6)
	{
		//用于验证的变量  
		int prov = 0;
		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 2)
			{
				prov++;
			}
			else
			{
				if (prov != 0)
				{
					break;
				}

			}
		}
		SumValue = i - 10;

		if (prov * 2 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgDOUBLE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三连类型  
	if (nCount >= 6)
	{
		//用于验证的变量  
		int prov = 0;

		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 3)
			{
				prov++;
			}
			else
			{
				if (prov != 0)
				{
					break;
				}

			}
		}
		SumValue = (i - 3) / 2;

		if (prov * 3 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgTHREE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三带一连类型  
	if (nCount >= 8)
	{
		//用于验证的变量  
		int prov1 = 0;
		int SumValue = 0;
		int i, j;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] >= 3)
			{
				prov1++;
			}
			else
			{
				if (prov1 != 0)
				{
					break;
				}

			}
		}
		SumValue = (i - 3) / 2;
		if (prov1 * 4 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgTHREE_TAKE_ONE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}

	}
	//三带二连类型  
	if (nCount >= 10)
	{
		//用于验证的变量  
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		int i, j;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 3)
			{
				prov1++;
			}
			else
			{
				if (prov1 != 0)
				{
					break;
				}
			}
		}
		for (j = 3; j < 16; j++)
		{
			if (arr[j] == 2 || arr[j] == 4)
			{
				prov2 += arr[j] / 2;
			}
		}
		SumValue = (i - 3) / 2;
		if (prov1 == prov2 && prov1 * 5 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgTHREE_TAKE_TWO_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}

	retCardGroupData.cgType = cgERROR;
	return retCardGroupData;
}

zero::CardGroupData zero::ins_SurCardsType(std::vector<int> list)
{
	int arr[18];
	memset(arr, 0, sizeof(arr));
	for (vector<int>::iterator iter = list.begin(); iter != list.end(); iter++)
	{
		arr[*iter]++;
	}
	return ins_SurCardsType(arr);
}

void zero::get_PutCardList_2_limit(GameSituation & clsGameSituation, HandCardData & clsHandCardData)
{
	clsHandCardData.ClearPutCardList();


	/*王炸——当前策略只处理王炸作为倒数第二手的优先出牌逻辑，后续版本会在此基础上优化*/
	if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
	{

		clsHandCardData.value_aHandCardList[17] --;
		clsHandCardData.value_aHandCardList[16] --;
		clsHandCardData.nHandCardCount -= 2;
		HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
		clsHandCardData.value_aHandCardList[16] ++;
		clsHandCardData.value_aHandCardList[17] ++;
		clsHandCardData.nHandCardCount += 2;
		if (tmpHandCardValue.NeedRound == 1)
		{
			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
			return;
		}
	}


	//错误牌型  不出  
	if (clsGameSituation.uctNowCardGroup.cgType == cgERROR)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgERROR, 0, 0);
		return;
	}
	//不出牌型，在被动出牌策略里也是错误数据 不出  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgZERO)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//单牌类型  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgSINGLE)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;

	}
	//对牌类型  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgDOUBLE)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三牌类型  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE)
	{

		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//单连类型  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgSINGLE_LINE)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//对连类型  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgDOUBLE_LINE)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;

	}
	//三连类型  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_LINE)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一单  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_ONE)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一对  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_TWO)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一单连  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_ONE_LINE)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一对连  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_TWO_LINE)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//四带两单  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgFOUR_TAKE_ONE)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//四带两对  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgFOUR_TAKE_TWO)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//炸弹类型   
	else if (clsGameSituation.uctNowCardGroup.cgType == cgBOMB_CARD)
	{
		//管不上  
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//王炸类型 人都王炸了你还出个毛  
	else if (clsGameSituation.uctNowCardGroup.cgType == cgKING_CARD)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//异常处理 不出  
	else
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
	}
	return;

}

void zero::Put_All_SurCards(GameSituation & clsGameSituation, HandCardData & clsHandCardData, CardGroupData SurCardGroupData)
{
	/*全部出完*/
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < clsHandCardData.value_aHandCardList[i]; j++)
			clsHandCardData.value_nPutCardList.push_back(i);
	/**********/
	clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = SurCardGroupData;
	return;
}

void zero::Put_All_SurCards(HandCardData & clsHandCardData, CardGroupData SurCardGroupData)
{
	/*全部出完*/
	for (int i = 0; i < 18; i++)
		for (int j = 0; j < clsHandCardData.value_aHandCardList[i]; j++)
			clsHandCardData.value_nPutCardList.push_back(i);
	/**********/
	clsHandCardData.uctPutCardType = SurCardGroupData;
	return;
}

void zero::get_PutCardList_2(HandCardData & clsHandCardData)
{
	clsHandCardData.ClearPutCardList();

	//剪枝：如果能出去最后一手牌直接出  
	CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
	//如果能一次性出去且不是四带二，因为主动出牌若手上剩四带二牌的话可以考虑先打一手然后炸，获得双倍积分  
	if (SurCardGroupData.cgType != cgERROR && SurCardGroupData.cgType != cgFOUR_TAKE_ONE && SurCardGroupData.cgType != cgFOUR_TAKE_TWO)
	{
	}

	/*王炸——当前策略只处理王炸作为倒数第二手的优先出牌逻辑，后续版本会在此基础上优化*/
	if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
	{
	}

	//暂存最佳的价值  
	HandCardValue BestHandCardValue;
	BestHandCardValue.NeedRound = 20;
	BestHandCardValue.SumValue = MinCardsValue;
	//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。  
	BestHandCardValue.NeedRound += 1;

	//暂存最佳的组合  
	CardGroupData BestCardGroup;

	//带出去的牌  
	int tmp_1 = 0;
	int tmp_2 = 0;
	int tmp_3 = 0;
	int tmp_4 = 0;
	//优先处理三牌、飞机等牌  
	for (int i = 3; i < 16; i++)
	{
	}
	//这部分出牌处理放到循环外  
	if (BestCardGroup.cgType == cgTHREE_TAKE_ONE)
	{

	}
	else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO)
	{

	}
	else if (BestCardGroup.cgType == cgTHREE_TAKE_ONE_LINE)
	{

	}
	else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO_LINE)
	{

	}


	//次之处理当前价值最低的牌，现在不必再考虑这张牌可能被三牌带出等情况  
	for (int i = 3; i < 16; i++)
	{
	}
	//如果没有3-2的非炸牌，则看看有没有单王  
	if (clsHandCardData.value_aHandCardList[16] == 1 && clsHandCardData.value_aHandCardList[17] == 0)
	{
	}
	if (clsHandCardData.value_aHandCardList[16] == 0 && clsHandCardData.value_aHandCardList[17] == 1)
	{
	}
	//单王也没有，出炸弹  
	for (int i = 3; i < 16; i++)
	{
	}

	//异常错误  
	clsHandCardData.uctPutCardType = get_GroupData(cgERROR, 0, 0);
	return;
}

int zero::LandScore(GameSituation & clsGameSituation, HandCardData & clsHandCardData)
{
	int SumValue = 0;

	clsHandCardData.uctHandCardValue = get_HandCardValue(clsHandCardData);

	SumValue = clsHandCardData.uctHandCardValue.SumValue;

	cout << "SumValue is :" << SumValue << ",";

	cout << "NeedRound is :" << clsHandCardData.uctHandCardValue.NeedRound << endl;

	if (SumValue < 10)
	{
		return 0;
	}
	else if (SumValue < 15)
	{
		return 1;
	}
	else if (SumValue < 20)
	{
		return 2;
	}
	else
	{
		return 3;
	}
}

void zero::InitCards(std::vector<int>& Cards)
{
	//先清空Cards  
	Cards.clear();

	vector <int> tmpCards;
	int i;

	//大王56，小王52，没有53，54，55号牌  
	for (i = 0; i < 53; i++) {
		tmpCards.push_back(i);
	}
	tmpCards.push_back(56);


	//顺序随机打乱  
	for (i = tmpCards.size(); i > 0; i--) {
		srand(unsigned(time(NULL)));
		// 选中的随机下标  
		int index = rand() % i;
		Cards.push_back(tmpCards[index]);
		tmpCards.erase(tmpCards.begin() + index);
	}

}

void zero::InitCards_Appoint(std::vector<int>& Cards)
{
	//先清空Cards  
	Cards.clear();

	/***********飞机与炸弹连续拆分逻辑测试**********/

	Cards.push_back(48); Cards.push_back(50); Cards.push_back(49);
	Cards.push_back(44); Cards.push_back(47); Cards.push_back(35);
	Cards.push_back(40); Cards.push_back(46); Cards.push_back(34);
	Cards.push_back(36); Cards.push_back(45); Cards.push_back(33);
	Cards.push_back(23); Cards.push_back(43); Cards.push_back(31);
	Cards.push_back(22); Cards.push_back(42);  Cards.push_back(30);
	Cards.push_back(21); Cards.push_back(41); Cards.push_back(29);
	Cards.push_back(19); Cards.push_back(39); Cards.push_back(27);
	Cards.push_back(18); Cards.push_back(38); Cards.push_back(26);
	Cards.push_back(17); Cards.push_back(37); Cards.push_back(25);
	Cards.push_back(15); Cards.push_back(32);  Cards.push_back(20);
	Cards.push_back(14); Cards.push_back(28); Cards.push_back(16);
	Cards.push_back(13); Cards.push_back(24); Cards.push_back(12);
	Cards.push_back(11); Cards.push_back(3); Cards.push_back(7);
	Cards.push_back(10); Cards.push_back(2); Cards.push_back(6);
	Cards.push_back(9); Cards.push_back(1); Cards.push_back(5);
	Cards.push_back(8); Cards.push_back(0); Cards.push_back(4);
	Cards.push_back(51); Cards.push_back(52); Cards.push_back(56);


}

void zero::SendCards(GameSituation & clsGameSituation, ALLCardsList & uctALLCardsList)
{
	//洗牌  
	vector <int> Cards;
	InitCards(Cards);
	//InitCards_Appoint(Cards);  
	int i, j, k;
	j = 0;
	for (k = 0; k < 17; k++) {
		for (i = 0; i < 3; i++, j++)
		{
			uctALLCardsList.arrCardsList[i].push_back(Cards[j]);
		}
	}

	//三张底牌  
	clsGameSituation.DiPai[0] = Cards[j];
	clsGameSituation.DiPai[1] = Cards[j + 1];
	clsGameSituation.DiPai[2] = Cards[j + 2];

	return;
}

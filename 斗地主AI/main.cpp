#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "Array.hpp"
#include "config.h"


/*
再然后就是模拟游戏过程，首先定义游戏全局类，与三名玩家的手牌信息类。调用发牌函数完成发牌环节，可以用手牌信息类里面的PrintAll输出你想要的数据信息。
*/
void process1() {
	GameSituation clsGameSituation;

	ALLCardsList  uctALLCardsList;

	//发牌  
	SendCards(clsGameSituation, uctALLCardsList);

	HandCardData arrHandCardData[3];

	arrHandCardData[0].color_nHandCardList = uctALLCardsList.arrCardsList[0];
	arrHandCardData[1].color_nHandCardList = uctALLCardsList.arrCardsList[1];
	arrHandCardData[2].color_nHandCardList = uctALLCardsList.arrCardsList[2];

	for (int i = 0; i < 3; i++)
	{
		arrHandCardData[i].Init();
		arrHandCardData[i].nOwnIndex = i;
	}

	cout << "0号玩家牌为：" << endl;
	arrHandCardData[0].PrintAll();
	cout << "1号玩家牌为：" << endl;
	arrHandCardData[1].PrintAll();
	cout << "2号玩家牌为：" << endl;
	arrHandCardData[2].PrintAll();

	cout << "底牌为：" << endl;
	cout << get_CardsName(clsGameSituation.DiPai[0]) << ','
		<< get_CardsName(clsGameSituation.DiPai[1]) << ','
		<< get_CardsName(clsGameSituation.DiPai[2]) << endl;

	cout << endl;
}
/*
发完牌后开始叫地主，调用LandScore函数返回其叫的分值，只有比当前已叫的分值更高才可以刷新叫地主记录。若无人叫地主重新开一局，否则将三张底牌给地主，同时刷新地主手牌，且将地主设置成将要出牌的玩家
*/
void process2() {
	for (int i = 0; i < 3; i++)
	{
		int  tmpLandScore = LandScore(clsGameSituation, arrHandCardData[i]);
		if (tmpLandScore > clsGameSituation.nNowLandScore)
		{
			clsGameSituation.nNowLandScore = tmpLandScore;
			clsGameSituation.nNowDiZhuID = i;
		}
	}

	if (clsGameSituation.nNowDiZhuID == -1)
	{
		cout << "无人叫地主" << endl;
		return;
	}

	cout << clsGameSituation.nNowDiZhuID << "号玩家是地主，叫分为：" << clsGameSituation.nNowLandScore << endl;
	clsGameSituation.nDiZhuID = clsGameSituation.nNowDiZhuID;
	clsGameSituation.nLandScore = clsGameSituation.nNowLandScore;


	//将三张底牌给地主  
	arrHandCardData[clsGameSituation.nDiZhuID].color_nHandCardList.push_back(clsGameSituation.DiPai[0]);
	arrHandCardData[clsGameSituation.nDiZhuID].color_nHandCardList.push_back(clsGameSituation.DiPai[1]);
	arrHandCardData[clsGameSituation.nDiZhuID].color_nHandCardList.push_back(clsGameSituation.DiPai[2]);

	//地主手牌刷新  
	arrHandCardData[clsGameSituation.nDiZhuID].Init();

	//出牌玩家ID  
	int indexID = clsGameSituation.nDiZhuID;

	cout << endl;


	cout << "0号玩家牌为：" << endl;
	arrHandCardData[0].PrintAll();
	cout << "1号玩家牌为：" << endl;
	arrHandCardData[1].PrintAll();
	cout << "2号玩家牌为：" << endl;
	arrHandCardData[2].PrintAll();
	//当前控手玩家先为地主  
	clsGameSituation.nCardDroit = indexID;
}


/*
接下来就是循环进行出牌了。在游戏全局类里我们设置了一个标志是否结束的变量，可以用于控制循环。出牌时我们只需调用get_PutCardList出牌函数即可。若某个玩家出完牌后手牌为0，则游戏结束。若玩家出过牌，则刷新游戏全局类里面当前牌型信息。
*/
void process3() {

	while (!clsGameSituation.Over)
	{
		get_PutCardList_2(clsGameSituation, arrHandCardData[indexID]);//获取出牌序列  
		arrHandCardData[indexID].PutCards();
		cout << indexID << "号玩家出牌：" << endl;
		for (vector<int>::iterator iter = arrHandCardData[indexID].color_nPutCardList.begin();
			iter != arrHandCardData[indexID].color_nPutCardList.end(); iter++)
			cout << get_CardsName(*iter) << (iter == arrHandCardData[indexID].color_nPutCardList.end() - 1 ? '\n' : ',');
		cout << endl;

		if (arrHandCardData[indexID].nHandCardCount == 0)
		{
			clsGameSituation.Over = true;

			if (indexID == clsGameSituation.nDiZhuID)
			{
				cout << "地主" << indexID << "号玩家获胜" << endl;
			}
			else
			{
				for (int i = 0; i < 3; i++) {
					if (i != clsGameSituation.nDiZhuID)
					{
						cout << "农民" << i << "号玩家获胜" << endl;
					}
				}
			}

		}

		if (arrHandCardData[indexID].uctPutCardType.cgType != cgZERO)
		{
			clsGameSituation.nCardDroit = indexID;
			clsGameSituation.uctNowCardGroup = arrHandCardData[indexID].uctPutCardType;
		}

		indexID == 2 ? indexID = 0 : indexID++;

	}
}



int main() {
	



	 getchar();
	 return 0;
}
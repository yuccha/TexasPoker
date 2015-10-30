//#include "stdafx.h"
#include "player.h"
#include <iostream>

using namespace std;


player::player(void)
{
	rank = -1;
	if_all_in = FALSE;
	if_fold = FALSE;
	if_flush = 0;
	max_type = 9;
	bet = 0;
	timer_hold = 0;
	for (int i = 0; i != 2; i++)
	{
		for (int j = 0; j != 7; j++)
		{
			if (j < 2)
				card_hand[i][j] = 0;
			if (j < 5)
				max_card[i][j] = 0;
			total_card[i][j] = 0;
		}
	}
}

void player::acting (int act)
{
	switch (act)
	{
	case 1: //bet
		{
			int bet_money = -1;
			while (bet_money < sb_bet)
			{
				cout << "How much would you like to bet?\nBet:" << endl;
				cin >> bet_money;
				if (bet_money < sb_bet)
					cout << "bet money is too small, it must be larger than $" << sb_bet << endl;
			}
			cout << "Player " << id << " bet " << bet_money << endl;
			timer_serve++;
			timer_hold = timer_serve;
			bet += bet_money;
			if_bet = TRUE;
			//correct_act = TRUE;
			last_rise = id-1;
			sb_bet = bet_money;
			last_one_bet = bet;
		}
		break;
	case 2: //rise
		{
			int rise_money = -1;
			while (rise_money < sb_bet)
			{
				cout << "How much would you like to rise?"
						"\nIf you rise $" << sb_bet << ", it means that you bet $" << sb_bet+last_one_bet << endl
						<< "Rise:";
				cin >> rise_money;
				if (rise_money < sb_bet)
					cout << "Rise money is too small, it must be larger than $" << sb_bet << endl;
			}
			cout << "Player " << id << " rise " << rise_money << endl;
			timer_serve++;
			timer_hold = timer_serve;
			bet += (rise_money + last_one_bet);
			last_one_bet = bet;
			sb_bet = rise_money;
			//correct_act = TRUE;
			last_rise = id-1;
		}
		break;
	case 3: //call
		cout << "Player " << id << " call" << endl;
		bet = last_one_bet;
		timer_serve++;
		timer_hold = timer_serve;
		break;
	case 4: //check
		cout << "Player " << id << " check" << endl;
		break;
	case 5: //fold
		cout << "Player " << id << " fold" << endl;
		if_fold = TRUE;
		max_type = 9;
		break;
	case 6: //all in
		cout << "Player " << id << " All-in" << endl;
		if_all_in = TRUE;
		if (money > (last_one_bet + sb_bet))
		{
			timer_serve++;
			timer_hold = timer_serve;
			sb_bet = money - last_one_bet;	// << right?
			last_one_bet = money;
			bet = money;
		}
		else
		{
			timer_serve++;
			timer_hold = timer_serve;
			last_one_bet = money;	// << right?
			bet = money;
		}
		last_rise = id-1;
		break;
	default:
		//cout << "WRONG!! ENTER AGAIN!!\n";
		break;
	}
	system ("PAUSE");
}

void player::computer_bet_rise (int act, int total_bet)			// toal_bet represent bet_money or rise num
{
	switch (act)
	{
	case 1:									// bet
		cout << "Player" << id << " bet " << total_bet << endl;
		timer_serve++;
		timer_hold = timer_serve;
		bet += total_bet;
		if_bet = TRUE;
		//correct_act = TRUE;
		last_rise = id-1;
		sb_bet = total_bet;
		last_one_bet = bet;
		break;
	case 2:									// rise
		cout << "Player" << id << " rise " << total_bet << endl;
		timer_serve++;
		timer_hold = timer_serve;
		bet += (total_bet + last_one_bet);
		last_one_bet = bet;
		sb_bet = total_bet;
		//correct_act = TRUE;
		last_rise = id-1;
		break;
	default:
		break;
	}
	system ("PAUSE");

}

void player::get_and_sort (void)
{
	for (int i = 0; i != 7; i++)
	{
		for (int j = 1; j != 7-i; j++)
			if (total_card[1][j-1] > total_card[1][j])
			{
				total_card[0][j-1] ^= total_card[0][j] ^= total_card[0][j-1] ^= total_card[0][j];
				total_card[1][j-1] ^= total_card[1][j] ^= total_card[1][j-1] ^= total_card[1][j];
			}
	}
}

void player::flush (int not_repeat[7], int repeat_time[7], int flushcard[7])
{
	if_flush = 0;
	int test_shape[4]={0};
	for (int i=0; i != 7; i++)
		test_shape[total_card[0][i]-1]++;
	for (int i=0; i != 4; i++)
	{
		if (test_shape[i] > 4)
		{
			if_flush = i+1;
			for (int j = 0, k = 0; j != 7; j++)
		    {
				if (total_card[0][j] == i+1)
				{
					flushcard[k] = total_card[1][j];
					k++;
				}
			}	
		}	
	}
}

int player::straight (int incard[7])
{
	int if_staight = 0;
	int endcard=0;
	int counter2 = 0;
	for(int j=1; j != 7; j++)
	{
		if ((incard[j-1]-(j-1)) == (incard[j]-j))
		{
			counter2++;
			endcard=j;
		}
		else
		{
			if(counter2 < 4)
				counter2 = 0;
		    else
				break;
		}	
	}
	if (counter2 > 3)		// 是的話就是順了啊！所以直接用max_card存！
	{
		if_staight = 1;
		for (int i=0; i != 5; i++)
		{
			max_card[1][i] = incard[endcard];
			endcard--;
			if (if_flush)
			{
				max_card[0][i] = if_flush;
				max_type = 0;
			}
			else
			{
				for (int j = 0; j != 7; j++)
				{
					if (max_card[1][i] == total_card[1][j])
					{
						max_card[0][i] = total_card[0][j];
						break;
					}
				}
				max_type = 4;
			}
		}
		/*cout << "\nStraight card:";
		for (int i = 0; i != 5; i++)
			cout  << "{" << max_card[0][i] << "," << max_card[1][i] << "} ";*/
	}
	return if_staight;
}

void player::other_type (int not_repeat[7], int repeat_time[7])
{
	int *maxsame = repeat_time;
	int maxplace = 0;	// place of max!
	for (int i = 1; i != 7; i++)
	{
		if (*maxsame <= repeat_time[i])
		{
			maxsame = &repeat_time[i];
			maxplace = i;
		}
	} // find the largest one!
	//test four of a kind
	if ((*maxsame) == 4)
	{
		max_type = 1;
		for (int i = 0; i != 4; i++)
		{
			max_card[0][i] = i+1; 
			max_card[1][i] = not_repeat[maxplace]; 
		}
		for (int i = 6; i != -1; i--)
		{
			if (i == maxplace)
				continue;
			else if (not_repeat[i])
			{
				max_card[1][4] = not_repeat[i];
				for (int j = 6; j != -1; j--)
					if (not_repeat[i] == total_card[1][j])
					{
						max_card[0][4] = total_card[0][j];
						break;
					}
				break;
			}
		}
	}
	//test full house and three of a kind
	if ((*maxsame) == 3)
	{
		for (int i=0; i != 3; i++)
			max_card[1][i] = not_repeat[maxplace];
		for (int j = 6, k = 0; j != -1; j--)
			if (not_repeat[maxplace] == total_card[1][j])
			{
				max_card[0][k] = total_card[0][j];
				k++;
				if (k == 3)
					break;
			}
		for (int i = 6; i != -1; i--)
		{ 
			if ((repeat_time[i] >= 2) && (i != maxplace))
			{
				max_card[1][3] = not_repeat[i];
				max_card[1][4] = not_repeat[i];
				for (int j = 6, k = 0; j != -1; j--)
					if (not_repeat[i] == total_card[1][j])
					{
						max_card[0][3+k] = total_card[0][j];
						k++;
						if (k == 2)
							break;
					}
				max_type = 2;
				break;
			}
		}	//full house
		if (if_flush)
			return;
		if (max_type != 2)
		{ 
			max_type = 5;
			for (int i = 6, j = 0; i != -1; i--)
			{
				if (not_repeat[maxplace] == total_card[1][i])
					continue;
				else
				{
					max_card[0][3+j] = total_card[0][i];
					max_card[1][3+j] = total_card[1][i];
					j++;
					if (j == 2)
						break;
				}
			}
		}	//three of a kind
	}
	//two pairs and one pair
	if (if_flush)
		return;
	if ((*maxsame) == 2)
	{
		for (int i=0; i!=2; i++)
			max_card[1][i] = not_repeat[maxplace];
		for (int j = 6, k = 0; j != -1; j--)
			if (not_repeat[maxplace] == total_card[1][j])
			{
				max_card[0][k] = total_card[0][j];
				k++;
				if (k == 2)
					break;
			}
		int i, secmax;
		(*maxsame) = 0;
		for (i = 6; i != -1; i--)
		{
			if (repeat_time[i] == 2)
			{
				secmax = i;
				break;
			}
		}
		if (i != -1)
		{
			max_type = 6;
			for (int i = 2; i != 4; i++)
				max_card[1][i] = not_repeat[secmax];
			for (int j = 6, k = 0; j != -1; j--)
				if (not_repeat[secmax] == total_card[1][j])
				{
					max_card[0][2+k] = total_card[0][j];
					k++;
					if (k == 2)
						break;
				}
			for (int i = 6; i != -1; i--)
			{
				if ((i == maxplace) || (i == secmax))
					continue;
				else if (not_repeat[i])
				{
					max_card[1][4] = not_repeat[i];
					for (int j = 6; j != -1; j--)
						if (not_repeat[secmax] == total_card[1][j])
						{
							max_card[0][4] = total_card[0][j];
							break;
						}
					break;
				}
			}
		}
		else
		{
			max_type=7;
			for (int i = 6, j = 0; i != -1; i--)
			{
				if (not_repeat[maxplace] == total_card[1][i])
					continue;
				else
				{
					max_card[0][2+j] = total_card[0][i];
					max_card[1][2+j] = total_card[1][i];
					j++;
					if (j == 3)
						break;
				}
			}
		}
	}	//one pair	
	if ((*maxsame) == 1)
	{
		max_type=8;
		for(int i = 0; i != 5; i++)
		{
			max_card[0][i] = total_card[0][6-i];
			max_card[1][i] = total_card[1][6-i];
		}
	}	//high card
}

int player::maximum_card(void)
{   
	int flushcard[7] = {0};
	int not_repeat[7] = {0};
	int repeat_time[7] = {0};
    get_and_sort();
	//two check array start
	not_repeat[0] = total_card[1][0];
	repeat_time[0]++;
	for (int i = 1, j = 0; i != 7; i++)
	{
		if (total_card[1][i] == total_card[1][i-1])
			repeat_time[j]++;
		else
		{
			j++;
			not_repeat[j] = total_card[1][i];
			repeat_time[j]++;
		}
	}
	/*cout<<"\nnot_repeat array"<<endl;
	for (int i = 0;i != 7; i++)
		cout << not_repeat[i] << ",";
	cout<<endl;
	cout<<"repeat_time array"<<endl;	
	for (int i = 0;i != 7; i++)
		cout<<repeat_time[i]<<",";*/
	// Check Flush!
	flush(not_repeat, repeat_time, flushcard);
	// Flush or not
	if (if_flush)
	{
		int tag = straight (flushcard);
		if (tag == 0)			// not a straight! Get flush
		{
			other_type (not_repeat, repeat_time);
			if (max_type > 3)	// not a full house or four of a kind >> FLUSH
			{
				max_type = 3;
				for (int j = 0, k = 0; j != 7; j++)
				{
					if (flushcard[6-j] == 0)
						continue;
					else
					{
						max_card[1][k] = flushcard[6-j];
						max_card[0][k] = if_flush;
						k++;
						if (k == 5)
							break;
					}
				}
			}
		}
	}
	else
	{
		int tag = straight (not_repeat);
		if (tag == 0)		// not a straight! Get others!
			other_type (not_repeat, repeat_time);  // Get others!
	}

	/*cout << "\nCard taken:";
	for (int i = 0; i != 5; i++)
		cout  << "{" << max_card[0][i] << "," << max_card[1][i] << "} ";
	cout << endl;*/
	return max_type;
}

void player::show_type (void)
{
	switch (max_type)
	{
	case 0:
		cout << "Staight Flush!\n";
		break;
	case 1:
		cout << "Four of a Kind!\n";
		break;
	case 2:
		cout << "Full House!\n";
		break;
	case 3:
		cout << "Flush!\n";
		break;
	case 4:
		cout << "Staight!\n";
		break;
	case 5:
		cout << "Three of a Kind!\n";
		break;
	case 6:
		cout << "Two Pairs!\n";
		break;
	case 7:
		cout << "Pair!\n";
		break;
	case 8:
		cout << "High Card!\n";
		break;
	}
}

void player::initialize_pturn (void)				// FOR THE NON STATIC MEMBER!!
{
	rank = -1;
	bet = 0;
	//if_fold = FALSE;
}

void player::initlz_pturn_sta (int big_blind)		// FOR THE STATIC MEMBER!!!!!
{
	if_bet = FALSE;
	sb_bet = big_blind;					// wait for a while!
	last_rise = 10;
	last_one_bet = 0;
}

/*
我的設定是：total_card前兩張為card_hold，後五張為card_desk
因為我們在求maximum_card的時候，難免會將前兩張調換順序
因此當我們要算機率的時候，我們應該要在寫一個簡易版本
同時，此簡易版本不用判定花色！（應該說不用存入花色！我用暴力法找花色！）
簡易版本就是利用copy！傳入東西，然後在裡面創造一個變數存取傳入的變數！
*/
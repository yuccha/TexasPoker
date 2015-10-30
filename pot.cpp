// pot.cpp
//

//#include "stdafx.h"
#include <iostream>
#include "player.h"
#include "Table.h"
#include "pot.h"

using namespace std;

pot::pot (void)
{
	for (int i = 0; i != N; i++)
	{
		pots[i] = bet_each_man[i] = residue[i] = 0;
		player_no[i] = -1;
	}
	all_pot = 0;
}

void pot::betting (int id, int betting_num)
{
	bet_each_man[id] += betting_num;
	all_pot += betting_num;
}

void pot::pot_distributing (player* play, int num_of_play, int smiler)
{
	int temp, all_in_tag = 0;
	for (int i = 0; i != num_of_play; i++)
		if (play[i].if_all_in == TRUE)
		{
			all_in_tag = 1;
			break;
		};
	// if sb. all in, then use the all-in method!!
	if (all_in_tag == 1)
	{
		cout << "pots:" << all_pot << endl;
		for (int i = 0, j = 0; i != N; i++)
		{
			if (bet_each_man[i] == 0)				// if player 3 out! and he didnt bet any
				continue;
			else
			{
				pots[j] = bet_each_man[i];
				player_no[j] = i;
				j++;
			}
		}
		// copying from bet_each_man => pots, ignoring people didn't bet!
		for (int i = 0; i != N; i++)
		{
			for (int j = 0; j != N-1-i; j++)
			{
				if (pots[j+1] == 0)
					break;
				else if (pots[j+1] < pots[j])
				{
					pots[j] ^= pots[j+1] ^= pots[j] ^= pots[j+1];
					player_no[j] ^= player_no[j+1] ^= player_no[j] ^= player_no[j+1];
				}
				else if ((pots[j+1] == pots[j]) && (play[player_no[j]].timer_hold > play[player_no[j+1]].timer_hold))			 // beting last should be attend last!, find the bigger timer!
				{
					pots[j] ^= pots[j+1] ^= pots[j] ^= pots[j+1];
					player_no[j] ^= player_no[j+1] ^= player_no[j] ^= player_no[j+1];
				}
			}
		}
		// end swapping!! Start manipulating!!
		// FROME HERE IS OK!!
		for (int i = 0; i != N; i++)
		{
			if (pots[i] == 0)
				continue;
			temp = pots[i];
			for (int j = i+1; j != N; j++)
			{
				if (pots[j] == 0)
					continue;
				pots[j] -= pots[i];
				temp += pots[i];
			}
			pots[i] = temp;
		}
		
		// end manipulating all pots (pots[0] == main pot!, pots[i>0] == side pots!!)
		// upper is right!!
		// Start distributing!!
		int repeat_time = 0, num_of_player_no;
		for (num_of_player_no = 0; num_of_player_no != N; num_of_player_no++)
			if (player_no[num_of_player_no] == -1)
				break;
		int *ranking = new int [num_of_player_no];
		int *same_rnk = new int [num_of_player_no];
		int arg_player[N] = {};			// the inverse func of player_no! player no means where he attended until!
		for (int i = 0; i != N; i++)
			arg_player[i] = -1;
		for (int i = 0; i != num_of_player_no; i++)
		{
			ranking[i] = player_no[i];
			same_rnk[i] = 0;
			arg_player[player_no[i]] = i;
		}
		// end initializing!!
		for (int i = 0; i != num_of_player_no; i++)
		{
			for (int j = 0; j != num_of_player_no-i-1; j++)
			{
				if (play[ranking[j]].rank < play[ranking[j+1]].rank)
					ranking[j] ^= ranking[j+1] ^= ranking[j] ^= ranking[j+1];
				else if ((play[ranking[j]].rank == play[ranking[j+1]].rank) && (arg_player[ranking[j]] > arg_player[ranking[j+1]]))
					ranking[j] ^= ranking[j+1] ^= ranking[j] ^= ranking[j+1];
			}
		}
		//cout << num_of_player_no << endl;
		// above ok?
		// end initializing ranking!!
		int count = 0;
		for (int i = 1; i != num_of_player_no; i++)
		{
			if (play[ranking[i]].rank == play[ranking[i-1]].rank)
				count++;
			else if (count != 0)		// somebody at the instance that !=
			{
				//cout << "HI?? " << i << " " << count << endl;
				for (int l = 0; l != count+1; l++)
				{
					same_rnk[i-1-l] = l;
					//cout << same_rnk[i-l] << endl;
				}
				count = 0;
			}
		}
		/*for (int i = 0; i != num_of_player_no; i++)
			cout << ranking[i] << " ";
		cout << endl;
		for (int i = 0; i != num_of_player_no; i++)
			cout << same_rnk[i] << " ";
		cout << endl;
		system ("PAUSE");//*/
		// above ok!!
		// end initializing same_rnk!!
		int side;
		for (int i = 0; i != num_of_player_no; i++)
		{
			//cout << "Hi\n";
			side = arg_player[ranking[i]];
			for (int j = 0; j != side+1; j++)
			{
				residue[side] = pots[j] - (same_rnk[i]+1) * (pots[j] / (same_rnk[i]+1));
				for (int k = i; k != same_rnk[i]+1+i; k++)
				{
					//cout << "WTF?\n";
					play[ranking[k]].money += pots[j] / (same_rnk[i]+1);
				}
				all_pot -= pots[j];
				pots[j] = 0;
			}
			if (all_pot == 0)
				break;
		}
	}
	else			// nobody all-in, so  <- maybe is okay, i think so!, do not try now!
	{
		int repeat = 1, rpt[N] = {};
		rpt[0] = smiler;
		for (int k = 0; k != num_of_play; k++)
		{
			if (k == smiler)
				continue;
			else if (play[k].rank == play[smiler].rank)
			{
				rpt[repeat] = k;
				repeat++;
				cout << "HERE\n";
			}
		}
		for (int i = 0; i != repeat; i++)
		{
			//cout << "HERE " << all_pot << endl;
			play[rpt[i]].money += (all_pot / repeat);
			all_pot -= (all_pot / repeat);
		}
		residue[0] += all_pot;
		all_pot = 0;
	}			// I think it's ok! right? Just try!!
}

void pot::residue_distributing (Table& table, player play[])		// no problem i think!!
{
	int distri_pos;
	distri_pos = table.dealer;
	for (int i = 0; i != N; i++)
	{
		play[distri_pos].money += residue[i];
		residue[i] = 0;
		table.next_people (distri_pos);
	}
	for (int i = 0; i != N; i++)
	{
		pots[i] = bet_each_man[i] = residue[i] = 0;
		player_no[i] = -1;
	}
	all_pot = 0;
}
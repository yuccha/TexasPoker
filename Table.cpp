// Table.cpp
//

#include "stdafx.h"
#include <iostream>
#include "Table.h"

using namespace std;

Table::Table (int delr, int num_of_play)			// NOTICE THAT THE DELR SHOULD -- BEFORE GIVING TO ME
	:dealer (delr), n (num_of_play)
{
	for (int i = 0; i != 2; i++)
		for (int j = 0; j != 5; j++)
			card_desk[i][j] = 0;
	play_now = n;
	for (int i = 0; i != N; i++)
	{
		if (i < n)
			play[i] = 2;
		else
			play[i] = 0;
	}
	play[dealer] = 1;
}

void Table::player_out (int ix)     // ix < 9;
{
	if (ix ==dealer)
		next_people (dealer);
	play[dealer] = 1;
	play[ix] = 0;
}

void Table::player_fold (int ix)
{
	if (ix == dealer)
		next_people (dealer);
	play[dealer] = 1;
	play[ix] = -1;
}

void Table::next_people (int& ix)
{
	ix++;
	ix %= N;
	while ((play[ix] == 0)||(play[ix] == -1))
	{
		ix++;
		ix %= N;
	}
}

void Table::initialize_pturn (void)
{
	for (int ix = 0; ix != n; ix++)
	{
		if (play[ix] != 0)
			play[ix] = 2;
	}
	next_people(dealer);
	play[dealer] = 1;
}

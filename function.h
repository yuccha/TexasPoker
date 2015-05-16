// function.h


#ifndef FUNCTION_H
#define FUNCTION_H
using namespace std;
int determin_which (player&, int);

void card_delivering (player* play, Table& table, int num_of_player)
{
	int pn = 1, color, number, check = 0, numf = 0;
	numf = 5 + 2*num_of_player;
	int **p = new int* [2];
	p[0] = new int [numf];
	p[1] = new int [numf];
	while(pn != numf+1)
	{
		color = rand()%52/13 + 1;
		number = rand()%52%13 + 1;
		if(pn == 1)
		{
			p[0][pn-1] = color;
			p[1][pn-1] = number;
		}
		else
		{
			for(int num = 0 ; num < pn-1 ; num++)
			{
				if((color == p[0][num]) && (number == p[1][num]))
				{
					check = 0;
					break;
				}
				else check = 1;
			}
			if(check == 1)
			{
				p[0][pn-1] = color;
				p[1][pn-1] = number;
			}
			if(pn > 1 && check == 0)
				pn--;
		}
		if(pn == 1 || check == 1)
			pn++;
		check = 0;
	}
	/*cout << "ALL CARDS!\n";
	for (int j = 0; j != numf; j++)
	{
			cout  << "{" << p[0][j] << "," << p[1][j] << "} ";
			if ((j % 7) == 6)
				cout << endl;
	}
	cout << endl;*/
	for (int i = 0; i != num_of_player; i++)
	{
		for (int j = 0; j != 5; j++)
		{
			play[i].total_card[0][2+j] = p[0][j];
			play[i].total_card[1][2+j] = p[1][j];
		}
		for (int j = 0; j != 2; j++)
		{
			play[i].card_hand[0][j] = play[i].total_card[0][j] = p[0][5+2*i+j];
			play[i].card_hand[1][j] = play[i].total_card[1][j] = p[1][5+2*i+j];
		}
	}
	for (int i = 0; i != 5; i++)
	{
		table.card_desk[0][i] = p[0][i];
		table.card_desk[1][i] = p[1][i];
	}
}

int who_wins (player* p, int num_of_player)
{
	int count = 0;
	int *same = new int [num_of_player];
	int *rank = new int [num_of_player];
	//int same[5] = {-1,-1,-1,-1,-1}; // means no same rank
	//int rank[5] = {0,1,2,3,4}; // Player ID 0~4
	for (int i = 0; i != num_of_player; i++)
	{
		same[i] = -1;
		rank[i] = i;
	}
	for (int i = 0 ; i != num_of_player ; i++)
	{
		for (int j = 0 ; j != num_of_player-1-i ; j++)
		{
			if (p[rank[j]].max_type < p[rank[j+1]].max_type)		// type 越大，rank 越低  <-rank前面代表較小？？
				rank[j+1] ^= rank[j] ^= rank[j+1] ^= rank[j];
		}
	}
	/*//
	cout << endl;
	for (int ix = 0; ix != num_of_player; ix++)
		cout << rank[ix] << " ";
	cout << endl;
	//*/
	for (int j = 0; j != num_of_player; j++)
	{
		for (int m = 0; m != num_of_player-1-j; m++)
		{
			if (p[rank[m]].max_type == p[rank[m+1]].max_type)		// use max card array check
			{
				int count = 0;
				for (int k = 0; k != 5; k++)
				{
					if (p[rank[m]].max_card[1][k] > p[rank[m+1]].max_card[1][k])
					{
						rank[m+1] ^= rank[m] ^= rank[m+1] ^= rank[m];
						break;
					}
					else if ((p[rank[m]].max_card[1][k] < p[rank[m+1]].max_card[1][k]))
						break;
					else
						continue;
				}
			}
			/*//
			cout << endl;
			for (int ix = 0; ix != num_of_player; ix++)
				cout << rank[ix] << " ";
			cout << endl;
			//*/
		}
		for (int kt = 0; kt != 5; kt++)
		{
			if (p[rank[j]].max_card[1][kt] == 0)
				break;
			else if ((j != num_of_player-1) && (p[rank[j]].max_card[1][kt] == p[rank[j+1]].max_card[1][kt]))
				count++;
		}
		if (count == 5)
			same[rank[j+1]] = rank[j]; // LABEL SAME
		count = 0;
	}
	/*//
	//system ("PAUSE");
	cout << endl;
	for (int i = 0; i != num_of_player; i++)
		cout << same[i] << " ";
	cout << endl;
	//*/
	/*//
	system ("PAUSE");
	cout << endl;
	for (int ix = 0; ix != num_of_player; ix++)
		cout << rank[ix] << " ";
	cout << endl;
	//*/

	//system ("PAUSE");
	// 改這個array吧！
	for (int i = 0; i != num_of_player; i++)
	{
		//cout << "HAHAH\n";
		if (same[rank[i]] == -1)
			p[rank[i]].rank = i;
		else
			p[rank[i]].rank = p[same[rank[i]]].rank;
	}
	//smiler = rank[num_of_player-1];
	int tga = 0;
	for (int i = 0; i != num_of_player; i++)
	{
		if (p[i].max_type == 9)
			tga = 1;
		else if (p[i].max_type == 10)
		{
			tga = 2;
			break;
		}
	}
	for (int i = 0; i != num_of_player; i++)
		p[i].rank -= tga;
	return rank[num_of_player-1];
}

void card_dilivering_sp (player *play, int num_of_player, int to_how)
{
	int pn = 1, color, number, numf = 0, crd;
	int card[52] = {};
	for (int i = 0; i != to_how; i++)
		card[((play[0].total_card[0][i]-1)*13) + (play[0].total_card[1][i]-1)] = -1;
	numf = 5-to_how + 2*(num_of_player-1);
	int **p = new int* [2];
	p[0] = new int [numf];
	p[1] = new int [numf];
	while (pn != numf+1)
	{
		crd = rand()%52;
		color = crd/13 + 1;
		number = crd%13 + 1;
		if (card[crd] != -1)
		{
			card[crd] = -1;
			p[0][pn-1] = color;
			p[1][pn-1] = number;
			pn++;
		}
		else
			continue;
	}
	for (int i = 0; i != num_of_player; i++)
	{
		for (int j = to_how+2, k = 0; j != 7; j++, k++)
		{
			play[i].total_card[0][j] = p[0][k];
			play[i].total_card[1][j] = p[1][k];
		}
		for (int j = 0; j != 2; j++)
		{
			if (i == 0)
				break;
			play[i].total_card[0][j] = p[0][5-to_how+2*(i-1)+j];
			play[i].total_card[1][j] = p[1][5-to_how+2*(i-1)+j];
		}
		
	}
}

double count_hand_strength (player play, int num_of_player, int to_how)
{
	
	double hs = 0;
	int repeated = 1, smiler, looping_num = 3000;
	player* playerpp = new player [num_of_player];
	for (int li = 0; li != looping_num; li++)
	{	
		
		playerpp[0] = play;
		for (int i = 1; i != num_of_player; i++)
		{
			for (int j = 2; j != to_how+2; j++)
			{
				playerpp[i].total_card[0][j] = playerpp[0].total_card[0][j];
				playerpp[i].total_card[1][j] = playerpp[0].total_card[1][j];
			}
		}
		
		card_dilivering_sp (playerpp, num_of_player, to_how);
		for (int j = 0; j != num_of_player; j++)
			playerpp[j].maximum_card();
		smiler = who_wins (playerpp, num_of_player);
		//cout << smiler << endl;
		/*if (i == 20)
		{
			cout << hs << endl;
			system ("PAUSE");
		}*/
		//cout << smiler << endl;
		if (playerpp[smiler].rank == playerpp[0].rank)
		{
			for (int k = 0; k != num_of_player; k++)
			{
				if (k == smiler)
					continue;
				else if (playerpp[k].rank == playerpp[0].rank)
				{
					repeated++;
					//cout << "HERE\n";
				}
			}
			
			hs += 1.0 / repeated;
		}
		//for (int m = 0; m != num_of_player; m++)
			//cout /*<< "RANK:" << playerpp[m].rank */<< "TYPE:" << playerpp[m].max_type << endl;
		//cout << repeated << endl << endl;;
		repeated = 1;
		//cout << "ALL RIGHT!!" << endl;
	}
	
	hs = hs / looping_num;
	return hs;
}

void drawing (player *play, Table &table, pot &jackpot, int to_how, int your_id, int playing_first, int true_dealer, int now_playing, bool during, bool if_show_maxcard)
{
	system ("cls");
	cout << setw (60) << "pots:" << setw(5) << jackpot.all_pot << endl;
	if (!player::if_bet)
		cout << setw (60) << "Minimum bet:" << setw (5) << player::sb_bet << endl;
	else
		cout << setw (60) << "Minimum rise:" << setw (5) << player::last_one_bet << endl;
	cout << endl;
	cout << setw(30) << "";
	for (int i = 0; i != 5; i++)
	{
		if (i <to_how)
		{
			
			switch (table.card_desk[0][i])
			{
			case 1:
				cout << "S";
				break;
			case 2:
				cout << "H";
				break;
			case 3:
				cout << "D";
				break;
			case 4:
				cout << "C";
				break;
			}
			switch (table.card_desk[1][i])
			{
			case 13:
				cout << "A";
				break;
			case 12:
				cout << "K";
				break;
			case 11:
				cout << "Q";
				break;
			case 10:
				cout << "J";
				break;
			case 9:
				cout << "T";
				break;
			default:
				cout << table.card_desk[1][i]+1;
				break;
			}
			if (i < 4)
				cout << ",";
		}
		else
		{
			cout << "??";
			if (i < 4)
				cout << ",";
		}
	}
	cout << endl << endl;
	cout << setw (10) << "";
	for (int i = 0; i != playing_first; i++)
		cout << setw (10) << right << "Player" << i+1;
	cout << endl;
	cout << setw (10) << right <<"Card";
	int to_show[N] = {0};
	for (int i = 0; i != playing_first; i++)
		if ((i == your_id) || ((play[i].if_all_in == TRUE) && during == FALSE) || ((if_show_maxcard == TRUE) && (play[i].if_fold == FALSE) && (table.play[i] != 0)))
			to_show[i]++;
	for (int i = 0; i != playing_first; i++)
	{
		if (to_show[i] == 0)	// no show!
			cout << setw(11) << right << "??,??";
		else					// show
		{
			cout << setw(6) << right << "";
			for (int j = 0; j != 2; j++)
			{
				switch (play[i].card_hand[0][j])
				{
				case 1:
					cout << "S";
					break;
				case 2:
					cout << "H";
					break;
				case 3:
					cout << "D";
					break;
				case 4:
					cout << "C";
					break;
				}
				switch (play[i].card_hand[1][j])
				{
				case 13:
					cout << "A";
					break;
				case 12:
					cout << "K";
					break;
				case 11:
					cout << "Q";
					break;
				case 10:
					cout << "J";
					break;
				case 9:
					cout << "T";
					break;
				default:
					cout << play[i].card_hand[1][j]+1;
					break;
				}
				if (j < 1)
					cout << ",";
			}
		}
	}
	cout << endl;
	cout << setw (10) << "money";
	cout << setw(5) << left << "";
	for (int i = 0; i != playing_first; i++)
		cout << "$" << left << setw (10) << play[i].money;
	cout << endl;
	cout << right;
	cout << setw (10) << "bet";
	cout << setw(5) << left << "";
	for (int i = 0; i != playing_first; i++)
		cout << "$" << left << setw (10) << play[i].bet;
	cout << endl;
	cout << right;
	cout << setw (10) << "State";
	for (int i = 0; i != N; i++)
		to_show[i] = 0;
	for (int i = 0; i != 3; i++)
	{
		to_show[true_dealer] += (i+1);
		table.next_people (true_dealer);
	}
	for (int i = 0; i != playing_first; i++)
	{
		switch (to_show[i])
		{
		case 0:
			cout << setw(11) << "";
			break;
		case 1:
			cout << setw(11) << "Dealer";
			break;
		case 2:
			cout << setw(11) << "S.Blind";
			break;
		case 3:
			cout << setw(11) << "B.Blind";
			break;
		default:
			break;
		}		
	}
	cout << endl;
	cout << setw (10) << "Current";
	for (int i = 0; i != N; i++)
		to_show[i] = 0;
	for (int i = 0; i != playing_first; i++)		// all-in = 1, fold = 2, out = 3;
	{
		if (play[i].if_all_in == TRUE)
			to_show[i] = 1;
		else if (play[i].if_fold == TRUE)
			to_show[i] = 2;
		else if (table.play[i] == 0)
			to_show[i] = 3;
	}
	for (int i = 0; i != playing_first; i++)
	{
		/*if (play[i].if_all_in == TRUE)
			cout << setw (11) << "All-In";
		else if (play[i].if_fold == TRUE)
			cout << setw (11) << "Fold";
		else if (table.play[i] == 0)
			cout << setw (11) << "$ = 0";*/
		switch (to_show[i])
		{
		case 1:
			cout << setw (11) << "All-In";
			break;
		case 2:
			cout << setw (11) << "Fold";
			break;
		case 3:
			cout << setw (11) << "$ = 0";
			break;
		case 0:
			cout << setw (11) << "";
			break;
		}
	}
	cout << endl;
	cout << setw (10) << "card type";
	if (if_show_maxcard == TRUE)
	{
		for (int i = 0; i != playing_first; i++)
		{
			if ((to_show[i] == 0) || (to_show[i] == 1))
			{
				switch (play[i].max_type)
				{
				case 0:
					cout << setw(11) << "S.Flush";
					break;
				case 1:
					cout << setw(11) << "4 Kind";
					break;
				case 2:
					cout << setw(11) << "F.House";
					break;
				case 3:
					cout << setw(11) <<  "Flush";
					break;
				case 4:
					cout << setw(11) << "Staight";
					break;
				case 5:
					cout << setw(11) << "3 Kind";
					break;
				case 6:
					cout << setw(11) << "2 Pairs";
					break;
				case 7:
					cout << setw(11) << "A Pair";
					break;
				case 8:
					cout << setw(11) << "High Card";
					break;
				}
			}
			else
				cout << setw (11) << "";
		}
		cout << endl;
	}
	else
		cout << endl;
	cout << setw (11*(2+now_playing)-2) << "*NOW*";
	cout << endl;
	cout << setw (11*(2+your_id)-2) << "*YOU*";
	cout << endl;
	cout << "==============================================================================\n";
	/*for (int i = 0; i != playing_first; i++)
		cout << to_show[i] << " ";
	cout << endl;//*/
}

void computer_strategy (player &play, int num_exclude_f_o, int to_how, int pot_current)
{
	double hand_strength;
	int decide_tag, act;
	if (to_how == 0)		// preflop region!
	{
		int p[2] = {};
		for (int i = 0; i != 2; i++)
			p[i] = play.card_hand[1][i];
		if (p[0] > p[1])
			p[0] ^= p[1] ^= p[0] ^= p[1];
		if (p[0] == p[1])
		{
			decide_tag = 2;
			if (p[0] > 9)
				decide_tag = 3;
			if (p[0] >= 12)
				decide_tag = 4;
		}
		else
		{
			if (p[0] < 6)
			{
				if (p[1] < 11)
					decide_tag = 1;
				else
					decide_tag = 2;
			}
			else
				decide_tag = 2;
		}
		act = determin_which (play, decide_tag);
	}
	else if (to_how == 5)	// river region!!		to_how == 5? maybe have to change! yap, its right!!
	{
		int money_larger;
		hand_strength = count_hand_strength (play, num_exclude_f_o, to_how);
		if (hand_strength > 0.41)
		{
			money_larger = (int)(1.0*pot_current / (2.0/hand_strength - 1));
			//srand(time(0));
			int rdm = rand();
			decide_tag = 7;			// you must add this if(statement), if(==7) then stop calling that func.!
			if (!player::if_bet)		// check, bet region! You wanna go as far as you wanna
			{
				if (money_larger > player::sb_bet)
				{
					if ((rdm%100) > 80)		
						play.acting(6);		// go to all-in!!
					else
						play.acting (5);	// fold!!
				}
				else
				{
					//decide_tag = 7;
					play.computer_bet_rise (1, money_larger);
				}
			}
			else
			{
				if (money_larger > player::last_one_bet)
				{
					//decide_tag = 7;
					if (play.money > player::last_one_bet)	// can call!
						play.acting (3);
					else
					{
						if ((rdm%100) > 80)		
							play.acting(6);		// go to all-in!!
						else
							play.acting (5);	// fold!!
					}
				}
				else
				{
					//decide_tag = 7;
					money_larger -= player::last_one_bet;
					play.computer_bet_rise (2, money_larger);
				}
			}
		}
		else if (hand_strength > 0.05)
			decide_tag = 3;
		else
			decide_tag = 0;
		if (decide_tag != 7)
			act = determin_which (play, decide_tag);
	}
	else					// flop & turn region!!
	{
		hand_strength = count_hand_strength (play, num_exclude_f_o, to_how);
		if (to_how == 3)
		{
			if (hand_strength < 0.031)
				decide_tag = 0;
			else if (hand_strength < 0.551)
				decide_tag = 1;
			else if (hand_strength < 0.08)
				decide_tag = 2;
			else if (hand_strength < 0.1)
				decide_tag = 3;
			else
				decide_tag = 4;
		}
		else
		{
			if (hand_strength < 0.061)
				decide_tag = 0;
			else if (hand_strength < 0.08)
				decide_tag = 1;
			else if (hand_strength < 0.1)
				decide_tag = 2;
			else if (hand_strength < 0.2)
				decide_tag = 3;
			else
				decide_tag = 4;
		}
		act = determin_which (play, decide_tag);
	}
}

int determin_which (player &play, int region_where)			// bet_money = 1 -> rise 1; bet_money = 2 -> rise 2; =3 -> rise 3, = 4 -> determined by yoursef
{															// bet_money enter with 0! so that we would not confuse!
	int decision = 0, random_num, money_tobet = 0, betting_money = 0;
	//srand(time(0));
	random_num = rand();
	
	switch (region_where)
	{
	case 0:
		if ((random_num % 100)>66)	// 34%fold, 66%call
			decision = 5;		
		else
			decision = 3;
		break;
		break;
	case 1:
		if ((random_num % 100)>75)	// 34%fold, 66%call
			decision = 5;		
		else
			decision = 3;
		break;
	case 2:
		if ((random_num %100) > 90)	
		{
			decision = 2;
			betting_money = 1;						
		}
		else if ((random_num %100) > 76)	// 15%fold
			decision = 5;
		else if ((random_num %100) > 73)	// 3%rise2
		{
			decision = 2;
			betting_money = 2;
		}
		else						// 55%call
			decision = 3;
		break;
	case 3:
		if ((random_num %100) > 96)	// 4%rise2
		{
			decision = 2;
			betting_money = 2;
		}
		else if ((random_num %100) > 90)
			decision = 5;
		else if ((random_num %100) > 58)
		{
			decision = 2;
			betting_money = 1;
		}
		else
			decision = 3;
		break;
	case 4:
		if ((random_num %100) > 98)
		{
			decision = 2;
			betting_money = 3;
		}
		else if ((random_num %100) > 92)
		{
			decision = 2;
			betting_money = 2;
		}
		else if ((random_num %100) > 63)
		{
			decision = 2;
			betting_money = 1;
		}
		else if ((random_num %100) > 7)
			decision = 3;
		else
			decision = 5;
		break;
	case 5:
		if ((random_num %100) > 85)
			decision = 5;
		else
			decision = 3;
		break;
	case 6:
		decision = 2;
		betting_money = 4;
		break;
	default:
		break;
	}
	
	if (player::if_bet == FALSE)
	{
		if (decision == 5)		// can check, so fold -> check!
		{
			decision = 4;
			play.acting(decision);
		}
		else if ((decision == 2) || (decision == 3))	// ask you to rise || call, -> bet!!
		{
			money_tobet = (1+betting_money)*player::sb_bet;
			if (play.money < money_tobet)
			{
				if (betting_money == 3)
				{
					decision = 6;
					play.acting(decision);
				}
				else
				{
					decision = 4;
					play.acting(decision);
				}
			}
			else
			{
				decision = 1;
				play.computer_bet_rise (decision, money_tobet);
			}
		}
	}
	else				// enable to rise, call, fold...
	{
		
		switch (decision)
		{
		case 2:			// rise
			money_tobet = betting_money * player::last_one_bet;
			if (play.money > (money_tobet + player::last_one_bet))
				play.computer_bet_rise (decision, money_tobet);
			else if (betting_money == 3)
			{
				decision = 6;
				play.acting (decision);	// allin
			}
			else
			{
				decision = 5;
				play.acting (decision);	// fold!!
			}
			break;
		case 3:	//call
			if (play.money > player::last_one_bet)
				play.acting (decision);
			else
			{
				decision = 5;
				play.acting (decision);	// fold
			}
			break;
		case 5:	// fold it
			play.acting (decision);
			break;
		case 6:
			break;
		}
	}
	return decision;
}

#endif


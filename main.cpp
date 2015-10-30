//
//  main.cpp
//  Texas Hold'em
//
//  Created by 陳昱嘉 on 2015/5/16.
//  Copyright (c) 2015年 yuchia. All rights reserved.
//

#include <stdio.h>
#include <curses.h>
#include <iostream>
#include <iomanip>
#include <time.h>
#include "player.h"
#include "Table.h"
#include "pot.h"
#include "function.h"


using namespace std;

int player::last_one_bet = 0;
bool player::if_bet = FALSE;
int player::sb_bet;
int player::last_rise = 10;
int player::timer_serve = 0;

int main(int argc, const char * argv[]) {
    // insert code here...
    //std::cout << "Hello, World!\n";
    srand(time(0));
    int num, mny, id, now_playing, sb, wanna_play;
    int remain_people_o, remain_people_f, true_dealer;
    cout << "There are 5 people, each one have $10000 on the board!"
    "\nAnd the big blind have to bet $20!\n"
    "And we want to play 15 turns\n";
    num = 5;
    mny = 10000;
    sb = 10;
    wanna_play = 15;
    player::sb_bet = sb;
    player *play_p = new player [num];
    for (int i = 0; i != num; i++)
    {
        play_p[i].money = mny;
        play_p[i].id = i+1;
    }
    //cout << "You are at position 3, where the dealer is 1!\n";
flag1:
    cout << "Where do you want to seat?(1~5)\nYou:";
    cin >> id;
    if ((id > 5)||(id < 1))
    {
        cout << "WRONG CASE! ENTER AGAIN!\n";
        system ("PAUSE");
        cout << "\r";
        goto flag1;
    }
    id--;
    int *dealer = new int;
flag2:
    cout << "Wher is the dealer?(1~5)\nDealer:";
    cin >> *dealer;
    if ((*dealer > 5)||(*dealer < 1))
    {
        cout << "WRONG CASE! ENTER AGAIN!\n";
        system ("PAUSE");
        cout << "\r";
        goto flag2;
    }
    (*dealer)--;
    Table table (*dealer, num);
    true_dealer = *dealer;
    delete dealer;
    remain_people_o = remain_people_f = num;
    // end initializing table & player, but haven't yet dilivering the card!!
    pot jackpot;
    // end initializing jackpot!!
    system ("PAUSE");
    system ("CLS");
    for (int game_range = 0; game_range != wanna_play; game_range++)
    {
        cout << "Round " << game_range << endl;
        card_delivering (play_p, table, num);
        // Small Blind & Big Blind start BETTING!!
        now_playing = table.dealer;
        table.next_people (now_playing);
        play_p[now_playing].bet = player::sb_bet;			//SB BET
        //cout << "Small Blind bet " << player::sb_bet << endl;
        player::if_bet = TRUE;
        table.next_people (now_playing);
        player::sb_bet *= 2;
        play_p[now_playing].bet = player::sb_bet;		// BB bet
        //cout << "Big Blind bet " << player::sb_bet << endl;
        table.next_people (now_playing);
        player::last_one_bet = player::sb_bet;
        //cout << "The dealer is at position " << table.dealer+1 << endl << endl;
        // SB, BB end beting!!
        //system ("PAUSE");
        int smiler;
        bool if_show_maxcard_type = FALSE;
        for (int region = 0, ix; region != 4; region++)			//region == 0-> preflop, 1-> flop, 2-> turn, 3-> river!
        {
            bool during = FALSE;
            ix = -1;
            int move_tag = 0;
            //cout << "123";
            /*for (int i = 0; i != num; i++)
             cout << play_p[i].if_fold << " ";	// if_fold have gone!!
             cout << endl;
             //system ("PAUSE");*/
            /*if (region == 0)
             drawing (play_p, table, jackpot, 0, id, num, true_dealer, now_playing, during, if_show_maxcard_type);
             else
             drawing (play_p, table, jackpot, 2+region, id, num, true_dealer, now_playing, during, if_show_maxcard_type);
             //cout << "The dealer is at position " << table.dealer+1 << endl << endl;
             switch (region)				// Show CARD_DESK!!
             {
             case 1:
             cout << "Show flops!\n";
             table.show_until (3);
             break;
             case 2:
             cout << "Show turn!\n";
             table.show_until (4);
             break;
             case 3:
             cout << "Show river!\n";
             table.show_until (5);
             break;
             default:
             break;
             }*/
            
            
            for (int i = 0; now_playing != player::last_rise;  i++)
            {
                during = TRUE;
                if (i == 0)
                    player::last_rise = now_playing;
                if (ix != -1)
                {
                    player::last_rise = ix;
                    ix = -1;
                }
                if (!play_p[now_playing].if_all_in)
                {
                    bool can_act[6] = {FALSE};
                    int sizof_canact = 0;
                    if (player::if_bet == FALSE)
                    {
                        if(play_p[now_playing].money > player::sb_bet)
                        {
                            can_act[0] = TRUE;			// bet
                            sizof_canact++;
                        }
                        can_act[3] = TRUE;				// check
                        sizof_canact++;
                    }
                    else if (play_p[now_playing].money > player::last_one_bet)
                    {
                        if (play_p[now_playing].money > (player::last_one_bet + player::sb_bet))
                        {
                            can_act[1] = TRUE;			// rise
                            sizof_canact++;
                        }
                        can_act[2] = TRUE;				// call
                        sizof_canact++;
                    }
                    can_act[4] = can_act[5] = TRUE;
                    sizof_canact += 2;;
                    if (region == 0)
                        drawing (play_p, table, jackpot, 0, id, num, true_dealer, now_playing, during, if_show_maxcard_type);
                    else
                        drawing (play_p, table, jackpot, 2+region, id, num, true_dealer, now_playing, during, if_show_maxcard_type);
                    if (now_playing == id)				// player's turn!!
                    {
                        cout << endl << setw(10) << "";
                        for (int ixx = 0; ixx != 6; ixx++)
                        {
                            if (can_act[ixx])
                            {
                                switch (ixx)
                                {
                                    case 0:
                                        cout << setw(10) << "BET";
                                        break;
                                    case 1:
                                        cout << setw(10) << "RISE";
                                        break;
                                    case 2:
                                        cout << setw(10) << "CALL";
                                        break;
                                    case 3:
                                        cout << setw(10) << "CHECK";
                                        break;
                                    case 4:
                                        cout << setw(10) << "FOLD";
                                        break;
                                    case 5:
                                        cout << setw(10) << "ALL-IN";
                                        break;
                                    default:
                                        break;
                                }
                            }
                        }
                        cout << endl;
                        while (1)
                        {
                            //if (kbhit())
                            if (getch())
                            {
                                cout << "\r";
                                int enter_tag = 0;
                                
                                cout << setw(10) << "";
                                switch (getch())
                                {
                                    case 13:
                                        enter_tag = 1;
                                        cout << "YOU ENTERS " << move_tag+1 << endl;
                                        break;
                                    case 100:
                                        move_tag++;
                                        if (move_tag >= sizof_canact)
                                            move_tag = sizof_canact-1;
                                        cout << setw (10*move_tag + 10) << "#" << setw(50) << " ";
                                        break;
                                    case 97:
                                        move_tag--;
                                        if (move_tag < 0)
                                            move_tag = 0;
                                        cout << setw (10*move_tag + 10) << "#" << setw(50) << " ";
                                        break;
                                    default:
                                        break;
                                }
                                if (enter_tag == 1)
                                {
                                    cout << "\nConfirm?\nPress Enter to Confirm\nPress Others to re-enter!\n";
                                    int j = getch();
                                    if (j == 13)
                                        break;
                                    else
                                    {
                                    }
                                }
                            }
                        }
                        int to_act = 0;
                        /*for (int ixx = 0; ixx != 6; ixx++)
                         cout << can_act[ixx] << " ";
                         cout << endl;
                         system ("PAUSE");//*/
                        //int a = 0;
                        for (int actually_act = -1; to_act != 6; to_act++)
                        {
                            if (can_act[to_act] == TRUE)
                                actually_act++;
                            if (actually_act == move_tag)
                                break;
                        }
                        play_p[now_playing].acting(to_act+1);		// move by the player!!
                    }
                    else						// move by computer!!
                    {
                        int sum_of_pots = 0, where_to_go;
                        for (int index = 0, k = table.dealer; index != remain_people_f; index++)
                        {
                            sum_of_pots += play_p[k].bet;
                            table.next_people (k);
                        }
                        sum_of_pots += jackpot.all_pot;
                        if (region == 0)
                            where_to_go = 0;
                        else
                            where_to_go = 2+region;
                        // end initilizing sum_of_pots;
                        computer_strategy (play_p[now_playing], remain_people_f, where_to_go, sum_of_pots);
                    }
                }
                
                if (play_p[now_playing].if_fold)
                {
                    remain_people_f--;
                    table.player_fold (now_playing);
                    if (i == 0)
                    {
                        ix = now_playing;
                        table.next_people (ix);
                    }
                }
                if (remain_people_f == 1)
                    break;	//maybe is, I dont know!!
                table.next_people(now_playing);
                if (region == 0)
                    drawing (play_p, table, jackpot, 0, id, num, true_dealer, now_playing, during, if_show_maxcard_type);
                else
                    drawing (play_p, table, jackpot, 2+region, id, num, true_dealer, now_playing, during, if_show_maxcard_type);
            } // end acting of ourselves!
            
            during = FALSE;
            for (int i = 0; i != num; i++)
            {
                play_p[i].money -= play_p[i].bet;
                jackpot.betting (i, play_p[i].bet);
            }
            if (region == 0)
                drawing (play_p, table, jackpot, 0, id, num, true_dealer, now_playing, during, if_show_maxcard_type);
            else
                drawing (play_p, table, jackpot, 2+region, id, num, true_dealer, now_playing, during, if_show_maxcard_type);
            if (remain_people_f == 1)
                break;
            for (int i = 0; i != num; i++)
                play_p[i].initialize_pturn();			// here!!
            player::initlz_pturn_sta(2*sb);
            now_playing = table.dealer;
            table.next_people (now_playing);
            switch (region)
            {
                case 0:
                    cout << "End Preflop!\n";
                    break;
                case 1:
                    cout << "End Flop!\n";
                    break;
                case 2:
                    cout << "End Turn!\n";
                    break;
                case 3:
                    cout << "End River!\n";
                    break;
            }
            system ("PAUSE");
        }
        
        // Now starting looking for winner and distributing the pots!
        if_show_maxcard_type = TRUE;
        for (int i = 0, k = table.dealer; i != remain_people_f; i++)
        {
            play_p[k].maximum_card();
            play_p[k].show_type();
            table.next_people (k);
        }
        drawing (play_p, table, jackpot, 5, id, num, true_dealer, now_playing, FALSE, if_show_maxcard_type);
        smiler = who_wins (play_p, num);				// entering all player number!!
        int rpt[N] = {}, repeat = 1;
        rpt[0] = smiler;
        for (int k = 0; k != num; k++)
        {
            if (k == smiler)
                continue;
            else if (play_p[k].rank == play_p[smiler].rank)
            {
                rpt[repeat] = k;
                repeat++;
                //cout << "HERE\n";
            }
        }
        cout << "\nWINER IS\n";
        for (int i = 0; i != repeat; i++)
        {
            cout << "Player " << rpt[i]+1 << ", having the card:\n";
            for (int j = 0; j != 5; j++)
            {
                //cout << "{" << play_p[rpt[i]].max_card[0][j] << "," << play_p[rpt[i]].max_card[1][j] << "} ";
                switch (play_p[rpt[i]].max_card[0][j])
                {
                    case 1:
                        cout << setw(7) <<"Spade";
                        break;
                    case 2:
                        cout << setw(7) << "Heart";
                        break;
                    case 3:
                        cout << setw(7) << "Dimond";
                        break;
                    case 4:
                        cout << setw(7) << "Club";
                        break;;
                }
                switch (play_p[rpt[i]].max_card[1][j])
                {
                    case 13:
                        cout << " A";
                        break;
                    case 12:
                        cout << " K";
                        break;
                    case 11:
                        cout << " Q";
                        break;
                    case 10:
                        cout << " J";
                        break;
                    case 9:
                        cout << " T";
                        break;
                    default:
                        cout << " " << play_p[rpt[i]].max_card[1][j]+1;
                        break;
                }
                cout << endl;
            }
            cout << "With type:";
            play_p[rpt[i]].show_type();
        }
        cout << "==================================\n";
        // start pot_distributing!!
        jackpot.pot_distributing (play_p, num, smiler);
        // Table having initializing!!
        table.dealer = true_dealer;
        table.initialize_pturn();
        true_dealer = table.dealer;
        jackpot.residue_distributing (table, play_p);
        // Pot having initializing!!
        cout << "MONEY SHOW\n";
        // player_out_question!
        for (int i = 0, rp = remain_people_o, k =table.dealer; i != rp; i++)
        {
            cout << "player " << k+1 << ":" << play_p[k].money << endl;
            if (play_p[k].money == 0)
            {
                remain_people_o--;
                play_p[k].if_all_in = FALSE;
                cout << "Player " << k+1 << " go bankruptcy!\n";
                play_p[k].max_type = 10;
                table.player_out (k);
            }
            table.next_people (k);
        }
        player::initlz_pturn_sta(sb);
        for (int i = 0, k = table.dealer; i != remain_people_o; i++)
        {
            play_p[k].initialize_pturn();
            play_p[k].max_type = 9;
            play_p[k].if_all_in = FALSE;
            play_p[k].if_fold = FALSE;
            table.next_people (k);
        }
        remain_people_f = remain_people_o;
        if (remain_people_o == 1)
            break;
        system ("PAUSE");
    }
    if (remain_people_o == 1)
    {
        int index;
        int survivor;
        cout << "Show money!\n";
        for (index = 0; index != num; index++)
        {
            cout << play_p[index].money << endl;
            if (play_p[index].money != 0)
                survivor = index;
        }
        cout << "Player " << index << " survives, and win all the money back!\n";
    }
    cout << "GAME IS OVER!\n";
    system ("PAUSE");
    return 0;
}

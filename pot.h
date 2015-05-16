// pot.h
//

#ifndef POT_H
#define POT_H

class pot
{
public:
	// DATA MEMBER:
	int pots[N];
	int bet_each_man[N];
	int player_no[N];
	int residue[N];
	int all_pot;
	// MEMBER FUNCTION:
	pot (void);
	void pot_distributing (player*, int, int);		// enter play, rather play_p_t (see CLASS_ALL_last_try)
	void residue_distributing (Table&, player[]);	// First initialize_p_turn, then give the residue to the dealer next!! 
													// So didnt have to initializ_p_turn outside!!
	void betting (int, int);
private:
};


#endif
#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

#ifndef	N
#define	N	9
#endif

#ifndef PLAYER_H
#define PLAYER_H

class player
{
public:
	// DATA MEMBER:
	int id;
	int money;
	int bet;
	int if_flush;			// == 0, no flush; == i, flush with color "i"
	bool if_all_in;
	bool if_fold;
	int card_hand[2][2];
	int total_card[2][7];
	int max_card[2][5];
	int max_type;
	int rank;
	int timer_hold;
	// STATIC:
	static int last_one_bet;		// last people's betting num
    static bool if_bet;				
    static int sb_bet;				// small blind's bet! -> the least betting num!
	static int last_rise;
	static int timer_serve;
	// MEMBER FUNCTION:
	player(void);
	void show_type(void);
	void acting(int);
	void flush (int [], int[], int[]);
	int straight (int []);
	void other_type (int [], int []);
	int maximum_card(void);
	void get_and_sort (void);
	void initialize_pturn (void);
	void computer_bet_rise (int, int);
	static void initlz_pturn_sta (int);
private:
};



#endif
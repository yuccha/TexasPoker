// Table.h
//

#ifndef N
#define N	9
#endif
#ifndef TABLE_H
#define TABLE_H

class Table
{
public:
	// DATA MEMBER:
	int n;	
	int dealer;
	int card_desk[2][5];	
	int play[9];
	int play_now;
	// MEMBER FUNCTION:
	Table (int, int);
	void player_out (int);   // 這和下面那個一樣，都用main去判斷！不要再func裡面寫判斷！
	void player_fold (int);
	void next_people (int&);
	void initialize_pturn (void);
private:
};

#endif

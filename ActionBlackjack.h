
#ifndef ACTION_BLACKJACK_H_
#define ACTION_BLACKJACK_H_

#include "BotAction.h"

class Deck
{
public:
	static enum { HEARTS, DIAMONDS, CLUBS, SPADES };
};



class ActionBlackjack : BotAction
{
public:
	void actionPerformed(user jid, Bot* bot, std::vector<std::string> &params);
	Deck getDeck(user jid);
};


#endif
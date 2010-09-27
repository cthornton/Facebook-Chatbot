#include "Bot.h"
#include "ActionDefs.h"
#include <string>
#include <iostream>
#include "gloox/jid.h"


int main()
{
	Bot b("chat.facebook.com", "username", "password");
	b.registerBotAction("help", new ActionHelp);
	b.registerBotAction("joke", new ActionJoke);
	b.registerBotAction("admin", new ActionAdmin);
	b.registerBotAction("counter", new ActionCounter);
	b.connect();
	std::cout << "It appears that the program has disconnected somehow..." << std::endl;
	system("pause");
	return 0;
}
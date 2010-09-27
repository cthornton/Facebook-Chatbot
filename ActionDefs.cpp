#include "ActionDefs.h"
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void ActionHelp::actionPerformed(const gloox::JID &jid, Bot* bot, std::vector<std::string> &params)
{
	if(params.size() > 1)
	{
		std::cout << params.size() << std::endl;
		std::cout << params.at(0) << std::endl;
		
		if(params.at(1).compare("list") == 0)
		{
			bot->sendMessage("Some commands:\n"
							 "/joke\n/help");
		}
		else
		{
			bot->sendMessage("Invalid paramater.");
		}
	}
	else
	{
	bot->sendMessage("This is a Facebook chatbot that is currently under development.\n"
					 "You enter commands by entering a slash, followed by the command "
					 "and some parameters. For example \"/help list\" lists several "
					 "available actions");
	}
}

std::string ActionHelp::defaultHelpText()
{
	return "You really need help using the /help feature??";
}

void ActionJoke::actionPerformed(const gloox::JID &jid, Bot* bot, std::vector<std::string> &params)
{
#include "jokes.h"

	int randNum;

	
	if(params.size() > 1 && params.at(1).compare("pickup") == 0)
	{
		randNum = rand() % num_pickup_jokes;
		bot->sendMessage("*Your pickup line:* " + pickup_jokes[randNum]);
	}
	else
	{ 
		randNum = rand() % num_jokes;
		bot->sendMessage(jokes[randNum]);
	 }

}

std::string ActionJoke::defaultHelpText()
{
	return "Gives a random joke. For pickup lines, try '/joke pickup'";
}

ActionAdmin::ActionAdmin()
{
	adminList.push_back("-100001646673875@chat.facebook.com");
}

bool ActionAdmin::isAdmin(const gloox::JID &jid)
{
	for(unsigned int i = 0; i < adminList.size(); i++)
	{
		if(adminList.at(i).compare(jid.bare()) == 0)
			return true;
	}

	return false;
}

void ActionAdmin::actionPerformed(const gloox::JID &jid, Bot* bot, std::vector<std::string> &params)
{
	if(!isAdmin(jid))
	{
		bot->sendMessage("Go away!");
		return;
	}

	bot->sendMessage("Welcome to the admin panel!");
}

void ActionCounter::actionPerformed(const gloox::JID &jid, Bot* bot, std::vector<std::string> &params)
{
	autoSessionRegister(jid);
	
	if(params.size() > 1 && params.at(1).compare("reset") == 0)
	{
		unregisterSession(jid);
		bot->sendMessage("Counter reset.");	
	}
	else
	{
	
		int *count;
		if(!keyExists(jid, "count"))
		{
			count = new int;
			*count = -1;
			setSessionData(jid, "count", count);
		}
		else
		{
			count = (int*) getSessionData(jid, "count");
		}
		
		(*count)++;

		std::stringstream st;
		st << (*count);
		bot->sendMessage("You have previously called this feature " + st.str() + " times.");

		setSessionData(jid, "count", count);

	}
}

std::string ActionCounter::defaultHelpText()
{
	return "A simple counter feature! use '/counter reset' to reset the counter.";
}
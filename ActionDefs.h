
#ifndef ACTION_DEFS_H
#define ACTION_DEFS_H

#include <string>
#include <vector>

#include "Bot.h"
#include "BotAction.h"
#include "gloox/jid.h"
#include "gloox/messagesession.h"

class ActionHelp : public BotAction
{
public:
	 void actionPerformed(const gloox::JID &jid, Bot* bot, std::vector<std::string> &params);
	 std::string defaultHelpText();
};

class ActionJoke : public BotAction
{
public:
	void actionPerformed(const gloox::JID &jid, Bot* bot, std::vector<std::string> &params);
	std::string defaultHelpText();
};

class ActionAdmin : public BotAction
{
public:
	ActionAdmin();
	void actionPerformed(const gloox::JID &jid, Bot* bot, std::vector<std::string> &params);
private:
	bool isAdmin(const gloox::JID &jid);
	std::vector<std::string> adminList;

};

class ActionCounter : public BotAction
{
public:
	void actionPerformed(const gloox::JID &jid, Bot* bot, std::vector<std::string> &params);
	std::string defaultHelpText();
};


#endif

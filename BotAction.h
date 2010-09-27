#ifndef BOT_ACTION_H
#define BOT_ACTION_H

#include "Bot.h"
#include "gloox/jid.h"
#include <vector>
#include <map>

typedef const gloox::JID& user;

class BotAction
{
public:
	virtual ~BotAction() { }
	virtual void actionPerformed(user jid, Bot* bot, std::vector<std::string> &params) = 0;
	void registerSession(user jid);
	void unregisterSession(user jid);
	void setSessionData(user jid, std::string key, void* value);
	void* getSessionData(user jid, std::string key);
	void deleteSessionData(user jid, std::string key);
	void autoSessionRegister(user jid);
	bool keyExists(user jid, std::string key);
	bool sessionRegistered(user jid);
	virtual std::string defaultHelpText();
protected:
	std::map<std::string, std::map<std::string, void*>> sessData;
};



#endif
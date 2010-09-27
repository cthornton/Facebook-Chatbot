
#include "BotAction.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>

std::string BotAction::defaultHelpText()
{
	 return "Sorry, help is not available for this command!";
}

void BotAction::registerSession(const gloox::JID& jid)
{
	std::map<std::string, void*> a;
	sessData.insert(
		std::pair<std::string, std::map<std::string, void*>>(jid.bare(), a));
}

void BotAction::unregisterSession(const gloox::JID& jid)
{
	if(sessionRegistered(jid))
		sessData.erase(jid.bare());
}

void BotAction::deleteSessionData(user jid, std::string key)
{
	if(keyExists(jid, key))
	{
		void* ptr = getSessionData(jid, key);
		delete ptr;
		ptr = 0;
		sessData[jid.bare()].erase(key);
	}
}

bool BotAction::keyExists(const gloox::JID& jid, std::string key)
{
	std::map<std::string, void*>::iterator it;
	it = sessData[jid.bare()].find(key);
	return (it != sessData[jid.bare()].end());
}

void BotAction::autoSessionRegister(const gloox::JID& jid)
{
	if(!sessionRegistered(jid))
	{
		std::cout << "New session registered" << std::endl;
		registerSession(jid);
	}
}

void* BotAction::getSessionData(const gloox::JID& jid, std::string key)
{
	return sessData[jid.bare()][key];
}

void BotAction::setSessionData(const gloox::JID& jid, std::string key, void* val)
{
	sessData[jid.bare()][key] =  val;
}

bool BotAction::sessionRegistered(const gloox::JID& jid)
{
	std::map<std::string, std::map<std::string, void*>>::iterator it;
	it = sessData.find(jid.bare());
	return (it != sessData.end());
}
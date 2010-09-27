#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <stdio.h>
#include <string.h>

#include "Bot.h"
#include "BotAction.h"
#include "gloox/client.h"
#include "gloox/messagehandler.h"
#include "gloox/loghandler.h"
#include "gloox/rosteritem.h"
#include "gloox/rostermanager.h"


Bot::Bot(std::string server, std::string username,
		 std::string password, std::string resource) :  m_session(0)
{
	logFile.open("activity.log");
	logFile << "Facebook Chat log file" << std::endl;
	gloox::JID jid(username + "@" + server + "/" + resource);
	client = new gloox::Client(jid, password);
	client->registerConnectionListener(this);
	client->registerMessageSessionHandler( this, 0 );
	client->registerMessageHandler(this);
	logFile << "[Info] Bot created. Server info: '" + username + "@"
			  + server + "/" + resource + "'" << std::endl;

#if _DEBUG
	debugLogFile.open("debug.log");
	client->logInstance().registerLogHandler(
		gloox::LogLevelDebug, gloox::LogAreaAll, this);
	debugLogFile << "Facebook Chat Bot DEBUG log file\n" << std::endl;
#endif
}

Bot::~Bot()
{
	// Delete all associated data...
	delete client;
	delete m_session;
	client = NULL;
	m_session = NULL;
	botActions.clear();

	logFile.close();
#if _DEBUG
	debugLogFile.close();
#endif
}

bool Bot::userActive(const gloox::JID &jid)
{
	for(unsigned int i = 0; i < activeJID.size(); i++)
	{
		if(activeJID[i] == jid)
			return true;
	}

	return false;
}

void Bot::addUserActive(const gloox::JID &jid, bool safe)
{
	// Make sure to avoid duplicates!
	if(safe)
	{
		if(userActive(jid))
			return;
	}

	// Now add the active user to the list
	activeJID.push_back(jid);
}

void Bot::registerBotAction(std::string action, BotAction *handle)
{
	botActions.insert(std::pair<std::string, BotAction*>(action, handle));
#if _DEBUG
	debugLogFile << "[Info] Registered bot action '" + action + "'" << std::endl;
#endif

}

void Bot::connect()
{
	std::cout << "Connecting to chat server..." << std::endl;
	logFile << "[Info] Creating connection to server" << std::endl;
	client->connect();
}

void Bot::sendMessage(std::string msg)
{
	m_session->send(msg, "");
	std::cout << "<You> " << msg << std::endl;
	logFile << "<You> " << msg << std::endl;
}


void Bot::handleMessageSession(gloox::MessageSession* session)
{
	client->disposeMessageSession(m_session);
	m_session = session;
	m_session->registerMessageHandler(this);
#if _DEBUG
	std::string name = getDisplayName(m_session->target());
	std::cout << "New message session recieved from " + name
			  << std::endl;
	debugLogFile << "[Info] New message session recieved from " 
				 + name << std::endl;
#endif

}

void Bot::handleLog(gloox::LogLevel level, gloox::LogArea area, 
							const std::string& message )
    {
#if _DEBUG
		// char *c;
		// sprintf(c, "log: level: %d, area: %d, %s\n", level, area, message.c_str() );
		std::string log_entry = "[log] " + message;
		debugLogFile << log_entry << std::endl;
#endif
    }

void Bot::handleMessage(gloox::Stanza *stanza, 
						gloox::MessageSession* session)
{
	using namespace std;

	std::cout << "<" << getDisplayName(stanza->from()) << "> "
			  << stanza->body() << std::endl;

	logFile << "<" << getDisplayName(stanza->from()) << "> "
			  << stanza->body() << std::endl;

	std::string body = stanza->body();

	if(!userActive(stanza->from()))
	{
		sendMessage("Welcome to ChrisBot 0.1\n"
					"You are currently talking to a bot. Type /help for help.");
		addUserActive(stanza->from());
	}

	// They're trying to enter a command...
	if(body.substr(0, 1).compare("/") == 0)
	{
		// Break it into mini pieces
		char *str = strtok(const_cast<char*>(body.c_str()), " ");

		// Now a list of args
		vector<string> args; 

		// And we'll be iterating through the registered commands!
		map <string, BotAction*>::iterator it;

		// Add all of the items into a command form!
		while(str != NULL)
		{
			args.push_back(string(str));
			str = strtok(NULL, " ");
		}

		string cmd = args.at(0);
		if(cmd.length() > 1)
			cmd = cmd.substr(1, cmd.length());

		it = botActions.find(cmd);

		if(it == botActions.end())
		{
			sendMessage("Command '" + cmd + "' not found.");
		}
		else
		{
			if(args.size() == 2 && args.at(1).compare("help") == 0)
				sendMessage(it->second->defaultHelpText());
			else
				it->second->actionPerformed(stanza->from(), this, args);
		}

	}
}

std::string Bot::getDisplayName(const gloox::JID &jid)
{
	return client->rosterManager()->getRosterItem(jid)->name();
}


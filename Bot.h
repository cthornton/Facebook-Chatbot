
#ifndef BOT_H
#define BOT_H

#include <vector>
#include <map>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "gloox/client.h"
#include "gloox/messagehandler.h"
#include "gloox/messagesession.h"
#include "gloox/messagesessionhandler.h"
#include "gloox/connectionlistener.h"
#include "gloox/loghandler.h"
#include "gloox/logsink.h"
#include "gloox/chatstatehandler.h"


// This needs to be here or else we get some really weird errors....
class BotAction;

class Bot : public gloox::MessageHandler, gloox::MessageSessionHandler,
				   gloox::LogHandler, gloox::ConnectionListener
{
public:
	Bot(std::string server, std::string username,
			   std::string password, std::string resource = "bot");
	~Bot();
	void registerBotAction(std::string action, BotAction *handle);
	void addUserActive(const gloox::JID &jid, bool safe = true);
	bool userActive(const gloox::JID &jid);
	void sendMessage(std::string msg);
	void connect();
	virtual void handleMessage(gloox::Stanza* stanza, 
								gloox::MessageSession* session = 0);
	virtual void handleMessageSession(gloox::MessageSession *session);
    virtual void handleLog(gloox::LogLevel level, gloox::LogArea area, 
							const std::string& message );

	virtual void onConnect()
	{
		printf("Connected to server; you will now begin to recieve messages\n");
		logFile << "[Info] Connection to server successful" << std::endl;
	}

	virtual void onDisconnect(gloox::ConnectionError e)
	{
     printf( "message_test: disconnected: %d\n", e );
      if( e == gloox::ConnAuthenticationFailed )
        printf( "auth failed. reason: %d\n", client->authError() );

	  logFile << "[Error] Disconnected from server!" << std::endl;
	  
	  std::stringstream s, ss;
	  s << MAX_RECONN;

	  if(numReconnects > MAX_RECONN)
	  {
		  logFile << "[Error] Reached max connection attempts (" << s << "), exiting program." << std::endl;
	  }
	  else
	  {
		ss << ++numReconnects;
		logFile << "[Log] Making re-connection attempt " << ss << " of " << s << std::endl;
		connect();
	  }

	}

	virtual bool onTLSConnect(const gloox::CertInfo& info)
	{
		return true;
	}

	virtual void handleIncomingConnection(gloox::ConnectionBase *) {}

protected:
	std::string getDisplayName(const gloox::JID &jid);
	gloox::Client *client;
	gloox::MessageSession *m_session;
	std::map<std::string, BotAction*> botActions;
	std::vector<const gloox::JID> activeJID;
	std::ofstream logFile;
	unsigned int numReconnects;
	static const unsigned int MAX_RECONN = 5;
#if _DEBUG
	std::ofstream debugLogFile;
#endif
	
};

#endif
#include "pch.h"
#include "Net.h"
#include "Settings.h"

using namespace LevelModSettings;
extern std::map<int, OverrideOption> overrideOptions;


namespace Network
{
	struct HostOptionData
	{
		int option;
		int value;
	};

	int HostOption_Changed(MsgHandlerContext* context)
	{
		HostOptionData* data = (HostOptionData*)context->pData;
		auto it = LevelModSettings::overrideOptions.find(data->option);
		if (it != LevelModSettings::overrideOptions.end())
		{
			it->second.value = data->value;
		}
		else
			_printf("Couldn't find HostOption %s...\n", data->option);

		return HANDLER_CONTINUE;
	}


	//Messages sent by host
	void MessageHandler::AddServerMessages(unsigned char msg_id, void* pCallback, int flags, Net* pNet, int prio)
	{
		//Add the original message that we have replaced with our hook
		AddMessage(msg_id, pCallback, flags, pNet, prio);

		//When host change an option that is linked to an override object the host will send out a message
		//When the client gets the message HostOption_Changed callback will be called
		AddMessage(MSG_ID_LM_HOSTOPTION_CHANGED, HostOption_Changed, HANDLER_LATE, pNet);
	}

	//Messages sent by clients
	void MessageHandler::AddClientMessages(unsigned char msg_id, void* pCallback, int flags, Net* pNet, int prio)
	{
		//Add the original message that we have replaced with our hook
		AddMessage(msg_id, pCallback, flags, pNet, prio);
		
		//TODO add new client messages here
	}
};
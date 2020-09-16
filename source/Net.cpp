#include "pch.h"
#include "Net.h"


namespace Network
{
	struct HostOptionData
	{
		DWORD name;
		DWORD value;
	};

	void HostOption_Changed(MsgHandlerContext* context)
	{
		HostOptionData* data = (HostOptionData*)context->pData;
	}


	//Messages sent by host
	void MessageHandler::AddServerMessages(unsigned char msg_id, void* pCallback, int flags, Net* pNet, int prio)
	{
		//Add the original message that we have replaced with our hook
		AddMessage(msg_id, pCallback, flags, pNet, prio);

		//When host change an option that is linked to an override object the host will send out a message
		//When the client gets the message HostOption_Changed callback will be called
		AddMessage(MSG_ID_LM_HOSTOPTION_CHANGED, HostOption_Changed, LATE, pNet);
	}

	//Messages sent by clients
	void MessageHandler::AddClientMessages(unsigned char msg_id, void* pCallback, int flags, Net* pNet, int prio)
	{
		//Add the original message that we have replaced with our hook
		AddMessage(msg_id, pCallback, flags, pNet, prio);
		
		//TODO add new client messages here
	}
};
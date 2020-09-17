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



    int Handle_HostOption_Changed(MsgHandlerContext* context)
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
    void MessageHandler::AddServerMessages(unsigned char msg_id, void* pCallback, int flags, NetHandler* net_handler, int prio)
    {
        //Add the original message that we have replaced with our hook
        AddMessage(msg_id, pCallback, flags, net_handler, prio);

        //When host change a HostOption the host will send out a message
        //When the client gets the message the option linked to the HostOption should get overriden
        //It's possible to override only true, only false, or both
        //To add a new HostOption, add them to the struct in settings.q
        AddMessage(MSG_ID_LM_HOSTOPTION_CHANGED, Handle_HostOption_Changed, HANDLER_LATE, net_handler);
    }

    //Messages sent by clients
    void MessageHandler::AddClientMessages(unsigned char msg_id, void* pCallback, int flags, NetHandler* net_handler, int prio)
    {
        //Add the original message that we have replaced with our hook
        AddMessage(msg_id, pCallback, flags, net_handler, prio);

        //TODO add new client messages here
    }
};
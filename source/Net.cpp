#include "pch.h"
#include "Settings.h"

using namespace LevelModSettings;
extern std::map<int, OverrideOption> overrideOptions;


namespace Network
{
    struct HostOptionMsg
    {
        int option;
        int value;
    };

    struct TestMsg
    {
        char test[128];
    };

    bool OnServer()
    {
        typedef bool(__cdecl* const pOnServer)();
        return pOnServer(0x00421FD0)();
    }

    void SendMessageToClients(unsigned char msg_id, DWORD len, void* data, int prio, int queue, int flag1, int flag2, bool include_observers)
    {
        if (OnServer())
        {
            NetHandler * net_handler = NetHandler::Instance();

            net_handler->SendMessageToClients(msg_id, len, data);
        }
    }


    int Handle_HostOption_Changed(MsgHandlerContext* context)
    {
        HostOptionMsg* msg = (HostOptionMsg*)context->msg;
        auto it = LevelModSettings::overrideOptions.find(msg->option);
        if (it != LevelModSettings::overrideOptions.end())
        {
            it->second.value = msg->value;
        }
        else
            _printf("Couldn't find HostOption %s...\n", msg->option);

        return HANDLER_CONTINUE;
    }

    int Handle_Test(MsgHandlerContext* context)
    {
        TestMsg* msg = (TestMsg*)context;
        MessageBox(0, msg->test, "Host sent test msg", 0);
        return HANDLER_CONTINUE;
    }


    //Messages sent by host
    void MessageHandler::AddServerMessages(unsigned char msg_id, void* pCallback, int flags, NetHandler* net_handler, int prio)
    {
        MessageHandler* handler;
        _asm mov handler, ecx;
        //Add the original message that we have replaced with our hook
        handler->AddMessage(msg_id, pCallback, flags, net_handler, prio);
        //When host change a HostOption the host will send out a message
        //When the client gets the message the option linked to the HostOption should get overriden
        //It's possible to override only true, only false, or both
        //To add a new HostOption, add them to the struct in settings.q
        handler->AddMessage(MSG_ID_LM_HOSTOPTION_CHANGED, Handle_HostOption_Changed, HANDLER_LATE, net_handler);
        handler->AddMessage(MSG_ID_LM_TEST, Handle_Test, HANDLER_LATE, net_handler);
    }

    //Messages sent by clients
    void MessageHandler::AddClientMessages(unsigned char msg_id, void* pCallback, int flags, NetHandler* net_handler, int prio)
    {
        MessageHandler* handler;
        _asm mov handler, ecx;
        //Add the original message that we have replaced with our hook
        handler->AddMessage(msg_id, pCallback, flags, net_handler, prio);

        //TODO add new client messages here
    }
};
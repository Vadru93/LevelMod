#pragma once
#ifndef NET_H
#define NET_H

namespace Network
{

	enum
	{
		MSG_ID_PING_TEST = 1,		// = 1 : Server->Client
		MSG_ID_PING_RESPONSE,					// = 2 : Client->Server
		MSG_ID_CONNECTION_REQ,					// = 3 : Client->Server
		MSG_ID_CONNECTION_ACCEPTED,				// = 4 : Server->Client
		MSG_ID_CONNECTION_REFUSED,				// = 5 : Server->Client
		MSG_ID_CONNECTION_TERMINATED,			// = 6 : Server->Client
		MSG_ID_SEQUENCED,						// = 7 : Bi-directional sequenced identifier
		MSG_ID_ACK,								// = 8 : Bi-directional acknowledgement
		MSG_ID_TIMESTAMP,						// = 9 : Server->Client
		MSG_ID_ALIAS,							// = 10 : Client->Server New alias request
		MSG_ID_DISCONN_REQ,						// = 11 : Client->Server Request to disconnect
		MSG_ID_DISCONN_ACCEPTED,				// = 12 : Server->Client Go ahead and quit
		MSG_ID_KEEPALIVE,						// = 13 : Bi-directional general keepalive message		
		MSG_ID_STREAM_START,					// = 14 : Bi-directional general stream start message
		MSG_ID_STREAM_DATA,						// = 15 : Bi-directional general stream data message
		MSG_ID_SERVER_RESPONSE,					// = 16 : Server->Client
		MSG_ID_FIND_SERVER,						// = 17 : Client->Server Broadcast

		MSG_ID_USER = 32,		// Game-Specific messages start here
	};

	enum
	{
		MSG_ID_SKATER_INPUT = 32,	//	= 32 : C->S The Client's Pad Input
		MSG_ID_YOUR_PLAYER_CREATE,				//	= 33 : S->C Create local skater
		MSG_ID_PLAYER_CREATE,					//	= 34 : S->C Each player's info
		MSG_ID_OBJ_UPDATE_STREAM,				//	= 35 : S->C Stream of object updates
		MSG_ID_NEW_ALIAS,						//	= 36 : C->S Alias suggestion
		MSG_ID_READY_QUERY,						//	= 37 : S->C Are you ready to receive more data?
		MSG_ID_READY_RESPONSE,                  //	= 38 : C->S or S->C Ready query response
		MSG_ID_UNK1,
		MSG_ID_UNK2,
		MSG_ID_UNK3,
		MSG_ID_PAUSE,							//	= 41 : S->C Pause instruction
		MSG_ID_UNPAUSE,							//	= 42 : S->C UnPause instruction
		MSG_ID_PRIM_ANIM_START,					//	= 43 : S->C Play primary animation
		MSG_ID_ROTATE_SKATEBOARD,				//	= 44 : S->C Rotate skateboard
		MSG_ID_SET_WOBBLE_TARGET,				//	= 45 : S->C Set wobble target
		MSG_ID_FLIP_ANIM,						//	= 46 : S->C Flip the animation
		MSG_ID_PLAYER_QUIT,						//	= 47 : S->C Notification of a player quitting
		MSG_ID_HOST_REQ,						//  = 48 : GameServer->MatchMaker Game Host Request
		MSG_ID_HOST_PROCEED,					//  = 49 : MatchMaker->GameServer Proceed hosting
		MSG_ID_GAMELIST_REQ,					//  = 50 : GameClient->MatchMaker Game list request
		MSG_ID_GAMELIST_RESPONSE,				//  = 51 : MatchMaker->GameClient Game list response
		MSG_ID_HOST_QUIT,						//	= 52 : S->C Server is shutting down
		MSG_ID_START_INFO,						//	= 53 : S->C Startup Game Option
		MSG_ID_GAME_INFO,						//	= 54 : S->C Game Options
		MSG_ID_PLAY_SOUND,						//	= 55 : S->C Play Sound
		MSG_ID_PLAY_LOOPING_SOUND,				// 	= 56 : S->C Play Looping Sound
		MSG_ID_SPARKS_ON,						//	= 57 : S->C Sparks on
		MSG_ID_SPARKS_OFF,						//	= 58 : S->C Sparks off
		MSG_ID_BLOOD_ON,						//	= 59 : S->C Blood on
		MSG_ID_BLOOD_OFF,						//	= 60 : S->C Blood off
		MSG_ID_SCORE,							//	= 61 : S->C General Score Msg: See SCORE_MSG_ID...
		MSG_ID_SET_WOBBLE_DETAILS,				//	= 62 : S->C Set the wobbling parameters
		MSG_ID_SET_LOOPING_TYPE,				//	= 63 : S->C Set the desired looping type
		MSG_ID_SET_HIDE_ATOMIC,					//	= 64 : S->C Hide an arbitrary atomic on the skin model
		MSG_ID_PANEL_MESSAGE,					//	= 65 : S->C Display this panel message
		MSG_ID_SCORE_UPDATE,					//	= 66 : S->C Score update for other clients
		MSG_ID_SET_ANIM_SPEED,					//	= 67 : S->C Set animation speed
		MSG_ID_PROCEED_TO_PLAY,					//	= 68 : S->C Init phase complete: Next state
		MSG_ID_SKATER_COLLIDE_LOST,				//	= 69 : S->C You were involved in a skater collision and lost
		MSG_ID_SKATER_COLLIDE_WON,				//	= 70 : S->C You were involved in a skater collision and won
		MSG_ID_BAIL_DONE,						//	= 71 : C->S Bail is done
		MSG_ID_CHANGE_LEVEL,					//	= 72 : S->C Change level
		MSG_ID_JOIN_REFUSED,					//	= 73 : S->C Server refused to allow player to join
		MSG_ID_RUN_SCRIPT,						//  = 74 : C->C Run this script
		MSG_ID_SPAWN_AND_RUN_SCRIPT,			//  = 75 : C->C Spawn and Run this script
		MSG_ID_OBSERVE,							//	= 76 : C->S A request to enter observer mode
		MSG_ID_STEAL_MESSAGE,					//	= 77 : S->C A request to display a "steal message" in graffiti
		MSG_ID_CHAT,							//	= 78 : C->C Chat message
		MSG_ID_OBSERVE_PROCEED,					//  = 79 : S->C Proceed to observe
		MSG_ID_OBSERVE_REFUSED,					//	= 80 : S->C Server says you can't observe
		MSG_ID_JOIN_PROCEED,					//	= 81 : S->C Go ahead and attempt to join
		MSG_ID_JOIN_REQ,						//  = 82 : C->S Request to join a connected server
		MSG_ID_KICKED,							//	= 83 : S->C You've been kicked
		MSG_ID_LANDED_TRICK,					//	= 84 : S->C Landed a trick
		MSG_ID_PLAYER_INFO_ACK_REQ,				//	= 85 : S->C Have you received all the player info data?
		MSG_ID_PLAYER_INFO_ACK,					//	= 86 : C->S Yes I have received all the player info data
		MSG_ID_NEW_KING,						//	= 87 : S->C Crown the new king
		MSG_ID_LEFT_OUT,						//  = 88 : S->C Sorry. We're playing without you
		MSG_ID_MOVED_TO_RESTART,				//	= 89 : C->S Client has moved to a restart
		MSG_ID_KING_DROPPED_CROWN,				//	= 90 : S->C Spawn a new crown in the world
		MSG_ID_RUN_HAS_ENDED,					//	= 91 : C->S My run has ended
		MSG_ID_GAME_OVER,						//	= 92 : S->C Game is over
		MSG_ID_OBSERVER_LOG_TRICK_OBJ,			//	= 93 : S->C Special-case observer trick object report
		MSG_ID_OBSERVER_INIT_GRAFFITI_STATE,	//	= 94 : S->C Special-case observer, to synchronize trick objects with the current graffiti state
		MSG_ID_ITEM_RESPAWNED,                  //  = 95 : S->C Crown/Flag/Ball respawned
		MSG_ID_UNK4,
		MSG_ID_UNK5,
		MSG_ID_END_GAME,                        //  = 98 : S->C Server has chosen to end the game
		MSG_ID_TEAM_CHANGE,						//	= 99 : S->C Team change update
		MSG_ID_REQUEST_CHANGE_TEAM,             //	= 100 : C->S Request to change teams
		MSG_ID_OWNZONE,					        //	= 101 : S->C Send own zone messages
		MSG_ID_XFER_BEGIN_REQ,                  //  = 102 : S->C 
		MSG_ID_UNK6,                       //  = 103 : C->S This calls MSG_ID 104
		MSG_ID_UNK7,		                //	= 104 : S->C Something about game?
		MSG_ID_SEND_PARK_DATA,                  //	= 105 : S->C CAP Data
		MSG_ID_RECV_PARK_DATA,					//	= 106 : C->S Cap Data

		//To make sure we don't overlap messages with unknown ID we add padding
		MSG_ID_PADDING1,
		MSG_ID_PADDING2,
		MSG_ID_PADDING3,
		MSG_ID_PADDING4,
		MSG_ID_PADDING5,
		MSG_ID_PADDING6,
		MSG_ID_PADDING7,
		MSG_ID_PADDING8,
		MSG_ID_PADDING9,
		MSG_ID_PADDING10,
		MSG_ID_PADDING11,
		MSG_ID_PADDING12,
		MSG_ID_PADDING13,
		MSG_ID_PADDING14,
		MSG_ID_PADDING15,
		MSG_ID_PADDING16,
		MSG_ID_PADDING17,
		MSG_ID_PADDING18,
		MSG_ID_PADDING19,
		MSG_ID_PADDING20,
		MSG_ID_PADDING21,
		MSG_ID_PADDING22,


		//LM Specific ID
		MSG_ID_LM_HOSTOPTION_CHANGED,	//  = 130 : S->C Host option changed
	};

	enum
	{
		LOW_PRIO = 0,
		NORMAL_PRIO = 128,
		HIGH_PRIO = 255,
	};

	enum
	{
		HANDLER_ERROR,
		HANDLER_CONTINUE,
		HANDLER_HALT,
		HANDLER_MSG_DONE,
		HANDLER_MSG_DESTROYED,
	};

	enum
	{
		HANDLER_FOREIGN = 0x0001,
		HANDLER_LATE = 0x0002,
		HANDLER_CRC_MISMATCH = 0x0004,
	};


	struct Net
	{
		//Not yet dissambled
	};

	struct MsgHandlerContext
	{
		char* msg;
		BYTE id;
		DWORD length;
		void* app;
		void* connection;
		int flags;
		void* pData;
	};

	struct MessageHandler
	{

		typedef bool(__thiscall* const pAddMessage)(MessageHandler* pThis, unsigned char msg_id, void* pCallback, int flags, Net* pNet, int prio);
		void AddMessage(unsigned char msg_id, void* pCallback, int flags, Net* pNet, int prio = NORMAL_PRIO)
		{
			pAddMessage(0x004D9090)(this, msg_id, pCallback, flags, pNet, prio);
		}

		void AddClientMessages(unsigned char msg_id, void* pCallback, int flags, Net* pNet, int prio);
		void AddServerMessages(unsigned char msg_id, void* pCallback, int flags, Net* pNet, int prio);
	};
};

#endif
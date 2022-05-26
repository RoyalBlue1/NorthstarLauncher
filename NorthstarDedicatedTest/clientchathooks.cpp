#include "pch.h"
#include "squirrel.h"
#include "serverchathooks.h"
#include "localchatwriter.h"

#include <rapidjson/document.h>

AUTOHOOK_INIT()

AUTOHOOK(CHudChat__AddGameLine, client.dll + 0x22E580, 
void,, (void* self, const char* message, int inboxId, bool isTeam, bool isDead), 
{
	// This hook is called for each HUD, but we only want our logic to run once.
	if (self != *CHudChat::allHuds)
	{
		return;
	}

	if (g_pClientSquirrel->setupfunc("CHudChat_ProcessMessageStartThread") != SQRESULT_ERROR)
	{
		int senderId = inboxId & CUSTOM_MESSAGE_INDEX_MASK;
		bool isAnonymous = senderId == 0;
		bool isCustom = isAnonymous || (inboxId & CUSTOM_MESSAGE_INDEX_BIT);

		// Type is set to 0 for non-custom messages, custom messages have a type encoded as the first byte
		int type = 0;
		const char* payload = message;
		if (isCustom)
		{
			type = message[0];
			payload = message + 1;
		}

		g_pClientSquirrel->pushinteger(g_pClientSquirrel->sqvm2, (int) senderId - 1);
		g_pClientSquirrel->pushstring(g_pClientSquirrel->sqvm2, payload);
		g_pClientSquirrel->pushbool(g_pClientSquirrel->sqvm2, isTeam);
		g_pClientSquirrel->pushbool(g_pClientSquirrel->sqvm2, isDead);
		g_pClientSquirrel->pushinteger(g_pClientSquirrel->sqvm2, type);
		g_pClientSquirrel->call(g_pClientSquirrel->sqvm2, 5);
	}
	else
	{
		for (CHudChat* hud = *CHudChat::allHuds; hud != NULL; hud = hud->next)
		{
			CHudChat__AddGameLine(hud, message, inboxId, isTeam, isDead);
		}
	}
})

// void NSChatWrite( int context, string str )
static SQRESULT SQ_ChatWrite(void* sqvm)
{
	int context = g_pClientSquirrel->getinteger(g_pClientSquirrel->sqvm2, 1);
	const char* str = g_pClientSquirrel->getstring(g_pClientSquirrel->sqvm2, 2);

	LocalChatWriter((LocalChatWriter::Context)context).Write(str);
	return SQRESULT_NOTNULL;
}

// void NSChatWriteRaw( int context, string str )
static SQRESULT SQ_ChatWriteRaw(void* sqvm)
{
	int context = g_pClientSquirrel->getinteger(g_pClientSquirrel->sqvm2, 1);
	const char* str = g_pClientSquirrel->getstring(g_pClientSquirrel->sqvm2, 2);

	LocalChatWriter((LocalChatWriter::Context)context).InsertText(str);
	return SQRESULT_NOTNULL;
}

// void NSChatWriteLine( int context, string str )
static SQRESULT SQ_ChatWriteLine(void* sqvm)
{
	int context = g_pClientSquirrel->getinteger(g_pClientSquirrel->sqvm2, 1);
	const char* str = g_pClientSquirrel->getstring(g_pClientSquirrel->sqvm2, 2);

	LocalChatWriter((LocalChatWriter::Context)context).WriteLine(str);
	return SQRESULT_NOTNULL;
}

ON_DLL_LOAD_CLIENT_RELIESON("client.dll", ClientChatHooks, ClientSquirrel, [](HMODULE baseAddress)
{
	AUTOHOOK_DISPATCH()

	g_pClientSquirrel->AddFuncRegistration("void", "NSChatWrite", "int context, string text", "", SQ_ChatWrite);
	g_pClientSquirrel->AddFuncRegistration("void", "NSChatWriteRaw", "int context, string text", "", SQ_ChatWriteRaw);
	g_pClientSquirrel->AddFuncRegistration("void", "NSChatWriteLine", "int context, string text", "", SQ_ChatWriteLine);
})

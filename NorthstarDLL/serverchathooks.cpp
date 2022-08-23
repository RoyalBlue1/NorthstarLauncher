#include "pch.h"
#include "serverchathooks.h"
#include "limits.h"
#include "squirrel.h"
#include "r2server.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

AUTOHOOK_INIT()

class CServerGameDLL;

class CRecipientFilter
{
	char unknown[58];
};

CServerGameDLL* g_pServerGameDLL;

void(__fastcall* CServerGameDLL__OnReceivedSayTextMessage)(
	CServerGameDLL* self, unsigned int senderPlayerId, const char* text, int channelId);

void(__fastcall* CRecipientFilter__Construct)(CRecipientFilter* self);
void(__fastcall* CRecipientFilter__Destruct)(CRecipientFilter* self);
void(__fastcall* CRecipientFilter__AddAllPlayers)(CRecipientFilter* self);
void(__fastcall* CRecipientFilter__AddRecipient)(CRecipientFilter* self, const R2::CBasePlayer* player);
void(__fastcall* CRecipientFilter__MakeReliable)(CRecipientFilter* self);

void(__fastcall* UserMessageBegin)(CRecipientFilter* filter, const char* messagename);
void(__fastcall* MessageEnd)();
void(__fastcall* MessageWriteByte)(int iValue);
void(__fastcall* MessageWriteString)(const char* sz);
void(__fastcall* MessageWriteBool)(bool bValue);

bool bShouldCallSayTextHook = false;

AUTOHOOK(_CServerGameDLL__OnReceivedSayTextMessage, server.dll + 0x1595C0,
void,, (CServerGameDLL* self, unsigned int senderPlayerId, const char* text, bool isTeam))
{
	// MiniHook doesn't allow calling the base function outside of anywhere but the hook function.
	// To allow bypassing the hook, isSkippingHook can be set.
	if (bShouldCallSayTextHook)
	{
		bShouldCallSayTextHook = false;
		_CServerGameDLL__OnReceivedSayTextMessage(self, senderPlayerId, text, isTeam);
		return;
	}

	// check chat ratelimits
	if (!g_pServerLimits->CheckChatLimits(&R2::g_pClientArray[senderPlayerId - 1]))
		return;

	if (g_pSquirrel<ScriptContext::SERVER>->setupfunc("CServerGameDLL_ProcessMessageStartThread") != SQRESULT_ERROR)
	{
		g_pSquirrel<ScriptContext::SERVER>->pushinteger(g_pSquirrel<ScriptContext::SERVER>->sqvm, (int)senderPlayerId - 1);
		g_pSquirrel<ScriptContext::SERVER>->pushstring(g_pSquirrel<ScriptContext::SERVER>->sqvm, text);
		g_pSquirrel<ScriptContext::SERVER>->pushbool(g_pSquirrel<ScriptContext::SERVER>->sqvm, isTeam);
		g_pSquirrel<ScriptContext::SERVER>->call(g_pSquirrel<ScriptContext::SERVER>->sqvm, 3);
	}
	else
		_CServerGameDLL__OnReceivedSayTextMessage(self, senderPlayerId, text, isTeam);
}

void ChatSendMessage(unsigned int playerIndex, const char* text, bool isteam)
{
	bShouldCallSayTextHook = true;
	CServerGameDLL__OnReceivedSayTextMessage(
		g_pServerGameDLL,
		// Ensure the first bit isn't set, since this indicates a custom message
		(playerIndex + 1) & CUSTOM_MESSAGE_INDEX_MASK,
		text,
		isteam);
}

void ChatBroadcastMessage(int fromPlayerIndex, int toPlayerIndex, const char* text, bool isTeam, bool isDead, CustomMessageType messageType)
{
	R2::CBasePlayer* toPlayer = NULL;
	if (toPlayerIndex >= 0)
	{
		toPlayer = R2::UTIL_PlayerByIndex(toPlayerIndex + 1);
		if (toPlayer == NULL)
			return;
	}

	// Build a new string where the first byte is the message type
	char sendText[256];
	sendText[0] = (char)messageType;
	strncpy_s(sendText + 1, 255, text, 254);

	// Anonymous custom messages use playerId=0, non-anonymous ones use a player ID with the first bit set
	unsigned int fromPlayerId = fromPlayerIndex < 0 ? 0 : ((fromPlayerIndex + 1) | CUSTOM_MESSAGE_INDEX_BIT);

	CRecipientFilter filter;
	CRecipientFilter__Construct(&filter);
	if (toPlayer == NULL)
	{
		CRecipientFilter__AddAllPlayers(&filter);
	}
	else
	{
		CRecipientFilter__AddRecipient(&filter, toPlayer);
	}
	CRecipientFilter__MakeReliable(&filter);

	UserMessageBegin(&filter, "SayText");
	MessageWriteByte(fromPlayerId);
	MessageWriteString(sendText);
	MessageWriteBool(isTeam);
	MessageWriteBool(isDead);
	MessageEnd();

	CRecipientFilter__Destruct(&filter);
}

// void function NSSendMessage( int playerIndex, string text, bool isTeam )
SQRESULT SQ_SendMessage(HSquirrelVM* sqvm)
{
	int playerIndex = g_pSquirrel<ScriptContext::SERVER>->getinteger(sqvm, 1);
	const char* text = g_pSquirrel<ScriptContext::SERVER>->getstring(sqvm, 2);
	bool isTeam = g_pSquirrel<ScriptContext::SERVER>->getbool(sqvm, 3);

	ChatSendMessage(playerIndex, text, isTeam);

	return SQRESULT_NULL;
}

// void function NSBroadcastMessage( int fromPlayerIndex, int toPlayerIndex, string text, bool isTeam, bool isDead, int messageType )
SQRESULT SQ_BroadcastMessage(HSquirrelVM* sqvm)
{
	int fromPlayerIndex = g_pSquirrel<ScriptContext::SERVER>->getinteger(sqvm, 1);
	int toPlayerIndex = g_pSquirrel<ScriptContext::SERVER>->getinteger(sqvm, 2);
	const char* text = g_pSquirrel<ScriptContext::SERVER>->getstring(sqvm, 3);
	bool isTeam = g_pSquirrel<ScriptContext::SERVER>->getbool(sqvm, 4);
	bool isDead = g_pSquirrel<ScriptContext::SERVER>->getbool(sqvm, 5);
	int messageType = g_pSquirrel<ScriptContext::SERVER>->getinteger(sqvm, 6);

	if (messageType < 1)
	{
		g_pSquirrel<ScriptContext::SERVER>->raiseerror(sqvm, fmt::format("Invalid message type {}", messageType).c_str());
		return SQRESULT_ERROR;
	}

	ChatBroadcastMessage(fromPlayerIndex, toPlayerIndex, text, isTeam, isDead, (CustomMessageType)messageType);

	return SQRESULT_NULL;
}

ON_DLL_LOAD("engine.dll", EngineServerChatHooks, (CModule module))
{
	g_pServerGameDLL = module.Offset(0x13F0AA98).As<CServerGameDLL*>();
}

ON_DLL_LOAD_RELIESON("server.dll", ServerChatHooks, ServerSquirrel, (CModule module))
{
	AUTOHOOK_DISPATCH_MODULE(server.dll)

	CServerGameDLL__OnReceivedSayTextMessage = module.Offset(0x1595C0).As<void(__fastcall*)(CServerGameDLL*, unsigned int, const char*, int)>();
	CRecipientFilter__Construct = module.Offset(0x1E9440).As<void(__fastcall*)(CRecipientFilter*)>();
	CRecipientFilter__Destruct = module.Offset(0x1E9700).As<void(__fastcall*)(CRecipientFilter*)>();
	CRecipientFilter__AddAllPlayers = module.Offset(0x1E9940).As<void(__fastcall*)(CRecipientFilter*)>();
	CRecipientFilter__AddRecipient = module.Offset(0x1E9B30).As<void(__fastcall*)(CRecipientFilter*, const R2::CBasePlayer*)>();
	CRecipientFilter__MakeReliable = module.Offset(0x1EA4E0).As<void(__fastcall*)(CRecipientFilter*)>();

	UserMessageBegin = module.Offset(0x15C520).As<void(__fastcall*)(CRecipientFilter*, const char*)>();
	MessageEnd = module.Offset(0x158880).As<void(__fastcall*)()>();
	MessageWriteByte = module.Offset(0x158A90).As<void(__fastcall*)(int)>();
	MessageWriteString = module.Offset(0x158D00).As<void(__fastcall*)(const char*)>();
	MessageWriteBool = module.Offset(0x158A00).As<void(__fastcall*)(bool)>();

	// Chat sending functions
	g_pSquirrel<ScriptContext::SERVER>->AddFuncRegistration("void", "NSSendMessage", "int playerIndex, string text, bool isTeam", "", SQ_SendMessage);
	g_pSquirrel<ScriptContext::SERVER>->AddFuncRegistration(
		"void",
		"NSBroadcastMessage",
		"int fromPlayerIndex, int toPlayerIndex, string text, bool isTeam, bool isDead, int messageType",
		"",
		SQ_BroadcastMessage);
}
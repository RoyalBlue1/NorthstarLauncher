#include "pch.h"
#include "squirrel.h"
#include "masterserver.h"
#include "serverauthentication.h"
#include "r2engine.h"
#include "r2client.h"

// functions for viewing server browser

// bool function NSIsMasterServerAuthenticated()
SQRESULT SQ_IsMasterServerAuthenticated(void* sqvm)
{
	g_pUISquirrel->pushbool(sqvm, g_MasterServerManager->m_bOriginAuthWithMasterServerDone);
	return SQRESULT_NOTNULL;
}

// void function NSRequestServerList()
SQRESULT SQ_RequestServerList(void* sqvm)
{
	g_MasterServerManager->RequestServerList();
	return SQRESULT_NULL;
}

// bool function NSIsRequestingServerList()
SQRESULT SQ_IsRequestingServerList(void* sqvm)
{
	g_pUISquirrel->pushbool(sqvm, g_MasterServerManager->m_bScriptRequestingServerList);
	return SQRESULT_NOTNULL;
}

// bool function NSMasterServerConnectionSuccessful()
SQRESULT SQ_MasterServerConnectionSuccessful(void* sqvm)
{
	g_pUISquirrel->pushbool(sqvm, g_MasterServerManager->m_bSuccessfullyConnected);
	return SQRESULT_NOTNULL;
}

// int function NSGetServerCount()
SQRESULT SQ_GetServerCount(void* sqvm)
{
	g_pUISquirrel->pushinteger(sqvm, g_MasterServerManager->m_vRemoteServers.size());
	return SQRESULT_NOTNULL;
}

// string function NSGetServerName( int serverIndex )
SQRESULT SQ_GetServerName(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get name of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushstring(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].name);
	return SQRESULT_NOTNULL;
}

// string function NSGetServerDescription( int serverIndex )
SQRESULT SQ_GetServerDescription(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get description of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushstring(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].description.c_str());
	return SQRESULT_NOTNULL;
}

// string function NSGetServerMap( int serverIndex )
SQRESULT SQ_GetServerMap(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get map of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushstring(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].map);
	return SQRESULT_NOTNULL;
}

// string function NSGetServerPlaylist( int serverIndex )
SQRESULT SQ_GetServerPlaylist(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get playlist of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushstring(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].playlist);
	return SQRESULT_NOTNULL;
}

// int function NSGetServerPlayerCount( int serverIndex )
SQRESULT SQ_GetServerPlayerCount(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get playercount of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushinteger(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].playerCount);
	return SQRESULT_NOTNULL;
}

// int function NSGetServerMaxPlayerCount( int serverIndex )
SQRESULT SQ_GetServerMaxPlayerCount(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get max playercount of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushinteger(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].maxPlayers);
	return SQRESULT_NOTNULL;
}

// string function NSGetServerID( int serverIndex )
SQRESULT SQ_GetServerID(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get id of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushstring(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].id);
	return SQRESULT_NOTNULL;
}

// bool function NSServerRequiresPassword( int serverIndex )
SQRESULT SQ_ServerRequiresPassword(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get hasPassword of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushbool(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].requiresPassword);
	return SQRESULT_NOTNULL;
}

// int function NSGetServerRequiredModsCount( int serverIndex )
SQRESULT SQ_GetServerRequiredModsCount(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get required mods count of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushinteger(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].requiredMods.size());
	return SQRESULT_NOTNULL;
}

// string function NSGetServerRequiredModName( int serverIndex, int modIndex )
SQRESULT SQ_GetServerRequiredModName(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);
	SQInteger modIndex = g_pUISquirrel->getinteger(sqvm, 2);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get hasPassword of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	if (modIndex >= g_MasterServerManager->m_vRemoteServers[serverIndex].requiredMods.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get required mod name of mod index {} when only {} mod are available",
				modIndex,
				g_MasterServerManager->m_vRemoteServers[serverIndex].requiredMods.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushstring(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].requiredMods[modIndex].Name.c_str());
	return SQRESULT_NOTNULL;
}

// string function NSGetServerRequiredModVersion( int serverIndex, int modIndex )
SQRESULT SQ_GetServerRequiredModVersion(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);
	SQInteger modIndex = g_pUISquirrel->getinteger(sqvm, 2);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get required mod version of server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	if (modIndex >= g_MasterServerManager->m_vRemoteServers[serverIndex].requiredMods.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to get required mod version of mod index {} when only {} mod are available",
				modIndex,
				g_MasterServerManager->m_vRemoteServers[serverIndex].requiredMods.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	g_pUISquirrel->pushstring(sqvm, g_MasterServerManager->m_vRemoteServers[serverIndex].requiredMods[modIndex].Version.c_str());
	return SQRESULT_NOTNULL;
}

// void function NSClearRecievedServerList()
SQRESULT SQ_ClearRecievedServerList(void* sqvm)
{
	g_MasterServerManager->ClearServerList();
	return SQRESULT_NULL;
}

// functions for authenticating with servers

// void function NSTryAuthWithServer( int serverIndex, string password = "" )
SQRESULT SQ_TryAuthWithServer(void* sqvm)
{
	SQInteger serverIndex = g_pUISquirrel->getinteger(sqvm, 1);
	const SQChar* password = g_pUISquirrel->getstring(sqvm, 2);

	if (serverIndex >= g_MasterServerManager->m_vRemoteServers.size())
	{
		g_pUISquirrel->raiseerror(
			sqvm, 
			fmt::format(
				"Tried to auth with server index {} when only {} servers are available",
				serverIndex,
				g_MasterServerManager->m_vRemoteServers.size())
				.c_str());
		return SQRESULT_ERROR;
	}

	// send off persistent data first, don't worry about server/client stuff, since m_additionalPlayerData should only have entries when
	// we're a local server note: this seems like it could create a race condition, test later
	for (auto& pair : g_ServerAuthenticationManager->m_additionalPlayerData)
		g_ServerAuthenticationManager->WritePersistentData(pair.first);

	// do auth
	g_MasterServerManager->AuthenticateWithServer(
		R2::g_pLocalPlayerUserID,
		g_MasterServerManager->m_sOwnClientAuthToken,
		g_MasterServerManager->m_vRemoteServers[serverIndex].id,
		(char*)password);

	return SQRESULT_NULL;
}

// bool function NSIsAuthenticatingWithServer()
SQRESULT SQ_IsAuthComplete(void* sqvm)
{
	g_pUISquirrel->pushbool(sqvm, g_MasterServerManager->m_bScriptAuthenticatingWithGameServer);
	return SQRESULT_NOTNULL;
}

// bool function NSWasAuthSuccessful()
SQRESULT SQ_WasAuthSuccessful(void* sqvm)
{
	g_pUISquirrel->pushbool(sqvm, g_MasterServerManager->m_bSuccessfullyAuthenticatedWithGameServer);
	return SQRESULT_NOTNULL;
}

// void function NSConnectToAuthedServer()
SQRESULT SQ_ConnectToAuthedServer(void* sqvm)
{
	if (!g_MasterServerManager->m_bHasPendingConnectionInfo)
	{
		g_pUISquirrel->raiseerror(
			sqvm, fmt::format("Tried to connect to authed server before any pending connection info was available").c_str());
		return SQRESULT_ERROR;
	}

	RemoteServerConnectionInfo info = g_MasterServerManager->m_pendingConnectionInfo;

	// set auth token, then try to connect
	// i'm honestly not entirely sure how silentconnect works regarding ports and encryption so using connect for now
	R2::Cbuf_AddText(
		R2::Cbuf_GetCurrentPlayer(), fmt::format("serverfilter {}", info.authToken).c_str(), R2::cmd_source_t::kCommandSrcCode);
	R2::Cbuf_AddText(
		R2::Cbuf_GetCurrentPlayer(),
		fmt::format(
			"connect {}.{}.{}.{}:{}",
			info.ip.S_un.S_un_b.s_b1,
			info.ip.S_un.S_un_b.s_b2,
			info.ip.S_un.S_un_b.s_b3,
			info.ip.S_un.S_un_b.s_b4,
			info.port)
			.c_str(),
		R2::cmd_source_t::kCommandSrcCode);

	g_MasterServerManager->m_bHasPendingConnectionInfo = false;
	return SQRESULT_NULL;
}

// void function NSTryAuthWithLocalServer()
SQRESULT SQ_TryAuthWithLocalServer(void* sqvm)
{
	// do auth request
	g_MasterServerManager->AuthenticateWithOwnServer(R2::g_pLocalPlayerUserID, g_MasterServerManager->m_sOwnClientAuthToken);

	return SQRESULT_NULL;
}

// void function NSCompleteAuthWithLocalServer()
SQRESULT SQ_CompleteAuthWithLocalServer(void* sqvm)
{
	// literally just set serverfilter
	// note: this assumes we have no authdata other than our own
	R2::Cbuf_AddText(
		R2::Cbuf_GetCurrentPlayer(),
		fmt::format("serverfilter {}", g_ServerAuthenticationManager->m_authData.begin()->first).c_str(),
		R2::cmd_source_t::kCommandSrcCode);

	return SQRESULT_NULL;
}

ON_DLL_LOAD_CLIENT_RELIESON("client.dll", ScriptServerBrowser, ClientSquirrel, [](HMODULE baseAddress)
{
	g_pUISquirrel->AddFuncRegistration("bool", "NSIsMasterServerAuthenticated", "", "", SQ_IsMasterServerAuthenticated);
	g_pUISquirrel->AddFuncRegistration("void", "NSRequestServerList", "", "", SQ_RequestServerList);
	g_pUISquirrel->AddFuncRegistration("bool", "NSIsRequestingServerList", "", "", SQ_IsRequestingServerList);
	g_pUISquirrel->AddFuncRegistration("bool", "NSMasterServerConnectionSuccessful", "", "", SQ_MasterServerConnectionSuccessful);
	g_pUISquirrel->AddFuncRegistration("int", "NSGetServerCount", "", "", SQ_GetServerCount);
	g_pUISquirrel->AddFuncRegistration("void", "NSClearRecievedServerList", "", "", SQ_ClearRecievedServerList);

	g_pUISquirrel->AddFuncRegistration("string", "NSGetServerName", "int serverIndex", "", SQ_GetServerName);
	g_pUISquirrel->AddFuncRegistration("string", "NSGetServerDescription", "int serverIndex", "", SQ_GetServerDescription);
	g_pUISquirrel->AddFuncRegistration("string", "NSGetServerMap", "int serverIndex", "", SQ_GetServerMap);
	g_pUISquirrel->AddFuncRegistration("string", "NSGetServerPlaylist", "int serverIndex", "", SQ_GetServerPlaylist);
	g_pUISquirrel->AddFuncRegistration("int", "NSGetServerPlayerCount", "int serverIndex", "", SQ_GetServerPlayerCount);
	g_pUISquirrel->AddFuncRegistration("int", "NSGetServerMaxPlayerCount", "int serverIndex", "", SQ_GetServerMaxPlayerCount);
	g_pUISquirrel->AddFuncRegistration("string", "NSGetServerID", "int serverIndex", "", SQ_GetServerID);
	g_pUISquirrel->AddFuncRegistration("bool", "NSServerRequiresPassword", "int serverIndex", "", SQ_ServerRequiresPassword);
	g_pUISquirrel->AddFuncRegistration("int", "NSGetServerRequiredModsCount", "int serverIndex", "", SQ_GetServerRequiredModsCount);
	g_pUISquirrel->AddFuncRegistration(
		"string", "NSGetServerRequiredModName", "int serverIndex, int modIndex", "", SQ_GetServerRequiredModName);
	g_pUISquirrel->AddFuncRegistration(
		"string", "NSGetServerRequiredModVersion", "int serverIndex, int modIndex", "", SQ_GetServerRequiredModVersion);

	g_pUISquirrel->AddFuncRegistration(
		"void", "NSTryAuthWithServer", "int serverIndex, string password = \"\"", "", SQ_TryAuthWithServer);
	g_pUISquirrel->AddFuncRegistration("bool", "NSIsAuthenticatingWithServer", "", "", SQ_IsAuthComplete);
	g_pUISquirrel->AddFuncRegistration("bool", "NSWasAuthSuccessful", "", "", SQ_WasAuthSuccessful);
	g_pUISquirrel->AddFuncRegistration("void", "NSConnectToAuthedServer", "", "", SQ_ConnectToAuthedServer);

	g_pUISquirrel->AddFuncRegistration("void", "NSTryAuthWithLocalServer", "", "", SQ_TryAuthWithLocalServer);
	g_pUISquirrel->AddFuncRegistration("void", "NSCompleteAuthWithLocalServer", "", "", SQ_CompleteAuthWithLocalServer);
})
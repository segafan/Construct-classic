// Include StdAfx
#include "StdAfx.h"

// Include any used common ACE definitions.
#ifdef RUN_ONLY
#include "..\..\Common\CommonAceDef.hpp"
#endif

//////////////////////////////////////////////////////////////////////////////////
// ACE table
//////////////////////////////////////////////////////////////////////////////////

void DefineACES(MicroAceTime* at)
{
	// Define your ACE tables here.
	// Use ADDPARAM before each ADDACT/ADDCND/ADDEXP to add parameters.
	// See the documentation for full details.

	// Note in the display string, %o is your object icon and %0-%9 make up parameters.
	// Note the script name should be a name suitable for the routine as if it were a member function in a
	// scripting language, eg. "Clear Stored Items" -> "ClearStoredItems"

	/////////////////////////////
	// Conditions
	// Format:
	// ADDCND(List name, Category, Display string, Function address, Script name, Flags)
	//ADDCND("OnDisconnect", "My category", "%o My condition", &ExtObject::cMyCondition, "MyCondition", CND_TRIGGERED);
	ADDCND("On disconnect", "Triggers", "%o On disconnect", &ExtObject::cOnDisconnect, "OnDisconnect", CND_TRIGGERED);
	ADDCND("On connection lost", "Triggers", "%o On connection Lost", &ExtObject::cOnConnectionLost, "OnConnectionLost", CND_TRIGGERED);
	ADDCND("On banned", "Triggers", "%o On banned", &ExtObject::cOnBanned, "OnBanned", CND_TRIGGERED);
	ADDCND("On connect", "Triggers", "%o On connect", &ExtObject::cOnConnect, "OnConnect", CND_TRIGGERED);
	ADDCND("On server full", "Triggers", "%o On server full", &ExtObject::cOnServerFull, "OnServerFull", CND_TRIGGERED);
	ADDCND("On player disconnect", "Triggers", "%o On player disconnect", &ExtObject::cOnPlayerDisconnect, "OnPlayerDisconnect", CND_TRIGGERED);
	ADDCND("On player connect", "Triggers", "%o On player connect", &ExtObject::cOnPlayerConnect, "OnPlayerConnect", CND_TRIGGERED);
	ADDPARAM(PARAM_STRING, "Message", "The text that was sent");
	ADDCND("On message", "Messages", "%o On message %0", &ExtObject::cOnMessage, "OnMessage", CND_TRIGGERED);
	ADDCND("On any message", "Messages", "%o On any message", &ExtObject::cOnAnyMessage, "OnAnyMessage", CND_TRIGGERED);
	ADDCND("On player here", "Triggers", "%o On player here", &ExtObject::cOnPlayerHere, "OnPlayerHere", CND_TRIGGERED);
	/////////////////////////////
	// Actions
	// Format:
	// ADDCND(List name, Category, Display string, Function address, Script name, Flags)
	ADDPARAMDEF(PARAM_STRING, "Host", "The IP address of the host", "\"localhost\"");
	ADDPARAMDEF(PARAM_VALUE, "Host Port", "The port we are connecting to", "1234");
	ADDPARAMDEF(PARAM_VALUE, "Local Port", "The port we are listening on", "0");
	ADDPARAMDEF(PARAM_STRING, "Password", "Here is an example parameter.", "\"construct\"");
	ADDACT("Connect", "Client", "Connect to %0 on port %1 using local port %2 (password %3)", &ExtObject::aConnect, "Connect", 0);
	ADDACT("Disconnect", "Client", "Disconnect", &ExtObject::aDisconnect, "Disconnect", 0);
	ADDPARAM(PARAM_UNDEFINED, "Param", "Any number of string for the param");
	ADDACT("Add parameter", "Message", "Add parameter %0", &ExtObject::aAddParameter, "AddParameter", 0);
	ADDPARAM(PARAM_STRING, "Message", "The message that will be sent and recieved by others");
	ADDACT("Send message", "Message", "Send message %0", &ExtObject::aSendMessage, "SendMessage", 0);
	ADDPARAMDEF(PARAM_VALUE, "Host port", "The port we are listening on", "1234");
	ADDPARAMDEF(PARAM_STRING, "Password", "Here is an example parameter.", "\"construct\"");
	ADDACT("Host server", "Server", "Host server on port %0", &ExtObject::aHostServer, "HostServer", 0);
	ADDACT("Forward message", "Server", "Forward message to other clients", &ExtObject::aForwardMessage, "ForwardMessage", 0);

	/////////////////////////////
	// Expressions
	// ADDEXP(List name, Category, Display string, Function address, Flags)
	ADDEXP("Get message", "Message", "Message", &ExtObject::eMessage, RETURN_STRING);
	ADDEXP("Number of params", "Message", "NumberOfParams", &ExtObject::eNumberOfParams, RETURN_INTEGER);
	ADDPARAM(PARAM_VALUE, "1", "Parameter");
	ADDEXP("Get param", "Message", "Param", &ExtObject::eParam, 0);
	ADDEXP("Get IP", "Information", "IP", &ExtObject::eGetIP, RETURN_STRING);
	ADDEXP("Get User ID", "Information", "UserID", &ExtObject::eGetUserID, RETURN_STRING);
	ADDEXP("Get Remote User ID", "Information", "RemoteUserID", &ExtObject::eGetRemoteUserID, RETURN_STRING);

#include "..\..\Common\CommonAceTable.hpp"
}



// Main.h
// Definitions and classes for the object workings

// Only include once
#ifndef MAIN_H
#define MAIN_H

#define MINIMUM_VERSION	1

// IDE FLAGS
// Can be a combination of:
//OF_NOSIZE	- Extension cannot be resized at edittime
//OF_NODRAW - Extension does not display anything
//OF_ALLOWANGLES - Can be rotated at edittime
//OF_PRIVATEVARIABLES - Uses private variables
//OF_MOVEMENTPLUGIN - Is a movement plugin
//OF_MOVEMENTS - Allow movement plugins to be added
//OF_EFFECTS - Allow display effects
//OF_NOCOMMONDEBUG - Don't show 'Common' field (X/Y/W/H/Angle/Opacity) in debugger
//OF_NODEBUG - Don't show at all in debugger
//OF_UNDEFINEDEXPRESSIONS - Allow ReturnUndefinedExpression to be called
#define IDE_FLAGS 	OF_NOCOMMONDEBUG | OF_NODRAW

#include "..\..\Common\ExpReturn.hpp"

#define OBJECTRECT CRect(editObject->objectX, editObject->objectY, editObject->objectX + editObject->objectWidth, editObject->objectY + editObject->objectHeight)

//////////// RUNTIME OBJECT ////////////
// Add any member functions or data you want to this class.
// Your extension inherits CRunObject.  See the definition
// for the default member values you have access to.

struct condition
{
	enum e
	{
		cOnDisconnect,
		cOnConnectionLost,
		cOnBanned,
		cOnConnect,
		cOnServerFull,
		cOnPlayerDisconnect,
		cOnPlayerConnect,
		cOnMessage,
		cOnAnyMessage,
		cOnPlayerHere,
	};
};

class Player
{
public:
	Player(){}
	Player(RakNet::RakNetGUID guid, int id) : guid(guid), id(id) {}

	Player(RakNet::BitStream& stream)
	{
		Serialize(false, stream);
	}

	void Serialize(bool write, RakNet::BitStream& stream)
	{
		stream.Serialize(write, guid);
		stream.Serialize(write, id);
	}

	RakNet::RakNetGUID guid;
	int id;
};

class ExtObject : public CRunObject
{
public:
	// Constructor
	ExtObject(initialObject* editObject, VRuntime* pVRuntime);
	// Destructor (called when Construct destroys the object)
	~ExtObject();

	IRenderer* const renderer;

	//////////////////////////
	// OnFrame: called once per frame just before the event list
	// OnFrame2: called once per frame just after the event list
	BOOL		OnFrame();
	BOOL		OnFrame2();
	// Draw: called when Construct wants you to render your object.
	void		Draw();
	// WindowProc is called if your extension creates a window and calls RegisterWindow().
	LRESULT		WindowProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
	BOOL		PreTranslateMessage(MSG* msg);
	// Return default expression editor value and undefined expressions when enabled.
	long		ReturnDefaultValue(LPVAL params, ExpReturn& ret);
	long		ReturnUndefinedExpression(CString& expName, LPVAL params, ExpReturn& ret);
	// Called just after the constructor when your object has been prepared.
	void		OnCreate();
	// Called when a frame changes and your object is global
	void		OnFrameChange(int oldFrame, int newFrame);
	// Called when your object should save/load itself at runtime
	void		Serialize(bin& ar);
	// Data functions
	long		GetData(int id, void* param);
	long		CallFunction(int id, void* param);
	// Debugging
	void		DebuggerUpdateDisplay(ExpStore*& pPrivateVars);
	void		OnDebuggerValueChanged(const char* name, const char* value);

	////////////////////////////////////////////////////
	// ACTIONS, CONDITIONS AND EXPRESSIONS
#include "..\..\Common\CommonAceDecl.hpp"

	long cOnDisconnect(LPVAL params);
	long cOnConnectionLost(LPVAL params);
	long cOnBanned(LPVAL params);
	long cOnConnect(LPVAL params);
	long cOnServerFull(LPVAL params);
	long cOnPlayerDisconnect(LPVAL params);
	long cOnPlayerConnect(LPVAL params);
	long cOnMessage(LPVAL params);
	long cOnAnyMessage(LPVAL params);
	long cOnPlayerHere(LPVAL params);

	long aConnect(LPVAL params);
	long aDisconnect(LPVAL params);
	long aAddParameter(LPVAL params);
	long aSendMessage(LPVAL params);
	long aHostServer(LPVAL params);
	long aForwardMessage(LPVAL params);

	long eMessage(LPVAL params, ExpReturn& ret);
	long eNumberOfParams(LPVAL params, ExpReturn& ret);
	long eParam(LPVAL params, ExpReturn& ret);
	long eGetIP(LPVAL params, ExpReturn& ret);
	long eGetUserID(LPVAL params, ExpReturn& ret);
	long eGetRemoteUserID(LPVAL params, ExpReturn& ret);

	void processPackets();
	////////////////////////////////////////////////////
	// Data

	// Use when private variables (OF_PRIVATEVARIABLES) are enabled.
	//vector<ExpStore> privateVars;

	RakNet::Packet *packet;
	RakNet::RakPeerInterface *raknet;
	
	vector<ExpStore> paramList; // For the send
	vector<RakNet::RakNetGUID> remoteUserID;

	int userID; //user ID..peers ask the server for an ID


	// The server can use this
	int uniqueUserID; 
	int getUniqueUserID()
	{
		return uniqueUserID++;
	}

	typedef map<RakNet::RakNetGUID, Player> PlayerMap;
	PlayerMap players;


	CString message;
	vector<ExpStore> messageParamList; // For the received
	bool isConnected;

	bool server;

};



//////////// EDITTIME INFO ////////////
class EditExt
{
public:

	///////////////////////////
	// Class data
	EditExt(class VEditTime* pVEditTime, class editInfo* pEInfo);
	~EditExt();

	void Draw();
	int  GetMenuIcons(int ACEMenu) {}
	void Initialize();
	void OnPut();
	void OnRemoved();
	int  OnShowACEMenu(TABLE ACEType) {}
	BOOL OnSizeObject();
	void Serialize(bin& ar);
	void OnPropertiesUpdate();
	void GetAnimationHandle(int& handle);

	class VEditTime* pEditTime;	// Pointer to Virtual Edittime
	class editInfo*  pInfo;		// Pointer to object edittime info

	///////////////////////////
	// Your edittime extension data goes here
	int iTexture;		// DX texture

	int myValue;
};

// Internal stuff include
#include "..\..\Common\Internal.hpp"
#include "..\..\Common\Properties.h"

// Only include once
#endif
// Include StdAfx
#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
// Actions
//////////////////////////////////////////////////////////////////////////////////




long ExtObject::aConnect(LPVAL params)
{
	string ip = params[0].GetString();
	int remotePort = params[1].GetInt();	
	int localPort = params[2].GetInt();
	string password = params[3].GetString();

	RakNet::SocketDescriptor socketDescriptor(localPort,0);
	if (raknet->Startup(1, &socketDescriptor, 1)==RakNet::RAKNET_STARTED)
	{
		raknet->SetOccasionalPing(true);
		if (raknet->Connect(ip.c_str(), remotePort, password.c_str(), password.length())==RakNet::CONNECTION_ATTEMPT_STARTED)
			printf("Connecting...\nNote: if the password is wrong the other system will ignore us.\n");
		else
			printf("Connect call failed.\n");
	}
	else
		printf("Initialize call failed.\n");	

	return 0;
}
long ExtObject::aDisconnect(LPVAL params)
{
	raknet->Shutdown(500, 0);
	return 0;
}

long ExtObject::aAddParameter(LPVAL params)
{
	paramList.push_back(params[0]);
	return 0;
}



long ExtObject::aSendMessage(LPVAL params)
{
	RakNet::MessageID useTimeStamp; // Assign this to ID_TIMESTAMP
	RakNet::Time timeStamp; // Put the system time in here returned by RakNet::GetTime()
	RakNet::MessageID typeId; // This will be assigned to a type I've added after ID_USER_PACKET_ENUM, lets say ID_SET_TIMED_MINE
	useTimeStamp = ID_TIMESTAMP;
	timeStamp = RakNet::GetTime();
	typeId = ID_CONSTRUCT_MESSAGE;
	RakNet::BitStream stream;
	stream.Write(useTimeStamp);
	stream.Write(timeStamp);
	stream.Write(typeId);
	// Assume we have a Mine* mine object
	stream.Write( RakNet::RakString(params[0].GetString()) );
	stream.Write( (int)paramList.size() );

	for( int i = 0; i < paramList.size(); i++)
	{
		ExpStore& s = paramList[i];
		stream.Write(s.Type()); 

		switch(s.Type())
		{
		case EXPTYPE_INTEGER:
			stream.Write(s.GetInt());
			break;
		case EXPTYPE_FLOAT:
			stream.Write(s.GetFloat());
			break;
		case EXPTYPE_STRING:
			RakNet::RakString string;
			string = s.GetString();
			stream.Write( string );
			break;
		}
	}


	stream.Write( raknet->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS));


	raknet->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);

	//stream.Write(mine->GetNetworkID()); // In the struct this is NetworkID networkId
	//stream.Write(mine->GetOwner()); // In the struct this is SystemAddress systemAddress

	paramList.clear();

	return 0;
}

long ExtObject::aHostServer(LPVAL params)
{
	server = true;

	int port = params[0].GetInt();
	string password = params[1].GetString();

	raknet->SetIncomingPassword(password.c_str(), password.length());
	raknet->SetTimeoutTime(30000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);

	RakNet::SocketDescriptor socketDescriptor( port, 0);
	bool b = raknet->Startup(4, &socketDescriptor, 1 ) == RakNet::RAKNET_STARTED;
	raknet->SetMaximumIncomingConnections(20);

	return 0;
}

long ExtObject::aForwardMessage(LPVAL params)
{
	RakNet::BitStream stream(packet->data, packet->length, false);
	raknet->Send(&stream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->guid, true);
	
	return 0;
}

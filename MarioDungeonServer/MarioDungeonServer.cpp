#include "MarioDungeonServer.h"

MarioDungeonServer::MarioDungeonServer(EventLoop *loop,const InetAddress& listenAddr):_server(loop,listenAddr,"MarioDungeonServer")
{
	_server.setConnectionCallback(std::bind(&Codec::onConnection,&_codec,_1));
	_server.setMessageCallback(std::bind(&MarioDungeonServer::onMessage,this,_1,_2,_3));
}
void MarioDungeonServer::onConnection(const TcpConnectionPtr& conn)
{
	
}
void MarioDungeonServer::onMessage(const TcpConnectionPtr& conn,Buffer* buf,Timestamp time)
{
	string msg(buf->retrieveAllAsString());
	//cout<<"retrieveAllString: "<<msg<<endl;
	splitRequestMsg(msg);
	for(int i=0;i<(int)RqMsgList.size();++i)
	{
		//cout<<"RqMsg:"<<RqMsgList[i]<<endl;
		string responseMsg=_codec.deCodeMessage(conn,RqMsgList[i],time);
		cout<<"ResponseMsg:"<<responseMsg<<endl;
		if(responseMsg!="ChatRoomMessage")
			conn->send(responseMsg);
	}
	RqMsgList.clear();
}
void MarioDungeonServer::start()
{
	_server.start();
}
void MarioDungeonServer::splitRequestMsg(string& msg)
{
	auto it=msg.find_first_of(ENDFLAG);
	if(it==0)
		return;
	string rqstr;
	RqMsgList.push_back(msg.substr(0,it));
	rqstr=msg.substr(it+1);
	while(rqstr.size()!=0)
	{
		auto idx=rqstr.find_first_of(ENDFLAG);
		if(idx==0)
			return;
		RqMsgList.push_back(rqstr.substr(0,idx));
		rqstr=rqstr.substr(idx+1);
	}
}

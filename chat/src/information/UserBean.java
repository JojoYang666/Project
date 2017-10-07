package information;

import java.io.Serializable;
import java.util.HashSet;

public class UserBean implements Serializable{
	private int type;//0.上线   -1下线  1上下线跟新  3上线  2聊天状态
	private String info;
	private String timer;
	private String name;
	private String name_diaplay;
	private String filename;
	private HashSet<String> clients;
	private String private_talk_name=null;
	
	public void SetPrivateTN(String Talk_name)
	{
		this.private_talk_name=Talk_name;
	}
	
	public String GetPrivateTN()
	{
		return private_talk_name;
	}
	public HashSet<String> getClients() {
		return clients;
	}

	public void setClients(HashSet<String> clients) {
		this.clients = clients;
	}
	
	public String getInfo() {
		return info;
	}

	public void setInfo(String info) {
		this.info = info;
	}
	
	public String getTimer() {
		return timer;
	}

	public void setTimer(String timer) {
		this.timer = timer;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}
	
	public String getName_diaplay() {
		return name_diaplay;
	}

	public void setName_dispaly(String name) {
		this.name_diaplay = name;
	}
	
	public void setFileName(String fileName) {
		this.filename = fileName;
	}
	
	public void setType(int type)
	{
		this.type=type;
	}
	
	public int getType()
	{
		return type;
	}

}

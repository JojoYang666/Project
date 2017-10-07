package information;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class ClientBean {

	private String name;
	private Socket socket;
	private ObjectOutputStream dos;
	private ObjectInputStream dis;
	
	public ObjectOutputStream getObjectOutputstream()
	{
		return dos;
	}
	
	public void setObjectOutputstream(ObjectOutputStream dos)
	{
		this.dos=dos;
	}
	
	
	public ObjectInputStream getObjectInputstream()
	{
		return dis;
	}
	
	public void setObjectInputstream(ObjectInputStream dis)
	{
		this.dis=dis;
	}
	
	
	public String getname()
	{
		return name;
	}
	
	public void setname(String name)
	{
		this.name=name;
	}
	
	public Socket getSocket()
	{
		return socket;
	}
	
	public void setsocket(Socket socket)
	{
		this.socket=socket;
	}
}

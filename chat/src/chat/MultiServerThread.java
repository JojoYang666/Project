package chat;

//MultiServerThread.java
import information.ClientBean;
import information.UserBean;

import java.net.*;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.io.*;

class MultiServerThread extends Thread 
{
  private Socket s = null;
  boolean started =false;
  private ObjectInputStream is=null;
  private ObjectOutputStream os=null;

  

	// onlines=;
		
	
	
  public MultiServerThread(Socket socket) 
	{
      super("MultiServerThread");
      this.s = socket;
      started=true;
  }

  public void run() 
	{
	  try{
		  
			is=new ObjectInputStream(s.getInputStream());
			 os=new ObjectOutputStream(s.getOutputStream());	 
			//Socket s=ss.accept();
			Client c=new Client(s,is,os);
System.out.println("a client connected");
			new Thread(c).start();
			//clients.add(c);
			//dis.close();
		
		  } catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	
		} 
		
	}
	
  
  
  class  Client implements Runnable
	{
		private Socket s;
		private ObjectInputStream dis=null;
		private ObjectOutputStream dos=null;
		private boolean bconnected=false;
		
		private UserBean bean;
		public Client(Socket s,ObjectInputStream dis,ObjectOutputStream dos)
		{
			this.s=s;
			this.dis=dis;
			this.dos=dos;
//			try {
//				dis=new ObjectInputStream(s.getInputStream());
//				dos=new ObjectOutputStream(s.getOutputStream());
//				bconnected=true;
//			} catch (IOException e) {
//				e.printStackTrace();
//			}
		}

		
		public void send(UserBean bean) 
		{
			Collection<ClientBean> clients=MultiServer.onlines.values();
			Iterator<ClientBean> it=clients.iterator();
		    ObjectOutputStream oos;
			while(it.hasNext()){
		    	ObjectInputStream c=it.next().getObjectInputstream();
		    	try {
					//oos=new ObjectOutputStream(c);
					dos.writeObject(bean);
			    	dos.flush();
		    	} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
		    }
//			try {
//				dos.writeObject(bean);
//			} catch (IOException e) {
//				clients.remove(this);
//				System.out.println("对方退出了  我从list里面去掉了");
//			}
		}
		
		@Override
		public void run() {
	
			try{
			while(bconnected)
			{
				UserBean bean;
			
					bean = (UserBean)dis.readObject();
				//上线
					if(bean.getType()==0)
					{
					ClientBean cbean=new ClientBean();
					cbean.setname(bean.getName());
					cbean.setObjectInputstream(dis);;
					// 添加在线用户
					MultiServer.onlines.put(bean.getName(), cbean);
					// 创建服务器的catbean，并发送给客户端
					UserBean serverbean=new UserBean();
					serverbean.setType(1);
					// 通知所有客户有人上线
					HashSet<String> set = new HashSet<String>();
					// 客户昵称
					set.addAll(MultiServer.onlines.keySet());
					serverbean.setClients(set);
						send(serverbean);
					
					break;
					}
					else if(bean.getType()==-1)//下线
					{
						UserBean serverBean = new UserBean();
						MultiServer.onlines.remove(bean.getName());
						serverBean.setType(1);
						HashSet<String> set = new HashSet<String>();
						set.addAll(MultiServer.onlines.keySet());
						serverBean.setClients(set);
						
							send(serverBean);
						break;
						
					}
					else if(bean.getType()==1||bean.getType()==2)//聊天
					{
						UserBean serverbean=new UserBean();
						bean.setType(2);
						serverbean.setClients(bean.getClients());
						serverbean.setInfo(bean.getInfo());
						serverbean.setName(bean.getName());
						serverbean.setTimer(bean.getTimer());
						serverbean.setType(bean.getType());
							send(serverbean);
					
					}
			
//			if(bean.getType()==-1)
//				bean.setType(1);e
			}
		}
			catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
				e.printStackTrace();
			}
		catch(EOFException e)
			{
			System.out.println("Client closed");
			}
		catch (IOException e) {			
			e.printStackTrace();
			}
			finally
		{
			try {
				if(dis!=null)dis.close();
				if(dos!=null)dos.close();
				if(s!=null)s.close();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
			
			
		} 
			
			}
			
		}


package chat;
import information.ClientBean;
import information.UserBean;

import java.awt.datatransfer.ClipboardOwner;
import java.util.*;
import java.io.*;
import java.net.*;

import org.omg.CORBA.PUBLIC_MEMBER;





public class Chatserver {

	ServerSocket ss=null;
	boolean started =false;
	public static HashMap<String, ClientBean> onlines;
	public static HashMap<String, ClientBean> onlines_private;
	HashSet<String> set;
	HashSet<String> set_private;


	public static void main(String[] args)
	{
		new Chatserver().start();
	}

	public void start()
	{
		try {
			 ss=new ServerSocket(8888);
			 onlines=new HashMap<String, ClientBean>();
			 onlines_private=new HashMap<String, ClientBean>();
			 started=true;
		}catch(BindException e)
		{
			System.out.println("端口使用中");
			System.out.println("请关掉相关程序并重新运行服务器");
			System.exit(0);
		}
		catch(IOException e){
			e.printStackTrace();
		}
			try
			{

			while(started)
			{
				Socket s=ss.accept();
				Client c=new Client(s);
System.out.println("a client connected");
				new Thread(c).start();
			}
		} catch(IOException e)
			{
			e.printStackTrace();
		}finally
		{
			try {
				ss.close();
			} catch (IOException e) {
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
		public Client(Socket s)
		{
			this.s=s;
			try {
				dis=new ObjectInputStream(s.getInputStream());
				dos=new ObjectOutputStream(s.getOutputStream());
				bconnected=true;
			} catch (IOException e) {
				e.printStackTrace();
			}
		}


		public void send(UserBean bean)
		{

			Collection<ClientBean> clients = onlines.values();
			Iterator<ClientBean> it = clients.iterator();

			while (it.hasNext()) {
				ObjectOutputStream oos=it.next().getObjectOutputstream();				
				try {
		
					bean.setClients(set);
					oos.writeObject(bean);
					oos.flush();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}

		public void sendAlone(UserBean bean)
		{
			if(bean.getType()==3)
			{
				ClientBean cbean=onlines.get(bean.getName());
			//ClientBean cbean=onlines.get(bean.getName());
			ObjectOutputStream oos=cbean.getObjectOutputstream();
			try {
				oos.writeObject(bean);
				oos.flush();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			}
			else if(bean.getType()==4)
			{
				ClientBean cbean_my=onlines_private.get(bean.getName());
				ClientBean cbean_opposite=onlines_private.get(bean.GetPrivateTN());
				ObjectOutputStream oos_my=cbean_my.getObjectOutputstream();
//System.out.println("我的名字是"+bean.getName()+"我的输出流是"+oos_my);
				ObjectOutputStream oos_opposite=cbean_opposite.getObjectOutputstream();
//System.out.println("我的名字是"+bean.GetPrivateTN()+"我的输出流是"+oos_opposite);
				
				try {
//System.out.println("我要传消息了");
					oos_my.writeObject(bean);
					oos_opposite.writeObject(bean);
//System.out.println("我要传消息了");
					oos_my.flush();
					oos_opposite.flush();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				
			}
			else if(bean.getType()==6)
			{
				if(onlines_private.isEmpty())
				{
					return;
				}
				ClientBean cbean_opposite=onlines_private.get(bean.GetPrivateTN());
				ObjectOutputStream oos_opposite=cbean_opposite.getObjectOutputstream();
				try {
					oos_opposite.writeObject(bean);
					oos_opposite.flush();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}	
				
			}
		}

		
		

		@Override
		public void run() {
			
			try{
				while(bconnected)
				{
					
					
						bean = (UserBean)dis.readObject();

						
					//上线
						if(bean.getType()==0)
					{
					ClientBean cbean=new ClientBean();
					cbean.setname(bean.getName());
						cbean.setsocket(s);

						cbean.setObjectInputstream(dis);
						cbean.setObjectOutputstream(dos);
					// 添加在线用户
						onlines.put(bean.getName(), cbean);
						// 创建服务器的catbean，并发送给客户端
						UserBean serverbean=new UserBean();
					serverbean.setType(1);
						// 通知所有客户有人上线
					 set = new HashSet<String>();
					// 客户昵称
					set.addAll(onlines.keySet());
					serverbean.setClients(set);

						send(serverbean);
					
					
						}
						else if(bean.getType()==-1)//下线
						{
							UserBean serverBean = new UserBean();
							onlines.remove(bean.getName());
							serverBean.setType(1);
							set = new HashSet<String>();
							set.addAll(onlines.keySet());
							serverBean.setClients(set);
								send(serverBean);
								
						}
						else if(bean.getType()==2)//聊天
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
						else if(bean.getType()==3)//私聊更新对方
						{
							String temp;
							temp=bean.GetPrivateTN();
							bean.SetPrivateTN(bean.getName());
							bean.setName(temp);
							sendAlone(bean);
						}
						
						else if(bean.getType()==4)//私聊进行时
						{
							    
//System.out.println("我正在和"+bean.GetPrivateTN()+"聊天");
//System.out.println("我的名字是"+bean.getName());
//System.out.println("我的对象的名字是"+bean.GetPrivateTN());
								sendAlone(bean);
						}
						else if(bean.getType()==5)//私聊  存储更新
						{
							ClientBean cbean=new ClientBean();
							cbean.setname(bean.getName());
								cbean.setsocket(s);

								cbean.setObjectInputstream(dis);
								cbean.setObjectOutputstream(dos);
//System.out.println("我的名字是"+bean.getName()+"我的输出流是"+dos);
							// 添加在线用户
								//onlines.put(bean.getName(), cbean);
								onlines_private.put(bean.getName(), cbean);
//System.out.println("我的名字是"+bean.getName());
//System.out.println("我的对象的名字是"+bean.GetPrivateTN());
								//set_private = new HashSet<String>();
								//set_private.addAll(onlines_private.keySet());
//System.out.println("我进来了要更新列表了");
								
						}
						else if(bean.getType()==6)//下线
						{
							onlines_private.remove(bean.getName());
System.out.println("我要下线了，我的名字是"+bean.getName());
System.out.println(onlines_private);
							sendAlone(bean);
						}
						

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



	}

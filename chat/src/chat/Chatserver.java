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
			System.out.println("�˿�ʹ����");
			System.out.println("��ص���س����������з�����");
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
//System.out.println("�ҵ�������"+bean.getName()+"�ҵ��������"+oos_my);
				ObjectOutputStream oos_opposite=cbean_opposite.getObjectOutputstream();
//System.out.println("�ҵ�������"+bean.GetPrivateTN()+"�ҵ��������"+oos_opposite);
				
				try {
//System.out.println("��Ҫ����Ϣ��");
					oos_my.writeObject(bean);
					oos_opposite.writeObject(bean);
//System.out.println("��Ҫ����Ϣ��");
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

						
					//����
						if(bean.getType()==0)
					{
					ClientBean cbean=new ClientBean();
					cbean.setname(bean.getName());
						cbean.setsocket(s);

						cbean.setObjectInputstream(dis);
						cbean.setObjectOutputstream(dos);
					// ��������û�
						onlines.put(bean.getName(), cbean);
						// ������������catbean�������͸��ͻ���
						UserBean serverbean=new UserBean();
					serverbean.setType(1);
						// ֪ͨ���пͻ���������
					 set = new HashSet<String>();
					// �ͻ��ǳ�
					set.addAll(onlines.keySet());
					serverbean.setClients(set);

						send(serverbean);
					
					
						}
						else if(bean.getType()==-1)//����
						{
							UserBean serverBean = new UserBean();
							onlines.remove(bean.getName());
							serverBean.setType(1);
							set = new HashSet<String>();
							set.addAll(onlines.keySet());
							serverBean.setClients(set);
								send(serverBean);
								
						}
						else if(bean.getType()==2)//����
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
						else if(bean.getType()==3)//˽�ĸ��¶Է�
						{
							String temp;
							temp=bean.GetPrivateTN();
							bean.SetPrivateTN(bean.getName());
							bean.setName(temp);
							sendAlone(bean);
						}
						
						else if(bean.getType()==4)//˽�Ľ���ʱ
						{
							    
//System.out.println("�����ں�"+bean.GetPrivateTN()+"����");
//System.out.println("�ҵ�������"+bean.getName());
//System.out.println("�ҵĶ����������"+bean.GetPrivateTN());
								sendAlone(bean);
						}
						else if(bean.getType()==5)//˽��  �洢����
						{
							ClientBean cbean=new ClientBean();
							cbean.setname(bean.getName());
								cbean.setsocket(s);

								cbean.setObjectInputstream(dis);
								cbean.setObjectOutputstream(dos);
//System.out.println("�ҵ�������"+bean.getName()+"�ҵ��������"+dos);
							// ��������û�
								//onlines.put(bean.getName(), cbean);
								onlines_private.put(bean.getName(), cbean);
//System.out.println("�ҵ�������"+bean.getName());
//System.out.println("�ҵĶ����������"+bean.GetPrivateTN());
								//set_private = new HashSet<String>();
								//set_private.addAll(onlines_private.keySet());
//System.out.println("�ҽ�����Ҫ�����б���");
								
						}
						else if(bean.getType()==6)//����
						{
							onlines_private.remove(bean.getName());
System.out.println("��Ҫ�����ˣ��ҵ�������"+bean.getName());
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

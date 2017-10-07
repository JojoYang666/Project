package GUI;
import information.*;

import java.applet.AudioClip;
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.File;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.SocketException;
import java.net.URL;
import java.net.UnknownHostException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Vector;

import function.*;

import javax.swing.AbstractListModel;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JProgressBar;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;




class UUListModel extends AbstractListModel{

	private Vector vs;

	public UUListModel(Vector vs){
		this.vs = vs;
	}

	@Override
	public Object getElementAt(int index) {
		// TODO Auto-generated method stub
		return vs.get(index);
	}

	@Override
	public int getSize() {
		// TODO Auto-generated method stub
		return vs.size();
	}

}




public class chatroom_frame extends JFrame{
	private static final long serialVersionUID = 6129126482250125466L;

	private static JPanel contentPane;
	private static String name;
	private static JTextArea textArea;
	private static AbstractListModel listmodel;
	private static JList list;
	private static JLabel lblNewLabel;
	private static JProgressBar progressBar;
	private static Vector onlines;
	String time;
	Socket s=null;
	ObjectOutputStream dos=null;
	ObjectInputStream dis=null;
	private boolean beconnected=false;


	Thread tRecv=new Thread(new RecvThread());



		public chatroom_frame(String u_name, Socket client )
		{
			name=u_name;
			s=client;
			onlines=new Vector();
			//setTitle(name);
			//�������������
			setResizable(false);
			setDefaultCloseOperation(JFrame.DO_NOTHING_ON_CLOSE);
			setBounds(200, 100, 688, 510);
			contentPane = new JPanel() {
				private static final long serialVersionUID = 1L;
				@Override
				protected void paintComponent(Graphics g) {
					super.paintComponent(g);
					g.drawImage(new ImageIcon("./images/background1.png").getImage(), 0, 0,
							getWidth(), getHeight(), null);
				}

			};
			setContentPane(contentPane);
			contentPane.setLayout(null);
			setTitle("��ӭ"+name+"����������");

			//// ������Ϣ��ʾ����
			JScrollPane scrollPane = new JScrollPane();
			scrollPane.setBounds(10, 10, 410, 300);
			getContentPane().add(scrollPane);

			textArea = new JTextArea();
			textArea.setEditable(false);
			textArea.setLineWrap(true);//�����Զ����й���
			textArea.setWrapStyleWord(true);//������в����ֹ���
			textArea.setFont(new Font("sdf", Font.BOLD, 13));
			scrollPane.setViewportView(textArea);

			// ��������
			JScrollPane scrollPane_1 = new JScrollPane();
			scrollPane_1.setBounds(10, 347, 411, 97);
			getContentPane().add(scrollPane_1);


			final JTextArea textArea_1 = new JTextArea();
			textArea_1.setLineWrap(true);//�����Զ����й���
			textArea_1.setWrapStyleWord(true);//������в����ֹ���
			scrollPane_1.setViewportView(textArea_1);


			// �رհ�ť
			final JButton btnNewButton = new JButton("\u5173\u95ED");
			btnNewButton.setBounds(214, 448, 60, 30);
			getContentPane().add(btnNewButton);

			// ���Ͱ�ť
			JButton btnNewButton_1 = new JButton("\u53D1\u9001");
			btnNewButton_1.setBounds(313, 448, 60, 30);
			getRootPane().setDefaultButton(btnNewButton_1);
			getContentPane().add(btnNewButton_1);

			//String u_name="������";
		// ���߿ͻ��б�
			listmodel = new UUListModel(onlines) ;
			list = new JList(listmodel);
//			list.setCellRenderer(new CellRenderer());
			list.setOpaque(false);
			Border etch = BorderFactory.createEtchedBorder();
//			list.setBorder(BorderFactory.createTitledBorder(etch, "��ǰ��¼�����ǣ�"+u_name, TitledBorder.LEADING, TitledBorder.TOP, new Font(
//					"sdf", Font.BOLD, 20), Color.black));
			list.setBorder(BorderFactory.createTitledBorder(etch, "���߿ͻ���", TitledBorder.LEADING, TitledBorder.TOP, new Font(
					"sdf", Font.BOLD, 20), Color.black));
			JScrollPane scrollPane_2 = new JScrollPane(list);
			scrollPane_2.setBounds(430, 10, 245, 375);
			scrollPane_2.setOpaque(false);
			scrollPane_2.getViewport().setOpaque(false);
    		getContentPane().add(scrollPane_2);

			// �رհ�ť

			btnNewButton.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(ActionEvent e) {
					// TODO Auto-generated method stub
					String str=null;
					UserBean bean=new UserBean();
					bean.setType(-1);
					bean.setInfo(str);
					bean.setName(name);
					bean.setTimer(time);
					try {
						dos.writeObject(bean);
						dos.flush();
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
					disconnect();
    				System.exit(0);


				}
			});



    		// ���Ͱ�ť����
    		btnNewButton_1.addActionListener(new ActionListener() {

				@Override
				public void actionPerformed(ActionEvent e) {
					// TODO Auto-generated method stub
					//List to = list.getSelectedValuesList();
					UserBean clientBean=new UserBean();
					clientBean.setName(name);
					time=Util.getTimer();
					clientBean.setTimer(time);
					clientBean.setType(2);
					String str= textArea_1.getText();
					clientBean.setInfo(str);
					//taContent.setText(str);
					HashSet set = new HashSet();


					textArea_1.setText("");
					try {
						dos.writeObject(clientBean);
						dos.flush();
					} catch (IOException e1) {
						e1.printStackTrace();
					}

				}
			});

    		//���ڹرռ���
    		this.addWindowListener(new WindowAdapter() {

    			@Override
    			public void windowClosing(WindowEvent arg0) {
    				String str=null;
					UserBean bean=new UserBean();
					bean.setType(-1);
					bean.setName(name);
					bean.setInfo(str);
					bean.setTimer(time);
					try {
						dos.writeObject(bean);
						dos.flush();
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
    				disconnect();
    				System.exit(0);
    			}

    		});


    		//�б����
    		list.addMouseListener(new MouseAdapter() {
    		public void mouseClicked(MouseEvent e)
    		{
    			List to =list.getSelectedValuesList();
    			if(e.getClickCount()==2)
    			{
String a=to.toString();
char[] str=a.toCharArray();
//System.out.println(a.length());
char[] s=new char[a.length()-2];
String privateTN="";
for(int i=0;;i++)
	{
	if(str[i+1]==']')
		break;
	s[i]=str[i+1];
	privateTN+=s[i];
	}
//System.out.println(newstr);
    				if(to.toString().contains(name))
    				{JOptionPane
    				.showMessageDialog(getContentPane(), "�������Լ�����Ϣ");
    				return;
    				}
    				UserBean bean=new UserBean();
    				bean.setName(name);
    				bean.SetPrivateTN(privateTN);
    				bean.setType(3);//3����˽��
    				try {
						dos.writeObject(bean);
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
    				Private_chattingroom c=new Private_chattingroom(privateTN,bean);
    				new Thread(c).start();
    				//Thread tRecv=new Thread(new RecvThread());
    				}
    			
    			
    		}
			});
    		
    		
    		
    		
    		
    		
    		//Get_Text(textArea_1);

    		setVisible(true);
    		connect();
    		tRecv.start();
		}



		public void connect()
		{
			try {
//				s=new Socket("127.0.0.1",8888);
				beconnected=true;
				UserBean login=new UserBean();
				login.setName(name);
				login.setType(0);
				login.setInfo("����");
		        dos=new ObjectOutputStream(s.getOutputStream());
		        dis=new ObjectInputStream(s.getInputStream());
		        dos.writeObject(login);
System.out.println(dos);
		        dos.flush();
System.out.println(s);
	System.out.println("connected");
			} catch (UnknownHostException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}


		public void disconnect()
		{
			try {
				dos.close();
				dis.close();
				s.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}


		private class RecvThread implements Runnable
		{

			@Override
			public void run() {
				try
				{
				while(beconnected)
				{
					  UserBean bean;


						bean = (UserBean)dis.readObject();
						//�����߸���
						if(bean.getType()==1)
						{
							onlines.clear();
							HashSet<String> clients=bean.getClients();
							Iterator<String>it=clients.iterator();
							while(it.hasNext())
							{
								String ele=it.next();
System.out.println(ele);
								onlines.add(ele);
							}

							listmodel=new UUListModel(onlines);
							list.setModel(listmodel);
							//break;
						}
						//textArea.setText(textArea.getText()+'\n');
						else if(bean.getType()==2)
						{
							textArea.append(bean.getTimer()+" "+bean.getName()+"  "+"˵"+'\n');
							textArea.append(bean.getInfo()+'\n');
						}
						else if(bean.getType()==3)
				      // c=new Private_chattingroom(bean.getName());
							//Thread tRecv=new Thread(new RecvThread());
						{
							//new Thread(new Private_chattingroom(bean.GetPrivateTN(),bean)).start();
							Private_chattingroom m=new Private_chattingroom(bean.GetPrivateTN(),bean);
		    				new Thread(m).start();
System.out.println("�ҳɹ������߳���");
}

					//System.out.println(str);

				}



			}catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();

			}catch(SocketException e)
			{
				System.out.println("�˳���  bye bye ");
			}
			catch(EOFException e)
			{
				System.out.println("�˳���  bye ");
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}

		}
		}

}

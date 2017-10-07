package GUI;

import function.Util;
import information.UserBean;

import java.awt.Font;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.HashSet;

import javax.swing.*;



public class Private_chattingroom extends JFrame implements Runnable {
	
	private static final long serialVersionUID = 6129126482250125466L;
	private static JPanel contentPane;
	private static JTextArea textArea;	
	private static JLabel lblNewLabel;
	private String privateTN;
	private UserBean bean;
	private String name;
	private String name_display;
	Socket s;
	String time;
	ObjectOutputStream dos=null;
	ObjectInputStream dis=null;
	private boolean beconnected=false;
	public Private_chattingroom(String newstr,UserBean bean) {
		// TODO Auto-generated constructor stub
		privateTN=newstr;
		this.bean=bean;
		name_display=bean.getName();
		name=this.bean.getName()+"private";
		setResizable(false);
		setTitle("亲爱的，你正在和"+privateTN+"聊天"+"  我是"+bean.getName()+"哦！");
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
		
		
	//// 聊天信息显示区域
		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(10, 10, 688, 300);
		getContentPane().add(scrollPane);

		textArea = new JTextArea();
		textArea.setEditable(false);
		textArea.setLineWrap(true);//激活自动换行功能
		textArea.setWrapStyleWord(true);//激活断行不断字功能
		textArea.setFont(new Font("sdf", Font.BOLD, 13));
		scrollPane.setViewportView(textArea);

		// 打字区域
		JScrollPane scrollPane_1 = new JScrollPane();
		scrollPane_1.setBounds(10, 347, 688, 97);
		getContentPane().add(scrollPane_1);
		
		final JTextArea textArea_1 = new JTextArea();
		textArea_1.setLineWrap(true);//激活自动换行功能
		textArea_1.setWrapStyleWord(true);//激活断行不断字功能
		scrollPane_1.setViewportView(textArea_1);
		
		
//		// 关闭按钮
//					final JButton btnNewButton = new JButton("\u5173\u95ED");
//					btnNewButton.setBounds(214, 448, 60, 30);
//					getContentPane().add(btnNewButton);

					// 发送按钮
					JButton btnNewButton_1 = new JButton("\u53D1\u9001");
					btnNewButton_1.setBounds(313, 448, 60, 30);
					getRootPane().setDefaultButton(btnNewButton_1);
					getContentPane().add(btnNewButton_1);

					
					
					
//					// 关闭按钮
//
//					btnNewButton.addActionListener(new ActionListener() {
//
//						@Override
//						public void actionPerformed(ActionEvent e) {
//							// TODO Auto-generated method stub
//							String str=null;
//							UserBean bean=new UserBean();
//							bean.setType(-1);
//							bean.setInfo(str);
//							bean.setName(name);
//							bean.setTimer(time);
//							try {
//								dos.writeObject(bean);
//								dos.flush();
//							} catch (IOException e1) {
//								// TODO Auto-generated catch block
//								e1.printStackTrace();
//							}
//							disconnect();
//		    				System.exit(0);
//
//
//						}
//					});



		    		// 发送按钮监听
		    		btnNewButton_1.addActionListener(new ActionListener() {

						@Override
						public void actionPerformed(ActionEvent e) {
							// TODO Auto-generated method stub
							//List to = list.getSelectedValuesList();
							UserBean clientBean=new UserBean();
							clientBean.setName(name);
							clientBean.setName_dispaly(name_display);
							time=Util.getTimer();
							clientBean.SetPrivateTN(privateTN+"private");
							clientBean.setTimer(time);
							clientBean.setType(4);//私聊进行时
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

		    		//窗口关闭监听
		    		this.addWindowListener(new WindowAdapter() {

		    			@Override
		    			public void windowClosing(WindowEvent arg0) {
							UserBean bean=new UserBean();
							bean.setType(6);//私聊下线
							bean.SetPrivateTN(privateTN+"private");
							bean.setName_dispaly(name_display);
							time=Util.getTimer();
							bean.setName(name);
							bean.setInfo(name_display+"要下线了");
							bean.setTimer(time);
							try {
								dos.writeObject(bean);
								dos.flush();
							} catch (IOException e1) {
								// TODO Auto-generated catch block
								e1.printStackTrace();
							}
		    				disconnect();
		    				setVisible(false);
		    				//System.exit(0);
		    			}

		    		});
		    		
		 connect();		
		setVisible(true);
	
	
	
	}

	public void connect()
	{
		try {
			s=new Socket("127.0.0.1",8888);
			beconnected=true;
	        dos=new ObjectOutputStream(s.getOutputStream());
	        dis=new ObjectInputStream(s.getInputStream());
System.out.println("我接受的输入流是并且我的名字是"+dis+name);
	        UserBean login=new UserBean();
			login.setName(name);
			login.setType(5);//私聊更新
			login.setInfo("杨甜");
			login.SetPrivateTN(privateTN+"private");
			dos.writeObject(login);
			dos.flush();
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

	
	
	@Override
	public void run() {
		// TODO Auto-generated method stub
		UserBean login=new UserBean();
		try{
		while(beconnected)
		{
			  UserBean bean;
System.out.println("我正在接受并且我接受的输入流是"+dis);
			  bean = (UserBean)dis.readObject();
		       if(bean.getType()==4)
				{
		    	   textArea.append(bean.getTimer()+" "+bean.getName_diaplay()+"  "+"说"+'\n');
		    	   textArea.append(bean.getInfo()+'\n');
				}
		       else if(bean.getType()==6)
		       {
		    	   textArea.append(bean.getTimer()+"  "+bean.getInfo()+'\n');
		       }
				
			  
		}
		}catch(SocketException e)
		{
			System.out.println("退出了  bye bye ");
		}
		catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	
	
}

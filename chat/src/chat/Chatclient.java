package chat;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.IOException;
import java.net.*;	
import java.io.*;

public class Chatclient extends Frame {
	Socket s=null;
	DataOutputStream dos=null;
	DataInputStream dis=null;
	private boolean beconnected=false;
	TextField tfTxt = new TextField();
	TextArea taContent = new TextArea();
	Thread tRecv=new Thread(new RecvThread());

	public static void main(String args[]) {
		new Chatclient().launchframe();
	}

	public void launchframe() {
		setLocation(400, 300);
		this.setSize(300, 300);
		add(tfTxt, BorderLayout.SOUTH);
		add(taContent, BorderLayout.NORTH);
		pack();
		this.addWindowListener(new WindowAdapter() {

			@Override
			public void windowClosing(WindowEvent arg0) {
				disconnect();
				System.exit(0);
			}
			
		});
		tfTxt.addActionListener(new TF_Listener());
		setVisible(true);
		connect();
		tRecv.start();

	}
	
	
	public void connect()
	{
		try {
			s=new Socket("127.0.0.1",8888);
			beconnected=true;
	        dos=new DataOutputStream(s.getOutputStream());
	        dis=new DataInputStream(s.getInputStream());
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
			/*try {
			beconnected=false;
			tRecv.join();
			合并  等到t结束后
				thread.Join把指定的线程加入到当前线程，
				可以将两个交替执行的线程合并为顺序执行的线程。
				比如在线程B中调用了线程A的Join()方法，
				直到线程A执行完毕后，
				才会继续执行线程B。 
			
		}*/
	}
	
	private class TF_Listener implements ActionListener
	{

		@Override
		public void actionPerformed(ActionEvent e) {
			String str= tfTxt.getText().trim();
			//taContent.setText(str);
			tfTxt.setText("");
			try {
				dos.writeUTF(str);
				dos.flush();
				//dos.close();
			} catch (IOException e1) {
				e1.printStackTrace();
			}
			
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
				String str=dis.readUTF();
				//System.out.println(str);
				taContent.setText(taContent.getText()+str+'\n');
			}
			}catch(SocketException e)
			{
				System.out.println("退出了  bye bye ");
			}
			catch(EOFException e)
			{
				System.out.println("退出了  bye ");
			}
			catch(IOException e)
			{
				e.printStackTrace();
			}

			
		}
		
	}
}

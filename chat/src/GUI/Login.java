package GUI;
import java.awt.Color;
import java.awt.EventQueue;
import java.awt.Graphics;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.HashMap;
import java.util.Properties;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;

import chat.*;


//import cat.function.ClientBean;


public class Login extends JFrame {
	private JPanel contentPane;
	private JTextField textField;
	private JPasswordField passwordField;
	private JLabel label1;
	private JLabel label2;
	//public static HashMap<String, ClientBean> onlines;
	public static void main(String[] args)
	{
		new Login().login_frame();
	}
	
	
	public void login_frame()
	{
		setTitle("Landing Yang_tian room\n");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(208, 88, 900, 552);
		contentPane =new JPanel()
		{
			private static final long serialVersionUID=1L;
			protected void paintComponent(Graphics g)//???????
			{
				super.paintComponent(g);
				g.drawImage(new ImageIcon(//images\\\u767B\u9646\u754C\u9762.jpg
						"./images/background1.png").getImage(), 0,0, getWidth(), getHeight(),null);
			}
		};
		contentPane.setBorder(new EmptyBorder(5,5,5,5));
		setContentPane(contentPane);
		contentPane.setLayout(null);
		
		
		label1=new JLabel();
		label2=new JLabel();
		label1.setText("用户名: ");
		
		label2.setText("密  码: ");
		label1.setBounds(610, 371, 50, 21);
		//label1.setLocation(500, 200);
		//label1.setSize(10, 10);
		label2.setBounds(610, 420, 50, 21);
	
		contentPane.add(label1);
		contentPane.add(label2);
		
		
		
		
		
		
		textField = new JTextField();
		textField.setBounds(673, 371, 104, 21);
		textField.setOpaque(false);
		contentPane.add(textField);
		textField.setColumns(10);
		
		passwordField = new JPasswordField();
		passwordField.setForeground(Color.BLACK);
		passwordField.setEchoChar('*');
		passwordField.setOpaque(false);
		passwordField.setBounds(673, 420, 104, 21);
		contentPane.add(passwordField);
		
		
		
		final JButton btnNewButton = new JButton();
		btnNewButton.setIcon(new ImageIcon("./images/login1.gif"));//images\\\u767B\u9646.jpg
		btnNewButton.setBounds(626, 464, 50, 25);
		getRootPane().setDefaultButton(btnNewButton);
		contentPane.add(btnNewButton);

		
		final JButton btnNewButton_1 = new JButton();
		btnNewButton_1.setIcon(new ImageIcon("./images/register2.gif"));//images\\\u6CE8\u518C.jpg
		btnNewButton_1.setBounds(737, 464, 50, 25);
		contentPane.add(btnNewButton_1);
		
		// 提示信息
				final JLabel lblNewLabel = new JLabel();
				lblNewLabel.setBounds(60, 220, 151, 21);
				lblNewLabel.setForeground(Color.red);
				getContentPane().add(lblNewLabel);

		// 监听登陆按钮
		btnNewButton.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				Properties userPro=new Properties();////////////
				File file=new File("Users.properties");///////////
				CatUtil.loadPro(userPro, file);/////
				String u_name = textField.getText();////
				if (file.length() != 0)
				{
					if (userPro.containsKey(u_name))
					{
					String u_pwd = new String(passwordField.getPassword());
					if (u_pwd.equals(userPro.getProperty(u_name))) 
					{
						try {
							Socket client = new Socket("127.0.0.1", 8888);
							btnNewButton.setEnabled(false);
							chatroom_frame frame=new chatroom_frame(u_name,client);
							//frame.launchframe();
							setVisible(false);					
						} catch (UnknownHostException e1) {
							errorTip("The connection with the server is interrupted, please login again");
							e1.printStackTrace();
						} catch (IOException e1) {
							errorTip("The connection with the server is interrupted, please login again");
							e1.printStackTrace();
						}
					}
					else
					{
						lblNewLabel.setText("您输入的密码有误");
						textField.setText("");
						passwordField.setText("");
						textField.requestFocus();
					}
					}
					else
					{
						lblNewLabel.setText("你输入的昵称不存在");
						textField.setText("");
						passwordField.setText("");
						textField.requestFocus();
					}
						
				}
				else
				{

					lblNewLabel.setText("你输入的昵称不存在");
					textField.setText("");
					passwordField.setText("");
					textField.requestFocus();
				}
				
				
			}
		});
		
		//注册按钮监听
		btnNewButton_1.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent arg0) {
				// TODO Auto-generated method stub
				btnNewButton_1.setEnabled(false);
				Resign frame=new Resign();
				frame.setVisible(true);
				setVisible(false);
			}
		});
		setVisible(true);
	}
	protected void errorTip(String str)
	{
		JOptionPane.showMessageDialog(contentPane, str,"Error Message",JOptionPane.ERROR_MESSAGE);
		textField.setText("");
		passwordField.setText("");
		textField.requestFocus();
	}
}

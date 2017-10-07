package GUI;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JPasswordField;
import javax.swing.JTextField;
import javax.swing.border.EmptyBorder;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Properties;

public class Resign extends JFrame {
	private JPanel contentPane;
	private JTextField textField;
	private JPasswordField passwordField;
	private JPasswordField passwordField_1;
	private JLabel lblNewLabel;
	private JLabel label1;
	private JLabel label2;
	private JLabel label3;
	public Resign()
	{
		setTitle("Registered Yang_Tian chat room\n");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(208, 88, 900, 552);
		contentPane =new JPanel()
		{
			private static final long serialVersionUID = 1L;
			protected void paintComponent(Graphics g)
			{
				super.paintComponent(g);
				g.drawImage(new ImageIcon("./images/background1.png").getImage(), 0,0, getWidth(), getHeight(), null);
			}
		};
		
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(null);
		
		textField = new JTextField();
		textField.setBounds(564, 281, 104, 21);
		textField.setOpaque(false);
		contentPane.add(textField);
		textField.setColumns(10);
		
		passwordField = new JPasswordField();
		passwordField.setEchoChar('*');
		passwordField.setOpaque(false);
		passwordField.setBounds(564, 330, 104, 21);
		contentPane.add(passwordField);
        
		passwordField_1 = new JPasswordField();
		passwordField_1.setBounds(564, 379, 104, 21);
		passwordField_1.setOpaque(false);
		contentPane.add(passwordField_1);
		
		
		label1=new JLabel();
		label2=new JLabel();
		label3=new JLabel();
		label1.setText("�û���: ");
		label2.setText("�û�����: ");
		label3.setText("ȷ������: ");
		label1.setForeground(Color.RED);
		label2.setForeground(Color.RED);
		label3.setForeground(Color.RED);
		label1.setBounds(503, 281, 60, 21);
		//label1.setLocation(500, 200);
		//label1.setSize(10, 10);
		label2.setBounds(503, 330, 60, 21);
		label3.setBounds(503, 379, 60, 21);
	
		contentPane.add(label1);
		contentPane.add(label2);
		contentPane.add(label3);
		
		//ע�ᰴť
		final JButton btnNewButton_1 = new JButton();
		btnNewButton_1.setIcon(new ImageIcon("images\\ע��1.jpg"));
		btnNewButton_1.setBounds(540, 428, 80, 40);
		getRootPane().setDefaultButton(btnNewButton_1);//����Ϊȱʡ�İ�ť
		contentPane.add(btnNewButton_1);
		
		//���ذ�ť
		final JButton btnNewButton = new JButton("");
		btnNewButton.setIcon(new ImageIcon("images\\����.jpg"));
		btnNewButton.setBounds(650, 428, 80, 40);
		contentPane.add(btnNewButton);
		
		//��ʾ��Ϣ
				lblNewLabel = new JLabel();
				lblNewLabel.setBounds(55, 218, 185, 20);
				lblNewLabel.setForeground(Color.red);
				contentPane.add(lblNewLabel);
		
				
		//���ذ�ť����
		btnNewButton.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
			//���ص�¼����
				Login frame =new Login();
				frame.login_frame();
				setVisible(false);
				}
		});
		
		
		//ע�ᰴť����
		btnNewButton_1.addActionListener(new ActionListener() {
			
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				Properties userPro = new Properties();
				File file = new File("Users.properties");
				CatUtil.loadPro(userPro, file);
				
				
				String u_name=textField.getText();
				String u_pwd=new String(passwordField.getPassword());
				String u_pwd_ag=new String(passwordField_1.getPassword());
				
				// �ж��û����Ƿ�����ͨ�û����Ѵ���
				if(u_name.length()!=0)
				{
					if(userPro.containsKey(u_name))
					{
						lblNewLabel.setText("�û����Ѵ���");
					}
					else
					{
						isPassword(userPro, file, u_name, u_pwd, u_pwd_ag);
					}
				}
				else{
					lblNewLabel.setText("�û�������Ϊ��");
				}
				
			}
			
			private void isPassword(Properties userPro,File file, String u_name, String u_pwd, String u_pwd_ag)
			{
			if(u_pwd.equals(u_pwd_ag)){
				if(u_pwd.length()!=0)
				{
					userPro.setProperty(u_name, u_pwd_ag);
					try {
						userPro.store(new FileOutputStream(file),
								"Copyright (c) Boxcode Studio");
					} catch (FileNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					btnNewButton_1.setEnabled(false);
					//���ص�¼����
					Login frame=new Login();
					frame.login_frame();
					setVisible(false);
				}
				else
				{
					lblNewLabel.setText("����Ϊ��");
				}
			}
				else
				{
					lblNewLabel.setText("���벻һ��");
				}
			}	
				
			
		});
		
		
		
		
	}

}

package chat;

import information.ClientBean;

import java.net.*;
import java.util.HashMap;
import java.io.*;

public class MultiServer 
{
	public static HashMap<String, ClientBean> onlines;
    public static void main(String[] args) throws IOException 
	{
        ServerSocket serverSocket = null;
        boolean listening = true;

        try 
		{
            serverSocket = new ServerSocket(8888);
        }
        catch(BindException e)
		{
			System.out.println("�˿�ʹ����");
			System.out.println("��ص���س����������з�����");
			System.exit(0);
		}
 		catch (IOException e) 
		{
 			e.printStackTrace();
        }
        
        
        
        
        try
        {
        while (listening)
        {
        	Socket ss=serverSocket.accept();
        	new MultiServerThread(ss).start();
        }
         }catch(IOException e)
		{
		e.printStackTrace();		
	    }
        finally
	 {
		try {
			 serverSocket.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	}
}


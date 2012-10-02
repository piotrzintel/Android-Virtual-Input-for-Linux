/*

   AndClient, part of Android™ Virtual Input for LInux project

   Copyright 2012 Piotr Zintel
   zintelpiotr@gmail.com

   Android is a trademark of Google Inc.

*/

/*

   AndClient is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   any later version.

   AndClient is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

package com.andclient;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.ConnectException;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.CertificateException;

import javax.net.SocketFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLHandshakeException;
import javax.net.ssl.SSLSocket;
import javax.net.ssl.SSLSocketFactory;
import javax.net.ssl.TrustManagerFactory;

import android.os.Environment;
import android.util.Log;

public class ConnectionHandler {
	protected String mHost;
	protected int mPort;

	protected SSLSocketFactory msslSocketFactory;
	protected SSLSocket msslSocket;
	protected Socket mSocket;
	
	public ConnectionHandler(String host, int port) throws Exception {
		if (host == null) {
			throw new NullPointerException("2011-12-19 22:20:40 host cannot be null");
		}
		if (port <= 0) {
			throw new IllegalArgumentException("2011-12-19 22:13:35 port must be a positive integer");
		}
		
		this.mHost = host;
		this.mPort = port;
		
		try {
			KeyStore trustStore = KeyStore.getInstance("BKS");
			FileInputStream trustIfstream = new FileInputStream(Environment.getExternalStorageDirectory().getPath() + "/andserver.bks");
						
			SSLContext sslContext = SSLContext.getInstance("TLSV1");
			
			try {
				trustStore.load(trustIfstream, "123456".toCharArray());
			} catch (NoSuchAlgorithmException e) {
				Log.e("ConnectionHandler", "2012-03-02 23:00:53 " + e.toString());
			} catch (CertificateException e) {
				Log.e("ConnectionHandler", "2012-03-02 23:00:47 " + e.toString());
			} catch (IOException e) {
				Log.e("ConnectionHandler", "2012-03-02 23:00:39 " + e.toString());
			}
			finally {
					trustIfstream.close();
			}
			
			TrustManagerFactory trustManagerFactory = TrustManagerFactory.getInstance("X509");
						
			trustManagerFactory.init(trustStore);
			
			sslContext.init(null, trustManagerFactory.getTrustManagers(), null);
			msslSocketFactory = sslContext.getSocketFactory();
			
		} catch (IOException e) {
			Log.e("ConnectionHandler", "2012-03-02 23:01:26 " + e.toString());
			throw new IOException("Certificate file " + Environment.getExternalStorageDirectory().getPath() + "/andserver.bks not found");
		} catch (KeyStoreException e) {
			Log.e("ConnectionHandler", "2012-03-02 23:01:26 " + e.toString());
			throw new Exception("Error preparing server certificate");
		} catch (NoSuchAlgorithmException e) {
			Log.e("ConnectionHandler", "2012-03-02 23:01:52 " + e.toString());
			throw new Exception("Error preparing server certificate");
		} catch (KeyManagementException e) {
			Log.e("ConnectionHandler", "2012-03-02 23:01:58 " + e.toString());
			throw new Exception("Error preparing server certificate");
		} catch (NullPointerException e) {
			Log.e("ConnectionHandler", "2012-03-09 03:00:29 " + e.toString());
		}
	}
	
	public boolean Connect() throws Exception {
		try {
			InetSocketAddress remoteAddr = new InetSocketAddress(mHost, mPort);
			mSocket = SocketFactory.getDefault().createSocket();
			mSocket.setTcpNoDelay(false);
			mSocket.setSoTimeout(5000);
			mSocket.connect(remoteAddr, 5000);
			msslSocket = (SSLSocket) msslSocketFactory.createSocket(mSocket, mHost, mPort, true);
			msslSocket.setUseClientMode(true);

			msslSocket.startHandshake();
			if (!msslSocket.isConnected()) {
				throw new Exception("Could not connect to the server");	
			}
			
		} catch (UnknownHostException e) {
			Log.e("ConnectionService","2012-03-06 15:23:26 " + e.toString());
			throw new Exception("Server with that address not found");
		} catch (ConnectException e) {
			Log.e("ConnectionService","2012-03-06 15:24:02 "+ e.toString());
			if ( e.getMessage().contains("Network is unreachable") ) {
				throw new Exception("No internet access");
			} else {
				throw new Exception("Could not connect to the server");
			}
		} catch (SSLHandshakeException e) {
			Log.e("ConnectionService","2012-03-09 01:31:27 " + e.toString());
			throw new Exception("SSL: Could not establish a secure connection");
		} catch (SocketException e) {
			Log.e("ConnectionService", "2012-03-09 02:26:23 " + e.toString());
			if (e.getMessage().equals("Permission denied"))  {
				throw new Exception("No internet access");
			} else {
				throw new Exception("Connection timed out");
			}
		} catch (IOException e) {
			Log.e("ConnectionService","2012-03-06 15:24:16 " + e.toString());
			throw new Exception("Could not connect to the server");
		}
		if (!ReceiveMsgReady()) {
			throw new Exception("No response from the server");
		} else {
			return true;
		}
	}
	
	public boolean ReceiveMsgReady() {
		byte msgType;
		try {
			msgType = (byte)msslSocket.getInputStream().read();
			if ( msgType == MessageTypes.MSG_READY) {
				return true;
			} else {
				return false;
			}
		} catch (IOException e) {
			Log.e("ConnectionHandler", "2012-03-02 23:03:12 " + e.toString());
			return false;
		} catch (NullPointerException e) {
			Log.e("ConnectionHandler", "2012-03-09 02:59:06 " + e.toString());
			return false;
		}
	}
	
	public void pollConnectionStatus() throws Exception {
		OutputStream os = msslSocket.getOutputStream();
		byte buffer[] = new byte[5];
		
		buffer[0] = MessageTypes.MSG_POLL;
		for (int i=1;i<5;++i) {
			buffer[i] = 0;
		}
		os.write(buffer);
	}
	
	public void close() {
		try {
				mSocket.close();
		} catch (IOException e) {
			Log.e("ConnectionHandler", "2012-03-02 23:03:20 " + e.toString());
		} catch (NullPointerException e) {
			Log.e("ConnectionHandler", "2012-03-09 02:58:03 " + e.toString());
		}
	}
}

/**
 * @file
 * An SSH private key transfer applet (from server to client).  This
 * applet takes care of getting a user's private key from the server
 * and writes it into the correct place in the mind term diectory.
 * 
 * The CloudSSHKeyManager is the compiled and signed version of this class.
 *
 * Copyright (c) 2010-2011 DOCOMO Innovations, Inc.
 */
package com.clanavi;

import java.applet.Applet;
import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import javax.swing.JOptionPane;


public class CloudSSHKeyManager extends Applet {

  private static final long serialVersionUID = 1L;

  public void init() {

    String file_url = getParameter("file_url");
    String fileName = getParameter("private-key");
    String cookie = getParameter("browser_cookie");
    
    System.out.println("browser cookie=" + cookie);
    System.out.println("File=" + file_url);
    System.out.println("KEY="  + fileName);

    String s2 = System.getProperty("java.version");
    String s3 = System.getProperty("user.home");

    if (s2.startsWith("1.0") || s2.startsWith("1.1") || s2.startsWith("1.2") || s2.startsWith("1.3") || s2.startsWith("1.4")) {
      JOptionPane.showMessageDialog(this, (new StringBuilder()).append("Please download latest version of Java ").toString());
    }

    String tgtFileName = (new StringBuilder()).append(s3).append("/mindterm/").append(fileName).toString();
    String tgtDir = (new StringBuilder()).append(s3).append("/mindterm" ).toString();

    File file_dir = new File(tgtDir);
    file_dir.mkdirs();
    String key = "";

    try  {
      URL url;
      URLConnection urlConn;
      DataInputStream dis;

      url = new URL(file_url);

      urlConn = url.openConnection();
      // Set the cookie explicitly
      urlConn.setRequestProperty("Cookie", cookie);
      urlConn.setDoInput(true);
      urlConn.setUseCaches(false);

      dis = new DataInputStream(urlConn.getInputStream());
      String line;
      StringBuilder stringBuilder = new StringBuilder();

      BufferedReader buffReader = new BufferedReader(new InputStreamReader( urlConn.getInputStream() ) ) ;

      while ((line = buffReader.readLine() ) != null) {
        stringBuilder.append( line );
        stringBuilder.append( "\n" );
      }
      dis.close();
      key = stringBuilder.toString();
    } catch(MalformedURLException mue) {
        mue.printStackTrace();
    } catch(IOException ioe) {
       ioe.printStackTrace();
    }

    try {
      FileWriter fp = new FileWriter(tgtFileName);
      fp.write(key);

      fp.flush();
      fp.close();

    } catch(Exception ex)  {
      ex.printStackTrace();
      return;
    }

    tgtFileName = (new StringBuilder()).append(s3).append("/.mindterm/").append(fileName).toString();
    tgtDir = (new StringBuilder()).append(s3).append("/.mindterm" ).toString();

    file_dir = new File(tgtDir);
    file_dir.mkdirs();

    try {
      FileWriter fp = new FileWriter(tgtFileName);
      fp.write(key);
      fp.write("\n");
      fp.flush();
      fp.close();

      System.out.println("Completed writing . file.. ");
      
    } catch(Exception ex)  {
      ex.printStackTrace();
      return;
    }
    System.out.println("Completed copying of File.");
  }
}

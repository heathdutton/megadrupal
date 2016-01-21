package ca.coldfrontlabs.shibboleth.idp.authn;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.Cookie;

import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import java.net.URL;
import java.net.URLConnection;

import java.util.Iterator;


import org.slf4j.Logger;

import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;


public class DrupalAuthValidator {
    /**
     * Retrieves the token for the cookies passed by the client
     */
    public static String resolveCookie(HttpServletRequest httpRequest, String authCookieName) {
      Cookie[] cookies = httpRequest.getCookies();

      if (null != cookies) {
        for (int i = 0; i < cookies.length; i++) {
          Cookie c = cookies[i];
          if (c.getName().equals(authCookieName)) {
            return c.getValue().toString();
          }
        }
      }
      return "";
    }

    /**
     * Validates a token via the configured rest service
     */
    public static String validateSession(HttpServletRequest httpRequest, String token, String authValidationEndpoint, String xforwardedHeader, boolean validateSessionIP, Logger log) {

      // Whether the session is valid
      boolean authenticated = false;

      // The username retrieved
      String username = "";

      // The client ip address
      String host = "";

      // POST the token to the REST service
      URLConnection connection;
      try {
        connection = new URL(authValidationEndpoint).openConnection();
      } catch (Exception e) {
        log.warn("Tried to connect to the DrupalAuth endpoint and got: " + e.getMessage());
        return "";
      }
      connection.setDoOutput(true); // Triggers POST.
      connection.setRequestProperty("Content-Type", "application/json");
      OutputStream output = null;
      PrintWriter writer = null;
      try {
        output = connection.getOutputStream();
        writer = new PrintWriter(new OutputStreamWriter(output), true);
        writer.append("{\"token\":\"" + token +"\"}").flush();
      } catch (Exception e) {
        log.warn("Tried to validate DrupalAuth token and got: " + e.getMessage());
        return "";
      } finally {
        if (output != null) {
          try {
            output.close();
          } catch (IOException logOrIgnore) {
            log.warn("Tried close connection to DrupalAuth and got: " + logOrIgnore.getMessage());
            return "";
          }
        }
      }

      // Now that we have the XML reply, parse it
      SAXReader reader;
      Document doc;
      Element root;
      Iterator i;

      try {
        reader = new SAXReader();
        doc = reader.read(connection.getInputStream());
      } catch (Exception e) {
        log.warn("Try to parse DrupalAuth return XML and got: " + e.getMessage());
        return "";
      }
      root = doc.getRootElement();
      i = root.elements().iterator();

      // The first element should whether the authentication was sucessful
      if (i.hasNext()) {
        Element elem = (Element)i.next();
        authenticated = Integer.parseInt(elem.getText()) == 1;
      }

      // If the token was authenticated, get the username
      if (authenticated && i.hasNext()) {
        Element elem = (Element)i.next();
        username = elem.getText();
      }

      // and then get the host
      if (authenticated && i.hasNext()) {
        Element elem = (Element)i.next();
        host = elem.getText();
      }

      // Check to make sure the client ip matches the ip on the Drupal session
      String clientip = httpRequest.getHeader(xforwardedHeader);


      if (validateSessionIP && authenticated && httpRequest.getRemoteAddr().equals(host)) {
        log.info(host + " matched " + httpRequest.getRemoteAddr() + ", authentication suscessful.");
      } else if (validateSessionIP && clientip != null && clientip.startsWith(host)) {
        log.info(xforwardedHeader + " contains " + clientip + " and matched " + host + ", authentication suscessful.");
      } else if (!validateSessionIP) {
        log.info("IP address validation disabled, authentication suscessful.");
      } else {
        authenticated = false;
        log.info(host + " didn't match " + httpRequest.getRemoteAddr() + " and " + xforwardedHeader + " contained " + clientip + ", authentication failed.");
      }

      // Return whether authentication was successful
      if (authenticated) {
        return username;
      } else {
        return "";
      }
    }

}

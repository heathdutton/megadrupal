package ca.coldfrontlabs.shibboleth.idp.authn.provider;
 
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.Cookie; 
import javax.servlet.http.HttpSession;

import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.URL;
import java.net.URLConnection;
import java.util.Iterator;

import org.opensaml.util.URLBuilder;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;
 
import edu.internet2.middleware.shibboleth.idp.authn.provider.AbstractLoginHandler;
import edu.internet2.middleware.shibboleth.idp.authn.AuthenticationEngine;
import edu.internet2.middleware.shibboleth.idp.authn.LoginHandler;
import edu.internet2.middleware.shibboleth.idp.authn.LoginContext;

/**
 *  Drupal authentication handler
 *
 *  This authenticates a user based on a cookie set by a Drupal (or compatible) site.
 *  The retrieved cookie is then validated via rest service running on a Drupal (or compatible) site.
 *  This rest service returns the username and host ip of the user with the session, if the token is valid.
 */
public class DrupalAuthLoginHandler extends AbstractLoginHandler {
 
     /** Class logger. */
    private final Logger log = LoggerFactory.getLogger(DrupalAuthLoginHandler.class);
 
    /** The URL of the login servlet. */
    private String authenticationServletURL;

    /** The name of the cookie to search for */
    private String authCookieName;

    /** The remote endpoint for validating sessions */
    private String authValidationEndpoint;

    /** The drupal login URL */
    private String drupalLoginURL;

    /** The remote header containing client ip */
    private String xforwardedHeader;

    /** Whether or not to validate the request ip and session ip */
    private Boolean validateSessionIP;
 
    /** Constructor. */
    public DrupalAuthLoginHandler(String servletURL) {
        super();
        setSupportsPassive(true);
        setSupportsForceAuthentication(true);
        authenticationServletURL = servletURL;
    }

    /** {@inheritDoc} */
    public boolean supportsPassive() {
        return true;
    }

    /**
     *  Gets the name of the cookie containing the authentication token
     */
    public String getAuthCookieName() {
        return authCookieName;
    }

    /**
     *  Sets the name of the cookie containing the authentication token
     */
    public void setAuthCookieName(String name) {
        authCookieName = name;
    }

    /**
     *  Gets the url of the rest service that validates the authentication tokens
     */
    public String getAuthValidationEndpoint() {
        return authValidationEndpoint;
    }

    /**
     *  Gets the url of the rest service that validates the authentication tokens
     */
    public void setAuthValidationEndpoint(String name) {
        authValidationEndpoint = name;
    }

    /**
     *  Gets the url of the rest service that validates the authentication tokens
     */
    public String getDrupalLoginURL() {
        return drupalLoginURL;
    }

    /**
     *  Gets the url of the rest service that validates the authentication tokens
     */
    public void setDrupalLoginURL(String name) {
        drupalLoginURL = name;
    }

    public void setXforwardedHeader(String header) {
        xforwardedHeader = header;
    }

    public String getXforwardedHeader() {
        return xforwardedHeader;
    }

    public void setValidateSessionIP(Boolean valid) {
        validateSessionIP = valid;
    }

    public Boolean getValidateSessionIP() {
        return validateSessionIP;
    }

 
    /** {@inheritDoc} */
    public void login(HttpServletRequest httpRequest, HttpServletResponse httpResponse) {
      log.info("Starting DrupalAuth authentication");
      HttpSession session = httpRequest.getSession();
      session.setAttribute("drupalauth.authCookieName", authCookieName);
      session.setAttribute("drupalauth.authValidationEndpoint", authValidationEndpoint);
      session.setAttribute("drupalauth.drupalLoginURL", drupalLoginURL);
      session.setAttribute("drupalauth.xforwardedHeader", xforwardedHeader);
      session.setAttribute("drupalauth.validateSessionIP", validateSessionIP);

        // forward control to the servlet.
        try {
            StringBuilder pathBuilder = new StringBuilder();
            pathBuilder.append(httpRequest.getContextPath());
            if (!authenticationServletURL.startsWith("/")) {
                pathBuilder.append("/");
            }
            pathBuilder.append(authenticationServletURL);

            URLBuilder urlBuilder = new URLBuilder();
            urlBuilder.setScheme(httpRequest.getScheme());
            urlBuilder.setHost(httpRequest.getServerName());
            urlBuilder.setPort(httpRequest.getServerPort());
            urlBuilder.setPath(pathBuilder.toString());

            log.debug("Redirecting to {}", urlBuilder.buildURL());
            httpResponse.sendRedirect(urlBuilder.buildURL());
            return;
        } catch (IOException ex) {
            log.error("Unable to redirect to authentication servlet.", ex);
        }
    }
}

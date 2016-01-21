package ca.coldfrontlabs.shibboleth.idp.authn.provider;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import javax.servlet.http.HttpSession;

import java.io.IOException;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import edu.internet2.middleware.shibboleth.idp.authn.AuthenticationEngine;
import edu.internet2.middleware.shibboleth.idp.authn.LoginHandler;

import ca.coldfrontlabs.shibboleth.idp.authn.DrupalAuthValidator;

public class DrupalAuthServlet extends HttpServlet {

    /** Serial version UID. */
    private static final long serialVersionUID = 1745674094856635526L;

    /** Class logger. */
    private final Logger log = LoggerFactory.getLogger(DrupalAuthServlet.class);

    /** {@inheritDoc} */
    protected void service(HttpServletRequest httpRequest, HttpServletResponse httpResponse) throws ServletException, IOException {
      log.info("Starting DrupalAuth authentication");
      HttpSession session = httpRequest.getSession();
      String authCookieName = (String) session.getAttribute("drupalauth.authCookieName");
      String authValidationEndpoint = (String) session.getAttribute("drupalauth.authValidationEndpoint");
      String drupalLoginURL = (String) session.getAttribute("drupalauth.drupalLoginURL");
      String xforwardedHeader  = (String) session.getAttribute("drupalauth.xforwardedHeader");
      Boolean validateSessionIP  = (Boolean) session.getAttribute("drupalauth.validateSessionIP");

      if (authCookieName == "" || authValidationEndpoint == "") {
        log.error("Missing critical settings");
        AuthenticationEngine.returnToAuthenticationEngine(httpRequest, httpResponse);
      }

      String token = DrupalAuthValidator.resolveCookie(httpRequest, authCookieName);
      String username = "";

      if (token != "") {
        log.info("DrupalAuth Authentication found: " + token);
        username = DrupalAuthValidator.validateSession(httpRequest, token, authValidationEndpoint, xforwardedHeader, validateSessionIP, log);
      } else {
        log.info("No DrupalAuth cookie found.");
      }

      if (username != "") {
        log.info("Drupal Authentication Successful, username: " + username);
        httpRequest.setAttribute(LoginHandler.PRINCIPAL_NAME_KEY, username);
        AuthenticationEngine.returnToAuthenticationEngine(httpRequest, httpResponse);
      } else {
        log.info("Drupal Authentication Failed");
        log.debug("Redirecting to Drupal login page {}", drupalLoginURL);
        httpResponse.sendRedirect(drupalLoginURL);
      }
      return;
    }
}

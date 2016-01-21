/*
 * Copyright [2006] [University Corporation for Advanced Internet Development, Inc.]
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package ca.coldfrontlabs.shibboleth.idp.authn.provider;

import java.io.IOException;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.opensaml.util.URLBuilder;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import edu.internet2.middleware.shibboleth.idp.authn.provider.AbstractLoginHandler;
import edu.internet2.middleware.shibboleth.idp.authn.AuthenticationEngine;
import edu.internet2.middleware.shibboleth.idp.authn.LoginHandler;
import edu.internet2.middleware.shibboleth.idp.authn.LoginContext;

/**
 * Authenticate a username and password against a JAAS source.
 * 
 * This authenticaiton handler requires a JSP to collect a username and password from the user. It also requires a JAAS
 * configuration file to validate the username and password.
 * 
 * If an Authentication Context Class or DeclRef URI is not specified, it will default to
 * "urn:oasis:names:tc:SAML:2.0:ac:classes:PasswordProtectedTransport".
 */
public class DrupalAuthUsernamePasswordLoginHandler extends AbstractLoginHandler {

    /** Class logger. */
    private final Logger log = LoggerFactory.getLogger(DrupalAuthUsernamePasswordLoginHandler.class);

    /** The URL of the servlet used to perform authentication. */
    private String authenticationServletURL;


    /** The name of the cookie to search for */
    private String authCookieName;

    /** The remote endpoint for validating sessions */
    private String authValidationEndpoint;

    /** The remote header containing client ip */
    private String xforwardedHeader;

    /** Whether or not to validate the request ip and session ip */
    private boolean validateSessionIP;

    /**
     * Constructor.
     * 
     * @param servletURL URL to the authentication servlet
     */
    public DrupalAuthUsernamePasswordLoginHandler(String servletURL) {
        super();
        setSupportsPassive(false);
        setSupportsForceAuthentication(true);
        authenticationServletURL = servletURL;
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
    public void login(final HttpServletRequest httpRequest, final HttpServletResponse httpResponse) {
      HttpSession session = httpRequest.getSession();
      session.setAttribute("drupalauth.authCookieName", authCookieName);
      session.setAttribute("drupalauth.authValidationEndpoint", authValidationEndpoint);
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

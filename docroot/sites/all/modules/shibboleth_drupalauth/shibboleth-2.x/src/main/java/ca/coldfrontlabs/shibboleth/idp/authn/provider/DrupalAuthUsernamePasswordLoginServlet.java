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
import java.security.Principal;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

import javax.security.auth.Subject;
import javax.security.auth.callback.Callback;
import javax.security.auth.callback.CallbackHandler;
import javax.security.auth.callback.NameCallback;
import javax.security.auth.callback.PasswordCallback;
import javax.security.auth.callback.UnsupportedCallbackException;
import javax.security.auth.login.LoginException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import org.opensaml.util.URLBuilder;
import org.opensaml.xml.util.DatatypeHelper;
import org.opensaml.xml.util.Pair;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import edu.internet2.middleware.shibboleth.idp.authn.AuthenticationEngine;
import edu.internet2.middleware.shibboleth.idp.authn.LoginHandler;
import edu.internet2.middleware.shibboleth.idp.session.Session;

import ca.coldfrontlabs.shibboleth.idp.authn.DrupalAuthValidator;

/**
 * This servlet should be protected by a filter which populates REMOTE_USER. The serlvet will then set the remote user
 * field in a LoginContext.
 */
public class DrupalAuthUsernamePasswordLoginServlet extends HttpServlet {

    /** Serial version UID. */
    private static final long serialVersionUID = -572799841125956990L;

    /** Class logger. */
    private final Logger log = LoggerFactory.getLogger(DrupalAuthUsernamePasswordLoginServlet.class);

    /** Name of JAAS configuration used to authenticate users. */
    private String jaasConfigName = "ShibUserPassAuth";

    /** init-param which can be passed to the servlet to override the default JAAS config. */
    private final String jaasInitParam = "jaasConfigName";

    /** Login page name. */
    private String loginPage = "login.jsp";

    /** init-param which can be passed to the servlet to override the default login page. */
    private final String loginPageInitParam = "loginPage";

    /** Parameter name to indicate login failure. */
    private final String failureParam = "loginFailed";

    /** HTTP request parameter containing the user name. */
    private final String usernameAttribute = "j_username";

    /** HTTP request parameter containing the user's password. */
    private final String passwordAttribute = "j_password";

    /** {@inheritDoc} */
    public void init() {
        if (getInitParameter(jaasInitParam) != null) {
            jaasConfigName = getInitParameter(jaasInitParam);
        }
        if (getInitParameter(loginPageInitParam) != null) {
            loginPage = getInitParameter(loginPageInitParam);
        }
    }

    /** {@inheritDoc} */
    protected void service(HttpServletRequest request, HttpServletResponse response) throws ServletException,
            IOException {

        HttpSession session = request.getSession();
        String authCookieName = (String) session.getAttribute("drupalauth.authCookieName");
        String authValidationEndpoint = (String) session.getAttribute("drupalauth.authValidationEndpoint");
        String xforwardedHeader  = (String) session.getAttribute("drupalauth.xforwardedHeader");
        Boolean validateSessionIP  = (Boolean) session.getAttribute("drupalauth.validateSessionIP");
        String username = "";

        if (authCookieName != "" && authValidationEndpoint != "") {
          String token = DrupalAuthValidator.resolveCookie(request, authCookieName);

 
          if (token != "") {
            log.info("DrupalAuth Authentication found: " + token);
            username = DrupalAuthValidator.validateSession(request, token, authValidationEndpoint, xforwardedHeader, validateSessionIP, log);
          } else {
            log.info("No DrupalAuth cookie found.");
          }

          if (username != "") {
            log.info("Drupal Authentication Successful, username: " + username);
            request.setAttribute(LoginHandler.PRINCIPAL_NAME_KEY, username);
            Session idpSession = (Session) request.getAttribute(Session.HTTP_SESSION_BINDING_ATTRIBUTE);
            if (idpSession != null) {
              log.info("Session with principal exists: " + idpSession.getPrincipalName());
              if (username != idpSession.getPrincipalName()) {
                request.setAttribute(Session.HTTP_SESSION_BINDING_ATTRIBUTE, null);
              }
            }
            AuthenticationEngine.returnToAuthenticationEngine(request, response);
            return;
          }
          username = null;
        }

        username = DatatypeHelper.safeTrimOrNullString(request.getParameter(usernameAttribute));
        String password = DatatypeHelper.safeTrimOrNullString(request.getParameter(passwordAttribute));

        if (username == null || password == null) {
            redirectToLoginPage(request, response, null);
            return;
        }

        if (authenticateUser(request)) {
            AuthenticationEngine.returnToAuthenticationEngine(request, response);
        } else {
            List<Pair<String, String>> queryParams = new ArrayList<Pair<String, String>>();
            queryParams.add(new Pair<String, String>(failureParam, "true"));
            redirectToLoginPage(request, response, queryParams);
        }
    }

    /**
     * Sends the user to the login page.
     * 
     * @param request current request
     * @param response current response
     * @param queryParams query parameters to pass to the login page
     */
    protected void redirectToLoginPage(HttpServletRequest request, HttpServletResponse response,
            List<Pair<String, String>> queryParams) {
        try {
            StringBuilder pathBuilder = new StringBuilder();
            pathBuilder.append(request.getContextPath());
            pathBuilder.append("/");
            pathBuilder.append(loginPage);

            URLBuilder urlBuilder = new URLBuilder();
            urlBuilder.setScheme(request.getScheme());
            urlBuilder.setHost(request.getServerName());
            urlBuilder.setPort(request.getServerPort());
            urlBuilder.setPath(pathBuilder.toString());

            if (queryParams == null) {
                queryParams = new ArrayList<Pair<String, String>>();
            }
            
            queryParams.add(new Pair<String, String>("actionUrl", request.getContextPath()
                    + request.getServletPath()));
            urlBuilder.getQueryParams().addAll(queryParams);

            log.debug("Redirecting to login page {}", urlBuilder.buildURL());
            response.sendRedirect(urlBuilder.buildURL());
            return;
        } catch (IOException ex) {
            log.error("Unable to redirect to login page.", ex);
        }
    }

    /**
     * Authenticate a username and password against JAAS. If authentication succeeds the name of the first principal, or
     * the username if that is empty, and the subject are placed into the request in their respective attributes.
     * 
     * @param request current authentication request
     * 
     * @return true of authentication succeeds, false if not
     */
    protected boolean authenticateUser(HttpServletRequest request) {

        try {
            String username = DatatypeHelper.safeTrimOrNullString(request.getParameter(usernameAttribute));
            String password = DatatypeHelper.safeTrimOrNullString(request.getParameter(passwordAttribute));

            SimpleCallbackHandler cbh = new SimpleCallbackHandler(username, password);

            javax.security.auth.login.LoginContext jaasLoginCtx = new javax.security.auth.login.LoginContext(
                    jaasConfigName, cbh);

            jaasLoginCtx.login();
            log.debug("Successfully authenticated user {}", username);

            Subject subject = jaasLoginCtx.getSubject();
            Set<Principal> principals = subject.getPrincipals();

            if (principals.isEmpty()) {
                request.setAttribute(LoginHandler.PRINCIPAL_NAME_KEY, username);
            } else {
                Principal principal = principals.iterator().next();
                String principalName = DatatypeHelper.safeTrimOrNullString(principal.getName());
                if (principalName == null) {
                    request.setAttribute(LoginHandler.PRINCIPAL_NAME_KEY, username);
                } else {
                    request.setAttribute(LoginHandler.PRINCIPAL_NAME_KEY, principal.getName());
                }
                request.setAttribute(LoginHandler.SUBJECT_KEY, jaasLoginCtx.getSubject());
            }

            return true;
        } catch (LoginException e) {
            log.debug("User authentication failed", e);
            return false;
        }
    }

    /**
     * A callback handler that provides static name and password data to a JAAS loging process.
     * 
     * This handler only supports {@link NameCallback} and {@link PasswordCallback}.
     */
    protected class SimpleCallbackHandler implements CallbackHandler {

        /** Name of the user. */
        private String uname;

        /** User's password. */
        private String pass;

        /**
         * Constructor.
         * 
         * @param username The username
         * @param password The password
         */
        public SimpleCallbackHandler(String username, String password) {
            uname = username;
            pass = password;
        }

        /**
         * Handle a callback.
         * 
         * @param callbacks The list of callbacks to process.
         * 
         * @throws UnsupportedCallbackException If callbacks has a callback other than {@link NameCallback} or
         *             {@link PasswordCallback}.
         */
        public void handle(final Callback[] callbacks) throws UnsupportedCallbackException {

            if (callbacks == null || callbacks.length == 0) {
                return;
            }

            for (Callback cb : callbacks) {
                if (cb instanceof NameCallback) {
                    NameCallback ncb = (NameCallback) cb;
                    ncb.setName(uname);
                } else if (cb instanceof PasswordCallback) {
                    PasswordCallback pcb = (PasswordCallback) cb;
                    pcb.setPassword(pass.toCharArray());
                }
            }
        }
    }
}

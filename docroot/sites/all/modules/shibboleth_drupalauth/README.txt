Shibboleth DrupalAuth
---------------------

This module allows you to use Drupal as an Authentication providor for Shibboleth.

This module includes a new Shibboleth login handler that must be configured in your Shibboleth instance to work.

This module also contains a submodule named "shibboleth_drupalauth_rest_service" which stores a features with a pre-configured service to help with the initial setup.

These instructions require that you have at least a basic understanding of the Shibboleth IdP and have some experience deploying/configuring it.


Caveat
------

The following items can be fixed with the subtle application of more code, but they are limitation of the current version:
- The Drupal username is used as the Shibboleth principale key, so Shibboleth attribute resolution must work using that key.


How this module works
---------------------

When enabled, this module sets a cookie for any logged in users.
This cookie:
- Is set on a domain and path accessable to both Drupal and the Shibboleth IdP
- Contains the uid of the user and a security token based on that users current session.

The DrupalAuth Shibboleth Login Handler, once installed and configured, looks for this cookie.
If the cookie is found, the token is validated using a service hosted by the shibboleth instance.
This validation returns whether or not the session is valid, and if it is, the username and IP address associated to that session.

If the cookie exists and token inside is valid, the IP address in the session is compared IP of request seeking Shibboleth authentication.
If the IP addresses match, the username is set to the principale and Shibboleth proceeds to attribute resolution.

Due to the nature of this step, if you Idp is behind a reverse proxy, authentication will always fail (until the DrupalAuthLoginHandler is updated to support reverse proxies.)

If the cookie was not available, was invalid, or the IP addresses didn't match, the user is forward to /shibboleth_continue on the authenticating Drupal site.
This location forwards authenticated users back to Shibboleth, are forward unauthenticated users to "/user?destination=shibboleth_continue".


Setup of the Shibboleth_DrupalAuth module in Authenticating Drupal Site
-----------------------------------------------------------------------

1. Enable the shibboleth_drupalauth module normally.

2. Either enable the shibboleth_drupalauth_rest_service module or configure the following:
  a. Enable the restserver services module.
  b. Create a new restserver services endpoint.
  c. Enable the following resource for that endpoint:  "user -> validation"

3. Go to admin/config/people/shibboleth_drupalauth
  a.  Configure the Cookie Domain.  The domain the authentication cookie is set on (i.e. if the drupal site is drupal.example.com and the Shibboleth IdP is idp.example.com, the cookie domain should be example.com).
  b.  Configure the URL to the IdP DrupalAuth Servelet.  This should be the URL of the IdP plus /Authn/DrupalAuth, unless you're changed this location on the IdP.

Now the Drupal site is configured to act as an authentication providor for Shibboleth


Compiling the DrupalAuth Shibboleth login handler
-------------------------------------------------

Shibboleth_DrupalAuth provides a custom login handler extension for Shibboleth, this extension must be compiled and packaged before use.

Requirements:  Apache Maven 2 or 3 and JDK 6 or 7.

Building of the login handler is automated with Apache Maven.
If maven and a JDK is installed correctly on a machine with internet access (required for Maven to download any needed libraries), running the following is sufficient to build the login handler:

$ cd shibboleth-2.x
$ mvn package

The login handler will be compiled and packaged into a JAR in the target/ directory (which will be created by Maven should it not exist.)


Setup of the Shibboleth IdP
---------------------------

There are two seperate extensions for Shibboleth IdP:

 -DrupalAuthLoginHandler
    This plugin uses Drupal's login page.

 -DrupalAuthUsernamePasswordAuthHandler
    This plugin uses Shibboleth's login.jsp page (the same as UsernamePasswordAuthHandler would.)

DrupalAuthLoginHandler Setup
----------------------------

1. Add the packaged jar located in shibboleth-2.x/target/drupalauth-1.0.jar to the /lib directory of the shibboleth-identityprovider-2.3.8-bin.zip directory used to deploy the identity provider.
2. Edit the web.xml file in the shibboleth-identityprovider/src/main/webapp/WEB-INF directory and add the following definitions with the similar definitions:

    <servlet>
            <servlet-name>DrupalAuthLoginHandler</servlet-name>
            <servlet-class>ca.coldfrontlabs.shibboleth.idp.authn.provider.DrupalAuthServlet</servlet-class>
            <load-on-startup>3</load-on-startup>
    </servlet>

    <servlet-mapping>
            <servlet-name>DrupalAuthLoginHandler</servlet-name>
            <url-pattern>/Authn/DrupalAuth</url-pattern>
     </servlet-mapping>

3. Run the install script in the shibboleth-identityprovider directory, and follow the steps to rebuild and redeploy the shibboleth WAR.

Your shibboleth installation should now have the DrupalAuthLoginHandler running in it, so all that remains is to configure Shibboleth to use the DrupalAuth login handler.

4. Edit conf/handler.xml in your IdP directory.
  a. At the beginning the xml file, update the ProfileHandlerGroup element to include the XML namespace of the DrupalAuth login handler.  It should look like this:

<ph:ProfileHandlerGroup xmlns:ph="urn:mace:shibboleth:2.0:idp:profile-handler"
                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
                        xmlns:cfli="http://coldfrontlabs.ca/shibboleth/authn"
                        xsi:schemaLocation="urn:mace:shibboleth:2.0:idp:profile-handler classpath:/schema/shibboleth-2.0-idp-profile-handler.xsd
                                            http://coldfrontlabs.ca/shibboleth/authn classpath:/schema/drupalauth-profile-handler.xsd">

  b. Remove (or comment out) all other login handlers (ph:UsernamePassword, ph:PreviousSession, ph:ExternalAuthn, and ph:RemoteUser.)
  c. In place of the login handler removed in the previous step, add the following:

    <ph:LoginHandler xsi:type="cfli:DrupalAuth" 
                  authCookieName = "drupalauth"
                  authValidationEndpoint = "http://drupal.example.com/auth/user/validate.xml"
                  drupalLoginURL = "http://drupal.example.com/shibboleth_continue"
                  xforwardedHeader = "X-Forwarded-For"
                  validateSessionIP = "true"
                  >
        <ph:AuthenticationMethod>urn:oasis:names:tc:SAML:2.0:ac:classes:PasswordProtectedTransport</ph:AuthenticationMethod>
    </ph:LoginHandler>

    i) In the previous block, update the authCookieName to be the Cookie Name as configured in Drupal.
    ii) In the previous block, update the authValidationEndpoint to point to the services endpoint configured in Drupal.
    iii) In the previous block, update the drupalLoginURL to point to '/shibboleth_continue' on the Drupal site.

5. Reload the IdP application.

If everything is configured correctly, your Shibboleth instance should now authenticate using the Drupal site.


DrupalAuthUsernamePasswordAuthHandler Setup
-------------------------------------------

1. Add the packaged jar located in shibboleth-2.x/target/drupalauth-1.0.jar to the /lib directory of the shibboleth-identityprovider-2.3.8-bin.zip directory used to deploy the identity provider.
2. Edit the web.xml file in the shibboleth-identityprovider/src/main/webapp/WEB-INF directory.
  a. Add the following definitions with the similar definitions:

    <!-- Servlet for doing Username/Password authentication -->
    <servlet>
        <servlet-name>DrupalAuthUsernamePasswordAuthHandler</servlet-name>
        <servlet-class>ca.coldfrontlabs.shibboleth.idp.authn.provider.DrupalAuthUsernamePasswordLoginServlet</servlet-class>
        <load-on-startup>3</load-on-startup>
    </servlet>

    <servlet-mapping>
        <servlet-name>DrupalAuthUsernamePasswordAuthHandler</servlet-name>
        <url-pattern>/Authn/UserPassword</url-pattern>
    </servlet-mapping>

  b. Remove or comment out the servlet and servlet-mappings for UsernamePasswordAuthHandler.

3. Run the install script in the shibboleth-identityprovider directory, and follow the steps to rebuild and redeploy the shibboleth WAR.

Your shibboleth installation should now have the DrupalAuthLoginHandler running in it, so all that remains is to configure Shibboleth to use the DrupalAuth login handler.

4. Edit conf/handler.xml in your IdP directory.
  a. At the beginning the xml file, update the ProfileHandlerGroup element to include the XML namespace of the DrupalAuth login handler.  It should look like this:

<ph:ProfileHandlerGroup xmlns:ph="urn:mace:shibboleth:2.0:idp:profile-handler"
                        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
                        xmlns:cfli="http://coldfrontlabs.ca/shibboleth/authn"
                        xsi:schemaLocation="urn:mace:shibboleth:2.0:idp:profile-handler classpath:/schema/shibboleth-2.0-idp-profile-handler.xsd
                                            http://coldfrontlabs.ca/shibboleth/authn classpath:/schema/drupalauth-profile-handler.xsd">

  b.  Remove (or comment out) all other login handlers (ph:UsernamePassword, ph:PreviousSession, ph:ExternalAuthn, and ph:RemoteUser.)
  c. In place of the login handler removed in the previous step, add the following:

    <ph:LoginHandler xsi:type="cfli:DrupalAuthUsernamePassword" 
                  authCookieName = "drupalauth"
                  authValidationEndpoint = "http://drupal.example.com/auth/user/validate.xml"
                  xforwardedHeader = "X-Forwarded-For"
                  validateSessionIP = "true"
                  jaasConfigurationLocation="file:///opt/shibboleth-idp/conf/login.config">
        <ph:AuthenticationMethod>urn:oasis:names:tc:SAML:2.0:ac:classes:PasswordProtectedTransport</ph:AuthenticationMethod>
    </ph:LoginHandler>

    i) In the previous block, update the authCookieName to be the Cookie Name as configured in Drupal.
    ii) In the previous block, update the authValidationEndpoint to point to the services endpoint configured in Drupal.

5. Reload the IdP application.

If everything is configured correctly, your Shibboleth instance should now authenticate using the Drupal site.


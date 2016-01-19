package ca.coldfrontlabs.shibboleth.idp.config.profile.authn;

import ca.coldfrontlabs.shibboleth.idp.authn.provider.DrupalAuthUsernamePasswordLoginHandler;
import edu.internet2.middleware.shibboleth.idp.config.profile.authn.AbstractLoginHandlerFactoryBean;

/**
 * Factory bean for {@link UsernamePasswordLoginHandler}s.
 */
public class DrupalAuthUsernamePasswordLoginHandlerFactoryBean extends AbstractLoginHandlerFactoryBean {

    /** URL to authentication servlet. */
    private String authenticationServletURL;

   /** The name of the cookie to search for */
    private String authCookieName;

    /** The remote endpoint for validating sessions */
    private String authValidationEndpoint;

    /** The remote header containing client ip */
    private String xforwardedHeader;

    /** Whether or not to validate the request ip and session ip */
    private Boolean validateSessionIP;

    /**
     * Gets the URL to authentication servlet.
     * 
     * @return URL to authentication servlet
     */
    public String getAuthenticationServletURL() {
        return authenticationServletURL;
    }

    /**
     * Sets URL to authentication servlet.
     * 
     * @param url URL to authentication servlet
     */
    public void setAuthenticationServletURL(String url) {
        authenticationServletURL = url;
    }


    public String getAuthCookieName() {
        return authCookieName;
    }

    public void setAuthCookieName(String name) {
        authCookieName = name;
    }

    public String getAuthValidationEndpoint() {
        return authValidationEndpoint;
    }

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
    protected Object createInstance() throws Exception {
        DrupalAuthUsernamePasswordLoginHandler handler = new DrupalAuthUsernamePasswordLoginHandler(
                authenticationServletURL);
        handler.setAuthCookieName(getAuthCookieName());
        handler.setAuthValidationEndpoint(getAuthValidationEndpoint());
        handler.setXforwardedHeader(getXforwardedHeader());
        handler.setValidateSessionIP(getValidateSessionIP());
        populateHandler(handler);

        return handler;
    }

    /** {@inheritDoc} */
    public Class getObjectType() {
        return DrupalAuthUsernamePasswordLoginHandler.class;
    }
}
